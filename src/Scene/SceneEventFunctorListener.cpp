#include "Scene/SceneEventFunctorListener.h"

#include "Library/Thread/FunctorV0M.h"

SceneEventFunctorListener::SceneEventFunctorListener(const al::FunctorBase& functor) {
    mFunctor = functor.clone();
}

void SceneEventFunctorListener::notify() {
    if (!mFunctor)
        return;

    (*mFunctor)();
}
