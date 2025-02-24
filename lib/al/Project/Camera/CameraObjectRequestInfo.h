#pragma once

#include <basis/seadTypes.h>

namespace al {

struct CameraObjectRequestInfo {
    bool mIsStopVerticalAbsorb;
    bool mIsResetPosition;
    bool mIsResetAngleV;
    bool mIsDownToDefaultAngleBySpeed;
    bool mIsUpToTargetAngleBySpeed;
    f32 mTargetAngleV;
    f32 mAngleSpeed;
    bool mMoveDownAngle;
    bool mSetAngleV;
    f32 mAngleV;
};

static_assert(sizeof(CameraObjectRequestInfo) == 0x18);

}  // namespace al
