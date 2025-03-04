#pragma once

#include "Library/LiveActor/LiveActor.h"

class AppearSwitchSave;

class StageSwitchSelector : public al::LiveActor {
public:
    StageSwitchSelector(const char* name);

    void init(const al::ActorInitInfo& info) override;

    void exeWaitStatUp();
    void exeWaitStartSwitching();
    void exeWaitDelaySwitching();

private:
    s32 mSwitchingStartFrameNum = 0;
    s32 mSwitchingDelay = 0;
    AppearSwitchSave* mAppearSwitchSave = nullptr;
};
