#pragma once

#include "Library/LiveActor/LiveActor.h"

class AppearSwitchFallMapParts : public al::LiveActor {
public:
    AppearSwitchFallMapParts(const char* name);

    void init(const al::ActorInitInfo& info) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
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

static_assert(sizeof(AppearSwitchFallMapParts) == 0x120);
