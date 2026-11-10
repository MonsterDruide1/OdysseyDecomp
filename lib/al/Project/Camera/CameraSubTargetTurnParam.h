#pragma once

#include <basis/seadTypes.h>

namespace al {
struct ActorInitInfo;

struct CameraSubTargetTurnParam {
    CameraSubTargetTurnParam() {}
    // Full-field ctor so a constant turn param can be defined as a static const
    // (constant-initialized into .rodata).
    constexpr CameraSubTargetTurnParam(f32 turnSpeedRate1, f32 turnSpeedRate2,
                                       f32 turnBrakeEndDistance, f32 turnBrakeStartDistance,
                                       f32 turnStopStartDistance, f32 turnStopEndDistance,
                                       s32 targetTurnRestartStep, bool isTurnV,
                                       bool isResetAfterTurnV, f32 minTurnDegreeV,
                                       f32 maxTurnDegreeV, f32 validTurnDegreeRangeH,
                                       f32 validFaceDegreeRangeH)
        : turnSpeedRate1(turnSpeedRate1), turnSpeedRate2(turnSpeedRate2),
          turnBrakeEndDistance(turnBrakeEndDistance),
          turnBrakeStartDistance(turnBrakeStartDistance),
          turnStopStartDistance(turnStopStartDistance),
          turnStopEndDistance(turnStopEndDistance),
          targetTurnRestartStep(targetTurnRestartStep), isTurnV(isTurnV),
          isResetAfterTurnV(isResetAfterTurnV), minTurnDegreeV(minTurnDegreeV),
          maxTurnDegreeV(maxTurnDegreeV), validTurnDegreeRangeH(validTurnDegreeRangeH),
          validFaceDegreeRangeH(validFaceDegreeRangeH) {}

    void init(const ActorInitInfo& info);

    f32 turnSpeedRate1 = 0.3f;
    f32 turnSpeedRate2 = 0.1f;
    f32 turnBrakeEndDistance = -1.0f;
    f32 turnBrakeStartDistance = -1.0f;
    f32 turnStopStartDistance = -1.0f;
    f32 turnStopEndDistance = -1.0f;
    s32 targetTurnRestartStep = 30;
    bool isTurnV = false;
    bool isResetAfterTurnV = false;
    f32 minTurnDegreeV = 0.0f;
    f32 maxTurnDegreeV = 0.0f;
    f32 validTurnDegreeRangeH = -1.0f;
    f32 validFaceDegreeRangeH = -1.0f;
};

static_assert(sizeof(CameraSubTargetTurnParam) == 0x30);
}  // namespace al
