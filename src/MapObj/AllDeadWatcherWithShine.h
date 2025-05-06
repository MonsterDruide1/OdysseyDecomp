#pragma once

#include "Library/LiveActor/LiveActor.h"

class Shine;

class AllDeadWatcherWithShine : public al::LiveActor {
public:
    AllDeadWatcherWithShine(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void appear() override;
    void kill() override;

    void exeWatch();
    void exeWait();

private:
    LiveActor** mChildActors = nullptr;
    s32 mChildCount = 0;
    s32 mSwitchOnDelayStep = 10;
    bool mIsNotControlActorAppear = false;
    Shine* mShine = nullptr;
};
