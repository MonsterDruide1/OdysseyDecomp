#pragma once

#include "Library/Stage/StageSwitchListener.h"

namespace al {
class FunctorBase;

class StageSwitchFunctorListener : public StageSwitchListener {
public:
    StageSwitchFunctorListener();

    void setOnFunctor(const FunctorBase& functor);
    void setOffFunctor(const FunctorBase& functor);
    void listenOn() override;
    void listenOff() override;

private:
    FunctorBase* mOnFunctor = nullptr;
    FunctorBase* mOffFunctor = nullptr;
};
}  // namespace al
