#pragma once

#include <basis/seadTypes.h>

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

    const char* getName() const { return mName; }

protected:
    s32 getActorCount() const { return mActorCount; }

    LiveActor** getActorArray() const { return mActors; }

private:
    const char* mName = nullptr;
    s32 mMaxActorCount = 0;
    s32 mActorCount = 0;
    LiveActor** mActors = nullptr;
};

static_assert(sizeof(ExecutorActorExecuteBase) == 0x20);

}  // namespace al
