#pragma once

namespace al {

struct CameraFlagCtrl {
    CameraFlagCtrl();

    bool isCameraReverseInputH = false;
    bool isCameraReverseInputV = false;
    bool isInvalidCameraGyro = false;
    bool isSnapShotModeRunning = false;
    bool isValidCameraAreaKids = false;
    bool isSeparatePlayMode = false;
    bool isInvalidEndEntranceCamera = false;
};

static_assert(sizeof(CameraFlagCtrl) == 0x7);

}  // namespace al
