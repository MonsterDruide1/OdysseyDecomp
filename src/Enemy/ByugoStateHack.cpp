#include "Enemy/ByugoStateHack.h"

#include <math/seadVector.h>

#include "Library/Camera/CameraUtil.h"
#include "Library/Collision/CollisionPartsKeeperUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nature/NatureUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Enemy/ByugoHackCameraTarget.h"
#include "Enemy/EnemyStateHackStart.h"
#include "Util/CameraUtil.h"
#include "Util/PlayerHackFunction.h"
#include "Util/PlayerHackInputFunction.h"
#include "Util/SensorMsgFunction.h"

namespace {
static inline void turnToHackMoveDir(sead::Vector3f* moveDir, al::LiveActor* const* actor,
                                     const IUsePlayerHack* playerHack, f32 turnSpeed) {
    rs::calcHackerMoveDir(moveDir, playerHack, sead::Vector3f::ey);
    al::turnToDirection(*actor, *moveDir, turnSpeed);
}

static inline sead::Vector3f calcBlowRippleOffset(s32 step, s32 interval, f32 distance) {
    s32 rippleStep = step % interval;
    f32 rippleStepF = rippleStep;
    return sead::Vector3f(0.0f, -350.0f, distance / interval * rippleStepF);
}

static inline void addBlowRipple(const al::LiveActor* actor, const sead::Vector3f& ripplePos,
                                 s32 step, s32 interval, f32 maxRange, f32 blur) {
    s32 rippleStep = step % interval;
    f32 intervalF = interval;
    f32 rippleRange = al::lerpValue(0.0f, maxRange, rippleStep / intervalF);
    al::tryAddRippleRandomBlur(actor, ripplePos, blur, rippleRange, 30.0f);
}

NERVE_IMPL(ByugoStateHack, Wait);
NERVE_IMPL(ByugoStateHack, StartDemo);
NERVE_IMPL(ByugoStateHack, Blow);
NERVE_IMPL(ByugoStateHack, BlowWide);
NERVE_IMPL(ByugoStateHack, Move);
NERVE_IMPL(ByugoStateHack, BlowEnd);
NERVE_IMPL(ByugoStateHack, BlowStart);

NERVES_MAKE_STRUCT(ByugoStateHack, Wait, StartDemo, Blow, BlowWide, Move, BlowEnd, BlowStart);
}  // namespace

ByugoStateHack::ByugoStateHack(al::LiveActor* actor, CapTargetInfo* capTargetInfo)
    : ActorStateBase("ビューゴーキャプチャ状態", actor), mCapTargetInfo(capTargetInfo) {
    initNerve(&NrvByugoStateHack.Wait, 1);

    mStateHackStartParam =
        new EnemyStateHackStartParam("HackStart", nullptr, nullptr, false, false);
    mStateHackStartParam->visAnimName = "HackOn";
    mStateHackStartParam->mtpAnimName = "HackOn";

    mStateHackStart = new EnemyStateHackStart(actor, mStateHackStartParam, nullptr);
    al::initNerveState(this, mStateHackStart, &NrvByugoStateHack.StartDemo, "キャプチャ開始");

    mBaseTransY = al::getTrans(mActor).y;
    mCameraTarget = new ByugoHackCameraTarget(mActor);
}

void ByugoStateHack::appear() {
    al::ActorStateBase::appear();
    al::setNerve(this, &NrvByugoStateHack.StartDemo);
    al::setCameraTarget(mActor, mCameraTarget);
    mWideBlowFrame = 0;
    rs::requestSetCameraAngleV(mActor, 10.0f, 0);
}

void ByugoStateHack::kill() {
    al::ActorStateBase::kill();
    al::startVisAnim(mActor, "HackOff");
    al::showModelIfHide(mActor);
    al::setVelocityZero(mActor);
    al::resetCameraTarget(mActor, mCameraTarget);
}

bool ByugoStateHack::receiveMsgInitCapTargetInfo(const al::SensorMsg* msg) {
    return rs::tryReceiveMsgInitCapTargetAndSetCapTargetInfo(msg, mCapTargetInfo);
}

bool ByugoStateHack::receiveMsgHackStart(const al::SensorMsg* msg, al::HitSensor* other,
                                         al::HitSensor* self) {
    if (!rs::isMsgStartHack(msg))
        return false;

    al::invalidateClipping(mActor);
    mPlayerHack = mStateHackStart->tryStart(msg, other, self);
    al::setNerve(this, &NrvByugoStateHack.StartDemo);
    return true;
}

bool ByugoStateHack::receiveMsg(const al::SensorMsg* msg, al::HitSensor* other,
                                al::HitSensor* self) {
    if (receiveMsgInitCapTargetInfo(msg) || rs::isMsgEnableMapCheckPointWarp(msg) ||
        rs::isMsgCapKeepLockOn(msg) || rs::isMsgCapStartLockOn(msg))
        return true;

    if (rs::isMsgHackSyncDamageVisibility(msg)) {
        if (mPlayerHack)
            rs::syncDamageVisibility(mActor, mPlayerHack);
        return true;
    }

    return al::tryReceiveMsgPushAndAddVelocityH(mActor, msg, other, self, 1.0f);
}

bool ByugoStateHack::receiveMsgHackEnd(const al::SensorMsg* msg, al::HitSensor* other,
                                       al::HitSensor* self) {
    if (rs::isMsgCancelHack(msg)) {
        al::startMtpAnim(mActor, "HackOff");
        al::startVisAnim(mActor, "HackOff");
        al::validateClipping(mActor);

        const sead::Vector3f trans = al::getTrans(mActor);
        sead::Vector3f hitPos{0.0f, 0.0f, 0.0f};
        sead::Vector3f frontDir;
        al::calcFrontDir(&frontDir, mActor);

        if (alCollisionUtil::getHitPosOnArrow(
                mActor, &hitPos, trans + (sead::Vector3f::ey * 800.0f) * 0.5f,
                (sead::Vector3f::ey * 800.0f) * 0.5f, nullptr, nullptr))
            rs::endHackFromTargetPos(&mPlayerHack, (trans + hitPos) * 0.5f, frontDir);
        else
            rs::endHackDir(&mPlayerHack, frontDir);

        kill();
        return true;
    }

    if (rs::isMsgHackMarioDemo(msg) || rs::isMsgHackMarioDead(msg)) {
        forceEndHack();
        return true;
    }

    return false;
}

void ByugoStateHack::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (rs::sendMsgHackerNoReaction(mPlayerHack, other, self))
        return;

    if (rs::sendMsgHackAttackMapObj(other, self))
        return;

    al::sendMsgPush(other, self);
}

f32 ByugoStateHack::calcHackBlowPowerRate() const {
    if (al::isNerve(this, &NrvByugoStateHack.Blow))
        return al::calcNerveRate(this, 20);

    if (isWideBlow())
        return 1.0f;

    return 0.0f;
}

bool ByugoStateHack::isWideBlow() const {
    return al::isNerve(this, &NrvByugoStateHack.BlowWide);
}

void ByugoStateHack::exeStartDemo() {
    if (al::updateNerveState(this))
        al::setNerve(this, &NrvByugoStateHack.Wait);
}

void ByugoStateHack::exeWait() {
    al::LiveActor* actor = mActor;

    if (al::isFirstStep(this))
        al::startAction(actor, "Wait");

    al::scaleVelocity(actor, 0.85f);
    calcMove();

    if (tryChangeNerveIfTriggerBlow())
        return;

    if (rs::isOnHackMoveStick(mPlayerHack))
        al::setNerve(this, &NrvByugoStateHack.Move);
}

void ByugoStateHack::calcMove() {
    al::LiveActor* actor = mActor;
    f32 transY = al::getTrans(actor).y - mBaseTransY;

    if (transY < 0.0f) {
        al::scaleVelocityY(actor, 0.0f);
        al::setTransY(actor, mBaseTransY);
    } else {
        al::addVelocityY(actor, transY * -0.01f);
        al::scaleVelocityY(actor, 0.7f);
    }

    al::reboundVelocityFromCollision(actor, 0.0f, 0.0f, 1.0f);
}

bool ByugoStateHack::tryChangeNerveIfTriggerBlow() {
    if (rs::isTriggerHackSwing(mPlayerHack)) {
        mIsWideBlow = true;
        al::setNerve(this, &NrvByugoStateHack.BlowStart);
        return true;
    }

    if (rs::isTriggerHackAnyButton(mPlayerHack)) {
        mIsWideBlow = false;
        al::setNerve(this, &NrvByugoStateHack.BlowStart);
        return true;
    }

    return false;
}

void ByugoStateHack::exeMove() {
    al::LiveActor* actor = mActor;

    if (al::isFirstStep(this))
        al::startAction(actor, "Walk");

    al::scaleVelocity(actor, 0.85f);
    calcMove();

    sead::Vector3f moveDir{0.0f, 0.0f, 0.0f};
    rs::addHackActorAccelStick(actor, mPlayerHack, &moveDir, 2.2f, sead::Vector3f::ey);
    al::turnToDirection(actor, moveDir, 8.0f);

    if (tryChangeNerveIfTriggerBlow())
        return;

    if (rs::isHackerStopMove(actor, mPlayerHack, 10.0f))
        al::setNerve(this, &NrvByugoStateHack.Wait);
}

void ByugoStateHack::exeBlowStart() {
    al::LiveActor* actor = mActor;

    if (al::isFirstStep(this))
        al::startAction(actor, "BlowStartHack");

    al::scaleVelocity(actor, 0.85f);
    calcMove();

    sead::Vector3f moveDir{0.0f, 0.0f, 0.0f};
    turnToHackMoveDir(&moveDir, &actor, mPlayerHack, 15.0f);

    if (al::isActionEnd(actor)) {
        if (mIsWideBlow)
            al::setNerve(this, &NrvByugoStateHack.BlowWide);
        else
            al::setNerve(this, &NrvByugoStateHack.Blow);
    }
}

void ByugoStateHack::exeBlow() {
    al::LiveActor* actor = mActor;

    if (al::isFirstStep(this))
        al::tryStartActionIfNotPlaying(actor, "Blow");

    al::scaleVelocity(actor, 0.85f);
    calcMove();

    s32 interval = 15;
    if (al::isLessEqualStep(this, 30))
        interval = 30;

    sead::Vector3f ripplePos;
    al::calcTransLocalOffset(&ripplePos, actor,
                             calcBlowRippleOffset(al::getNerveStep(this), interval, 1500.0f));
    addBlowRipple(actor, ripplePos, al::getNerveStep(this), interval, 400.0f, 0.25f);

    sead::Vector3f moveDir{0.0f, 0.0f, 0.0f};
    turnToHackMoveDir(&moveDir, &actor, mPlayerHack, 2.0f);

    if (rs::isTriggerHackSwing(mPlayerHack)) {
        al::setNerve(this, &NrvByugoStateHack.BlowWide);
        return;
    }

    if (al::isGreaterEqualStep(this, 50) && !rs::isHoldHackAnyButton(mPlayerHack))
        al::setNerve(this, &NrvByugoStateHack.BlowEnd);
}

void ByugoStateHack::exeBlowWide() {
    al::LiveActor* actor = mActor;

    if (al::isFirstStep(this))
        al::tryStartActionIfNotPlaying(mActor, "BlowWide");

    al::scaleVelocity(mActor, 0.85f);
    calcMove();

    sead::Vector3f moveDir{0.0f, 0.0f, 0.0f};
    turnToHackMoveDir(&moveDir, &mActor, mPlayerHack, 2.0f);

    s32 interval = 15;
    if (mWideBlowFrame < 30)
        interval = 30;

    sead::Vector3f ripplePos;
    al::calcTransLocalOffset(&ripplePos, actor,
                             calcBlowRippleOffset(mWideBlowFrame, interval, 1800.0f));
    addBlowRipple(actor, ripplePos, mWideBlowFrame, interval, 600.0f, 0.3f);

    mWideBlowFrame++;

    if (rs::isTriggerHackSwing(mPlayerHack)) {
        al::setNerve(this, &NrvByugoStateHack.BlowWide);
        return;
    }

    if (al::isGreaterEqualStep(this, 60)) {
        mWideBlowFrame = 0;
        if (rs::isHoldHackAnyButton(mPlayerHack))
            al::setNerve(this, &NrvByugoStateHack.Blow);
        else
            al::setNerve(this, &NrvByugoStateHack.BlowEnd);
    }
}

void ByugoStateHack::exeBlowEnd() {
    al::LiveActor* actor = mActor;

    if (al::isFirstStep(this))
        al::startAction(actor, "BlowEnd");

    al::scaleVelocity(actor, 0.85f);
    calcMove();

    sead::Vector3f moveDir{0.0f, 0.0f, 0.0f};
    if (rs::addHackActorAccelStick(actor, mPlayerHack, &moveDir, 2.2f, sead::Vector3f::ey))
        al::turnToDirection(actor, moveDir, 8.0f);

    if (al::isActionEnd(actor))
        al::setNerve(this, &NrvByugoStateHack.Wait);
}

void ByugoStateHack::forceEndHack() {
    al::startMtpAnim(mActor, "HackOff");
    al::startVisAnim(mActor, "HackOff");
    al::validateClipping(mActor);
    rs::endHack(&mPlayerHack);
    kill();
}
