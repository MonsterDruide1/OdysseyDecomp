#pragma once

#include <gfx/seadColor.h>
#include <math/seadVector.h>

namespace al {
class EffectLight {
public:
    void set(const sead::Vector3f& pos, f32 intensity, const sead::Color4f& color, bool enabled);

private:
    // TODO: The member variables are guesses based on this being a effect light object. 
    sead::Vector3f mPosition;
    f32 mIntensity;
    sead::Color4f mColor;
    bool mIsEnabled;
};
}  // namespace al
