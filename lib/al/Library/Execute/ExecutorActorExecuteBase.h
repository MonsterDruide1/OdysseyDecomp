#pragma once

#include <container/seadPtrArray.h>

#include "Library/HostIO/HioNode.h"

namespace al {
class LiveActor;

class ExecutorActorExecuteBase : public HioNode {
public:
    ExecutorActorExecuteBase(const char* name);

    void registerActor(LiveActor* actor);
    void createExecutorTable();
    void addActor(LiveActor* actor);
    void removeActor(LiveActor* actor);

    virtual void execute() const = 0;

private:
    const char* mName;
    sead::PtrArray<LiveActor> mActors;
};

static_assert(sizeof(ExecutorActorExecuteBase) == 0x20);

}  // namespace al
