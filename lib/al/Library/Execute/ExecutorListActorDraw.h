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
    virtual ExecutorActorExecuteBase* createExecutor(char*) const = 0;

    void registerActor(LiveActor* actor);
    void createList();

private:
    char filler[0x10];
};

class ExecutorListActorDraw : public ExecutorListActorExecuteBase {
public:
    ExecutorListActorDraw(const char* name, s32);
    ExecutorActorExecuteBase* createExecutor(char*) const override;
};

}  // namespace al
