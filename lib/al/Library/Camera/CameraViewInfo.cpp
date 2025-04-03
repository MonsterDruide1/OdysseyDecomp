#include "Library/Camera/CameraViewInfo.h"

#include "Library/Projection/Projection.h"

namespace al {

CameraViewInfo::CameraViewInfo(s32 index, const sead::LookAtCamera& lookAtCam,
                               const Projection& projection, const CameraViewFlag& flag,
                               const OrthoProjectionInfo& orthoProjectionInfo)
    : mIndex(index), mLookAtCam(lookAtCam), mProjection(projection), mViewFlag(flag),
      mOrthoProjectionInfo(orthoProjectionInfo) {}

const sead::Projection& CameraViewInfo::getProjectionSead() const {
    return mProjection.getProjectionSead();
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

}  // namespace al
