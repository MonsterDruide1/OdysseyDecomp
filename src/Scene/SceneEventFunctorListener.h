#pragma once

#include "Scene/SceneEventListener.h"

namespace al {
class FunctorBase;
}  // namespace al

class SceneEventFunctorListener : public SceneEventListener {
public:
    SceneEventFunctorListener(const al::FunctorBase& functor);

    void notify() override;

private:
    al::FunctorBase* mFunctor = nullptr;
};
