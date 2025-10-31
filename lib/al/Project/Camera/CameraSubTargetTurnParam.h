#pragma once

#include <basis/seadTypes.h>

namespace al {
struct ActorInitInfo;

class CameraSubTargetTurnParam {
public:
    CameraSubTargetTurnParam() {}

    void init(const ActorInitInfo& info);

    f32 getValidTurnDegreeRangeH() const { return mValidTurnDegreeRangeH; }

    f32 getValidFaceDegreeRangeH() const { return mValidFaceDegreeRangeH; }

    f32 getTurnSpeedRate1() const { return mTurnSpeedRate1; }

    f32 getTurnSpeedRate2() const { return mTurnSpeedRate2; }

    s32 getTargetTurnRestartStep() const { return mTargetTurnRestartStep; }

    f32 getTurnStopStartDistance() const { return mTurnStopStartDistance; }

    f32 getTurnBrakeEndDistance() const { return mTurnBrakeEndDistance; }

    f32 getTurnBrakeStartDistance() const { return mTurnBrakeStartDistance; }

    f32 getTurnStopEndDistance() const { return mTurnStopEndDistance; }

    bool isTurnV() const { return mIsTurnV; }

    bool isResetAfterTurnV() const { return mIsResetAfterTurnV; }

    f32 getMinTurnDegreeV() const { return mMinTurnDegreeV; }

    f32 getMaxTurnDegreeV() const { return mMaxTurnDegreeV; }

private:
    f32 mTurnSpeedRate1 = 0.3f;
    f32 mTurnSpeedRate2 = 0.1f;
    f32 mTurnBrakeEndDistance = -1.0f;
    f32 mTurnBrakeStartDistance = -1.0f;
    f32 mTurnStopStartDistance = -1.0f;
    f32 mTurnStopEndDistance = -1.0f;
    s32 mTargetTurnRestartStep = 30;
    bool mIsTurnV = false;
    bool mIsResetAfterTurnV = false;
    f32 mMinTurnDegreeV = 0.0f;
    f32 mMaxTurnDegreeV = 0.0f;
    f32 mValidTurnDegreeRangeH = -1.0f;
    f32 mValidFaceDegreeRangeH = -1.0f;
};
}  // namespace al
