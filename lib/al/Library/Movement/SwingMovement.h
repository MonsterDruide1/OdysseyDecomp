#pragma once

#include "Library/Nerve/NerveExecutor.h"

namespace al {
struct ActorInitInfo;

class SwingMovement : public NerveExecutor {
public:
    SwingMovement();
    SwingMovement(const ActorInitInfo& info);

    bool updateRotate();

    void exeMove();
    void exeStop();

    bool isLeft() const;
    bool isStop() const;

    f32 getCurrentAngle() const { return mCurrentAngle; }

private:
    s32 mFrameInCycle = 0;
    s32 mDelayRate = 0;
    f32 mSwingAngle = 45.0f;
    s32 mSwingCycle = 240;
    s32 mStopTime = 6;
    f32 mOffsetRotate = 0.0f;
    f32 mCurrentAngle = 0.0f;
};

static_assert(sizeof(SwingMovement) == 0x30);
}  // namespace al
