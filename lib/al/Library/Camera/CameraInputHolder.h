#pragma once

#include <basis/seadTypes.h>

namespace al {
class SimpleCameraInput;

class CameraInputHolder {
public:
    explicit CameraInputHolder(s32 size);
    void initAfterPlacement();
    SimpleCameraInput* getInput(s32 size) const;

private:
    SimpleCameraInput* mSimpleCameraInput = nullptr;
    SimpleCameraInput** mSimpleCameraInputArray;
    s32 mArraySize;
};  // class CameraInputHolder
}  // namespace al
