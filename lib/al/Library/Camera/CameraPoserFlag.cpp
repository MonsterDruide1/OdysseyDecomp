#include "Library/Camera/CameraPoserFlag.h"

#include "Library/Yaml/ByamlUtil.h"

namespace al {

CameraPoserFlag::CameraPoserFlag() = default;

void CameraPoserFlag::load(const ByamlIter& iter) {
    tryGetByamlBool(&isInvalidChangeSubjective, iter, "IsInvalidChangeSubjective");
    tryGetByamlBool(&isValidKeepPreSelfPoseNextCameraByParam, iter,
                    "IsValidKeepPreSelfPoseNextCameraByParam");
    tryGetByamlBool(&isInvalidKeepPreSelfPoseNextCameraOverWriteProgram, iter,
                    "IsInvalidKeepPreSelfPoseNextCameraOverWriteProgram");
    tryGetByamlBool(&isInvalidKeepDistanceNextCamera, iter, "IsInvalidKeepDistanceNextCamera");
}

bool CameraPoserFlag::isValidKeepPreSelfPoseNextCamera() const {
    return isOverWriteProgram ? !isInvalidKeepPreSelfPoseNextCameraOverWriteProgram :
                                isValidKeepPreSelfPoseNextCameraByParam;
}
}  // namespace al
