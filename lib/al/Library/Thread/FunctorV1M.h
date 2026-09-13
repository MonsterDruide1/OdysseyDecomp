#pragma once

#include "Library/Thread/FunctorV0M.h"

namespace al {
template <class T, class F, class A>
class FunctorV1M : public FunctorBase {
public:
    inline FunctorV1M() = default;

    inline FunctorV1M(T objPointer, F functPointer, A argument)
        : mObjPointer(objPointer), mFunctPointer(functPointer), mArgument(argument) {}

    inline FunctorV1M(const FunctorV1M<T, F, A>& objPointer) = default;

    void operator()() const override { (mObjPointer->*mFunctPointer)(mArgument); }

    FunctorV1M<T, F, A>* clone() const override { return new FunctorV1M<T, F, A>(*this); }

private:
    T mObjPointer = nullptr;
    F mFunctPointer = nullptr;
    A mArgument;
};
}  // namespace al
