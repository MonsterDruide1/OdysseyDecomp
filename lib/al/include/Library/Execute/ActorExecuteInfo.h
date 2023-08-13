#pragma once

namespace al {
class ExecuteRequestKeeper;
class ExecutorActorExecuteBase;
class ModelDrawerBase;

class ActorExecuteInfo {
public:
    ActorExecuteInfo(ExecuteRequestKeeper* keeper);
    void addUpdater(ExecutorActorExecuteBase* updater);
    void addDrawer(ModelDrawerBase* drawer);

private:
    ExecuteRequestKeeper* mRequestKeeper;
    int mUpdaterCount = 0;
    ExecutorActorExecuteBase* mUpdaters[4] = {};
    int mDrawerCount = 0;
    ModelDrawerBase* mDrawers[11] = {};
};
static_assert(sizeof(ActorExecuteInfo) == 0x90);

}  // namespace al
