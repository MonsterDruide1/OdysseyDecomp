#pragma once

#include "Library/Execute/ExecutorActorExecuteBase.h"

namespace al {

class ExecutorActorMovementCalcAnim : public ExecutorActorExecuteBase {
public:
    ExecutorActorMovementCalcAnim(const char* name);
    void execute() const override;
};

static_assert(sizeof(ExecutorActorMovementCalcAnim) == 0x20);

}  // namespace al
