#pragma once

#include "Library/LiveActor/LiveActor.h"
#include "Library/Nerve/NerveSetupUtil.h"

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

namespace {
NERVE_IMPL(AllDeadWatcher, Watch);
NERVE_IMPL(AllDeadWatcher, Wait);

NERVES_MAKE_STRUCT(AllDeadWatcher, Watch, Wait);
}  // namespace
}  // namespace al
