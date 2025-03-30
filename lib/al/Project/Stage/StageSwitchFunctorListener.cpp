#include "Project/Stage/StageSwitchFunctorListener.h"

#include "Library/Thread/FunctorV0M.h"

namespace al {
StageSwitchFunctorListener::StageSwitchFunctorListener() = default;

void StageSwitchFunctorListener::setOnFunctor(const FunctorBase& functor) {
    mOnFunctor = functor.clone();
}

void StageSwitchFunctorListener::setOffFunctor(const FunctorBase& functor) {
    mOffFunctor = functor.clone();
}

void StageSwitchFunctorListener::listenOn() {
    if (mOnFunctor)
        (*mOnFunctor)();
}

void StageSwitchFunctorListener::listenOff() {
    if (mOffFunctor)
        (*mOffFunctor)();
}
}  // namespace al
