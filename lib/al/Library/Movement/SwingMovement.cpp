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

bool SwingMovement::updateRotate() {
    s32 iVar2;
    s32 iVar3;
    f32 fVar5;
    f32 fVar6;
    f32 fVar7;

    fVar7 = (f32)_10 * 360.0f / (f32)mSwingCycle;
    fVar6 = sead::Mathf::abs(mSwingAngle);
    if (fVar6 < 180.0f) {
        fVar6 = sead::Mathf::sin(sead::Mathf::deg2rad(fVar7));
        _28 = fVar6 * mSwingAngle + mOffsetRotate;
        iVar2 = _10 - _10 / mSwingCycle * mSwingCycle;
        if (mSwingCycle <= -1)
            iVar3 = mSwingCycle + 3;
        else
            iVar3 = mSwingCycle;

        if (iVar2 == iVar3 >> 2)
            return true;

        if (mSwingCycle * 3 <= -1)
            iVar3 = mSwingCycle * 3 + 3;
        else
            iVar3 = mSwingCycle * 3;

        if (iVar2 == iVar3 >> 2)
            return true;

        return false;
    }

    fVar5 = sign(mSwingAngle);
    if (fVar6 < 360.0f) {
        fVar6 = modf(fVar7 + 90.0f + 180.0f, 180.0f);
        fVar6 = sead::Mathf::sin(sead::Mathf::deg2rad(fVar6 - 90.0f));
        fVar5 = fVar5 * fVar6 * 180.0f;

        _28 = mOffsetRotate + fVar5;

        return false;
    }

    fVar6 = modf(fVar7 + fVar7 + 360.0f, 360.0f);
    fVar5 = fVar5 * (fVar6 + 0.0f);

    _28 = mOffsetRotate + fVar5;

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
