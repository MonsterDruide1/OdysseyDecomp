#include "Library/Movement/SwingMovement.h"

#include "Library/Math/MathLengthUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
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

    _10 = (s32)(((f32)mDelayRate - 25.0f) / 100.0f * (f32)mSwingCycle);
    updateRotate();

    initNerve(&Move, 0);
}

inline s32 test(s32 a) {
    return (a < 0 ? a + 3 : a) >> 2;
}

// NON_MATCHING: regswap + wrong register https://decomp.me/scratch/qM0wp
bool SwingMovement::updateRotate() {
    f32 fVar7 = 360.0f * (f32)_10 / (f32)mSwingCycle;
    f32 fVar6 = sead::Mathf::abs(mSwingAngle);

    if (fVar6 < 180.0f) {
        _28 = sead::Mathf::sin(sead::Mathf::deg2rad(fVar7)) * mSwingAngle + mOffsetRotate;
        s32 iVar2 = _10 - _10 / mSwingCycle * mSwingCycle;

        return iVar2 == test(mSwingCycle) || iVar2 == test(mSwingCycle * 3);
    }

    f32 fVar5 = sign(mSwingAngle);
    if (fVar6 < 360.0f) {
        _28 = fVar5 *
                  sead::Mathf::sin(
                      sead::Mathf::deg2rad(modf(fVar7 + 90.0f + 180.0f, 180.0f) - 90.0f)) *
                  180.0f +
              mOffsetRotate;
    } else {
        _28 = fVar5 * (modf(fVar7 * 2 + 360.0f, 360.0f) + 0.0f) + mOffsetRotate;
    }

    return false;
}

void SwingMovement::exeMove() {
    if (updateRotate())
        setNerve(this, &Stop);

    _10 = modi(_10 + mSwingCycle + 1, mSwingCycle);
}

void SwingMovement::exeStop() {
    if (isGreaterEqualStep(this, mStopTime))
        setNerve(this, &Move);
}

bool SwingMovement::isLeft() const {
    f32 angle = (f32)_10 * 360.0f / (f32)mSwingCycle;

    return 90.0f <= angle && angle < 270.0f;
}

bool SwingMovement::isStop() const {
    return isNerve(this, &Stop);
}
}  // namespace al
