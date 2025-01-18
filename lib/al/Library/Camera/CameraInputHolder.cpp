#include "Library/Camera/CameraInputHolder.h"

#include <basis/seadTypes.h>

#include "Library/Camera/SimpleCameraInput.h"

al::CameraInputHolder::CameraInputHolder(const s32 size) {
    SimpleCameraInput** inputs{};

    mSimpleCameraInput = nullptr;
    mArraySize = size;

    inputs = new SimpleCameraInput*[mArraySize];

    mCameraInputArray = inputs;

    if (mArraySize > 0) {
        s64 index = 0;
        do {
            mCameraInputArray[index] = nullptr;
            index++;
        } while (index < mArraySize);
    }
}

void al::CameraInputHolder::initAfterPlacement() {
    if (mSimpleCameraInput == nullptr) {
        auto* simpleCameraInput = new SimpleCameraInput(-1);

        mSimpleCameraInput = simpleCameraInput;
    }
}

al::SimpleCameraInput* al::CameraInputHolder::getInput(const s32 size) const {
    al::SimpleCameraInput* simpleCameraInput = mCameraInputArray[size];

    if (simpleCameraInput == nullptr)
        return mSimpleCameraInput;
    return simpleCameraInput;
}
