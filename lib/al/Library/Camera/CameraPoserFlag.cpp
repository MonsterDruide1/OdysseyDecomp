#include "Library/Camera/CameraPoserFlag.h"

#include "Library/Yaml/ByamlUtil.h"

namespace al {

CameraPoserFlag::CameraPoserFlag() = default;

void CameraPoserFlag::load(const ByamlIter& iter) {
    tryGetByamlBool(&mIsInvalidChangeSubjective, iter, "IsInvalidChangeSubjective");
    tryGetByamlBool(&mIsValidKeepPreSelfPoseNextCameraByParam, iter,
                    "IsValidKeepPreSelfPoseNextCameraByParam");
    tryGetByamlBool(&mIsInvalidKeepPreSelfPoseNextCameraOverWriteProgram, iter,
                    "IsInvalidKeepPreSelfPoseNextCameraOverWriteProgram");
    tryGetByamlBool(&mIsInvalidKeepDistanceNextCamera, iter, "IsInvalidKeepDistanceNextCamera");
}

bool CameraPoserFlag::isValidKeepPreSelfPoseNextCamera() const {
    return mIsOverWriteProgram ? !mIsInvalidKeepPreSelfPoseNextCameraOverWriteProgram :
                                 mIsValidKeepPreSelfPoseNextCameraByParam;
}
}  // namespace al
