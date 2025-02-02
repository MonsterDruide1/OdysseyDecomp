#include "Library/Camera/CameraViewInfo.h"

#include "Library/Camera/CameraViewFlag.h"
#include "Library/Projection/Projection.h"

namespace al {

const Projection& CameraViewInfo::getProjectionSead() const {
    return mProjection;
}

const sead::Matrix44f& CameraViewInfo::getProjMtx() const {
    return getProjectionSead().getProjMtx();
}

const sead::Matrix44f& CameraViewInfo::getProjMtxStd() const {
    return getProjectionSead().getMtxStd();
}

f32 CameraViewInfo::getAspect() const {
    return getProjectionSead().getAspect();
}

f32 CameraViewInfo::getNear() const {
    return getProjectionSead().getNear();
}

f32 CameraViewInfo::getFar() const {
    return getProjectionSead().getFar();
}

}  // namespace al
