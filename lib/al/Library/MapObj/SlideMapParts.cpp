#include "Library/MapObj/SlideMapParts.h"

#include "Library/Effect/EffectSystemInfo.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorMsgFunction.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Se/SeFunction.h"
#include "Library/Stage/StageSwitchKeeper.h"
#include "Library/Thread/FunctorV0M.h"

namespace {
using namespace al;

NERVE_ACTION_IMPL(SlideMapParts, StandBy)
NERVE_ACTION_IMPL(SlideMapParts, Delay)
NERVE_ACTION_IMPL(SlideMapParts, Wait)
NERVE_ACTION_IMPL(SlideMapParts, Move)

NERVE_ACTIONS_MAKE_STRUCT(SlideMapParts, StandBy, Delay, Wait, Move)
}  // namespace

namespace al {
SlideMapParts::SlideMapParts(const char* name) : LiveActor(name) {}

void SlideMapParts::init(const ActorInitInfo& info) {
    using SlideMapPartsFunctor = FunctorV0M<SlideMapParts*, void (SlideMapParts::*)()>;

    initNerveAction(this, "Move", &NrvSlideMapParts.mCollector, 0);
    initMapPartsActor(this, info, tryGetMapPartsSuffix(info, "SlideMapParts"));
    registerAreaHostMtx(this, info);

    mTrans = getTrans(this);
    tryGetArg((s32*)&mMoveAxis, info, "MoveAxis");
    tryGetArg(&mMoveDistance, info, "MoveDistance");
    tryGetArg(&mMoveSpeed, info, "MoveSpeed");
    tryGetArg(&mWaitTime, info, "WaitTime");
    if (mWaitTime < 0)
        mWaitTime = 0;
    tryGetArg(&mMoveTime, info, "MoveTime");
    tryGetArg(&mDelayTime, info, "DelayTime");

    f32 surfaceHeight = 0.0f;
    tryGetArg(&surfaceHeight, info, "SurfaceHeight");

    initMaterialCode(this, info);

    sead::Vector3f t;
    calcQuatLocalAxis(&t, getQuat(this), (s32)mMoveAxis);

    sead::Vector3f ts;
    ts.x = surfaceHeight * t.x + mTrans.x;
    ts.y = surfaceHeight * t.y + mTrans.y;
    ts.z = surfaceHeight * t.z + mTrans.z;

    mSurfaceEffectMtx.makeQT(getQuat(this), ts);

    trySetEffectNamedMtxPtr(this, "Surface", &mSurfaceEffectMtx);

    if (listenStageSwitchOnStart(this, SlideMapPartsFunctor(this, &SlideMapParts::start)))
        startNerveAction(this, "StandBy");
    else if (mDelayTime >= 1)
        startNerveAction(this, "Delay");

    trySyncStageSwitchAppear(this);
}

void SlideMapParts::start() {
    if (!isNerve(this, NrvSlideMapParts.StandBy.data()))
        return;

    if (mDelayTime >= 1) {
        startNerveAction(this, "Delay");

        return;
    }

    startNerveAction(this, "Move");
}

bool SlideMapParts::receiveMsg(const SensorMsg* message, HitSensor* other, HitSensor* self) {
    if (isMsgShowModel(message)) {
        showModelIfHide(this);

        return true;
    }

    if (isMsgHideModel(message)) {
        hideModelIfShow(this);

        return true;
    }

    return false;
}

void SlideMapParts::exeStandBy() {}

void SlideMapParts::exeDelay() {
    if (isGreaterEqualStep(this, mDelayTime))
        startNerveAction(this, "Move");
}

void SlideMapParts::exeWait() {
    if (isGreaterStep(this, mWaitTime))
        startNerveAction(this, "Move");
}

void SlideMapParts::exeMove() {
    if (isFirstStep(this)) {
        if (mIsMoveForwards)
            tryStartSe(this, "MoveStart1");
        else
            tryStartSe(this, "MoveStart2");
    }

    f32 rate = calcNerveRate(this, calcMoveTime());
    if (!mIsMoveForwards)
        rate = 1.0f - rate;

    setTransOffsetLocalDir(this, getQuat(this), mTrans, mMoveDistance * rate, (s32)mMoveAxis);

    if (isGreaterEqualStep(this, calcMoveTime())) {
        if (mIsMoveForwards)
            tryStartSe(this, "MoveEnd1");
        else
            tryStartSe(this, "MoveEnd2");

        mIsMoveForwards = !mIsMoveForwards;
        tryStartSe(this, "MoveEnd");
        startNerveAction(this, "Wait");
    }
}

s32 SlideMapParts::calcMoveTime() const {
    if (mMoveTime >= 0)
        return mMoveTime;

    if (mMoveSpeed < 1.0f)
        return 0;

    return (s32)sead::Mathf::abs(mMoveDistance / mMoveSpeed);
}
}  // namespace al
