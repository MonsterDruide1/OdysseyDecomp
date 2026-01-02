#pragma once

#include <basis/seadTypes.h>

#include "Library/Execute/ExecutorListActorExecuteBase.h"

namespace al {
class ExecutorActorExecuteBase;

class ExecutorListActorDraw : public ExecutorListActorExecuteBase {
public:
    ExecutorListActorDraw(const char* name, s32 size);
    ExecutorActorExecuteBase* createExecutor(const char* name) const override;
};

}  // namespace al
