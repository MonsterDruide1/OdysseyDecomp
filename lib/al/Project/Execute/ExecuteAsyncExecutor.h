#pragma once

#include <basis/seadTypes.h>
#include <mc/seadCoreInfo.h>

#include "Library/HostIO/HioNode.h"

namespace sead {
class DelegateThread;
class Event;
class Thread;
}  // namespace sead

namespace al {
class ExecuteDirector;

class ExecuteAsyncExecutor : public HioNode {
public:
    ExecuteAsyncExecutor(const ExecuteDirector* director, const char* name, sead::CoreId coreId);
    virtual ~ExecuteAsyncExecutor();

    virtual void execute() = 0;

    void executeAsync(sead::Thread*, s64);
    void executeAsync();
    void waitAsync();

private:
    ExecuteDirector* mDirector = nullptr;
    const char* mName = nullptr;
    sead::Event* mEvent = nullptr;
    sead::DelegateThread* mDelegateThread = nullptr;
};

static_assert(sizeof(ExecuteAsyncExecutor) == 0x28);

class ExecuteAsyncExecutorUpdate : public ExecuteAsyncExecutor {
public:
    ExecuteAsyncExecutorUpdate(const ExecuteDirector* director, const char* name,
                               sead::CoreId coreId)
        : ExecuteAsyncExecutor(director, name, coreId) {}

    ~ExecuteAsyncExecutorUpdate() override;
    void execute() override;
};

static_assert(sizeof(ExecuteAsyncExecutorUpdate) == 0x28);

class ExecuteAsyncExecutorDraw : public ExecuteAsyncExecutor {
public:
    ExecuteAsyncExecutorDraw(const ExecuteDirector* director, const char* name, sead::CoreId coreId)
        : ExecuteAsyncExecutor(director, name, coreId) {}

    ~ExecuteAsyncExecutorDraw() override;
    void execute() override;
};

static_assert(sizeof(ExecuteAsyncExecutorUpdate) == 0x28);

}  // namespace al
