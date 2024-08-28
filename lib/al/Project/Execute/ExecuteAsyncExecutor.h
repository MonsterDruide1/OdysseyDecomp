#pragma once

#include <mc/seadCoreInfo.h>
#include <thread/seadThread.h>

#include "Library/HostIO/HioNode.h"

namespace al {
class ExecuteDirector;

class ExecuteAsyncExecutor : public HioNode {
public:
    ExecuteAsyncExecutor(const ExecuteDirector*, const char*, sead::CoreId);
    virtual ~ExecuteAsyncExecutor();

    virtual void execute() = 0;

    void executeAsync(sead::Thread*, s64);
    void executeAsync();
    void waitAsync();

private:
    void* filler[4];
};

static_assert(sizeof(ExecuteAsyncExecutor) == 0x28);

class ExecuteAsyncExecutorUpdate : public ExecuteAsyncExecutor {
public:
    ExecuteAsyncExecutorUpdate(const ExecuteDirector* director, const char* name,
                               sead::CoreId coreId)
        : ExecuteAsyncExecutor(director, name, coreId) {}

    ~ExecuteAsyncExecutorUpdate() override;
    void execute() override;

private:
    // missing
};

static_assert(sizeof(ExecuteAsyncExecutorUpdate) == 0x28);

class ExecuteAsyncExecutorDraw : public ExecuteAsyncExecutor {
public:
    ExecuteAsyncExecutorDraw(const ExecuteDirector* director, const char* name, sead::CoreId coreId)
        : ExecuteAsyncExecutor(director, name, coreId) {}

    ~ExecuteAsyncExecutorDraw() override;
    void execute() override;

private:
    // missing
};

static_assert(sizeof(ExecuteAsyncExecutorUpdate) == 0x28);

}  // namespace al
