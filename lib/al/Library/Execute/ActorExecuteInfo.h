#pragma once

#include <basis/seadTypes.h>

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

    s32 getUpdaterCount() const { return mUpdaterCount; }

    ExecutorActorExecuteBase* getUpdater(s32 idx) const { return mUpdaters[idx]; }

    s32 getDrawerCount() const { return mDrawerCount; }

    ModelDrawerBase* getDrawer(s32 idx) const { return mDrawers[idx]; }

private:
    ExecuteRequestKeeper* mRequestKeeper = nullptr;
    s32 mUpdaterCount = 0;
    ExecutorActorExecuteBase* mUpdaters[4] = {};
    s32 mDrawerCount = 0;
    ModelDrawerBase* mDrawers[11] = {};
};

static_assert(sizeof(ActorExecuteInfo) == 0x90);

}  // namespace al
