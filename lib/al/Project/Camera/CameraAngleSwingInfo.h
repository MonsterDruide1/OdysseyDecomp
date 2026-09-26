#pragma once

#include <gfx/seadCamera.h>
#include <math/seadVector.h>

namespace al {
class ByamlIter;

struct CameraAngleSwingInfo {
    CameraAngleSwingInfo();

    void load(const ByamlIter& iter);
    void update(const sead::Vector2f& stickInput, f32 stickSensitivity);
    void makeLookAtCamera(sead::LookAtCamera* camera) const;

    // all of them are default-initialized in the constructor, but some can't be inlined here
    bool isInvalidSwing = false;
    sead::Vector2f currentAngle;
    f32 maxSwingDegreeH = 15.0f;
    f32 maxSwingDegreeV = 15.0f;
    // unknown purpose at the moment, only seem to be read
    f32 _14;
    f32 _18;
};

}  // namespace al
