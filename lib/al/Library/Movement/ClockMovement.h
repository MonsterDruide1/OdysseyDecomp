#pragma once

#include <math/seadQuat.h>

#include "Library/Nerve/NerveExecutor.h"

namespace al {
struct ActorInitInfo;

class ClockMovement : public NerveExecutor {
public:
    ClockMovement(const ActorInitInfo& info);
    void exeDelay();
    void exeRotateSign();
    void exeRotate();
    void exeWait();
    bool isFirstStepDelay() const;
    bool isFirstStepRotateSign() const;
    bool isFirstStepRotate() const;
    bool isFirstStepWait() const;

private:
    sead::Quatf mActorQuat = sead::Quatf::unit;
    sead::Quatf mInitialActorQuat = sead::Quatf::unit;
    s32 mRotateAxis = 0;
    s32 mClockAngleDegree = 90;
    s32 mCurrentStepIndex = 0;
    s32 mMaxStepCount = 4;
    s32 mDelayTime = 0;
    s32 mRotateSignTime = 36;
    s32 mRotateTime = 60;
    s32 mWaitTime = 0;
};

static_assert(sizeof(ClockMovement) == 0x50);
}  // namespace al
