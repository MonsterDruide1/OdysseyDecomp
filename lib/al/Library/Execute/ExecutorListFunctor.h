#pragma once

#include <basis/seadTypes.h>

#include "Library/Execute/ExecutorListBase.h"

namespace al {
class FunctorBase;

class ExecutorListFunctor : public ExecutorListBase {
public:
    ExecutorListFunctor(const char* name, s32);

    bool isActive() const override;
    void executeList() const override;

    void registerFunctor(const FunctorBase&);

private:
    char filler[0x10];
};

static_assert(sizeof(ExecutorListFunctor) == 0x20);

}  // namespace al
