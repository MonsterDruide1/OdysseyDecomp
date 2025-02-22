#pragma once

#include <basis/seadTypes.h>

namespace al {

struct CameraObjectRequestInfo {
    bool mIsStopVerticalAbsorb;
    bool mIsResetPosition;
    bool mIsResetAngleV;
    bool mIsDownToDefaultAngleBySpeed;
    f32 _;
    f32 mIargetAngleV;
    f32 mAngleSpeed;
    bool mMoveDownAngle;
    bool mSetAngleV;
    f32 mAngleV;
};

}  // namespace al
