#pragma once

#include <basis/seadTypes.h>

#include "Library/Execute/ExecutorListBase.h"

namespace al {
class IUseExecutor;

class ExecutorListIUseExecutorBase : public ExecutorListBase {
public:
    ExecutorListIUseExecutorBase(const char* name, s32 size);

    bool isActive() const override;

    void registerUser(IUseExecutor* user);

    s32 getSize() const { return mSize; }

    IUseExecutor* getExecutor(s32 index) const { return mList[index]; }

private:
    s32 mCapacity = 0;
    s32 mSize = 0;
    IUseExecutor** mList = nullptr;
};

class ExecutorListIUseExecutorUpdate : public ExecutorListIUseExecutorBase {
public:
    ExecutorListIUseExecutorUpdate(const char* name, s32 size);

    void executeList() const override;
};

class ExecutorListIUseExecutorDraw : public ExecutorListIUseExecutorBase {
public:
    ExecutorListIUseExecutorDraw(const char* name, s32 size);

    void executeList() const override;
};

}  // namespace al
