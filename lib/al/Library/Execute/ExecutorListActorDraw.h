#pragma once

#include <basis/seadTypes.h>

#include "Library/Execute/ExecutorListActorExecute.h"

namespace al {
class ExecutorActorExecuteBase;

class ExecutorListActorDraw : public ExecutorListActorExecuteBase {
public:
    ExecutorListActorDraw(const char* name, s32 size);
    ExecutorActorExecuteBase* createExecutor(const char* name) const override;
};

class ExecutorListActorMovement : public ExecutorListActorExecuteBase {
public:
    ExecutorListActorMovement(const char* name, s32 size);
    ExecutorActorExecuteBase* createExecutor(const char* name) const override;
};

class ExecutorListActorCalcAnim : public ExecutorListActorExecuteBase {
public:
    ExecutorListActorCalcAnim(const char* name, s32 size);
    ExecutorActorExecuteBase* createExecutor(const char* name) const override;
};

class ExecutorListActorMovementCalcAnim : public ExecutorListActorExecuteBase {
public:
    ExecutorListActorMovementCalcAnim(const char* name, s32 size);
    ExecutorActorExecuteBase* createExecutor(const char* name) const override;
};

class ExecutorListActorCalcView : public ExecutorListActorExecuteBase {
public:
    ExecutorListActorCalcView(const char* name, s32 size);
    ExecutorActorExecuteBase* createExecutor(const char* name) const override;
};

}  // namespace al
