#pragma once

#include "al/Library/Nerve/Nerve.h"
#include "al/Library/Nerve/NerveKeeper.h"

#define MAKE_NERVE_FUNC(Class, Action, Func)                                                       \
    class ClassNrv##Action : public al::Nerve {                                                    \
        void execute(al::NerveKeeper* keeper) const override {                                     \
            keeper->getParent<Class>()->exe##Func();                                               \
        }                                                                                          \
    };                                                                                             \
    ClassNrv##Action nrv##Class##Action
#define MAKE_NERVE(Class, Action) MAKE_NERVE_FUNC(Class, Action, Action)
