#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
class RumbleCalculator {
public:
    RumbleCalculator(f32, f32, f32, u32);
    void setParam(f32, f32, f32, u32);
    void start(u32);
    void calc();
    void reset();

    virtual void calcValues(sead::Vector3<f32>*, const sead::Vector3<f32>&) = 0;

protected:
    u32 mFrame;
    u32 mRampTime;
    sead::Vector3<f32> mOut;
    f32 mFrequency;
    f32 mAngleDev;
    f32 mAmplitude;
};

class RumbleCalculatorCosAddOneMultLinear : RumbleCalculator {
    RumbleCalculatorCosAddOneMultLinear(f32, f32, f32, u32);
    void calcValues(sead::Vector3<f32>*, const sead::Vector3<f32>&);
};

class RumbleCalculatorCosMultLinear : RumbleCalculator {
    RumbleCalculatorCosMultLinear(f32, f32, f32, u32);
    void calcValues(sead::Vector3<f32>*, const sead::Vector3<f32>&);
};
}  // namespace al
