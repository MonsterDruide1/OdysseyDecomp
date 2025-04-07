#pragma once

#include "Library/Camera/ICameraInput.h"

namespace al {

class SimpleCameraInput : public ICameraInput {
public:
    SimpleCameraInput(s32 port = -1);

    void calcInputStick(sead::Vector2f* inputStick) const override;
    bool isTriggerReset() const override;
    bool isHoldZoom() const override;
    bool isHoldSnapShotZoomIn() const override;
    bool isHoldSnapShotZoomOut() const override;

private:
    s32 mPort;
};

}  // namespace al
