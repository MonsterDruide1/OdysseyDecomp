#include "Library/MapObj/SlideMapParts.h"

#include "Library/Effect/EffectSystemInfo.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/ActorSensorMsgFunction.h"
#include "Library/Nerve/NerveSetupUtil.h"
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

// NON_MATCHING
void SlideMapParts::init(const ActorInitInfo& info) {
    using SlideMapPartsFunctor = FunctorV0M<SlideMapParts*, void (SlideMapParts::*)()>;

    initNerveAction(this, "Move", &NrvSlideMapParts.mCollector, 0);
    initMapPartsActor(this, info, tryGetMapPartsSuffix(info, "SlideMapParts"));
    registerAreaHostMtx(this, info);

    mTrans = getTrans(this);
    tryGetArg(&mMoveAxis, info, "MoveAxis");
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
    calcQuatLocalAxis(&t, getQuat(this), mMoveAxis);

    f32 offsetX = surfaceHeight * t.x + mTrans.x;
    f32 offsetY = surfaceHeight * t.y;
    f32 offsetZ = surfaceHeight * t.z;

    sead::Quatf q = getQuat(this);

    const f32 xx = 2 * q.x * q.x;
    const f32 yy = 2 * q.y * q.y;
    const f32 ww = 2 * q.w * q.w;
    const f32 wx = 2 * q.w * q.x;
    const f32 wy = 2 * q.w * q.y;
    const f32 xy = 2 * q.x * q.y;
    const f32 zx = 2 * q.z * q.x;
    const f32 zy = 2 * q.z * q.y;
    const f32 zw = 2 * q.z * q.w;

    mSurfaceEffectMtx.m[0][0] = 1 - xx - yy;
    mSurfaceEffectMtx.m[0][1] = wx - zy;
    mSurfaceEffectMtx.m[0][3] = offsetX;
    mSurfaceEffectMtx.m[0][2] = wy + zx;

    mSurfaceEffectMtx.m[1][0] = wx + zy;
    mSurfaceEffectMtx.m[1][2] = xy - zw;

    mSurfaceEffectMtx.m[2][0] = wy - zx;
    mSurfaceEffectMtx.m[2][1] = xy + zw;

    mSurfaceEffectMtx.m[2][3] = offsetZ + mTrans.z;
    mSurfaceEffectMtx.m[1][3] = offsetY + mTrans.y;

    mSurfaceEffectMtx.m[1][1] = 1 - ww - yy;
    mSurfaceEffectMtx.m[2][2] = 1 - ww - xx;

    trySetEffectNamedMtxPtr(this, "Surface", &mSurfaceEffectMtx);

    if (listenStageSwitchOnStart(this, SlideMapPartsFunctor(this, &SlideMapParts::start)))
        startNerveAction(this, "StandBy");
    else if (mDelayTime >= 1)
        startNerveAction(this, "Delay");

    trySyncStageSwitchAppear(this);
}

void SlideMapParts::start() {
    if (isNerve(this, NrvSlideMapParts.StandBy.data())) {
        if (mDelayTime >= 1) {
            startNerveAction(this, "Delay");

            return;
        }

        startNerveAction(this, "Move");
    }
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
        if (_15c)
            tryStartSe(this, "MoveStart1");
        else
            tryStartSe(this, "MoveStart2");
    }

    f32 rate = calcNerveRate(this, calcMoveTime());
    if (!_15c)
        rate = 1.0f - rate;

    setTransOffsetLocalDir(this, getQuat(this), mTrans, mMoveDistance * rate, mMoveAxis);

    if (isGreaterEqualStep(this, calcMoveTime())) {
        if (_15c)
            tryStartSe(this, "MoveEnd1");
        else
            tryStartSe(this, "MoveEnd2");

        _15c = !_15c;
        tryStartSe(this, "MoveEnd");
        startNerveAction(this, "Wait");
    }
}

s32 SlideMapParts::calcMoveTime() const {
    if (mMoveTime >= 0)
        return mMoveTime;

    if (mMoveSpeed < 1.0f)
        return 0;

    f32 moveTime = mMoveDistance / mMoveSpeed;
    if (!(moveTime > 0.0f))
        moveTime = -moveTime;

    return (s32)moveTime;
}
}  // namespace al
