#include "Project/Camera/CameraAngleSwingInfo.h"

#include "Library/Math/MathUtil.h"
#include "Library/Yaml/ByamlUtil.h"

namespace al {

CameraAngleSwingInfo::CameraAngleSwingInfo() {
    // these need to be explicit in this constructor, otherwise mismatch
    currentAngle = {0.0f, 0.0f};
    _14 = 0.3f;
    _18 = 0.1f;
}

void CameraAngleSwingInfo::load(const ByamlIter& iter) {
    tryGetByamlBool(&isInvalidSwing, iter, "IsInvalidSwing");
    if (isInvalidSwing)
        return;

    tryGetByamlF32(&maxSwingDegreeH, iter, "MaxSwingDegreeH");
    tryGetByamlF32(&maxSwingDegreeV, iter, "MaxSwingDegreeV");
}

void CameraAngleSwingInfo::update(const sead::Vector2f& stickInput, f32 stickSensitivity) {
    if (isInvalidSwing) {
        currentAngle = {0.0f, 0.0f};
        return;
    }

    sead::Vector2f swingDegrees = {
        -stickInput.x * maxSwingDegreeH,
        stickInput.y * maxSwingDegreeV,
    };

    lerpVec(&swingDegrees, currentAngle, swingDegrees, _14 * stickSensitivity);
    lerpVec(&currentAngle, currentAngle, swingDegrees, _18);
}

void CameraAngleSwingInfo::makeLookAtCamera(sead::LookAtCamera* camera) const {
    sead::Vector3f cameraLookDirection = camera->getAt() - camera->getPos();
    f32 cameraLookDistance = cameraLookDirection.length();
    normalize(&cameraLookDirection);
    sead::Vector3f cameraLookHDir = sead::Vector3f::ey;
    verticalizeVec(&cameraLookHDir, cameraLookDirection, cameraLookHDir);
    if (!tryNormalizeOrZero(&cameraLookHDir))
        return;

    rotateVectorDegree(&cameraLookDirection, cameraLookDirection, cameraLookHDir, currentAngle.x);
    normalize(&cameraLookDirection);
    sead::Vector3f cameraSideDir;
    cameraSideDir.setCross(cameraLookDirection, cameraLookHDir);
    normalize(&cameraSideDir);
    rotateVectorDegree(&cameraLookDirection, cameraLookDirection, cameraSideDir, currentAngle.y);
    camera->setAt((cameraLookDistance * cameraLookDirection) + camera->getPos());
}

}  // namespace al
