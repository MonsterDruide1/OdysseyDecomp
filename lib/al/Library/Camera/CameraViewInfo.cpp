#include "Library/Camera/CameraViewInfo.h"

#include "Library/Projection/Projection.h"

namespace al {

CameraViewInfo::CameraViewInfo(s32 index, const sead::LookAtCamera& lookAtCam,
                               const Projection& projection, const CameraViewFlag& flag,
                               const OrthoProjectionInfo& orthoProjectionInfo)
    : index(index), lookAtCam(lookAtCam), projection(projection), flag(flag),
      orthoProjectionInfo(orthoProjectionInfo) {}

const sead::Projection& CameraViewInfo::getProjectionSead() const {
    return projection.getProjectionSead();
}

const sead::Matrix44f& CameraViewInfo::getProjMtx() const {
    return projection.getProjMtx();
}

const sead::Matrix44f& CameraViewInfo::getProjMtxStd() const {
    return projection.getMtxStd();
}

f32 CameraViewInfo::getAspect() const {
    return projection.getAspect();
}

f32 CameraViewInfo::getNear() const {
    return projection.getNear();
}

f32 CameraViewInfo::getFar() const {
    return projection.getFar();
}

}  // namespace al
