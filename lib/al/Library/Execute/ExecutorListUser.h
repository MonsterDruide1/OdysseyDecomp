#pragma once

#include <basis/seadTypes.h>

#include "Library/Execute/ExecutorListBase.h"

namespace al {
class IUseExecutor;

class ExecutorListIUseExecutorBase : public ExecutorListBase {
public:
    ExecutorListIUseExecutorBase(const char* name, s32);

    bool isActive() const override;

    void registerUser(IUseExecutor* user);

private:
    char filler[0x10];
};

class ExecutorListIUseExecutorUpdate : public ExecutorListIUseExecutorBase {
public:
    ExecutorListIUseExecutorUpdate(const char* name, s32);

    void executeList() const override;
};

class ExecutorListIUseExecutorDraw : public ExecutorListIUseExecutorBase {
public:
    ExecutorListIUseExecutorDraw(const char* name, s32);

    void executeList() const override;
};

}  // namespace al
