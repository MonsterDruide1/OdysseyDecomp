#pragma once

#include "Library/Execute/ExecutorActorExecuteBase.h"

namespace al {

class ExecutorActorCalcView : public ExecutorActorExecuteBase {
public:
    ExecutorActorCalcView(const char* name);
    void execute() const override;
};

static_assert(sizeof(ExecutorActorCalcView) == 0x20);

}  // namespace al
