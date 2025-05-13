#pragma once

namespace al {

struct CameraFlagCtrl {
    CameraFlagCtrl();

    bool isCameraReverseInputH = false;
    bool isCameraReverseInputV = false;
    bool isValidCameraGyro = false;
    bool isSnapShotModeRunning = false;
    bool _4 = false;
    bool isValidCameraAreaKids = false;
    bool _6 = false;
};

}  // namespace al
