#pragma once

#include <basis/seadTypes.h>

namespace al {

struct CameraObjectRequestInfo {
    bool isStopVerticalAbsorb;
    bool isResetPosition;
    bool isResetAngleV;
    bool isDownToDefaultAngleBySpeed;
    bool isUpToTargetAngleBySpeed;
    f32 targetAngleV;
    f32 angleSpeed;
    bool moveDownAngle;
    bool setAngleV;
    f32 angleV;
};

}  // namespace al
