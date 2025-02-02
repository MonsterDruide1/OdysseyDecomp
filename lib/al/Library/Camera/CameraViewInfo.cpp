#include "Library/Camera/CameraViewInfo.h"

#include "Library/Projection/Projection.h"

namespace al {

CameraViewInfo::CameraViewInfo(s32 num, const sead::LookAtCamera& lookAtCam,
                               const Projection& projection, const CameraViewFlag& flag,
                               const OrthoProjectionInfo& orthoProjectionInfo)
    : mIndex(num), _6(false), _5(true), mIsValid(true), mLookAtCam(lookAtCam),
      mProjection(projection), mCamViewFlag(flag), mOrthoProjectionInfo(orthoProjectionInfo) {}

const Projection* CameraViewInfo::getProjectionSead() const {
    return &mProjection;
}

const sead::Matrix44f& CameraViewInfo::getProjMtx() const {
    return mProjection.getProjMtx();
}

const sead::Matrix44f& CameraViewInfo::getProjMtxStd() const {
    return mProjection.getMtxStd();
}

f32 CameraViewInfo::getAspect() const {
    return mProjection.getAspect();
}

f32 CameraViewInfo::getNear() const {
    return mProjection.getNear();
}

f32 CameraViewInfo::getFar() const {
    return mProjection.getFar();
}

// namespace al
}  // namespace al
