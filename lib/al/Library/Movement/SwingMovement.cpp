#include "Library/Movement/SwingMovement.h"

#include "Library/Math/MathLengthUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"

namespace {
using namespace al;

NERVE_IMPL(SwingMovement, Move)
NERVE_IMPL(SwingMovement, Stop)

NERVES_MAKE_NOSTRUCT(SwingMovement, Move, Stop)
}  // namespace

namespace al {
SwingMovement::SwingMovement() : NerveExecutor("スイング動作計算") {
    initNerve(&Move, 0);
}

SwingMovement::SwingMovement(const ActorInitInfo& info) : NerveExecutor("スイング動作計算") {
    tryGetArg(&mSwingAngle, info, "SwingAngle");
    tryGetArg(&mSwingCycle, info, "SwingCycle");
    tryGetArg(&mDelayRate, info, "DelayRate");
    tryGetArg(&mStopTime, info, "StopTime");
    tryGetArg(&mOffsetRotate, info, "OffsetRotate");

    mFrameInCycle = (s32)(((f32)mDelayRate - 25.0f) / 100.0f * (f32)mSwingCycle);
    updateRotate();

    initNerve(&Move, 0);
}

bool SwingMovement::updateRotate() {
    f32 degree = (f32)mFrameInCycle * 360.0f / (f32)mSwingCycle;

    f32 swingAngle = sead::Mathf::abs(mSwingAngle);
    if (swingAngle < 180.0f) {
        mCurrentAngle =
            sead::Mathf::sin(sead::Mathf::deg2rad(degree)) * mSwingAngle + mOffsetRotate;

        return mFrameInCycle % mSwingCycle == mSwingCycle / 4 ||
               mFrameInCycle % mSwingCycle == 3 * mSwingCycle / 4;
    }

    f32 swingAngleSign = sign(mSwingAngle);
    if (swingAngle < 360.0f) {
        f32 rad = sead::Mathf::deg2rad(modf(degree + 90.0f + 180.0f, 180.0f) - 90.0f);
        mCurrentAngle = swingAngleSign * sead::Mathf::sin(rad) * 180.0f + mOffsetRotate;
    } else {
        mCurrentAngle = swingAngleSign * (modf(degree * 2 + 360.0f, 360.0f) + 0.0f) + mOffsetRotate;
    }

    return false;
}

void SwingMovement::exeMove() {
    if (updateRotate())
        setNerve(this, &Stop);

    mFrameInCycle = modi((mFrameInCycle + 1) + mSwingCycle, mSwingCycle);
}

void SwingMovement::exeStop() {
    if (isGreaterEqualStep(this, mStopTime))
        setNerve(this, &Move);
}

bool SwingMovement::isLeft() const {
    f32 angle = (f32)mFrameInCycle * 360.0f / (f32)mSwingCycle;

    return 90.0f <= angle && angle < 270.0f;
}

bool SwingMovement::isStop() const {
    return isNerve(this, &Stop);
}
}  // namespace al
