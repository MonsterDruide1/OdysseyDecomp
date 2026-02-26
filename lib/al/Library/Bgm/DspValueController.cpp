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
DspSinValueController::DspSinValueController(f32 shift, f32 amplitude)
    : mShift(shift), mAmplitude(amplitude) {
    mLinearValue = new DspLinearValueController(0.0f);
}

void DspSinValueController::init(f32 amplitude) {
    mAmplitude = amplitude;
    mLinearValue->init(0.0f);
}

void DspSinValueController::update() {
    if (mFreq <= 0.0f)
        return;

    mLinearValue->update();

    mAmplitude = sead::Mathf::sin(mAngle) * mLinearValue->getCurrent();
    mAngle += mDelta;

    while (sead::Mathf::pi2() <= mAngle)
        mAngle -= sead::Mathf::pi2();
}

void DspSinValueController::changeTarget(f32 target, s32 stepCount) {
    mLinearValue->changeTarget(target, stepCount);
}

void DspSinValueController::changeFreq(f32 frequency) {
    mFreq = frequency;
    mAngle = 0.0f;
    mAmplitude = 0.0f;
    mDelta = sead::Mathf::pi2() / mShift * mFreq;
}

}  // namespace al
