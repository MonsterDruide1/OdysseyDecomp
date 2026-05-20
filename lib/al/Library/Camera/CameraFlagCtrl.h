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
    bool isInvalidEndEntraceCamera = false;

    void invalidateEndEntranceCamera() { isInvalidEndEntraceCamera = true; }

    void validateEndEntranceCamera() { isInvalidEndEntraceCamera = false; }
};

}  // namespace al
