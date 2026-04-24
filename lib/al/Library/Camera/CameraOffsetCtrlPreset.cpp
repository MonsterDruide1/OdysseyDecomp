#include "Library/Camera/CameraOffsetCtrlPreset.h"

#include "Library/Camera/CameraOffsetPreset.h"
#include "Library/Yaml/ByamlUtil.h"

namespace al {

void CameraOffsetCtrlY::load(const ByamlIter& iter) {
    tryGetByamlF32(&mOffset.y, iter, "OffsetY");
}

CameraOffsetCtrlPreset::CameraOffsetCtrlPreset() : mPreset(nullptr) {
    mPreset = new CameraOffsetPreset();
}

void CameraOffsetCtrlPreset::load(const ByamlIter& iter) {
    mPreset->loadParam(iter);
}

const sead::Vector3f& CameraOffsetCtrlPreset::getOffset() const {
    return mPreset->getOffset();
}

const sead::Vector3f& CameraOffsetCtrlY::getOffset() const {
    return mOffset;
}

}  // namespace al
