#include "Library/Obj/KeyMoveFollowTarget.h"

#include "Library/KeyPose/KeyPoseKeeper.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
using namespace al;

NERVE_ACTION_IMPL(KeyMoveFollowTarget, Wait)
NERVE_ACTION_IMPL(KeyMoveFollowTarget, Move)
NERVE_ACTION_IMPL(KeyMoveFollowTarget, Stop)

NERVE_ACTIONS_MAKE_STRUCT(KeyMoveFollowTarget, Wait, Move, Stop)
}  // namespace

namespace al {

KeyMoveFollowTarget::KeyMoveFollowTarget(const char* name) : LiveActor(name) {}

void KeyMoveFollowTarget::initKeyMoveFollowTarget(const ActorInitInfo& info,
                                                  const char* archiveName, const char* suffix) {
    initNerveAction(this, "Wait", &NrvKeyMoveFollowTarget.mCollector, 0);
    initActorWithArchiveName(this, info, archiveName, suffix);

    mKeyPoseKeeper = createKeyPoseKeeper(info);

    makeActorAlive();
}

void KeyMoveFollowTarget::exeWait() {
    if (isFirstStep(this)) {
        s32 timer = calcKeyMoveWaitTime(mKeyPoseKeeper);
        if (timer > -1)
            mMoveWaitTime = timer;
    }

    if (isGreaterEqualStep(this, mMoveWaitTime)) {
        if (isRestart(mKeyPoseKeeper)) {
            restartKeyPose(mKeyPoseKeeper, getTransPtr(this), getQuatPtr(this));
            resetPosition(this);
            startNerveAction(this, "Wait");

            return;
        }

        startNerveAction(this, "Move");
    }
}

void KeyMoveFollowTarget::exeMove() {
    if (isFirstStep(this))
        mTimer = calcKeyMoveWaitTime(mKeyPoseKeeper);

    f32 rate = calcNerveRate(this, mTimer);
    calcLerpKeyTrans(getTransPtr(this), mKeyPoseKeeper, rate);
    calcSlerpKeyQuat(getQuatPtr(this), mKeyPoseKeeper, rate);

    if (isGreaterEqualStep(this, mTimer)) {
        nextKeyPose(mKeyPoseKeeper);

        if (!isStop(mKeyPoseKeeper)) {
            startHitReaction(this, "移動終了");
            startNerveAction(this, "Wait");

            return;
        }

        startNerveAction(this, "Stop");
    }
}

void KeyMoveFollowTarget::exeStop() {
    if (isFirstStep(this) && isInvalidClipping(this))
        validateClipping(this);
}

void KeyMoveFollowTarget::init(const ActorInitInfo& info) {}
}  // namespace al
