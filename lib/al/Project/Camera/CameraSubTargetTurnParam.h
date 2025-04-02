#pragma once

#include <basis/seadTypes.h>

namespace al {
struct ActorInitInfo;

class CameraSubTargetTurnParam {
public:
    CameraSubTargetTurnParam() {}

    void init(const ActorInitInfo& info);

private:
    f32 mTurnSpeedRate1 = 0.3f;
    f32 mTurnSpeedRate2 = 0.1f;
    f32 mTurnBrakeEndDistance = -1.0f;
    f32 mTurnBrakeStartDistance = -1.0f;
    f32 mTurnStopStartDistance = -1.0f;
    f32 mTurnStopEndDistance = -1.0f;
    s32 _18 = 30;
    bool mIsTurnV = false;
    bool mIsResetAfterTurnV = false;
    f32 mMinTurnDegreeV = 0.0f;
    f32 mMaxTurnDegreeV = 0.0f;
    f32 mValidTurnDegreeRangeH = -1.0f;
    f32 mValidFaceDegreeRangeH = -1.0f;
};
}  // namespace al
