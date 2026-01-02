#pragma once

#include <basis/seadTypes.h>

#include "Library/Execute/ExecutorListBase.h"

namespace al {
struct ExecuteSystemInitInfo;
class LayoutActor;

class ExecutorListLayoutDrawBase : public ExecutorListBase {
public:
    ExecutorListLayoutDrawBase(const char* name, s32, const ExecuteSystemInitInfo&);

    bool isActive() const override;
    void executeList() const override;
    virtual void startDraw() const = 0;

    void registerLayout(LayoutActor* layout);

private:
    char filler[0x18];
};

class ExecutorListLayoutDrawNormal : public ExecutorListLayoutDrawBase {
public:
    ExecutorListLayoutDrawNormal(const char*, s32, const ExecuteSystemInitInfo&);

    void startDraw() const override;
};

}  // namespace al
