#include "Library/Camera/CameraInputHolder.h"

#include <basis/seadTypes.h>

#include "Library/Camera/SimpleCameraInput.h"

al::CameraInputHolder::CameraInputHolder(s32 size) : mInputsSize(size) {
    mInputs = new SimpleCameraInput*[mInputsSize];
    for (s32 index = 0; index < mInputsSize; index++)
        mInputs[index] = nullptr;
}

void al::CameraInputHolder::initAfterPlacement() {
    if (mDefaultInput == nullptr)
        mDefaultInput = new SimpleCameraInput(-1);
}

al::SimpleCameraInput* al::CameraInputHolder::getInput(s32 index) const {
    return mInputs[index] ?: mDefaultInput;
}
