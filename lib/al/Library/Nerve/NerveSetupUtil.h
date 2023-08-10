#pragma once

#include "al/Library/Nerve/Nerve.h"
#include "al/Library/Nerve/NerveKeeper.h"

#define MAKE_NERVE_FUNC_NAMESPACE(Class, Action, Func, Namespace)                                  \
    class Class##Nrv##Action : public al::Nerve {                                                  \
        void execute(al::NerveKeeper* keeper) const override {                                     \
            keeper->getParent<Namespace Class>()->exe##Func();                                     \
        }                                                                                          \
                                                                                                   \
    public:                                                                                        \
        static Class##Nrv##Action sInstance;                                                       \
    };                                                                                             \
    Class##Nrv##Action Class##Nrv##Action::sInstance;
#define MAKE_NERVE(Class, Action) MAKE_NERVE_FUNC_NAMESPACE(Class, Action, Action, )
#define MAKE_NERVE_FUNC(Class, Action, Func) MAKE_NERVE_FUNC_NAMESPACE(Class, Action, Func, )

#define MAKE_NERVE_NAMESPACE(Class, Action, Namespace)                                             \
    MAKE_NERVE_FUNC_NAMESPACE(Class, Action, Action, Namespace::)
