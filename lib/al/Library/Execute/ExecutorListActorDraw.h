#pragma once

#include <basis/seadTypes.h>

#include "Library/Execute/ExecutorListBase.h"

namespace al {
class LiveActor;

class ExecutorListActorExecuteBase : public ExecutorListBase {
public:
    ExecutorListActorExecuteBase(const char* name, s32);

    bool isActive() const override;
    void executeList() const override;

    void registerActor(LiveActor* actor);
    void createList();

private:
    char filler[0x10];
};

class ExecutorListActorDraw : public ExecutorListActorExecuteBase {
public:
    ExecutorListActorDraw(const char* name, s32);
};

}  // namespace al
