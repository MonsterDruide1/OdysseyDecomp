#pragma once

#include <prim/seadStorageFor.h>

#include "Library/Base/Macros.h"
#include "Library/Nerve/Nerve.h"
#include "Library/Nerve/NerveAction.h"
#include "Library/Nerve/NerveKeeper.h"

/*

Proper usage of these nerve macros:

namespace {
    NERVE_IMPL(ExampleUseCase, Wait);
    NERVE_IMPL_(ExampleUseCase, WaitHack, Wait);
    NERVE_IMPL(ExampleUseCase, HackEnd);
    ...

    NERVES_MAKE_STRUCT(ExampleUseCase, Wait, WaitHack, HackEnd, ...);
    // or NERVES_MAKE_NOSTRUCT if they are not supposed to be in a struct

}

al::setNerve(this, &NrvExampleUseCase.Wait);


Similar setup for NerveAction:

namespace {
    NERVE_ACTION_IMPL(ExampleUseCase, Wait);
    NERVE_ACTION_IMPL_(ExampleUseCase, WaitHack, Wait);
    NERVE_ACTION_IMPL(ExampleUseCase, HackEnd);
    ...

    NERVE_ACTIONS_MAKE_STRUCT(ExampleUseCase, Wait, WaitHack, HackEnd, ...);
    // no NOSTRUCT variant, as the struct also contains a NerveActionCollector
    // and no variants without it have been found so far
}

al::initNerveAction(this, "Hide", &NrvExampleUseCase.collector, 0);

*/

#define NERVE_IMPL_(Class, Action, ActionFunc)                                                     \
    class Class##Nrv##Action : public al::Nerve {                                                  \
    public:                                                                                        \
        void execute(al::NerveKeeper* keeper) const override {                                     \
            (keeper->getParent<Class>())->exe##ActionFunc();                                       \
        }                                                                                          \
    };

#define NERVE_IMPL(Class, Action) NERVE_IMPL_(Class, Action, Action)

#define NERVE_END_IMPL_(Class, Action, ActionFunc)                                                 \
    class Class##Nrv##Action : public al::Nerve {                                                  \
    public:                                                                                        \
        void execute(al::NerveKeeper* keeper) const override {                                     \
            (keeper->getParent<Class>())->exe##ActionFunc();                                       \
        }                                                                                          \
        void executeOnEnd(al::NerveKeeper* keeper) const override {                                \
            (keeper->getParent<Class>())->end##ActionFunc();                                       \
        }                                                                                          \
    };

#define NERVE_END_IMPL(Class, Action) NERVE_END_IMPL_(Class, Action, Action)

#define NERVE_HOST_TYPE_IMPL_(Class, Action, ActionFunc)                                           \
    class HostType##Nrv##Action : public al::Nerve {                                               \
    public:                                                                                        \
        void execute(al::NerveKeeper* keeper) const override {                                     \
            (keeper->getParent<Class>())->exe##ActionFunc();                                       \
        }                                                                                          \
    };

#define NERVE_HOST_TYPE_IMPL(Class, Action) NERVE_HOST_TYPE_IMPL_(Class, Action, Action)

#define NERVE_MAKE(Class, Action) [[maybe_unused]] Class##Nrv##Action Action;

#define NERVES_MAKE_STRUCT(Class, ...)                                                             \
    struct {                                                                                       \
        FOR_EACH(NERVE_MAKE, Class, __VA_ARGS__)                                                   \
    } Nrv##Class;

#define NERVES_MAKE_NOSTRUCT(Class, ...) FOR_EACH(NERVE_MAKE, Class, __VA_ARGS__)

#define NERVE_ACTION_IMPL_(Class, Action, ActionFunc)                                              \
    class Class##Nrv##Action : public al::NerveAction {                                            \
    public:                                                                                        \
        void execute(al::NerveKeeper* keeper) const override {                                     \
            (keeper->getParent<Class>())->exe##ActionFunc();                                       \
        }                                                                                          \
                                                                                                   \
        const char* getActionName() const override {                                               \
            return #Action;                                                                        \
        }                                                                                          \
    };

#define NERVE_ACTION_IMPL(Class, Action) NERVE_ACTION_IMPL_(Class, Action, Action)

#define NERVE_ACTION_MAKE(Class, Action) sead::StorageFor<Class##Nrv##Action> Action;

#define NERVE_ACTION_CONSTRUCT(Class, Action) Action.constructDefault();

#define NERVE_ACTIONS_MAKE_STRUCT(Class, ...)                                                      \
    struct NrvStruct##Class {                                                                      \
        FOR_EACH(NERVE_ACTION_MAKE, Class, __VA_ARGS__)                                            \
                                                                                                   \
        alNerveFunction::NerveActionCollector collector;                                           \
                                                                                                   \
        NrvStruct##Class() {                                                                       \
            FOR_EACH(NERVE_ACTION_CONSTRUCT, Class, __VA_ARGS__)                                   \
        }                                                                                          \
    } Nrv##Class;
