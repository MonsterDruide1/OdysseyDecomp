#include "Library/Camera/CameraViewCtrlScene.h"

namespace al {

SceneCameraViewCtrl::SceneCameraViewCtrl() = default;

SceneCameraCtrl::SceneCameraCtrl() = default;

void SceneCameraCtrl::init(s32 viewNum) {
    mViewNum = viewNum;
    mViewArray = new SceneCameraViewCtrl[viewNum];
}

}  // namespace al
