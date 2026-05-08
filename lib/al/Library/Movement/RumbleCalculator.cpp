#include "Library/Movement/RumbleCalculator.h"

al::RumbleCalculator::RumbleCalculator(f32 frequency, f32 angleOffset, f32 amplitude, u32 maxFrame)
    : mFrame(maxFrame), mMaxFrame(maxFrame), mFrequency(frequency), mAngleOffset(angleOffset),
      mAmplitude(amplitude) {}

void al::RumbleCalculator::setParam(f32 frequency, f32 angleOffset, f32 amplitude, u32 maxFrame) {
    mFrequency = frequency;
    mAngleOffset = angleOffset;
    mAmplitude = amplitude;
    mMaxFrame = maxFrame;
}

void al::RumbleCalculator::start(u32 maxFrame) {
    if (maxFrame != 0)
        mMaxFrame = maxFrame;
    mFrame = 0;
    mOut.set(0.0f, 0.0f, 0.0f);
}

// NON-MATCHING: https://decomp.me/scratch/4Dxv4
void al::RumbleCalculator::calc() {
    if (mFrame >= mMaxFrame) {
        mOut.set(0.0f, 0.0f, 0.0f);
        return;
    }

    sead::Vector3f vec;
    f32 rate = (f32)mFrame / mMaxFrame;
    f32 invRate = 1.0f - rate;

    f32 angle = rate * sead::Mathf::pi2() * mFrequency;

    vec.x = angle;
    vec.y = angle;
    vec.y += mAngleOffset;
    vec.z = angle + mAngleOffset + mAngleOffset;
    calcValues(&mOut, vec);

    mOut *= invRate * mAmplitude;
    mFrame++;
}

void al::RumbleCalculator::reset() {
    mFrame = mMaxFrame;
    mOut.set(0.0f, 0.0f, 0.0f);
}

al::RumbleCalculatorCosAddOneMultLinear::RumbleCalculatorCosAddOneMultLinear(f32 frequency,
                                                                             f32 angleOffset,
                                                                             f32 amplitude,
                                                                             u32 maxFrame)
    : RumbleCalculator(frequency, angleOffset, amplitude, maxFrame) {}

void al::RumbleCalculatorCosAddOneMultLinear::calcValues(sead::Vector3f* out,
                                                         const sead::Vector3f& in) {
    out->set(cosf(in.x) + 1.0f, cosf(in.y) + 1.0f, cosf(in.z) + 1.0f);
}

al::RumbleCalculatorCosMultLinear::RumbleCalculatorCosMultLinear(f32 frequency, f32 angleOffset,
                                                                 f32 amplitude, u32 maxFrame)
    : RumbleCalculator(frequency, angleOffset, amplitude, maxFrame) {}

void al::RumbleCalculatorCosMultLinear::calcValues(sead::Vector3f* out, const sead::Vector3f& in) {
    out->set(cosf(in.x), cosf(in.y), cosf(in.z));
}
