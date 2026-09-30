#pragma once

#include "Library/Execute/ExecutorActorExecuteBase.h"

namespace al {

class ExecutorActorMovement : public ExecutorActorExecuteBase {
public:
    ExecutorActorMovement(const char* name);
    void execute() const override;
};

static_assert(sizeof(ExecutorActorMovement) == 0x20);

}  // namespace al
