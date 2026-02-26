#include "Library/Bgm/DspValueController.h"

#include <math/seadMathCalcCommon.h>

namespace al {
DspLinearValueController::DspLinearValueController(f32 value) : mCurrent(value), mTarget(value) {}

void DspLinearValueController::init(f32 value) {
    mCurrent = value;
    mTarget = value;
}

void DspLinearValueController::update() {
    if (mStep > 0.0f && mTarget > mCurrent) {
        mCurrent += mStep;
        if (mTarget < mCurrent)
            mCurrent = mTarget;
        return;
    }

    if (mStep < 0.0f && mTarget < mCurrent) {
        mCurrent += mStep;
        if (mTarget > mCurrent)
            mCurrent = mTarget;
    }
}

void DspLinearValueController::changeTarget(f32 target, s32 stepCount) {
    mTarget = target;
    f32 difference = target - mCurrent;

    if (stepCount <= 0) {
        mCurrent = target;
        mStep = difference;
        return;
    }

    mStep = difference / stepCount;
}

// NON_MATCHING: https://decomp.me/scratch/q2k8I
DspSinValueController::DspSinValueController(f32 updateFreq, f32 value)
    : mUpdateFreq(updateFreq), mCurrent(value) {
    mAmplitude = new DspLinearValueController(0.0f);
}

void DspSinValueController::init(f32 value) {
    mCurrent = value;
    mAmplitude->init(0.0f);
}

void DspSinValueController::update() {
    if (mFreq <= 0.0f)
        return;

    mAmplitude->update();

    mCurrent = sead::Mathf::sin(mAngle) * mAmplitude->getCurrent();
    mAngle += mDelta;

    while (mAngle >= sead::Mathf::pi2())
        mAngle -= sead::Mathf::pi2();
}

void DspSinValueController::changeTarget(f32 target, s32 stepCount) {
    mAmplitude->changeTarget(target, stepCount);
}

void DspSinValueController::changeFreq(f32 frequency) {
    mFreq = frequency;
    mAngle = 0.0f;
    mCurrent = 0.0f;
    mDelta = sead::Mathf::pi2() / mUpdateFreq * mFreq;
}

}  // namespace al
