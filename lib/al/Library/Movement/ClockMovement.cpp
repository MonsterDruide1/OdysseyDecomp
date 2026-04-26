#include "Library/Movement/ClockMovement.h"

#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"

namespace {
using namespace al;

NERVE_IMPL(ClockMovement, Delay);
NERVE_IMPL(ClockMovement, RotateSign);
NERVE_IMPL(ClockMovement, Rotate);
NERVE_IMPL(ClockMovement, Wait);

NERVES_MAKE_STRUCT(ClockMovement, Delay, RotateSign, Rotate);
NERVES_MAKE_NOSTRUCT(ClockMovement, Wait);
}  // namespace

namespace al {

ClockMovement::ClockMovement(const ActorInitInfo& info) : NerveExecutor("クロックパーツ動作") {
    getQuat(&mInitialQuat, info);
    mCurrentQuat = mInitialQuat;
    tryGetArg(&mClockAngleDegree, info, "ClockAngleDegree");
    tryGetArg(&mRotateAxis, info, "RotateAxis");
    tryGetArg(&mDelayTime, info, "DelayTime");
    tryGetArg(&mRotateSignTime, info, "RotateSignTime");
    tryGetArg(&mRotateTime, info, "RotateTime");
    tryGetArg(&mWaitTime, info, "WaitTime");

    if (mDelayTime == 0)
        initNerve(&NrvClockMovement.RotateSign, 0);
    else
        initNerve(&NrvClockMovement.Delay, 0);

    s32 stepsPerCycle = 1;
    if (mClockAngleDegree != 0) {
        stepsPerCycle = sead::Mathi::lcm(sead::Mathi::abs(mClockAngleDegree), 360) /
                        sead::Mathi::abs(mClockAngleDegree);
    }
    mMaxStepIndex = stepsPerCycle;
}

void ClockMovement::exeDelay() {
    if (isGreaterEqualStep(this, mDelayTime - 1)) {
        if (mRotateSignTime > 0)
            setNerve(this, &NrvClockMovement.RotateSign);
        else
            setNerve(this, &NrvClockMovement.Rotate);
    }
}

void ClockMovement::exeRotateSign() {
    f32 rotateAngle = wrapValue(static_cast<f32>(mCurrentStepIndex * mClockAngleDegree), 360.0f);

    rotateQuatLocalDirDegree(&mCurrentQuat, mInitialQuat, mRotateAxis,
                             rotateAngle +
                                 sead::Mathf::sin(getNerveStep(this) * sead::Mathf::pi2() / 18.0f));

    if (isGreaterEqualStep(this, mRotateSignTime - 1))
        setNerve(this, &NrvClockMovement.Rotate);
}

void ClockMovement::exeRotate() {
    f32 rotateAngle = wrapValue(
        (calcNerveRate(this, mRotateTime) + mCurrentStepIndex) * mClockAngleDegree, 360.0f);
    rotateQuatLocalDirDegree(&mCurrentQuat, mInitialQuat, mRotateAxis, rotateAngle);

    if (isGreaterEqualStep(this, mRotateTime)) {
        mCurrentStepIndex = wrapValue(mCurrentStepIndex + 1, mMaxStepIndex);
        setNerve(this, &Wait);
    }
}

void ClockMovement::exeWait() {
    if (isGreaterEqualStep(this, mWaitTime)) {
        if (mRotateSignTime > 0)
            setNerve(this, &NrvClockMovement.RotateSign);
        else
            setNerve(this, &NrvClockMovement.Rotate);
    }
}

bool ClockMovement::isFirstStepDelay() const {
    return isNerve(this, &NrvClockMovement.Delay) && isFirstStep(this);
}

bool ClockMovement::isFirstStepRotateSign() const {
    return isNerve(this, &NrvClockMovement.RotateSign) && isFirstStep(this);
}

bool ClockMovement::isFirstStepRotate() const {
    return isNerve(this, &NrvClockMovement.Rotate) && isFirstStep(this);
}

bool ClockMovement::isFirstStepWait() const {
    return isNerve(this, &Wait) && isFirstStep(this);
}

}  // namespace al
