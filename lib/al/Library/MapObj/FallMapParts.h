#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class FallMapParts : public LiveActor {
public:
    FallMapParts(const char* name);

    void init(const ActorInitInfo& info) override;
    bool receiveMsg(const SensorMsg* message, HitSensor* other, HitSensor* self) override;
    void appearAndSetStart();
    void exeAppear();
    void exeWait();
    void exeFallSign();
    bool isEndFallSign() const;
    void exeFall();
    void exeEnd();

private:
    sead::Vector3f mPos = sead::Vector3f::zero;
    s32 mFallTime = 75;
    bool mIsStartAction = false;
    bool mIsInvalidAutoRestart = false;
};

static_assert(sizeof(FallMapParts) == 0x120);
}  // namespace al
