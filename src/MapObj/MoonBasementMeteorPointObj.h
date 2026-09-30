#pragma once

#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

class MoonBasementFallObj;

class MoonBasementMeteorPointObj : public al::LiveActor {
public:
    MoonBasementMeteorPointObj(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void initAfterPlacement() override;
    void movement() override;

    void onChangeTrans();
    void offChangeTrans();
    void onSwitchStart();
    void offSwitchStart();

    void exeDelay();
    void exeWait();
    void exeStop();

private:
    friend void executeWaitNerveImpl(MoonBasementMeteorPointObj* actor);

    MoonBasementFallObj* mFallObj;
    bool mIsUseChangeTrans;
    u8 _111[3];
    sead::Vector3f mChangeTrans;
    s32 mDelayStep;
    s32 mIntervalStep;
};

static_assert(sizeof(MoonBasementMeteorPointObj) == 0x128);
