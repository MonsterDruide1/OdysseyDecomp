#include "Library/Obj/KeyMoveFollowTarget.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Nerve/NerveUtil.h"

namespace al {

KeyMoveFollowTarget::KeyMoveFollowTarget(const char* name) : LiveActor(name) {}

void KeyMoveFollowTarget::initKeyMoveFollowTarget(const al::ActorInitInfo& info,
                                                  const char* archiveName, const char* suffix) {
    initNerveAction(this, "Wait", &NrvKeyMoveFollowTarget.mCollector, 0);
    initActorWithArchiveName(this, info, archiveName, suffix);

    mKeyPoseKeeper = createKeyPoseKeeper(info);

    this->makeActorAlive();
}

void KeyMoveFollowTarget::exeWait() {
    s32 timer;
    if (isFirstStep(this) && (timer = calcKeyMoveWaitTime(mKeyPoseKeeper), timer > -1))
        mMoveWaitTime = timer;

    if (!isGreaterEqualStep(this, mMoveWaitTime))
        return;

    const char* action = "Move";
    if (isRestart(mKeyPoseKeeper)) {
        restartKeyPose(mKeyPoseKeeper, getTransPtr(this), getQuatPtr(this));
        resetPosition(this);
        action = "Wait";
    }

    startNerveAction(this, action);
}

void KeyMoveFollowTarget::exeMove() {
    if (isFirstStep(this))
        mTimer = calcKeyMoveWaitTime(mKeyPoseKeeper);

    float rate = calcNerveRate(this, mTimer);
    calcLerpKeyTrans(getTransPtr(this), mKeyPoseKeeper, rate);
    calcSlerpKeyQuat(getQuatPtr(this), mKeyPoseKeeper, rate);

    if (!isGreaterEqualStep(this, mTimer))
        return;

    nextKeyPose(mKeyPoseKeeper);

    const char* action = "Stop";
    if (!isStop(mKeyPoseKeeper)) {
        startHitReaction(this, "移動終了");
        action = "Wait";
    }

    startAction(this, action);
}

void KeyMoveFollowTarget::exeStop() {
    if (!isFirstStep(this) || !isInvalidClipping(this))
        return;

    validateClipping(this);
}

void KeyMoveFollowTarget::init(const al::ActorInitInfo& info) {}
}  // namespace al
