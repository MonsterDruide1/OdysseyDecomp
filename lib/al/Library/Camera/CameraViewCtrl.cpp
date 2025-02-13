#include "Library/Camera/CameraViewCtrl.h"

namespace al {

PauseCameraCtrl::PauseCameraCtrl(f32 v) : mIsPause(false), _4(v) {}

SceneCameraViewCtrl::SceneCameraViewCtrl() : _8("Start") {}

SceneCameraCtrl::SceneCameraCtrl() : mViewNumMax(0), _10(0), _18(0) {}

void SceneCameraCtrl::init(s32 viewNum) {
    mViewNumMax = viewNum;
    mViewArray = new SceneCameraViewCtrl[viewNum];
}

}  // namespace al
