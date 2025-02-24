#pragma once

#include <gfx/seadCamera.h>

namespace al {
class ByamlIter;

class CameraAngleSwingInfo {
public:
    CameraAngleSwingInfo();

    void load(const ByamlIter& iter);
    void update(const sead::Vector2f& stickInput, f32 stickSensitivity);
    void makeLookAtCamera(sead::LookAtCamera* camera) const;

    bool isInvalidSwing() const { return mIsInvalidSwing; }

    void setCurrentAngle(sead::Vector2f currentAngle) { mCurrentAngle = currentAngle; }

    // all of them are default-initialized in the constructor, but some can't be inlined here
private:
    bool mIsInvalidSwing = false;
    sead::Vector2f mCurrentAngle;
    f32 mMaxSwingDegreeH = 15.0f;
    f32 mMaxSwingDegreeV = 15.0f;
    // unknown purpose at the moment, only seem to be read
    f32 _14;
    f32 _18;
};

}  // namespace al
