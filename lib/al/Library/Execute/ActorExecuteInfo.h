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

    ExecuteRequestKeeper* getRequestKeeper() const { return mRequestKeeper; }

    ModelDrawerBase* getDrawer(s32 idx) const { return mDrawers[idx]; }

    s32 getDrawerCount() const { return mDrawerCount; }

private:
    ExecuteRequestKeeper* mRequestKeeper = nullptr;
    s32 mUpdaterCount = 0;
    ExecutorActorExecuteBase* mUpdaters[4] = {};
    s32 mDrawerCount = 0;
    ModelDrawerBase* mDrawers[11] = {};
};

static_assert(sizeof(ActorExecuteInfo) == 0x90);

}  // namespace al
