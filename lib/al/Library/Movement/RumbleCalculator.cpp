#include "Library/Movement/RumbleCalculator.h"

al::RumbleCalculator::RumbleCalculator(f32 frequency, f32 angleDev, f32 amplitude, u32 rampTime) {
    mFrame = rampTime;
    mRampTime = rampTime;
    mOut.set(0.0f, 0.0f, 0.0f);
    mFrequency = frequency;
    mAngleDev = angleDev;
    mAmplitude = amplitude;
}

void al::RumbleCalculator::setParam(f32 frequency, f32 angleDev, f32 amplitude, u32 rampTime) {
    mFrequency = frequency;
    mAngleDev = angleDev;
    mAmplitude = amplitude;
    mRampTime = rampTime;
}

void al::RumbleCalculator::start(u32 rampTime) {
    if (rampTime != 0)
        mRampTime = rampTime;
    mFrame = 0;
    mOut.set(0.0f, 0.0f, 0.0f);
}

void al::RumbleCalculator::calc() {
    if (mFrame >= mRampTime) {
        mOut.set(0.0f, 0.0f, 0.0f);
        return;
    }

    sead::Vector3f vec;
    f32 rate = (f32)mFrame / mRampTime;
    f32 invRate = 1.0f - rate;

    f32 angle = rate * sead::Mathf::pi2() * mFrequency;

    vec.x = angle;
    vec.y = angle;
    vec.y += mAngleDev;
    vec.z = mAngleDev + vec.y;
    calcValues(&mOut, vec);

    mOut *= invRate * mAmplitude;
    mFrame++;
}

void al::RumbleCalculator::reset() {
    mFrame = mRampTime;
    mOut.x = 0;
    mOut.y = 0;
    mOut.z = 0;
}

al::RumbleCalculatorCosAddOneMultLinear::RumbleCalculatorCosAddOneMultLinear(f32 frequency,
                                                                             f32 angleDev,
                                                                             f32 amplitude,
                                                                             u32 rampTime)
    : RumbleCalculator(frequency, angleDev, amplitude, rampTime) {}

void al::RumbleCalculatorCosAddOneMultLinear::calcValues(sead::Vector3f* mOut,
                                                         const sead::Vector3f& in) {
    f32 x = cosf(in.x) + 1;
    f32 y = cosf(in.y) + 1;
    f32 z = cosf(in.z) + 1;
    mOut->x = x;
    mOut->y = y;
    mOut->z = z;
}

al::RumbleCalculatorCosMultLinear::RumbleCalculatorCosMultLinear(f32 frequency, f32 angleDev,
                                                                 f32 amplitude, u32 rampTime)
    : RumbleCalculator(frequency, angleDev, amplitude, rampTime) {}

void al::RumbleCalculatorCosMultLinear::calcValues(sead::Vector3f* mOut,
                                                   const sead::Vector3f& in) {
    f32 x = cosf(in.x);
    f32 y = cosf(in.y);
    f32 z = cosf(in.z);
    mOut->x = x;
    mOut->y = y;
    mOut->z = z;
}
