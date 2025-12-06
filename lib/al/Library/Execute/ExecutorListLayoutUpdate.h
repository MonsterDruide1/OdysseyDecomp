#pragma once

#include <basis/seadTypes.h>

#include "Library/Execute/ExecutorListBase.h"

namespace agl {
class DrawContext;
}

namespace al {
struct ExecuteSystemInitInfo;
class LayoutActor;

class ExecutorListLayoutUpdate : public ExecutorListBase {
public:
    ExecutorListLayoutUpdate(const char* name, s32 size);

    void registerLayout(LayoutActor* layout);
    void executeList() const override;
    bool isActive() const override;

private:
    s32 mCapacity = 0;
    s32 mSize = 0;
    LayoutActor** mList = nullptr;
};

static_assert(sizeof(ExecutorListLayoutUpdate) == 0x20);

}  // namespace al
