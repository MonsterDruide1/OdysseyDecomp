#pragma once

#include <gfx/seadColor.h>
#include <math/seadMathCalcCommon.h>

namespace al {
class EffectLight {
public:
    void set(const sead::Mathi& pos, f32 intensity, const sead::Color4f& color, bool enabled);

private:
    sead::Mathi mPosition;
    f32 mIntensity;
    sead::Color4f mColor;
    bool mIsEnabled;
};
}  // namespace al
