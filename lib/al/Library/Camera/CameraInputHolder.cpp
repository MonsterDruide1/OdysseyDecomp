#include "Library/Camera/CameraInputHolder.h"

#include <basis/seadTypes.h>

#include "Library/Camera/SimpleCameraInput.h"

al::CameraInputHolder::CameraInputHolder(const s32 size) {
    mArraySize = size;
    mSimpleCameraInputArray = new SimpleCameraInput*[mArraySize];
    if (mArraySize > 0)
        for (s64 index = 0; index < mArraySize; index++)
            mSimpleCameraInputArray[index] = nullptr;
}

void al::CameraInputHolder::initAfterPlacement() {
    if (mSimpleCameraInput == nullptr) {
        auto* simpleCameraInput = new SimpleCameraInput(-1);

        mSimpleCameraInput = simpleCameraInput;
    }
}

al::SimpleCameraInput* al::CameraInputHolder::getInput(const s32 size) const {
    al::SimpleCameraInput* simpleCameraInput = mSimpleCameraInputArray[size];

    if (simpleCameraInput == nullptr)
        return mSimpleCameraInput;
    return simpleCameraInput;
}
