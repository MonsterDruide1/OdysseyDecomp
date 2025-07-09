#include "EffectLight.h"

#include "gfx/seadColor.h"
#include "math/seadVector.h"

namespace al {
    void EffectLight::set(const sead::Vector3<float>& pos, float intensity, const sead::Color4f& color, bool enabled) {
        mPosition = pos;
        mIntensity = intensity;
        mColor = color;
        mEnabled = enabled;
    }
}