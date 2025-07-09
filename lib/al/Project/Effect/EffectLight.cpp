#include "Project/Effect/EffectLight.h"

#include <math/seadMathCalcCommon.h>

namespace al {
void EffectLight::set(const sead::Mathi& pos, f32 intensity, const sead::Color4f& color,
                      bool enabled) {
    mPosition = pos;
    mIntensity = intensity;
    mColor = color;
    mIsEnabled = enabled;
}
}  // namespace al
