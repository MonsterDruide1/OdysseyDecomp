#include "Library/Camera/SimpleCameraInput.h"

#include "Library/Controller/InputFunction.h"

namespace al {

SimpleCameraInput::SimpleCameraInput(s32 port) : mPort(port) {
    if (mPort < 0)
        mPort = getTouchPanelPort();
}

void SimpleCameraInput::calcInputStick(sead::Vector2f* inputStick) const {
    *inputStick = {0.0f, 0.0f};
    inputStick->set(getRightStick(mPort));
}

bool SimpleCameraInput::isTriggerReset() const {
    return isPadTriggerL(mPort) || isPadTriggerR(mPort);
}

bool SimpleCameraInput::isHoldZoom() const {
    // has to be repeated here to avoid virtual calls
    return isPadHoldZL(mPort) || isPadHoldZR(mPort);
}

bool SimpleCameraInput::isHoldSnapShotZoomIn() const {
    return isPadHoldZL(mPort);
}

bool SimpleCameraInput::isHoldSnapShotZoomOut() const {
    return isPadHoldZR(mPort);
}

}  // namespace al
