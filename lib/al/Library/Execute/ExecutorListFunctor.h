#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>

#include "Library/Execute/ExecutorListBase.h"

namespace al {
class FunctorBase;

class ExecutorListFunctor : public ExecutorListBase {
public:
    ExecutorListFunctor(const char* name, s32 size);

    bool isActive() const override;
    void executeList() const override;

    void registerFunctor(const FunctorBase& functor);

private:
    sead::PtrArray<FunctorBase> mList;
};

static_assert(sizeof(ExecutorListFunctor) == 0x20);

}  // namespace al
