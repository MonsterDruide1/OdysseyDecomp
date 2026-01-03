#pragma once

#include <basis/seadTypes.h>

#include "Library/Execute/ExecutorListBase.h"

namespace al {
class ExecutorActorExecuteBase;
class LiveActor;

class ExecutorListActorExecuteBase : public ExecutorListBase {
public:
    ExecutorListActorExecuteBase(const char* name, s32);

    bool isActive() const override;
    void executeList() const override;
    virtual ExecutorActorExecuteBase* createExecutor(const char*) const = 0;

    void registerActor(LiveActor* actor);
    void createList();

private:
    s32 mCapacity = 0;
    s32 mSize = 0;
    ExecutorActorExecuteBase** mList = nullptr;
};

static_assert(sizeof(ExecutorListActorExecuteBase) == 0x20);

}  // namespace al
