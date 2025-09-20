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
    NERVES_MAKE_STRUCT(Megane, Wait, WaitHack, Hack, HackSlow, SpectaclesOn, Trampled, HackStart,
                       Rebirth, Die, Appear, Find, RunAway, Fall, Land, SpectaclesOff, HackFall,
                       HackSubjective, HackLand)

    PlayerHackStartShaderParam playerHackStartShaderParam =
        PlayerHackStartShaderParam(false, 400.0f, 10, 20);

    MeganeJointProperties jointProperties[4] = {
        {{200.0f, 0.0f, 0.0f}, 0.025f, 0.9f, 10.0f},
        {{200.0f, 0.0f, 0.0f}, 0.025f, 0.9f, 10.0f},
        {{0.0f, -50.0f, -50.0f}, 0.1f, 0.95f, 30.0f},
        {{0.0f, -50.0f, 50.0f}, 0.1f, 0.95f, 30.0f},
    };
} MeganeData;

}  // namespace

Megane::Megane(const char* name) : al::LiveActor(name) {}

void Megane::init(const al::ActorInitInfo& info) {
    using MeganeFunctor = al::FunctorV0M<Megane*, void (Megane::*)()>;

    al::initActorWithArchiveName(this, info, "Megane", nullptr);
    al::initNerve(this, &MeganeData.NrvMegane.Wait, 3);
    mStateSwoon = new EnemyStateSwoon(this, "SwoonStart", "Swoon", "SwoonEnd", false, true);
    mStateSwoon->initParams(180, nullptr);
    al::initNerveState(this, mStateSwoon, &MeganeData.NrvMegane.WaitHack, "気絶");

    mHackerState = new HackerStateNormalMove(this, &mPlayerHack, "Wait", "HackRun", "Fall");
    mHackerState->set_54(2.0f);
    mHackerState->set_5c(0.9f);
    mHackerState->setMoveVelocity(0.8f);
    mHackerState->set_49(true);
    mHackerState->setTurnAngle(30.0f);
    mHackerState->usePlayerTurnControl(true);
    mHackerState->getTurnControl()->setup(3.0f, 55.0f, 8.0f, 8.0f, 20, 1, 30);
    al::initNerveState(this, mHackerState, &MeganeData.NrvMegane.Hack, "憑依");

    mHackerStateSpectacles = new HackerStateNormalMove(this, &mPlayerHack, "Wait", "Walk", "Fall");
    mHackerStateSpectacles->set_54(1.2f);
    mHackerStateSpectacles->setTurnAngle(30.0f);
    al::initNerveState(this, mHackerStateSpectacles, &MeganeData.NrvMegane.HackSlow, "憑依メガネ");

    mPlayerPushReceiver = new PlayerPushReceiver(this);
    if (!al::trySyncStageSwitchAppear(this))
        makeActorAlive();

    al::listenStageSwitchOnKill(this, MeganeFunctor(this, &Megane::notifyKillSwitch));
    mCollisionPartsFilterMoveLimit = new al::CollisionPartsFilterSpecialPurpose("MoveLimit");
    al::setColliderFilterCollisionParts(this, mCollisionPartsFilterMoveLimit);

    al::LiveActor* subActor = al::getSubActor(this, "メガネ");
    al::startAction(subActor, "SpectaclesOn");
    al::setActionFrame(subActor, al::getActionFrameMax(subActor, "SpectaclesOn") - 1.0f);

    mTestFilterGlasses =
        new TestFilterGlasses("メガネーレイアウト", al::getLayoutInitInfo(info), nullptr);
    mStartingQuat.set(al::getQuat(this));
    mStartingPos.set(al::getTrans(this));

    mCapTargetInfo = rs::createCapTargetInfo(this, nullptr);
    mCameraPoserSubjective = new al::CameraPoserSubjective("主観");
    mCameraTicket = alCameraFunction::initCamera(mCameraPoserSubjective, this, info, nullptr, 9);
    mSpectaclesNerveKeeper = new al::NerveKeeper(this, &MeganeData.NrvMegane.SpectaclesOn, 0);

    mPlayerHackStartShaderCtrl =
        new PlayerHackStartShaderCtrl(this, &MeganeData.playerHackStartShaderParam);

    al::initJointControllerKeeper(this, 4);

    const char* jointNames[4] = {"Grass1", "Grass2", "EarringL", "EarringR"};

    for (s32 i = 0; i < 4; i++) {
        al::JointSpringController* jointController =
            al::initJointSpringController(this, jointNames[i]);
        jointController->setChildLocalPos(MeganeData.jointProperties[i].position);
        jointController->setStability(MeganeData.jointProperties[i].stability);
        jointController->setFriction(MeganeData.jointProperties[i].friction);
        jointController->setLimitDegree(MeganeData.jointProperties[i].limitDegree);
        al::StringTmp<128>("ダイナミクス[%s]", jointNames[i]).cstr();
    }

    mCapReceiverSensor = al::getHitSensor(this, "CapReceiver");
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

    f32 geyserRaiseDistance = 0.0f;
    f32 geyserRaiseSpeed = 0.0f;
    if (rs::tryGetSandGeyserRaise(message, &geyserRaiseDistance, &geyserRaiseSpeed)) {
        if (self == mCapReceiverSensor)
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

    if (self != mCapReceiverSensor && isNormalNerveWithWaitHack() &&
        rs::tryReceiveMsgNpcScareByEnemyIgnoreTargetHack(message, mCapTargetInfo))
        return true;

    if (al::isMsgPushAll(message)) {
        if (self == mCapReceiverSensor)
            return false;
        if (isNormalNerveWithWaitHack())
            return al::tryReceiveMsgPushAndAddVelocityH(this, message, other, self, 3.0f);
    }

    if (al::isMsgPlayerTrampleReflect(message)) {
        if (self == mCapReceiverSensor)
            return false;

        if (al::isNerve(this, &MeganeData.NrvMegane.WaitHack)) {
            if (mStateSwoon->tryReceiveMsgTrampleReflect(message)) {
                rs::requestHitReactionToAttacker(message, self, other);
                return true;
            }
            return false;
        }

        if (isNormalNerveWithoutWaitHack() && al::isSensorName(self, "Head")) {
            rs::requestHitReactionToAttacker(message, self, other);
            al::setNerve(this, &MeganeData.NrvMegane.Trampled);
            return true;
        }
        return false;
    }

    if (rs::isMsgThrowObjHitReflect(message) || rs::isMsgSphinxRideAttackReflect(message)) {
        rs::requestHitReactionToAttacker(message, self, other);
        if (isNormalNerveWithoutWaitHack())
            al::setNerve(this, &MeganeData.NrvMegane.Trampled);
        return true;
    }

    if (rs::isMsgPlayerAndCapObjHipDropReflectAll(message)) {
        if (self == mCapReceiverSensor)
            return false;

        if (al::isNerve(this, &MeganeData.NrvMegane.WaitHack)) {
            if (mStateSwoon->tryReceiveMsgObjHipDropReflect(message)) {
                rs::requestHitReactionToAttacker(message, self, other);
                return true;
            }
            return false;
        }

        if (isNormalNerveWithoutWaitHack() && al::isSensorName(self, "Head")) {
            rs::requestHitReactionToAttacker(message, self, other);
            al::setNerve(this, &MeganeData.NrvMegane.Trampled);
            return true;
        }
        return false;
    }

    if (rs::isMsgKillByShineGet(message)) {
        if (self == mCapReceiverSensor)
            return false;
        if (isNormalNerveWithWaitHack())
            return tryReceiveMsgKillByShineGet(message);
    }

    if (rs::isMsgCapEnableLockOn(message) &&
        (isNormalNerveWithWaitHack() || (al::isNerve(this, &MeganeData.NrvMegane.WaitHack) &&
                                         mStateSwoon->tryReceiveMsgStartLockOn(message)))) {
        mIsHack = true;
        return true;
    }

    if (rs::isMsgCapCancelLockOn(message)) {
        mIsHack = false;
        if (al::isNerve(this, &MeganeData.NrvMegane.WaitHack))
            mStateSwoon->tryReceiveMsgEndSwoon(message);
        return true;
    }

    if (rs::isMsgStartHack(message)) {
        if (isNormalNerveWithoutWaitHack()) {
            prepareHack(self, other);
            al::startAction(this, "Wait");
            al::setNerve(this, &MeganeData.NrvMegane.HackStart);
            return true;
        }

        if (al::isNerve(this, &MeganeData.NrvMegane.WaitHack) &&
            mStateSwoon->tryReceiveMsgStartHack(message)) {
            prepareHack(self, other);
            al::setNerve(this, &MeganeData.NrvMegane.HackStart);
            return true;
        }
    }

    if (rs::isMsgHackMarioCheckpointFlagWarp(message)) {
        endCameraSubjective();
        al::setColliderFilterCollisionParts(this, mCollisionPartsFilterMoveLimit);
        rs::tryEndHackStartDemo(mPlayerHack, this);
        rs::endHack(&mPlayerHack);
        al::showModelIfHide(this);
        al::endBgmSituation(this, "WearMeganeGlasses", false);
        al::tryStartVisAnimIfNotPlaying(this, "HackOff");
        al::tryStartMtpAnimIfNotPlaying(this, "HackOff");
        al::tryStartVisAnimIfNotPlayingForAction(this, "CapOff");
        al::setNerve(this, &MeganeData.NrvMegane.WaitHack);
        return true;
    }

    if (rs::isMsgCancelHack(message)) {
        endCameraSubjective();
        al::setColliderFilterCollisionParts(this, mCollisionPartsFilterMoveLimit);
        rs::tryEndHackStartDemo(mPlayerHack, this);
        rs::endHack(&mPlayerHack);
        al::showModelIfHide(this);
        al::endBgmSituation(this, "WearMeganeGlasses", false);
        al::setNerve(this, &MeganeData.NrvMegane.WaitHack);
        return true;
    }

    if (rs::isMsgHackMarioDemo(message)) {
        endCameraSubjective();
        al::setColliderFilterCollisionParts(this, mCollisionPartsFilterMoveLimit);
        rs::tryEndHackStartDemo(mPlayerHack, this);
        rs::endHack(&mPlayerHack);
        al::hideModelIfShow(this);
        al::hideModelIfShow(al::getSubActor(this, "メガネ"));
        al::setVelocityZero(this);
        al::invalidateClipping(this);
        al::endBgmSituation(this, "WearMeganeGlasses", false);
        al::startAction(this, "Wait");
        al::tryKillEmitterAndParticleAll(this);
        al::setNerve(this, &MeganeData.NrvMegane.Rebirth);
        return true;
    }

    if (rs::isMsgHackMarioDead(message)) {
        endCameraSubjective();
        al::setColliderFilterCollisionParts(this, mCollisionPartsFilterMoveLimit);
        rs::tryEndHackStartDemo(mPlayerHack, this);
        rs::endHack(&mPlayerHack);
        al::showModelIfHide(this);
        al::endBgmSituation(this, "WearMeganeGlasses", false);
        al::invalidateClipping(this);
        al::setNerve(this, &MeganeData.NrvMegane.Die);
        return true;
    }

    if (rs::isMsgCancelHackByDokan(message)) {
        endCameraSubjective();
        al::setColliderFilterCollisionParts(this, mCollisionPartsFilterMoveLimit);
        rs::tryEndHackStartDemo(mPlayerHack, this);
        rs::endHack(&mPlayerHack);
        al::showModelIfHide(this);
        al::endBgmSituation(this, "WearMeganeGlasses", false);
        al::tryAddRippleLarge(this);
        al::invalidateClipping(this);
        al::setNerve(this, &MeganeData.NrvMegane.Die);
        return true;
    }

    if (rs::isMsgPushToPlayer(message) && isHack() && self != mCapReceiverSensor) {
        mPlayerPushReceiver->receivePushMsg(message, other, self, 3.0f);
        return true;
    }
    return false;
}

void Megane::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (self == mCapReceiverSensor)
        return;

    if ((al::isNerve(this, &MeganeData.NrvMegane.Wait) ||
         al::isNerve(this, &MeganeData.NrvMegane.Appear) ||
         al::isNerve(this, &MeganeData.NrvMegane.Find) ||
         al::isNerve(this, &MeganeData.NrvMegane.RunAway) ||
         al::isNerve(this, &MeganeData.NrvMegane.Fall) ||
         al::isNerve(this, &MeganeData.NrvMegane.Land) ||
         al::isNerve(this, &MeganeData.NrvMegane.Trampled) ||
         al::isNerve(this, &MeganeData.NrvMegane.WaitHack)) &&
        al::isSensorEnemyBody(self)) {
        if (rs::sendMsgMeganeAttack(other, self))
            return;
        if (al::sendMsgPushAndKillVelocityToTargetH(this, self, other))
            return;
        rs::sendMsgPushToPlayer(other, self);
        return;
    }

    if (isHack()) {
        if (isEnableTrample(self, other) && rs::sendMsgMeganeHackTrample(other, self))
            return;
        if (rs::sendMsgHackAttackKick(other, self))
            return;
        if (rs::sendMsgHackAttackMapObj(other, self))
            return;
        if (rs::sendMsgMeganeAttack(other, self))
            return;
        if (rs::sendMsgHackerNoReaction(mPlayerHack, other, self))
            return;
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

    if (al::isNerve(this, &MeganeData.NrvMegane.HackStart))
        getCollider()->collide(sead::Vector3f::ey * -10.0f);
    else
        al::getTransPtr(this)->add(getCollider()->collide(pushVelocity));
    mPlayerPushReceiver->clear();
}

void Megane::control() {
    updateDither();
    mSpectaclesNerveKeeper->update();

    if (isHack() && !al::isHideModel(this))
        al::showSilhouetteModelIfHide(this);
    else
        al::hideSilhouetteModelIfShow(this);

    if (isHack()) {
        if (rs::isHackCapSeparateFlying(mPlayerHack))
            al::tryStartVisAnimIfNotPlayingForAction(this, "CapOff");
        else
            al::tryStartVisAnimIfNotPlayingForAction(this, "CapOn");

        if (!al::isNerve(this, &MeganeData.NrvMegane.HackStart) ||
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
        if (isHack()) {
            endCameraSubjective();
            al::setColliderFilterCollisionParts(this, mCollisionPartsFilterMoveLimit);
            rs::tryEndHackStartDemo(mPlayerHack, this);
            rs::endHack(&mPlayerHack);
            al::showModelIfHide(this);
        }
        al::tryAddRippleLarge(this);
        al::invalidateClipping(this);
        al::setNerve(this, &MeganeData.NrvMegane.Die);
    }

    if (!isHack()) {
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

    if (isHack()) {
        endCameraSubjective();
        al::setColliderFilterCollisionParts(this, mCollisionPartsFilterMoveLimit);
        rs::tryEndHackStartDemo(mPlayerHack, this);
        rs::endHack(&mPlayerHack);
        al::showModelIfHide(this);
    }

    al::invalidateClipping(this);
    al::setNerve(this, &MeganeData.NrvMegane.Die);
}

bool Megane::isNerveDieOrRebirth() const {
    return al::isNerve(this, &MeganeData.NrvMegane.Die) ||
           al::isNerve(this, &MeganeData.NrvMegane.Rebirth);
}

bool Megane::isNormalNerveWithWaitHack() const {
    return isNormalNerveWithoutWaitHack() || al::isNerve(this, &MeganeData.NrvMegane.WaitHack);
}

bool Megane::isNormalNerveWithoutWaitHack() const {
    return al::isNerve(this, &MeganeData.NrvMegane.Wait) ||
           al::isNerve(this, &MeganeData.NrvMegane.Appear) ||
           al::isNerve(this, &MeganeData.NrvMegane.Find) ||
           al::isNerve(this, &MeganeData.NrvMegane.Fall) ||
           al::isNerve(this, &MeganeData.NrvMegane.Land) ||
           al::isNerve(this, &MeganeData.NrvMegane.Trampled) ||
           al::isNerve(this, &MeganeData.NrvMegane.RunAway);
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
        al::setNerve(this, &MeganeData.NrvMegane.Rebirth);
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
    if (al::isNerve(this, &MeganeData.NrvMegane.HackFall) && al::isSensorName(self, "Body")) {
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
        al::setNerve(this, &MeganeData.NrvMegane.Fall);
        return true;
    }
    return false;
}

void Megane::updateRunAwayDirection() {
    sead::Vector3f runAwayDirection;
    al::calcDirToActorH(&runAwayDirection, this, al::getPlayerActor(this, 0));
    runAwayDirection.negate();
    recordWallNormal();

    if (mRunAwayWallTime != 0) {
        f32 rundot = mWallNormal.dot(runAwayDirection);
        if (rundot < -0.99f) {
            f32 tmp = -runAwayDirection.x;
            runAwayDirection.x = runAwayDirection.z;
            runAwayDirection.z = tmp;
        } else if (rundot < 0.0f) {
            al::verticalizeVec(&runAwayDirection, mWallNormal, runAwayDirection);
            al::normalize(&runAwayDirection);
        }
        mRunAwayWallTime--;
    }
    al::turnToDirection(this, runAwayDirection, 4.0f);
}

void Megane::shiftWaitOrRunAway() {
    if (al::calcDistanceH(this, al::getPlayerActor(this, 0)) < 1000.0f)
        al::setNerve(this, &MeganeData.NrvMegane.RunAway);
    else
        al::setNerve(this, &MeganeData.NrvMegane.Wait);
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
        if (mSpectaclesNerveKeeper->getCurrentNerve() == &MeganeData.NrvMegane.SpectaclesOn)
            mSpectaclesNerveKeeper->setNerve(&MeganeData.NrvMegane.SpectaclesOff);
        else
            mSpectaclesNerveKeeper->setNerve(&MeganeData.NrvMegane.SpectaclesOn);
    }
}

void Megane::shiftHackNerveOnGround() {
    if (mSpectaclesNerveKeeper->getCurrentNerve() == &MeganeData.NrvMegane.SpectaclesOn)
        al::setNerve(this, &MeganeData.NrvMegane.HackSlow);
    else
        al::setNerve(this, &MeganeData.NrvMegane.Hack);
}

void Megane::putOnSpectaclesForce() {
    if (mSpectaclesNerveKeeper->getCurrentNerve() != &MeganeData.NrvMegane.SpectaclesOn)
        mSpectaclesNerveKeeper->setNerve(&MeganeData.NrvMegane.SpectaclesOn);
    al::LiveActor* subActor = al::getSubActor(this, "メガネ");
    al::startAction(subActor, "SpectaclesOn");
    al::setActionFrame(subActor, al::getActionFrameMax(subActor, "SpectaclesOn") - 1.0f);
}

bool Megane::isWearingGlasses() const {
    return isHack() &&
           mSpectaclesNerveKeeper->getCurrentNerve() == &MeganeData.NrvMegane.SpectaclesOn;
}

void Megane::updateDither() {
    const sead::Vector3f& cameraPos = al::getCameraPos(this, 0);
    sead::Vector3f cameraDir = al::getCameraAt(this, 0) - cameraPos;
    al::tryNormalizeOrZero(&cameraDir);

    sead::Vector3f actorDir = al::getTrans(this) - cameraPos;
    f32 actorDistance = actorDir.length();
    al::tryNormalizeOrZero(&actorDir);

    al::LiveActor* subActor = al::getSubActor(this, "メガネ");
    f32 targetAlphaMask = 0.0f;
    if (cameraDir.dot(actorDir) > 0.0f)
        targetAlphaMask =
            sead::Mathf::clamp((actorDistance - 700.0f + 500.0f) / 500.0f, 0.0f, 1.0f);

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

    f32 lerpFactor = mRunAwayWallTime / 30.0f;
    normal = normal * (1.0f - lerpFactor) + mWallNormal * lerpFactor;
    if (al::tryNormalizeOrZero(&normal)) {
        mWallNormal.set(normal);
        mRunAwayWallTime = 30;
    }
}

void Megane::exeWait() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Wait");
        al::getVelocityPtr(this)->set({0.0f, 0.0f, 0.0f});
    }

    sead::Vector3f frontDir;
    al::calcFrontDir(&frontDir, this);
    bool isFacePlayer = al::isFaceToTargetDegreeH(this, al::getPlayerPos(this, 0), frontDir, 10.0f);
    if (isFacePlayer) {
        if (mWaitDelay != 0)
            mWaitDelay--;
    } else {
        mWaitDelay = 30;
    }

    if (al::isActionPlaying(this, "Wait")) {
        if (!isFacePlayer)
            al::startAction(this, "Turn");
    } else {
        sead::Vector3f playerDirection;
        al::calcDirToActorH(&playerDirection, this, al::getPlayerActor(this, 0));
        if (al::turnToDirection(this, playerDirection, 3.0f) && mWaitDelay == 0)
            al::startAction(this, "Wait");
    }

    updateMovement();

    if (al::isGreaterEqualStep(this, 4) && tryShiftFall())
        return;

    if (al::isGreaterEqualStep(this, 30)) {
        if (al::calcDistanceH(this, al::getPlayerActor(this, 0)) < 1000.0f) {
            al::setNerve(this, &MeganeData.NrvMegane.Find);
            return;
        }
    }

    if (mSpectaclesNerveKeeper->getCurrentNerve() != &MeganeData.NrvMegane.SpectaclesOn &&
        al::isActionPlaying(this, "Wait")) {
        mSpectaclesNerveKeeper->setNerve(&MeganeData.NrvMegane.SpectaclesOn);
    }
}

void Megane::exeFind() {
    if (al::isFirstStep(this))
        al::startAction(this, "Find");

    al::scaleVelocity(this, 0.9f);
    if (al::isGreaterEqualStep(this, 2) &&
        mSpectaclesNerveKeeper->getCurrentNerve() != &MeganeData.NrvMegane.SpectaclesOff) {
        mSpectaclesNerveKeeper->setNerve(&MeganeData.NrvMegane.SpectaclesOff);
    }

    if (tryShiftFall())
        return;

    if (al::isActionEnd(this))
        al::setNerve(this, &MeganeData.NrvMegane.RunAway);
}

void Megane::exeRunAway() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "EnemyRun");
        if (mSpectaclesNerveKeeper->getCurrentNerve() != &MeganeData.NrvMegane.SpectaclesOff)
            mSpectaclesNerveKeeper->setNerve(&MeganeData.NrvMegane.SpectaclesOff);
        mRunAwayWallTime = 0;
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
        al::isGreaterEqualStep(this, 360)) {
        al::setNerve(this, &MeganeData.NrvMegane.Wait);
    }
}

void Megane::exeFall() {
    if (al::isFirstStep(this))
        al::startAction(this, "Fall");

    al::scaleVelocity(this, 0.95f);
    al::addVelocityToGravity(this, 2.0f);

    if (al::isOnGround(this, 0)) {
        al::validateClipping(this);
        al::setNerve(this, &MeganeData.NrvMegane.Land);
    }
}

void Megane::exeLand() {
    if (al::isFirstStep(this))
        al::startAction(this, "Land");

    updateMovement();

    if (!al::isOnGroundNoVelocity(this, 4)) {
        al::invalidateClipping(this);
        al::setNerve(this, &MeganeData.NrvMegane.Fall);
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
        mSpectaclesNerveKeeper->setNerve(&MeganeData.NrvMegane.SpectaclesOff);
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
            al::setNerve(this, &MeganeData.NrvMegane.Hack);
        }
    }
}

void Megane::exeHack() {
    if (al::isFirstStep(this)) {
        al::setColliderFilterCollisionParts(this, nullptr);
        mSpectaclesNerveKeeper->setNerve(&MeganeData.NrvMegane.SpectaclesOff);
        al::endBgmSituation(this, "WearMeganeGlasses", false);
    }

    al::updateNerveState(this);
    if (isTriggerStare() && (u32)mSpectaclesNerveKeeper->getCurrentStep() > 4)
        mSpectaclesNerveKeeper->setNerve(&MeganeData.NrvMegane.SpectaclesOn);

    if (!al::isOnGroundNoVelocity(this, 5)) {
        al::setNerve(this, &MeganeData.NrvMegane.HackFall);
        return;
    }

    if (isTriggerSubjective() && rs::isEnableStartSubjectiveCamera(this)) {
        al::setNerve(this, &MeganeData.NrvMegane.HackSubjective);
        return;
    }

    if (mSpectaclesNerveKeeper->getCurrentNerve() == &MeganeData.NrvMegane.SpectaclesOn)
        al::setNerve(this, &MeganeData.NrvMegane.HackSlow);
}

void Megane::exeHackSlow() {
    if (al::isFirstStep(this))
        al::startBgmSituation(this, "WearMeganeGlasses", false);

    al::holdSe(this, "MoveOverSpectacles");
    al::updateNerveState(this);

    if (isTriggerStare() && (u32)mSpectaclesNerveKeeper->getCurrentStep() > 4)
        mSpectaclesNerveKeeper->setNerve(&MeganeData.NrvMegane.SpectaclesOff);

    if (!al::isOnGroundNoVelocity(this, 5)) {
        al::setNerve(this, &MeganeData.NrvMegane.HackFall);
        return;
    }

    if (isTriggerSubjective() && rs::isEnableStartSubjectiveCamera(this)) {
        al::setNerve(this, &MeganeData.NrvMegane.HackSubjective);
        return;
    }

    if (mSpectaclesNerveKeeper->getCurrentNerve() == &MeganeData.NrvMegane.SpectaclesOff)
        al::setNerve(this, &MeganeData.NrvMegane.Hack);
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
        al::setNerve(this, &MeganeData.NrvMegane.HackFall);
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

    if (al::isOnGround(this, 0)) {
        if (al::isGreaterEqualStep(this, 15))
            al::setNerve(this, &MeganeData.NrvMegane.HackLand);
        else
            shiftHackNerveOnGround();
    }
}

void Megane::exeHackLand() {
    if (al::isFirstStep(this))
        al::startAction(this, "Land");

    al::scaleVelocity(this, al::isOnGround(this, 0) ? 0.0f : 0.95f);
    al::addVelocityToGravityNaturalOrFittedGround(this, 2.0f);

    tryToggleSpectaclesNerve();

    if (!al::isOnGround(this, 5)) {
        al::setNerve(this, &MeganeData.NrvMegane.HackFall);
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
        al::setNerve(this, &MeganeData.NrvMegane.Rebirth);
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
        al::setNerve(this, &MeganeData.NrvMegane.Appear);
    }
}

void Megane::exeAppear() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Appear");
        al::getVelocityPtr(this)->set({0.0f, 0.0f, 0.0f});
        putOnSpectaclesForce();
    }

    updateMovement();

    if (al::isGreaterEqualStep(this, 4) && tryShiftFall())
        return;

    if (al::isActionEnd(this))
        al::setNerve(this, &MeganeData.NrvMegane.Wait);
}

void Megane::exeSpectaclesOff() {
    if (mSpectaclesNerveKeeper->getCurrentStep() == 0 && isHack())
        al::startHitReaction(this, "メガネを外した");

    al::LiveActor* subActor = al::tryGetSubActor(this, "メガネ");

    if (!mTestFilterGlasses->isEnd()) {
        mTestFilterGlasses->end();
        if (subActor)
            al::startSe(subActor, "SpectaclesOff");
    }

    if (subActor) {
        if (isHack() && !rs::isHackCapSeparateFlying(mPlayerHack)) {
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
    if (mSpectaclesNerveKeeper->getCurrentStep() == 0 && isHack())
        al::startHitReaction(this, "メガネをかけた");

    al::LiveActor* subActor = al::tryGetSubActor(this, "メガネ");
    if (isHack()) {
        if (!mTestFilterGlasses->isAlive()) {
            mTestFilterGlasses->startAppear();
            if (subActor)
                al::startSe(subActor, "SpectaclesOn");
        }
    } else if (!mTestFilterGlasses->isEnd()) {
        mTestFilterGlasses->end();
    }

    mSpectaclesOnCounter++;
    if (subActor && !al::isActionPlaying(subActor, "SpectaclesOn")) {
        al::startAction(subActor, "SpectaclesOn");
        mSpectaclesOnCounter = 0;
    }
}
