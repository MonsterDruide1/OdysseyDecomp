#include "Enemy/KaronWingStateHack.h"

#include "Library/Base/Macros.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Se/SeFunction.h"
#include "Library/Shadow/ActorShadowUtil.h"

#include "Enemy/EnemyStateHackStart.h"
#include "Enemy/HackerDepthShadowMapCtrl.h"
#include "Player/CollisionShapeInfo.h"
#include "Player/CollisionShapeKeeper.h"
#include "Player/HackerJudge.h"
#include "Player/HackerStateWingFly.h"
#include "Player/PlayerActionTurnControl.h"
#include "Player/PlayerCollider.h"
#include "Util/Hack.h"
#include "Util/JudgeUtil.h"
#include "Util/PlayerCollisionUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_HOST_TYPE_IMPL(KaronWingStateHack, Wait);
NERVE_HOST_TYPE_IMPL(KaronWingStateHack, WingFly);
NERVE_HOST_TYPE_IMPL(KaronWingStateHack, Cancel);
NERVE_HOST_TYPE_IMPL(KaronWingStateHack, Walk);
NERVE_HOST_TYPE_IMPL(KaronWingStateHack, Reset);
NERVE_HOST_TYPE_IMPL(KaronWingStateHack, Damage);
NERVE_HOST_TYPE_IMPL(KaronWingStateHack, Land);

NERVES_MAKE_STRUCT(HostType, Wait, WingFly, Cancel, Walk, Reset, Damage, Land);
}  // namespace

const sead::Vector3f gHead{0.0f, 160.0f, 30.0f};

// TODO: This function might be implemented in another file. Parameters need to be renamed.
ALWAYS_INLINE void createLegCollider(CollisionShapeKeeper* collisionShapeKeeper,
                                     const sead::Vector3f& pos, f32 valA, f32 valB, f32 radius) {
    const char* bodyParts[] = {"LegFront", "LegLeft", "LegRight"};
    sead::Vector3f up = sead::Vector3f::ey;
    sead::Vector3f upDirA = valA * up;
    sead::Vector3f upDirB = valB * up;
    sead::Vector3f axis = radius * sead::Vector3f::ez;

    for (s32 i = 0; i < 3; i++) {
        sead::Quatf quat;
        sead::QuatCalcCommon<f32>::setAxisAngle(quat, sead::Vector3f::ey, i * 120.0f);

        sead::Vector3f start;
        start.setRotated(quat, axis);
        start += pos;
        start -= upDirA;

        collisionShapeKeeper->createShapeArrow(bodyParts[i], start, upDirA - upDirB, 20.0f, i);
    }
}

KaronWingStateHack::KaronWingStateHack(al::LiveActor* parent, const al::ActorInitInfo& info,
                                       IUsePlayerHack** playerHack)
    : ActorStateBase("ハック状態", parent), mPlayerHack(playerHack) {
    mPlayerActionTurnControl = new PlayerActionTurnControl(mActor);
    mPlayerActionTurnControl->set_88(true);
    mPlayerActionTurnControl->set_89(true);
    mPlayerActionTurnControl->setup(1.0f, 135.0f, 10.0f, 25.0f, 20, 1, 10);
    initNerve(&NrvHostType.Wait, 8);
    mStateWingFly = new HackerStateWingFly(mActor, mPlayerHack, this);

    HackerStateWingFly::HackerStateWingFlyParam flyParam{
        .actionFly = "HackFly",
        .actionFall = "Fall",
        .actionTrample = "AttackTrample",
        .actionUpperPunch = "AttackUp",
        .defaultAccel = 1.0f,
        .swingAccel = 1.5f,
        .defaultVelocityY = 15.0f,
        .swingVelocityY = 23.0f,
        .defaultFramerate = 1.6f,
        .swingFramerate = 2.3f,
        .yOvershootMax = 230.0f,
        .gravity = 0.6f,
        .turnAngle = 15.0f,
        .flyStartLerpTime = 0.1f,
    };
    mStateWingFly->setFlyParam(flyParam);
    al::initNerveState(this, mStateWingFly, &NrvHostType.WingFly, "状態:はばたき");
    al::hideSilhouetteModelIfShow(mActor);
    mCollisionShapeKeeper = new CollisionShapeKeeper(5, 64, 16);
    mCollisionShapeKeeper->set54(35.0f);
    mCollisionShapeKeeper->createShapeSphere("Body", 70.0f, 90.0f * sead::Vector3f::ey);

    CollisionShapeInfoSphere* infoSphere = mCollisionShapeKeeper->getShapeInfoSphere(0);
    if (infoSphere)
        infoSphere->set48(40.0f);

    mCollisionShapeKeeper->createShapeSphereIgnoreGround("Head", 70.0f, gHead);
    createLegCollider(mCollisionShapeKeeper, {0.0f, 0.0f, 0.0f}, -40.0f, 20.0f, 40.0f);
    mCollisionShapeKeeper->updateShape();
    mPlayerCollider = new PlayerCollider(mActor->getCollisionDirector(), mActor->getBaseMtx(),
                                         al::getTransPtr(mActor), al::getGravityPtr(mActor), false);
    mPlayerCollider->setCollisionShapeKeeper(mCollisionShapeKeeper);
    mPlayerCollider->set1b0(60.0f);

    mJudgeNormalFall = new HackerJudgeNormalFall(mActor, 5);
    mJudgeNormalFall->setPlayerCollision(this);
    mJudgeStartRun = new HackerJudgeStartRun(mActor, playerHack);
    mJudgeStartRun->setPlayerCollision(this);
    al::invalidateDepthShadowMap(mActor);
    mDepthShadowMapCtrl = new HackerDepthShadowMapCtrl(mActor, "Ground", 100.0f, 0.3f, 0.5f);
}

void KaronWingStateHack::appear() {
    al::LiveActor* actor = mActor;
    al::ActorStateBase::appear();

    bool isOnGround = al::isOnGround(actor, 0);
    al::offCollide(mActor);
    rs::resetCollision(this);
    rs::resetJudge(mJudgeStartRun);
    rs::resetJudge(mJudgeNormalFall);
    al::showSilhouetteModelIfHide(mActor);
    mShadowMaskDropLength = al::getShadowMaskDropLength(mActor, "JointRoot");
    al::setShadowMaskDropLength(mActor, 50000.0f);
    al::setSeKeeperPlayNamePrefix(mActor, "PHack");
    mStateWingFly->updateFlyLimit();

    if (isOnGround) {
        al::setNerve(this, &NrvHostType.Wait);
    } else {
        mStateWingFly->setIsJudgeFall(true);
        al::setNerve(this, &NrvHostType.WingFly);
    }
}

void KaronWingStateHack::kill() {
    al::LiveActor* actor = mActor;
    al::onCollide(actor);
    al::hideSilhouetteModelIfShow(actor);
    al::startVisAnim(actor, "CapOff");
    al::showModelIfHide(actor);
    al::setShadowMaskDropLength(actor, mShadowMaskDropLength);
    EnemyStateHackFunction::endHackSwitchShadow(actor, nullptr);
    al::resetSeKeeperPlayNamePrefix(actor);
    if (*mPlayerHack)
        rs::endHack(mPlayerHack);
    al::ActorStateBase::kill();
}

void KaronWingStateHack::control() {
    if (rs::isTouchHackCancelCollisionCode(mActor, this)) {
        al::startHitReaction(mActor, "死亡");
        al::setNerve(this, &NrvHostType.Cancel);
        kill();
        return;
    }

    if (rs::isTouchHackDamageCollisionCode(mActor, this)) {
        rs::requestDamage(*mPlayerHack);
        return;
    }

    mDepthShadowMapCtrl->update(mPlayerCollider);
}

PlayerCollider* KaronWingStateHack::getPlayerCollider() const {
    return mPlayerCollider;
}

void KaronWingStateHack::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isNerve(this, &NrvHostType.Wait) || al::isNerve(this, &NrvHostType.Walk))
        return;

    if (al::isNerve(this, &NrvHostType.WingFly))
        mStateWingFly->attackSensor(self, other);
}

bool KaronWingStateHack::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                    al::HitSensor* self) {
    al::LiveActor* actor = mActor;
    if (rs::isMsgEnableMapCheckPointWarp(message))
        return true;

    if (rs::isMsgHackerDamageAndCancel(message))
        return rs::requestDamage(*mPlayerHack);

    if (rs::isMsgHackSyncDamageVisibility(message)) {
        rs::syncDamageVisibility(mActor, *mPlayerHack);
        return true;
    }

    if (rs::isMsgCancelHack(message)) {
        al::setNerve(this, &NrvHostType.Cancel);
        kill();
        return true;
    }

    if (rs::isMsgCancelHackByDokan(message)) {
        al::setNerve(this, &NrvHostType.Reset);
        kill();
        return true;
    }

    if (rs::receiveMsgRequestTransferHack(message, *mPlayerHack, other)) {
        al::setNerve(this, &NrvHostType.Cancel);
        kill();
        return true;
    }

    if (rs::isMsgHackMarioDead(message) || rs::isMsgHackMarioDemo(message)) {
        al::setNerve(this, &NrvHostType.Damage);
        kill();
        return true;
    }

    if (rs::isMsgHackMarioCheckpointFlagWarp(message)) {
        al::setNerve(this, &NrvHostType.Cancel);
        kill();
        return true;
    }

    return al::tryReceiveMsgPushAndAddVelocity(actor, message, other, self, 3.0f);
}

void KaronWingStateHack::updateCollider() {
    const sead::Vector3f& velocity = al::getVelocity(mActor);
    *al::getTransPtr(mActor) += mPlayerCollider->collide(velocity);
}

void KaronWingStateHack::resetFlyLimit(const sead::Vector3f& flyLimit) {
    mStateWingFly->setFlyLimit(flyLimit);
}

bool KaronWingStateHack::isEndCancel() const {
    return al::isNerve(this, &NrvHostType.Cancel);
}

bool KaronWingStateHack::isEndReset() const {
    return al::isNerve(this, &NrvHostType.Reset);
}

bool KaronWingStateHack::isEndDamage() const {
    return al::isNerve(this, &NrvHostType.Damage);
}

void KaronWingStateHack::updateBasePos() {
    if (rs::isOnGround(mActor, this))
        mStateWingFly->updateFlyLimit();
}

bool KaronWingStateHack::tryFly() {
    if (mStateWingFly->judgeStart()) {
        al::setNerve(this, &NrvHostType.WingFly);
        return true;
    }

    return false;
}

bool KaronWingStateHack::tryLanding() {
    if (!rs::isOnGround(mActor, this))
        return false;

    if (rs::updateJudgeAndResult(mJudgeStartRun)) {
        al::startHitReaction(mActor, "着地");
        al::setNerve(this, &NrvHostType.Walk);
    } else {
        al::setVelocityZero(mActor);
        al::startAction(mActor, "Land");
        al::setNerve(this, &NrvHostType.Land);
    }

    return true;
}

void KaronWingStateHack::exeWait() {
    if (al::isFirstStep(this)) {
        al::tryStartActionIfNotPlaying(mActor, "Wait");
        rs::resetJudge(mJudgeNormalFall);
    }

    if (!rs::isOnGround(mActor, this))
        al::addVelocityToGravity(mActor, 1.4f);

    al::scaleVelocity(mActor, 0.7f);
    if (rs::updateJudgeAndResult(mJudgeNormalFall)) {
        mStateWingFly->setIsJudgeFall(true);
        al::setNerve(this, &NrvHostType.WingFly);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeStartRun)) {
        al::setNerve(this, &NrvHostType.Walk);
        return;
    }

    updateBasePos();
    tryFly();
}

void KaronWingStateHack::exeWalk() {
    al::LiveActor* actor = mActor;
    if (al::isFirstStep(this)) {
        al::tryStartActionIfNotPlaying(actor, "Walk");
        rs::resetJudge(mJudgeNormalFall);
        mPlayerActionTurnControl->reset();
    }

    if (rs::updateJudgeAndResult(mJudgeNormalFall)) {
        mStateWingFly->setIsJudgeFall(true);
        al::setNerve(this, &NrvHostType.WingFly);
        return;
    }

    updateBasePos();

    al::addVelocityToGravity(actor, 1.4f);
    al::scaleVelocityHV(actor, al::calcNerveEaseInValue(this, 12, 0.0f, 0.7f), 1.0f);

    if (!rs::isOnHackMoveStick(*mPlayerHack)) {
        al::setNerve(this, &NrvHostType.Wait);
        return;
    }

    sead::Vector3f up = -al::getGravity(actor);
    sead::Vector3f moveVec = sead::Vector3f::zero;

    rs::calcHackerMoveVec(&moveVec, *mPlayerHack, up);
    mPlayerActionTurnControl->update(moveVec, up);
    al::faceToDirection(actor, mPlayerActionTurnControl->get_5c());
    al::addVelocity(actor, mPlayerActionTurnControl->get_5c() * 4.0f *
                               rs::calcHackMovePower(*mPlayerHack));

    if (tryFly())
        return;

    rs::reboundVelocityPart(actor, this, 0.0f, 0.0f, 0.0f, 0.0f);
}

void KaronWingStateHack::exeLand() {
    al::addVelocityToGravity(mActor, 1.4f);
    al::scaleVelocity(mActor, 0.7f);

    if (al::isGreaterStep(this, 10) && rs::isOnHackMoveStick(*mPlayerHack)) {
        al::setNerve(this, &NrvHostType.Walk);
        return;
    }

    if (tryFly())
        return;

    if (al::isActionEnd(mActor))
        al::setNerve(this, &NrvHostType.Wait);
}

void KaronWingStateHack::exeWingFly() {
    if (al::updateNerveState(this)) {
        if (rs::updateJudgeAndResult(mJudgeStartRun)) {
            al::startHitReaction(mActor, "着地");
            al::setNerve(this, &NrvHostType.Walk);
        } else {
            al::setVelocityZero(mActor);
            al::startAction(mActor, "Land");
            al::setNerve(this, &NrvHostType.Land);
        }
    }
}
