#pragma once

#include "Library/Execute/ExecutorActorExecuteBase.h"

namespace al {

class ExecutorActorDraw : public ExecutorActorExecuteBase {
public:
    ExecutorActorDraw(const char* name);
    void execute() const override;
};

static_assert(sizeof(ExecutorActorDraw) == 0x20);

}  // namespace al
