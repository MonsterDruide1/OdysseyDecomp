#pragma once

namespace al {
class ExecuteRequestKeeper;
class ExecutorActorExecuteBase;
class ModelDrawerBase;

class ActorExecuteInfo {
public:
    ActorExecuteInfo(ExecuteRequestKeeper*);
    void addUpdater(al::ExecutorActorExecuteBase*);
    void addDrawer(ModelDrawerBase*);

private:
    char mPadding[0x90];
};
static_assert(sizeof(ActorExecuteInfo) == 0x90);
}  // namespace al
