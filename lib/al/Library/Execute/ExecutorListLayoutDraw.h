#pragma once

#include <basis/seadTypes.h>

#include "Library/Execute/ExecutorListBase.h"

namespace agl {
class DrawContext;
}

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
    s32 mCapacity = 0;
    s32 mSize = 0;
    LayoutActor** mList = nullptr;
    agl::DrawContext* mContext = nullptr;
};

static_assert(sizeof(ExecutorListLayoutDrawBase) == 0x28);

class ExecutorListLayoutDrawNormal : public ExecutorListLayoutDrawBase {
public:
    ExecutorListLayoutDrawNormal(const char*, s32, const ExecuteSystemInitInfo&);

    void startDraw() const override;
};

}  // namespace al
