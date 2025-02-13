#include "Library/Camera/CameraPoserFix.h"

#include "Library/Yaml/ByamlUtil.h"

namespace al {
const char* CameraPoserFix::getFixAbsoluteCameraName() {
    return "完全固定";
}

const char* CameraPoserFix::getFixDoorwayCameraName() {
    return "出入口専用固定";
}

void CameraPoserFix::loadParam(const ByamlIter& iter) {
    tryGetByamlV3f(&mLookAtPos, iter, "LookAtPos");
    tryGetByamlF32(&mDistance, iter, "Distance");
    tryGetByamlF32(&mAngleV, iter, "AngleV");
    tryGetByamlF32(&mAngleH, iter, "AngleH");
    tryGetByamlBool(&mIsCalcNearestAtFromPreAt, iter, "IsCalcNearestAtFromPreAt");
}

}  // namespace al
