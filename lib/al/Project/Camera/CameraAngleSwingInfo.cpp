#include "Project/Camera/CameraAngleSwingInfo.h"

#include "Library/Math/MathUtil.h"
#include "Library/Yaml/ByamlUtil.h"

namespace al {

CameraAngleSwingInfo::CameraAngleSwingInfo() {
    // these need to be explicit in this constructor, otherwise mismatch
    mCurrentAngle = {0.0f, 0.0f};
    _14 = 0.3f;
    _18 = 0.1f;
}

void CameraAngleSwingInfo::load(const ByamlIter& iter) {
    tryGetByamlBool(&mIsInvalidSwing, iter, "IsInvalidSwing");
    if (mIsInvalidSwing)
        return;

    tryGetByamlF32(&mMaxSwingDegreeH, iter, "MaxSwingDegreeH");
    tryGetByamlF32(&mMaxSwingDegreeV, iter, "MaxSwingDegreeV");
}

void CameraAngleSwingInfo::update(const sead::Vector2f& stickInput, f32 stickSensitivity) {
    if (mIsInvalidSwing) {
        mCurrentAngle = {0.0f, 0.0f};
        return;
    }

    sead::Vector2f swingDegrees = {
        -stickInput.x * mMaxSwingDegreeH,
        stickInput.y * mMaxSwingDegreeV,
    };

    lerpVec(&swingDegrees, mCurrentAngle, swingDegrees, _14 * stickSensitivity);
    lerpVec(&mCurrentAngle, mCurrentAngle, swingDegrees, _18);
}

void CameraAngleSwingInfo::makeLookAtCamera(sead::LookAtCamera* camera) const {
    sead::Vector3f cameraLookDirection = camera->getAt() - camera->getPos();
    f32 cameraLookDistance = cameraLookDirection.length();
    normalize(&cameraLookDirection);
    sead::Vector3f cameraLookHDir = sead::Vector3f::ey;
    verticalizeVec(&cameraLookHDir, cameraLookDirection, cameraLookHDir);
    if (!tryNormalizeOrZero(&cameraLookHDir))
        return;

    rotateVectorDegree(&cameraLookDirection, cameraLookDirection, cameraLookHDir, mCurrentAngle.x);
    normalize(&cameraLookDirection);
    sead::Vector3f cameraSideDir;
    cameraSideDir.setCross(cameraLookDirection, cameraLookHDir);
    normalize(&cameraSideDir);
    rotateVectorDegree(&cameraLookDirection, cameraLookDirection, cameraSideDir, mCurrentAngle.y);
    camera->setAt((cameraLookDistance * cameraLookDirection) + camera->getPos());
}

}  // namespace al
