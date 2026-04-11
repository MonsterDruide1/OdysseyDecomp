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

    const sead::Quatf& getQuat() const { return mQuat; }

private:
    sead::Quatf mQuat;
    unsigned char _20[0x30];
};

static_assert(sizeof(ClockMovement) == 0x50);
}  // namespace al
