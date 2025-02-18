#include "Library/MapObj/ClockMapParts.h"

#include "Library/Fluid/RippleCtrl.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorMsgFunction.h"
#include "Library/MapObj/ChildStep.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Se/SeFunction.h"
#include "Library/Stage/StageSwitchKeeper.h"
#include "Library/Thread/FunctorV0M.h"

#include "MapObj/SubActorLodFixPartsScenarioAction.h"

namespace {
using namespace al;

NERVE_ACTION_IMPL(ClockMapParts, StandBy)
NERVE_ACTION_IMPL(ClockMapParts, Delay)
NERVE_ACTION_IMPL(ClockMapParts, RotateSign)
NERVE_ACTION_IMPL(ClockMapParts, Rotate)
NERVE_ACTION_IMPL(ClockMapParts, Wait)
NERVE_ACTION_IMPL(ClockMapParts, AssistStop)
NERVE_ACTION_IMPL(ClockMapParts, AssistStopSync)
NERVE_ACTION_IMPL(ClockMapParts, AssistStopEndWait)

NERVE_ACTIONS_MAKE_STRUCT(ClockMapParts, StandBy, Delay, RotateSign, Rotate, Wait, AssistStop,
                          AssistStopSync, AssistStopEndWait)
}  // namespace

namespace al {
ClockMapParts::ClockMapParts(const char* name) : LiveActor(name) {}

void ClockMapParts::init(const ActorInitInfo& info) {
    using ClockMapPartsFunctor = FunctorV0M<ClockMapParts*, void (ClockMapParts::*)()>;

    tryInitSubActorKeeperChildStep(this, info);
    initNerveAction(this, "Rotate", &NrvClockMapParts.mCollector, 0);
    initMapPartsActor(this, info, nullptr);
    tryGetQuatPtr(this);
    registerAreaHostMtx(this, info);
    registSupportFreezeSyncGroup(this, info);

    mQuat.set(getQuat(this));

    tryGetArg(&mClockAngleDegree, info, "ClockAngleDegree");

    if (mClockAngleDegree == 0) {
        mTurnStepCount = 1;
    } else {
        s32 lcm = sead::Mathi::lcm(sead::Mathi::abs(mClockAngleDegree), 360);
        s32 clockAngleDegree = sead::Mathi::abs(mClockAngleDegree);

        mTurnStepCount = lcm / clockAngleDegree;
    }

    tryGetArg(&mRotateAxis, info, "RotateAxis");
    createChildStep(info, this, true);
    tryGetArg(&mDelayTime, info, "DelayTime");
    tryGetArg(&mWaitTime, info, "WaitTime");
    tryGetArg(&mRotateTime, info, "RotateTime");

    initMaterialCode(this, info);

    if (isExistAction(this, "MiddleSign")) {
        mRotateSignTime = (s32)getActionFrameMax(this, "MiddleSign");
    } else {
        mRotateSignTime = 36;
        tryGetArg(&mRotateSignTime, info, "RotateSignTime");
    }

    mRotateTimer = mRotateSignTime + mRotateTime + 1;
    mActiveTimer = mRotateTimer + mWaitTime + 1;

    if (mDelayTime >= 1) {
        mTimer = 0;
        startNerveAction(this, "Delay");
    } else {
        mTimer = mRotateSignTime;
    }

    if (listenStageSwitchOnStart(this, ClockMapPartsFunctor(this, &ClockMapParts::start)))
        startNerveAction(this, "StandBy");

    if (isExistModel(this)) {
        mRippleCtrl = RippleCtrl::tryCreate(this);

        if (mRippleCtrl != nullptr)
            mRippleCtrl->init(info);
    }

    listenStageSwitchOn(this, "SwitchStop", ClockMapPartsFunctor(this, &ClockMapParts::standBy));

    trySyncStageSwitchAppear(this);
}

void ClockMapParts::start() {
    if (!isNerve(this, NrvClockMapParts.StandBy.data()))
        return;

    if (mDelayTime >= 1) {
        startNerveAction(this, "Delay");

        return;
    }

    setRotateStartNerve();
}

void ClockMapParts::standBy() {
    startNerveAction(this, "StandBy");
}

bool ClockMapParts::receiveMsg(const SensorMsg* message, HitSensor* other, HitSensor* self) {
    if (isNerve(this, NrvClockMapParts.StandBy.data()))
        return false;

    if (isMsgTouchAssist(message)) {
        mAssistStopTimer = 45;
        if (isNerve(this, NrvClockMapParts.AssistStop.data()))
            return true;

        if (isExistAction(this))
            stopAction(this);

        startNerveAction(this, "AssistStop");

        return true;
    }

    if (isMsgShowModel(message)) {
        showModelIfHide(this);

        return true;
    }

    if (isMsgHideModel(message)) {
        hideModelIfShow(this);

        return true;
    }

    if (isMsgRestart(message)) {
        appearAndSetStart();

        return true;
    }

    if (isMsgIsNerveSupportFreeze(message))
        return isNerve(this, NrvClockMapParts.AssistStop.data());

    if (isMsgOnSyncSupportFreeze(message)) {
        if (isNerve(this, NrvClockMapParts.AssistStop.data()))
            return true;

        if (isExistAction(this))
            stopAction(this);

        startNerveAction(this, "AssistStopSync");

        return true;
    }

    if (isMsgOffSyncSupportFreeze(message)) {
        if (!isNerve(this, NrvClockMapParts.AssistStopSync.data()))
            return true;

        setRestartNerve();

        return true;
    }

    return false;
}

void ClockMapParts::appearAndSetStart() {
    mCurrentStep = 0;
    mTimer = 0;
    mAssistStopTimer = 0;

    setQuat(this, mQuat);

    if (mDelayTime >= 1)
        startNerveAction(this, "Delay");
    else
        setRotateStartNerve();

    makeActorAlive();
}

void ClockMapParts::setRestartNerve() {
    if (isExistAction(this))
        restartAction(this);

    if (mTimer >= mRotateTimer)
        startNerveAction(this, "AssistStopEndWait");
    else if (mTimer >= mRotateSignTime)
        startNerveAction(this, "Rotate");
    else
        startNerveAction(this, "RotateSign");
}

void ClockMapParts::control() {
    if (mRippleCtrl == nullptr)
        return;

    mRippleCtrl->update();
}

void ClockMapParts::setRotateStartNerve() {
    if (mRotateSignTime > 0)
        startNerveAction(this, "RotateSign");
    else
        startNerveAction(this, "Rotate");
}

void ClockMapParts::exeStandBy() {}

void ClockMapParts::exeDelay() {
    if (isGreaterEqualStep(this, mDelayTime - 1))
        setRotateStartNerve();
}

void ClockMapParts::exeRotateSign() {
    if (isFirstStep(this))
        tryStartAction(this, "MiddleSign");

    f32 angle = modf((f32)(mCurrentStep * mClockAngleDegree) + 360.0f, 360.0f) + 0.0f;

    rotateQuatLocalDirDegree(this, mQuat, mRotateAxis,
                             angle + sead::Mathf::sin((f32)mTimer * sead::Mathf::pi2() / 18.0f));

    mTimer++;
    if (mTimer >= mRotateSignTime)
        startNerveAction(this, "Rotate");
}

void ClockMapParts::exeRotate() {
    f32 time = (f32)(mTimer - mRotateSignTime) / (f32)(mRotateTimer + ~mRotateSignTime);
    f32 angle = modf((time + (f32)mCurrentStep) * (f32)mClockAngleDegree + 360.0f, 360.0f) + 0.0f;
    rotateQuatLocalDirDegree(this, mQuat, mRotateAxis, angle);

    mTimer++;
    if (mTimer >= mRotateTimer) {
        mCurrentStep = modi(mCurrentStep + mTurnStepCount + 1, mTurnStepCount);
        startNerveAction(this, "Wait");
        tryStartSe(this, "RotateEnd");
    }
}

void ClockMapParts::exeWait() {
    mTimer++;
    if (mTimer >= mActiveTimer) {
        mTimer -= mActiveTimer;
        setRotateStartNerve();
    }
}

void ClockMapParts::exeAssistStop() {
    mAssistStopTimer--;
    if (mAssistStopTimer <= 0) {
        mAssistStopTimer = 0;
        setRestartNerve();
    }
}

void ClockMapParts::exeAssistStopSync() {}

void ClockMapParts::exeAssistStopEndWait() {
    exeWait();
}
}  // namespace al
