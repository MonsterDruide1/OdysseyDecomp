#pragma once

#include "Library/Nerve/Nerve.h"
#include "Library/Nerve/NerveKeeper.h"

/*

Proper usage of these nerve macros:

namespace {
    NERVE_IMPL(ExampleUseCase, Wait);
    NERVE_IMPL_(ExampleUseCase, WaitHack, Wait);
    NERVE_IMPL(ExampleUseCase, HackEnd);
    ...

    struct {
        NERVE_MAKE(ExampleUseCase, Wait);
        NERVE_MAKE(ExampleUseCase, WaitHack);
        NERVE_MAKE(ExampleUseCase, HackEnd);
        ...
    } NrvExampleUseCase;

}

al::setNerve(this, &NrvExampleUseCase.Wait);

*/

#define NERVE_IMPL_(Class, Action, ActionFunc)                                                     \
    class Class##Nrv##Action : public al::Nerve {                                                  \
    public:                                                                                        \
        void execute(al::NerveKeeper* keeper) const override {                                     \
            (keeper->getParent<Class>())->exe##ActionFunc();                                       \
        }                                                                                          \
    };

#define NERVE_IMPL(Class, Action) NERVE_IMPL_(Class, Action, Action)

#define NERVE_MAKE(Class, Action) Class##Nrv##Action Action;
