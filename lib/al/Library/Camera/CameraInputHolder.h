#pragma once

namespace al {
class SimpleCameraInput;

class CameraInputHolder {
public:
    CameraInputHolder(s32 size);
    void initAfterPlacement();
    SimpleCameraInput* getInput(s32 index) const;

private:
    SimpleCameraInput* mDefaultInput = nullptr;
    SimpleCameraInput** mInputs;
    s32 mInputsSize;
};  // class CameraInputHolder
}  // namespace al
