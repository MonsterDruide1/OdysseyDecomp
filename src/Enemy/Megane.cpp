#include "Enemy/Megane.h"

#include "Library/Base/StringUtil.h"
#include "Library/Bgm/BgmLineFunction.h"
#include "Library/Camera/CameraUtil.h"
#include "Library/Collision/Collider.h"
#include "Library/Collision/KCollisionServer.h"
#include "Library/Effect/EffectSystemInfo.h"
#include "Library/Joint/JointControllerKeeper.h"
#include "Library/Joint/JointSpringController.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/LiveActor/LiveActorFunction.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nature/NatureUtil.h"
#include "Library/Nerve/NerveKeeper.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Play/Camera/CameraPoserSubjective.h"
#include "Library/Player/PlayerUtil.h"
#include "Library/Se/SeFunction.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"

#include "Enemy/EnemyStateSwoon.h"
#include "Layout/TestFilterGlasses.h"
#include "Player/HackerStateNormalMove.h"
#include "Player/PlayerActionTurnControl.h"
#include "Player/PlayerHackStartShaderCtrl.h"
#include "Player/PlayerPushReceiver.h"
#include "Util/CameraUtil.h"
#include "Util/Hack.h"
#include "Util/InputInterruptTutorialUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(Megane, Wait)
NERVE_IMPL(Megane, Find)
NERVE_IMPL(Megane, RunAway)
NERVE_IMPL(Megane, Fall)
NERVE_IMPL(Megane, Land)
NERVE_IMPL(Megane, Trampled)
NERVE_IMPL(Megane, WaitHack)
NERVE_IMPL(Megane, HackStart)
NERVE_IMPL(Megane, Hack)
NERVE_IMPL(Megane, HackSlow)
NERVE_IMPL(Megane, HackSubjective)
NERVE_IMPL(Megane, HackFall)
NERVE_IMPL(Megane, HackLand)
NERVE_IMPL(Megane, Die)
NERVE_IMPL(Megane, Rebirth)
NERVE_IMPL(Megane, Appear)
NERVE_IMPL(Megane, SpectaclesOff)
NERVE_IMPL(Megane, SpectaclesOn)

struct MeganeJointProperties {
    sead::Vector3f position;
    f32 stability;
    f32 friction;
    f32 limitDegree;
};

// TODO: Find a memory layout that fits these globals perfectly
struct {
    NERVE_MAKE(Megane, Wait)
    NERVE_MAKE(Megane, WaitHack)
    NERVE_MAKE(Megane, Hack)
    NERVE_MAKE(Megane, HackSlow)
    NERVE_MAKE(Megane, SpectaclesOn)
    NERVE_MAKE(Megane, Trampled)
    NERVE_MAKE(Megane, HackStart)
    NERVE_MAKE(Megane, Rebirth)
    NERVE_MAKE(Megane, Die)
    NERVE_MAKE(Megane, Appear)
    NERVE_MAKE(Megane, Find)
    NERVE_MAKE(Megane, RunAway)
    NERVE_MAKE(Megane, Fall)
    NERVE_MAKE(Megane, Land)
    NERVE_MAKE(Megane, SpectaclesOff)
    NERVE_MAKE(Megane, HackFall)
    NERVE_MAKE(Megane, HackSubjective)
    NERVE_MAKE(Megane, HackLand)

    PlayerHackStartShaderParam playerHackStartShaderParam =
        PlayerHackStartShaderParam(false, 400.0f, 10, 20);

    MeganeJointProperties jointProperties[4] = {
        {{200.0f, 0.0f, 0.0f}, 0.025f, 0.9f, 10.0f},
        {{200.0f, 0.0f, 0.0f}, 0.025f, 0.9f, 10.0f},
        {{0.0f, -50.0f, -50.0f}, 0.1f, 0.95f, 30.0f},
        {{0.0f, -50.0f, 50.0f}, 0.1f, 0.95f, 30.0f},
    };
} NrvMegane;

}  // namespace

Megane::Megane(const char* name) : al::LiveActor(name) {}

void Megane::init(const al::ActorInitInfo& initInfo) {
    using MeganeFunctor = al::FunctorV0M<Megane*, void (Megane::*)()>;

    al::initActorWithArchiveName(this, initInfo, "Megane", nullptr);
    al::initNerve(this, &NrvMegane.Wait, 3);
    mStateSwoon = new EnemyStateSwoon(this, "SwoonStart", "Swoon", "SwoonEnd", false, true);
    mStateSwoon->initParams(180, nullptr);
    al::initNerveState(this, mStateSwoon, &NrvMegane.WaitHack, "気絶");

    mHackerState = new HackerStateNormalMove(this, &mPlayerHack, "Wait", "HackRun", "Fall");
    mHackerState->set_54(2.0f);
    mHackerState->set_5c(0.9f);
    mHackerState->setMoveVelocity(0.8f);
    mHackerState->set_49(true);
    mHackerState->setTurnAngle(30.0f);
    mHackerState->usePlayerTurnControl(true);
    mHackerState->getTurnControl()->setup(3.0f, 55.0f, 8.0f, 8.0f, 20, 1, 30);
    al::initNerveState(this, mHackerState, &NrvMegane.Hack, "憑依");

    mHackerStateSpectacles = new HackerStateNormalMove(this, &mPlayerHack, "Wait", "Walk", "Fall");
    mHackerStateSpectacles->set_54(1.2f);
    mHackerStateSpectacles->setTurnAngle(30.0f);
    al::initNerveState(this, mHackerStateSpectacles, &NrvMegane.HackSlow, "憑依メガネ");

    mPlayerPushReceiver = new PlayerPushReceiver(this);
    if (!al::trySyncStageSwitchAppear(this))
        makeActorAlive();

    al::listenStageSwitchOnKill(this, MeganeFunctor(this, &Megane::notifyKillSwitch));
    mCollisionPartsFilterBase = new al::CollisionPartsFilterSpecialPurpose("MoveLimit");
    al::setColliderFilterCollisionParts(this, mCollisionPartsFilterBase);

    al::LiveActor* subActor = al::getSubActor(this, "メガネ");
    al::startAction(subActor, "SpectaclesOn");
    al::setActionFrame(subActor, al::getActionFrameMax(subActor, "SpectaclesOn") - 1.0f);

    mTestFilterGlasses =
        new TestFilterGlasses("メガネーレイアウト", al::getLayoutInitInfo(initInfo), nullptr);
    mStartingQuat.set(al::getQuat(this));
    mStartingPos.set(al::getTrans(this));

    mCapTargetInfo = rs::createCapTargetInfo(this, nullptr);
    mCameraPoserSubjective = new al::CameraPoserSubjective("主観");
    mCameraTicket =
        alCameraFunction::initCamera(mCameraPoserSubjective, this, initInfo, nullptr, 9);
    mNerveKeeper = new al::NerveKeeper(this, &NrvMegane.SpectaclesOn, 0);

    mPlayerHackStartShaderCtrl =
        new PlayerHackStartShaderCtrl(this, &NrvMegane.playerHackStartShaderParam);

    al::initJointControllerKeeper(this, 4);

    const char* jointNames[4] = {"Grass1", "Grass2", "EarringL", "EarringR"};

    for (s32 i = 0; i < 4; i++) {
        al::JointSpringController* jointController =
            al::initJointSpringController(this, jointNames[i]);
        jointController->setChildLocalPos(NrvMegane.jointProperties[i].position);
        jointController->setStability(NrvMegane.jointProperties[i].stability);
        jointController->setFriction(NrvMegane.jointProperties[i].friction);
        jointController->setLimitDegree(NrvMegane.jointProperties[i].limitDegree);
        al::StringTmp<128>("ダイナミクス[%s]", jointNames[i]).cstr();
    }

    mHitSensor = al::getHitSensor(this, "CapReceiver");
}

bool Megane::receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self) {
    if (rs::tryReceiveMsgInitCapTargetAndSetCapTargetInfo(message, mCapTargetInfo))
        return true;

    if (rs::isMsgTargetMarkerPosition(message)) {
        rs::setMsgTargetMarkerPosition(message, al::getSensorPos(this, "Head") +
                                                    (al::getSensorRadius(this, "Head") + 50.0f) *
                                                        sead::Vector3f::ey);
        return true;
    }

    if (al::isMsgPlayerDisregard(message))
        return isNerveDieOrRebirth();

    f32 pa1 = 0.0f;
    f32 pa2 = 0.0f;
    if (rs::tryGetSandGeyserRaise(message, &pa1, &pa2)) {
        if (mHitSensor == self)
            return false;
        if (al::getVelocity(this).y < 15.0f) {
            al::addVelocityY(this, 2.0f);
            al::limitVelocityY(this, 15.0f);
            al::scaleVelocityHV(this, 0.9f, 1.0f);
        }
        return true;
    }

    if (rs::isMsgEnableMapCheckPointWarp(message))
        return rs::isMsgEnableMapCheckPointWarpCollidedGround(message, this);

    if (mHitSensor != self && isNormalNerveWithWaitHack() &&
        rs::tryReceiveMsgNpcScareByEnemyIgnoreTargetHack(message, mCapTargetInfo))
        return true;

    if (al::isMsgPushAll(message)) {
        if (mHitSensor == self)
            return false;
        if (isNormalNerveWithWaitHack())
            return al::tryReceiveMsgPushAndAddVelocityH(this, message, other, self, 3.0f);
    }

    if (al::isMsgPlayerTrampleReflect(message)) {
        if (mHitSensor == self)
            return false;

        if (al::isNerve(this, &NrvMegane.WaitHack)) {
            if (mStateSwoon->tryReceiveMsgTrampleReflect(message)) {
                rs::requestHitReactionToAttacker(message, self, other);
                return true;
            }
            return false;
        }

        if (isNormalNerveWithoutWaitHack() && al::isSensorName(self, "Head")) {
            rs::requestHitReactionToAttacker(message, self, other);
            al::setNerve(this, &NrvMegane.Trampled);
            return true;
        }
        return false;
    }

    if (rs::isMsgThrowObjHitReflect(message) || rs::isMsgSphinxRideAttackReflect(message)) {
        rs::requestHitReactionToAttacker(message, self, other);
        if (isNormalNerveWithoutWaitHack())
            al::setNerve(this, &NrvMegane.Trampled);
        return true;
    }

    if (rs::isMsgPlayerAndCapObjHipDropReflectAll(message)) {
        if (mHitSensor == self)
            return false;

        if (al::isNerve(this, &NrvMegane.WaitHack)) {
            if (mStateSwoon->tryReceiveMsgObjHipDropReflect(message)) {
                rs::requestHitReactionToAttacker(message, self, other);
                return true;
            }
            return false;
        }

        if (isNormalNerveWithoutWaitHack() && al::isSensorName(self, "Head")) {
            rs::requestHitReactionToAttacker(message, self, other);
            al::setNerve(this, &NrvMegane.Trampled);
            return true;
        }
        return false;
    }

    if (rs::isMsgKillByShineGet(message)) {
        if (mHitSensor == self)
            return false;
        if (isNormalNerveWithWaitHack())
            return tryReceiveMsgKillByShineGet(message);
    }

    if (rs::isMsgCapEnableLockOn(message) &&
        (isNormalNerveWithWaitHack() || (al::isNerve(this, &NrvMegane.WaitHack) &&
                                         mStateSwoon->tryReceiveMsgStartLockOn(message)))) {
        mIsHack = true;
        return true;
    }

    if (rs::isMsgCapCancelLockOn(message)) {
        mIsHack = false;
        if (al::isNerve(this, &NrvMegane.WaitHack))
            mStateSwoon->tryReceiveMsgEndSwoon(message);
        return true;
    }

    if (rs::isMsgStartHack(message)) {
        if (isNormalNerveWithoutWaitHack()) {
            prepareHack(self, other);
            al::startAction(this, "Wait");
            al::setNerve(this, &NrvMegane.HackStart);
            return true;
        }

        if (al::isNerve(this, &NrvMegane.WaitHack) &&
            mStateSwoon->tryReceiveMsgStartHack(message)) {
            prepareHack(self, other);
            al::setNerve(this, &NrvMegane.HackStart);
            return true;
        }
    }

    if (rs::isMsgHackMarioCheckpointFlagWarp(message)) {
        endCameraSubjective();
        al::setColliderFilterCollisionParts(this, mCollisionPartsFilterBase);
        rs::tryEndHackStartDemo(mPlayerHack, this);
        rs::endHack(&mPlayerHack);
        al::showModelIfHide(this);
        al::endBgmSituation(this, "WearMeganeGlasses", false);
        al::tryStartVisAnimIfNotPlaying(this, "HackOff");
        al::tryStartMtpAnimIfNotPlaying(this, "HackOff");
        al::tryStartVisAnimIfNotPlayingForAction(this, "CapOff");
        al::setNerve(this, &NrvMegane.WaitHack);
        return true;
    }

    if (rs::isMsgCancelHack(message)) {
        endCameraSubjective();
        al::setColliderFilterCollisionParts(this, mCollisionPartsFilterBase);
        rs::tryEndHackStartDemo(mPlayerHack, this);
        rs::endHack(&mPlayerHack);
        al::showModelIfHide(this);
        al::endBgmSituation(this, "WearMeganeGlasses", false);
        al::setNerve(this, &NrvMegane.WaitHack);
        return true;
    }

    if (rs::isMsgHackMarioDemo(message)) {
        endCameraSubjective();
        al::setColliderFilterCollisionParts(this, mCollisionPartsFilterBase);
        rs::tryEndHackStartDemo(mPlayerHack, this);
        rs::endHack(&mPlayerHack);
        al::hideModelIfShow(this);
        al::hideModelIfShow(al::getSubActor(this, "メガネ"));
        al::setVelocityZero(this);
        al::invalidateClipping(this);
        al::endBgmSituation(this, "WearMeganeGlasses", false);
        al::startAction(this, "Wait");
        al::tryKillEmitterAndParticleAll(this);
        al::setNerve(this, &NrvMegane.Rebirth);
        return true;
    }

    if (rs::isMsgHackMarioDead(message)) {
        endCameraSubjective();
        al::setColliderFilterCollisionParts(this, mCollisionPartsFilterBase);
        rs::tryEndHackStartDemo(mPlayerHack, this);
        rs::endHack(&mPlayerHack);
        al::showModelIfHide(this);
        al::endBgmSituation(this, "WearMeganeGlasses", false);
        al::invalidateClipping(this);
        al::setNerve(this, &NrvMegane.Die);
        return true;
    }

    if (rs::isMsgCancelHackByDokan(message)) {
        endCameraSubjective();
        al::setColliderFilterCollisionParts(this, mCollisionPartsFilterBase);
        rs::tryEndHackStartDemo(mPlayerHack, this);
        rs::endHack(&mPlayerHack);
        al::showModelIfHide(this);
        al::endBgmSituation(this, "WearMeganeGlasses", false);
        al::tryAddRippleLarge(this);
        al::invalidateClipping(this);
        al::setNerve(this, &NrvMegane.Die);
        return true;
    }

    if (rs::isMsgPushToPlayer(message) && mPlayerHack && mHitSensor != self) {
        mPlayerPushReceiver->receivePushMsg(message, other, self, 3.0f);
        return true;
    }
    return false;
}

void Megane::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (mHitSensor == self)
        return;

    if ((al::isNerve(this, &NrvMegane.Wait) || al::isNerve(this, &NrvMegane.Appear) ||
         al::isNerve(this, &NrvMegane.Find) || al::isNerve(this, &NrvMegane.RunAway) ||
         al::isNerve(this, &NrvMegane.Fall) || al::isNerve(this, &NrvMegane.Land) ||
         al::isNerve(this, &NrvMegane.Trampled) || al::isNerve(this, &NrvMegane.WaitHack)) &&
        al::isSensorEnemyBody(self)) {
        if (!rs::sendMsgMeganeAttack(other, self) &&
            !al::sendMsgPushAndKillVelocityToTargetH(this, self, other)) {
            rs::sendMsgPushToPlayer(other, self);
        }
        return;
    }

    if (mPlayerHack &&
        (!isEnableTrample(self, other) || !rs::sendMsgMeganeHackTrample(other, self)) &&
        !rs::sendMsgHackAttackKick(other, self) && !rs::sendMsgHackAttackMapObj(other, self) &&
        !rs::sendMsgMeganeAttack(other, self) &&
        !rs::sendMsgHackerNoReaction(mPlayerHack, other, self)) {
        al::sendMsgPushAndKillVelocityToTargetH(this, self, other);
    }
}

void Megane::updateCollider() {
    sead::Vector3f pushVelocity;
    mPlayerPushReceiver->calcPushedVelocity(&pushVelocity, al::getVelocity(this));

    if (al::isNoCollide(this)) {
        al::getTransPtr(this)->add(pushVelocity);
        getCollider()->onInvalidate();
        mPlayerPushReceiver->clear();
        return;
    }

    if (al::isNerve(this, &NrvMegane.HackStart))
        getCollider()->collide(sead::Vector3f::ey * -10.0f);
    else
        al::getTransPtr(this)->add(getCollider()->collide(pushVelocity));
    mPlayerPushReceiver->clear();
}

void Megane::control() {
    updateDither();
    mNerveKeeper->update();

    if (mPlayerHack && !al::isHideModel(this))
        al::showSilhouetteModelIfHide(this);
    else
        al::hideSilhouetteModelIfShow(this);

    if (mPlayerHack) {
        if (rs::isHackCapSeparateFlying(mPlayerHack))
            al::tryStartVisAnimIfNotPlayingForAction(this, "CapOff");
        else
            al::tryStartVisAnimIfNotPlayingForAction(this, "CapOn");

        if (!al::isNerve(this, &NrvMegane.HackStart) ||
            rs::isHackStartDemoEnterMario(mPlayerHack)) {
            al::tryStartVisAnimIfNotPlaying(this, "HackOff");
            al::tryStartMtpAnimIfNotPlaying(this, "HackOff");
        } else {
            al::tryStartVisAnimIfNotPlaying(this, "HackOn");
            al::tryStartMtpAnimIfNotPlaying(this, "HackOn");
        }
    } else {
        al::tryStartVisAnimIfNotPlaying(this, "HackOff");
        al::tryStartMtpAnimIfNotPlaying(this, "HackOff");
        al::tryStartVisAnimIfNotPlayingForAction(this, "CapOff");
    }

    if (!isNerveDieOrRebirth() &&
        (((al::isCollidedFloorCode(this, "DamageFire") || al::isCollidedFloorCode(this, "Needle") ||
           al::isCollidedFloorCode(this, "Poison")) &&
          !al::isCollidedFloorCode(this, "Needle")) ||
         al::isInDeathArea(this))) {
        if (mPlayerHack) {
            endCameraSubjective();
            al::setColliderFilterCollisionParts(this, mCollisionPartsFilterBase);
            rs::tryEndHackStartDemo(mPlayerHack, this);
            rs::endHack(&mPlayerHack);
            al::showModelIfHide(this);
        }
        al::tryAddRippleLarge(this);
        al::invalidateClipping(this);
        al::setNerve(this, &NrvMegane.Die);
    }

    if (!mPlayerHack) {
        mIsHackTutorialVisible = false;
        return;
    }

    if (al::isInAreaObj(this, "MeganePlayGuideChangeArea")) {
        if (!mIsHackTutorialVisible) {
            rs::changeHackTutorialText(this, "HackEnd");
            mIsHackTutorialVisible = true;
        }
        return;
    }

    if (mIsHackTutorialVisible) {
        rs::resetHackTutorialText(this);
        mIsHackTutorialVisible = false;
    }
}

void Megane::notifyKillSwitch() {
    if (isNerveDieOrRebirth())
        return;

    if (mPlayerHack) {
        endCameraSubjective();
        al::setColliderFilterCollisionParts(this, mCollisionPartsFilterBase);
        rs::tryEndHackStartDemo(mPlayerHack, this);
        rs::endHack(&mPlayerHack);
        al::showModelIfHide(this);
    }

    al::invalidateClipping(this);
    al::setNerve(this, &NrvMegane.Die);
}

bool Megane::isNerveDieOrRebirth() const {
    return al::isNerve(this, &NrvMegane.Die) || al::isNerve(this, &NrvMegane.Rebirth);
}

bool Megane::isNormalNerveWithWaitHack() const {
    return isNormalNerveWithoutWaitHack() || al::isNerve(this, &NrvMegane.WaitHack);
}

bool Megane::isNormalNerveWithoutWaitHack() const {
    if (al::isNerve(this, &NrvMegane.Wait) || al::isNerve(this, &NrvMegane.Appear) ||
        al::isNerve(this, &NrvMegane.Find) || al::isNerve(this, &NrvMegane.Fall) ||
        al::isNerve(this, &NrvMegane.Land) || al::isNerve(this, &NrvMegane.Trampled)) {
        return true;
    }
    return al::isNerve(this, &NrvMegane.RunAway);
}

bool Megane::tryReceiveMsgKillByShineGet(const al::SensorMsg* message) {
    if (rs::isMsgKillByShineGet(message)) {
        al::deleteEffectAll(this);
        al::hideModelIfShow(this);
        al::hideModelIfShow(al::getSubActor(this, "メガネ"));
        al::setVelocityZero(this);
        al::invalidateClipping(this);
        al::startAction(this, "Wait");
        al::resetPosition(this, mStartingPos);
        al::tryKillEmitterAndParticleAll(this);
        al::setNerve(this, &NrvMegane.Rebirth);
        return true;
    }
    return false;
}

void Megane::prepareHack(al::HitSensor* self, al::HitSensor* other) {
    al::invalidateClipping(this);
    mPlayerHack = rs::startHack(self, other, nullptr);
    rs::startHackStartDemo(mPlayerHack, this);
    rs::addHackStartDemoActor(mPlayerHack, al::getSubActor(this, "メガネ"));
}

void Megane::endCameraSubjective() {
    if (al::isActiveCamera(mCameraTicket))
        al::endCamera(this, mCameraTicket, -1, false);
}

bool Megane::isHack() const {
    return mPlayerHack != nullptr;
}

bool Megane::isEnableTrample(al::HitSensor* self, al::HitSensor* other) const {
    if (al::isNerve(this, &NrvMegane.HackFall) && al::isSensorName(self, "Body")) {
        sead::Vector3f dir = al::getSensorPos(other) - al::getSensorPos(self);
        if (al::tryNormalizeOrZero(&dir))
            return 0.0f < dir.dot(al::getGravity(this));
    }
    return false;
}

void Megane::updateMovement() {
    al::reboundVelocityPart(this, 0.0f, 0.0f, 0.0f, 0.0f);
    if (al::isCollidedGround(this))
        al::addVelocityToGravityFittedGround(this, 2.0f, 0);
    else
        al::addVelocityToGravity(this, 2.0f);
    al::scaleVelocity(this, 0.95f);
}

bool Megane::tryShiftFall() {
    if (!al::isOnGround(this, 4)) {
        al::invalidateClipping(this);
        al::setNerve(this, &NrvMegane.Fall);
        return true;
    }
    return false;
}

void Megane::updateRunAwayDirection() {
    sead::Vector3f dirToActor;
    al::calcDirToActorH(&dirToActor, this, al::getPlayerActor(this, 0));
    dirToActor.negate();
    recordWallNormal();

    if (mRunAwayTime != 0) {
        f32 rundot = mWallNormal.dot(dirToActor);
        if (-0.99f > rundot) {
            f32 tmp = -dirToActor.x;
            dirToActor.x = dirToActor.z;
            dirToActor.z = tmp;
        } else if (rundot < 0.0f) {
            al::verticalizeVec(&dirToActor, mWallNormal, dirToActor);
            al::normalize(&dirToActor);
        }
        mRunAwayTime--;
    }
    al::turnToDirection(this, dirToActor, 4.0f);
}

void Megane::shiftWaitOrRunAway() {
    if (al::calcDistanceH(this, al::getPlayerActor(this, 0)) < 1000.0f) {
        al::setNerve(this, &NrvMegane.RunAway);
        return;
    }
    al::setNerve(this, &NrvMegane.Wait);
}

bool Megane::isTriggerStare() const {
    return rs::isTriggerHackAnyButton(mPlayerHack) || rs::isTriggerHackSwing(mPlayerHack);
}

bool Megane::isTriggerSubjective() const {
    return rs::isTriggerSubjectiveCamera(mPlayerHack) && al::isOnGround(this, 0);
}

void Megane::startCameraSubjective() {
    if (al::isActiveCamera(mCameraTicket))
        return;
    sead::Vector3f frontDir;
    al::calcFrontDir(&frontDir, this);
    frontDir.y = 0.0;
    if (al::tryNormalizeOrZero(&frontDir)) {
        f32 angle = sead::Mathf::rad2deg(sead::Mathf::atan2(frontDir.x, frontDir.z));
        mCameraPoserSubjective->setStartAngleH(angle);
    }
    mCameraPoserSubjective->setCameraOffsetUp(100.0f);
    al::startCamera(this, mCameraTicket, -1);
}

void Megane::faceToCameraFront() {
    sead::Vector3f lookDir;
    al::calcCameraLookDir(&lookDir, this, 0);
    lookDir.y = 0.0f;
    if (al::tryNormalizeOrZero(&lookDir))
        al::faceToDirection(this, lookDir);
}

void Megane::tryToggleSpectaclesNerve() {
    if (isTriggerStare()) {
        if (mNerveKeeper->getCurrentNerve() == &NrvMegane.SpectaclesOn) {
            mNerveKeeper->setNerve(&NrvMegane.SpectaclesOff);
            return;
        }
        mNerveKeeper->setNerve(&NrvMegane.SpectaclesOn);
    }
}

void Megane::shiftHackNerveOnGround() {
    if (mNerveKeeper->getCurrentNerve() == &NrvMegane.SpectaclesOn) {
        al::setNerve(this, &NrvMegane.HackSlow);
        return;
    }
    al::setNerve(this, &NrvMegane.Hack);
}

void Megane::putOnSpectaclesForce() {
    if (mNerveKeeper->getCurrentNerve() != &NrvMegane.SpectaclesOn)
        mNerveKeeper->setNerve(&NrvMegane.SpectaclesOn);
    al::LiveActor* subActor = al::getSubActor(this, "メガネ");
    al::startAction(subActor, "SpectaclesOn");
    al::setActionFrame(subActor, al::getActionFrameMax(subActor, "SpectaclesOn") - 1.0f);
}

bool Megane::isWearingGlasses() const {
    return mPlayerHack && mNerveKeeper->getCurrentNerve() == &NrvMegane.SpectaclesOn;
}

void Megane::updateDither() {
    const sead::Vector3f& cameraPos = al::getCameraPos(this, 0);
    sead::Vector3f pos = al::getCameraAt(this, 0) - cameraPos;
    al::tryNormalizeOrZero(&pos);

    sead::Vector3f actorPos = al::getTrans(this) - cameraPos;
    f32 length = actorPos.length();
    al::tryNormalizeOrZero(&actorPos);

    al::LiveActor* subActor = al::getSubActor(this, "メガネ");
    f32 targetAlphaMask = 0.0f;
    if (0.0f < pos.dot(actorPos))
        targetAlphaMask = sead::Mathf::clamp((length - 700.0f + 500.0f) / 500.0f, 0.0f, 1.0f);

    if (mModelAlphaMask > targetAlphaMask)
        mModelAlphaMask = targetAlphaMask;
    else if (targetAlphaMask - mModelAlphaMask > 0.1f)
        mModelAlphaMask += 0.1f;
    else
        mModelAlphaMask = targetAlphaMask;

    al::setModelAlphaMask(this, mModelAlphaMask);
    al::setModelAlphaMask(subActor, mModelAlphaMask);
}

void Megane::recordWallNormal() {
    if (!al::isCollidedWall(this))
        return;

    sead::Vector3f velocity = al::getVelocity(this);
    if (!al::tryNormalizeOrZero(&velocity) || velocity.dot(al::getCollidedWallNormal(this)) >= 0.0f)
        return;

    sead::Vector3f normal = al::getCollidedWallNormal(this);
    normal.y = 0.0f;
    if (!al::tryNormalizeOrZero(&normal))
        return;

    f32 scale = mRunAwayTime / 30.0f;
    normal = normal * (1.0f - scale) + mWallNormal * scale;
    if (al::tryNormalizeOrZero(&normal)) {
        mWallNormal.set(normal);
        mRunAwayTime = 30;
    }
}

void Megane::exeWait() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Wait");
        al::getVelocityPtr(this)->set({0.0f, 0.0f, 0.0f});
    }

    sead::Vector3f frontDir;
    al::calcFrontDir(&frontDir, this);
    bool isFaceToTarget =
        al::isFaceToTargetDegreeH(this, al::getPlayerPos(this, 0), frontDir, 10.0f);
    if (isFaceToTarget) {
        if (mWaitDelay != 0)
            mWaitDelay--;
    } else {
        mWaitDelay = 30;
    }

    if (al::isActionPlaying(this, "Wait")) {
        if (!isFaceToTarget)
            al::startAction(this, "Turn");
    } else {
        sead::Vector3f dirToActor;
        al::calcDirToActorH(&dirToActor, this, al::getPlayerActor(this, 0));
        if (al::turnToDirection(this, dirToActor, 3.0f) && mWaitDelay == 0)
            al::startAction(this, "Wait");
    }

    updateMovement();

    if (al::isGreaterEqualStep(this, 4) && tryShiftFall())
        return;

    if (al::isGreaterEqualStep(this, 30)) {
        if (al::calcDistanceH(this, al::getPlayerActor(this, 0)) < 1000.0f) {
            al::setNerve(this, &NrvMegane.Find);
            return;
        }
    }

    if (mNerveKeeper->getCurrentNerve() != &NrvMegane.SpectaclesOn &&
        al::isActionPlaying(this, "Wait")) {
        mNerveKeeper->setNerve(&NrvMegane.SpectaclesOn);
    }
}

void Megane::exeFind() {
    if (al::isFirstStep(this))
        al::startAction(this, "Find");

    al::scaleVelocity(this, 0.9f);
    if (al::isGreaterEqualStep(this, 2) &&
        mNerveKeeper->getCurrentNerve() != &NrvMegane.SpectaclesOff) {
        mNerveKeeper->setNerve(&NrvMegane.SpectaclesOff);
    }

    if (tryShiftFall())
        return;

    if (al::isActionEnd(this))
        al::setNerve(this, &NrvMegane.RunAway);
}

void Megane::exeRunAway() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "EnemyRun");
        if (mNerveKeeper->getCurrentNerve() != &NrvMegane.SpectaclesOff)
            mNerveKeeper->setNerve(&NrvMegane.SpectaclesOff);
        mRunAwayTime = 0;
    }

    updateRunAwayDirection();

    sead::Vector3f front;
    al::calcQuatFront(&front, this);
    f32 nerveRate = al::calcNerveRate(this, 150, 180);
    al::addVelocityToDirection(this, front, nerveRate * 0.5f + (1.0f - nerveRate) * 0.7f);
    updateMovement();

    if (tryShiftFall())
        return;

    if (al::calcDistanceH(this, al::getPlayerActor(this, 0)) > 1200.0f ||
        al::isGreaterEqualStep(this, 0x168)) {
        al::setNerve(this, &NrvMegane.Wait);
    }
}

void Megane::exeFall() {
    if (al::isFirstStep(this))
        al::startAction(this, "Fall");

    al::scaleVelocity(this, 0.95f);
    al::addVelocityToGravity(this, 2.0f);

    if (al::isOnGround(this, 0)) {
        al::validateClipping(this);
        al::setNerve(this, &NrvMegane.Land);
    }
}

void Megane::exeLand() {
    if (al::isFirstStep(this))
        al::startAction(this, "Land");

    updateMovement();

    if (!al::isOnGroundNoVelocity(this, 4)) {
        al::invalidateClipping(this);
        al::setNerve(this, &NrvMegane.Fall);
        return;
    }

    if (al::isActionEnd(this))
        shiftWaitOrRunAway();
}

void Megane::exeTrampled() {
    if (al::isFirstStep(this))
        al::startAction(this, "Reaction");

    updateMovement();

    if (al::isActionEnd(this))
        shiftWaitOrRunAway();
}

void Megane::exeWaitHack() {
    if (al::isFirstStep(this)) {
        al::getVelocityPtr(this)->set({0.0f, 0.0f, 0.0f});
        mIsHack = false;
        mNerveKeeper->setNerve(&NrvMegane.SpectaclesOff);
        mPlayerPos.set(al::getPlayerPos(this, 0));
    }

    updateMovement();

    if (al::updateNerveState(this) && !mIsHack) {
        al::validateClipping(this);
        shiftWaitOrRunAway();
    }
}

void Megane::exeHackStart() {
    if (al::isFirstStep(this))
        al::setVelocityZero(this);

    if (rs::isHackStartDemoEnterMario(mPlayerHack)) {
        if (!al::isActionPlaying(this, "HackStart")) {
            al::startAction(this, "HackStart");
            mPlayerHackStartShaderCtrl->start();
            return;
        }
        mPlayerHackStartShaderCtrl->update();
        if (al::isActionEnd(this)) {
            mPlayerHackStartShaderCtrl->end();
            rs::endHackStartDemo(mPlayerHack, this);
            al::setNerve(this, &NrvMegane.Hack);
        }
    }
}

void Megane::exeHack() {
    if (al::isFirstStep(this)) {
        al::setColliderFilterCollisionParts(this, nullptr);
        mNerveKeeper->setNerve(&NrvMegane.SpectaclesOff);
        al::endBgmSituation(this, "WearMeganeGlasses", false);
    }

    al::updateNerveState(this);
    if (isTriggerStare() && (u32)mNerveKeeper->getCurrentStep() > 4)
        mNerveKeeper->setNerve(&NrvMegane.SpectaclesOn);

    if (!al::isOnGroundNoVelocity(this, 5)) {
        al::setNerve(this, &NrvMegane.HackFall);
        return;
    }

    if (isTriggerSubjective() && rs::isEnableStartSubjectiveCamera(this)) {
        al::setNerve(this, &NrvMegane.HackSubjective);
        return;
    }

    if (mNerveKeeper->getCurrentNerve() == &NrvMegane.SpectaclesOn)
        al::setNerve(this, &NrvMegane.HackSlow);
}

void Megane::exeHackSlow() {
    if (al::isFirstStep(this))
        al::startBgmSituation(this, "WearMeganeGlasses", false);

    al::holdSe(this, "MoveOverSpectacles");
    al::updateNerveState(this);

    if (isTriggerStare() && 4 < (u32)mNerveKeeper->getCurrentStep())
        mNerveKeeper->setNerve(&NrvMegane.SpectaclesOff);

    if (!al::isOnGroundNoVelocity(this, 5)) {
        al::setNerve(this, &NrvMegane.HackFall);
        return;
    }

    if (isTriggerSubjective() && rs::isEnableStartSubjectiveCamera(this)) {
        al::setNerve(this, &NrvMegane.HackSubjective);
        return;
    }

    if (mNerveKeeper->getCurrentNerve() == &NrvMegane.SpectaclesOff)
        al::setNerve(this, &NrvMegane.Hack);
}

void Megane::exeHackSubjective() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Wait");
        startCameraSubjective();
    }

    if (al::isOnGroundNoVelocity(this, 0))
        al::setVelocityZero(this);
    al::addVelocityToGravityNaturalOrFittedGround(this, 2.0f);

    faceToCameraFront();
    tryToggleSpectaclesNerve();

    if (!al::isOnGroundNoVelocity(this, 5)) {
        endCameraSubjective();
        al::setNerve(this, &NrvMegane.HackFall);
        return;
    }

    if (isTriggerSubjective() || !rs::isEnableStartSubjectiveCamera(this)) {
        endCameraSubjective();
        shiftHackNerveOnGround();
    }
}

void Megane::exeHackFall() {
    if (al::isFirstStep(this))
        al::startAction(this, "Fall");

    al::scaleVelocity(this, 0.95f);
    al::addVelocityToGravity(this, 2.0f);
    tryToggleSpectaclesNerve();

    if (!al::isOnGround(this, 0))
        return;

    if (al::isGreaterEqualStep(this, 15)) {
        al::setNerve(this, &NrvMegane.HackLand);
        return;
    }

    shiftHackNerveOnGround();
}

void Megane::exeHackLand() {
    if (al::isFirstStep(this))
        al::startAction(this, "Land");

    al::scaleVelocity(this, al::isOnGround(this, 0) ? 0.0f : 0.95f);
    al::addVelocityToGravityNaturalOrFittedGround(this, 2.0f);

    tryToggleSpectaclesNerve();

    if (!al::isOnGround(this, 5)) {
        al::setNerve(this, &NrvMegane.HackFall);
        return;
    }

    if (al::isActionEnd(this))
        shiftHackNerveOnGround();
}

void Megane::exeDie() {
    if (al::isFirstStep(this)) {
        al::hideModelIfShow(this);
        al::startHitReaction(this, "死亡");
        al::setVelocityZero(this);
        al::startAction(this, "Wait");
        al::stopAction(this);
        al::endBgmSituation(this, "WearMeganeGlasses", false);
    }

    if (al::isGreaterEqualStep(this, 120))
        al::setNerve(this, &NrvMegane.Rebirth);
}

void Megane::exeRebirth() {
    if (al::isFirstStep(this)) {
        al::setQuat(this, mStartingQuat);
        al::resetPosition(this, mStartingPos);
    }

    if (al::isStep(this, 180))
        al::startHitReaction(this, "出現予兆");

    if (al::isGreaterEqualStep(this, 300)) {
        al::validateClipping(this);
        al::showModelIfHide(this);
        al::showModelIfHide(al::getSubActor(this, "メガネ"));
        al::tryDeleteEffect(this, "AppearSign");
        al::startHitReaction(this, "出現");
        al::faceToPlayer(this);
        al::setNerve(this, &NrvMegane.Appear);
    }
}

void Megane::exeAppear() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Appear");
        al::getVelocityPtr(this)->set({0.0f, 0.0f, 0.0f});
        if (mNerveKeeper->getCurrentNerve() != &NrvMegane.SpectaclesOn)
            mNerveKeeper->setNerve(&NrvMegane.SpectaclesOn);
        al::LiveActor* subActor = al::getSubActor(this, "メガネ");
        al::startAction(subActor, "SpectaclesOn");
        al::setActionFrame(subActor, al::getActionFrameMax(subActor, "SpectaclesOn") - 1.0f);
    }

    updateMovement();

    if (al::isGreaterEqualStep(this, 4) && tryShiftFall())
        return;

    if (al::isActionEnd(this))
        al::setNerve(this, &NrvMegane.Wait);
}

void Megane::exeSpectaclesOff() {
    if (mNerveKeeper->getCurrentStep() == 0 && mPlayerHack)
        al::startHitReaction(this, "メガネを外した");

    al::LiveActor* subActor = al::tryGetSubActor(this, "メガネ");

    if (!mTestFilterGlasses->isEnd()) {
        mTestFilterGlasses->end();
        if (subActor)
            al::startSe(subActor, "SpectaclesOff");
    }

    if (subActor) {
        if (mPlayerHack && !rs::isHackCapSeparateFlying(mPlayerHack)) {
            if (!al::isActionPlaying(subActor, "SpectaclesOff")) {
                bool isActionPlaying = al::isActionPlaying(subActor, "SpectaclesOffEnemy");
                al::startAction(subActor, "SpectaclesOff");
                if (isActionPlaying)
                    al::setActionFrame(subActor, al::getActionFrameMax(this) - 1.0f);
            }
            return;
        }

        if (!al::isActionPlaying(subActor, "SpectaclesOffEnemy")) {
            bool isActionPlaying = al::isActionPlaying(subActor, "SpectaclesOff");
            al::startAction(subActor, "SpectaclesOffEnemy");
            if (isActionPlaying)
                al::setActionFrame(subActor, al::getActionFrameMax(this) - 1.0f);
        }
    }
}

void Megane::exeSpectaclesOn() {
    if (mNerveKeeper->getCurrentStep() == 0 && mPlayerHack)
        al::startHitReaction(this, "メガネをかけた");

    al::LiveActor* subActor = al::tryGetSubActor(this, "メガネ");
    if (mPlayerHack) {
        if (!mTestFilterGlasses->isAlive()) {
            mTestFilterGlasses->startAppear();
            if (subActor)
                al::startSe(subActor, "SpectaclesOn");
        }
    } else if (!mTestFilterGlasses->isEnd()) {
        mTestFilterGlasses->end();
    }

    mGlassFilterDelay++;
    if (subActor && !al::isActionPlaying(subActor, "SpectaclesOn")) {
        al::startAction(subActor, "SpectaclesOn");
        mGlassFilterDelay = 0;
    }
}
