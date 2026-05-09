#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/HostIO/HioNode.h"

namespace al {
class RumbleCalculator : public HioNode {
public:
    RumbleCalculator(f32 frequency, f32 angleOffset, f32 amplitude, u32 maxFrame);
    void setParam(f32 frequency, f32 angleOffset, f32 amplitude, u32 maxFrame);
    void start(u32 maxFrame);
    void calc();
    void reset();

    virtual void calcValues(sead::Vector3f* out, const sead::Vector3f& in) = 0;

protected:
    u32 mFrame;
    u32 mMaxFrame;
    sead::Vector3f mOut = {0.0f, 0.0f, 0.0f};
    f32 mFrequency;
    f32 mAngleOffset;
    f32 mAmplitude;
};

class RumbleCalculatorCosAddOneMultLinear : public RumbleCalculator {
public:
    RumbleCalculatorCosAddOneMultLinear(f32 frequency, f32 angleOffset, f32 amplitude,
                                        u32 maxFrame);
    void calcValues(sead::Vector3f* out, const sead::Vector3f& in);
};

class RumbleCalculatorCosMultLinear : public RumbleCalculator {
public:
    RumbleCalculatorCosMultLinear(f32 frequency, f32 angleOffset, f32 amplitude, u32 maxFrame);
    void calcValues(sead::Vector3f* out, const sead::Vector3f& in);
};
}  // namespace al
