#pragma once

#include "Library/Execute/ExecutorActorExecuteBase.h"

namespace al {

class ExecutorActorCalcAnim : public ExecutorActorExecuteBase {
public:
    ExecutorActorCalcAnim(const char* name);
    void execute() const override;
};

static_assert(sizeof(ExecutorActorCalcAnim) == 0x20);

}  // namespace al
