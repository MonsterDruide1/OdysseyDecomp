#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
class AllDeadWatcher : public LiveActor {
public:
    AllDeadWatcher(const char* name);

    void appear() override;
    void init(const ActorInitInfo& info) override;
    void kill() override;

    void exeWait();
    void exeWatch();

private:
    LiveActor** mTargets = nullptr;
    s32 mTargetCount = 0;
    s32 mSwitchOnDelayStep = 10;
    bool mIsNotControlActorAppear = false;
};

static_assert(sizeof(AllDeadWatcher) == 0x120);
}  // namespace al
