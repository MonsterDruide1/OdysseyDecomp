#pragma once

#include "Library/Camera/SimpleCameraInput.h"

namespace al {
class CameraInputHolder {
public:
    CameraInputHolder(s32 size);
    void initAfterPlacement();
    SimpleCameraInput* getInput(s32 size) const;

private:
    SimpleCameraInput* mSimpleCameraInput;
    SimpleCameraInput** mCameraInputArray;
    s32 mArraySize;
};  // class CameraInputHolder
}  // namespace al
