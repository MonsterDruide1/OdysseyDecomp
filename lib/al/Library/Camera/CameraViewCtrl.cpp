#include "Library/Camera/CameraViewCtrl.h"

namespace al {

PauseCameraCtrl::PauseCameraCtrl(f32 v) : _4(v) {}

SceneCameraViewCtrl::SceneCameraViewCtrl() = default;

SceneCameraCtrl::SceneCameraCtrl() = default;

void SceneCameraCtrl::init(s32 viewNum) {
    mViewNum = viewNum;
    mViewArray = new SceneCameraViewCtrl[viewNum];
}

}  // namespace al
