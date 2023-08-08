#include "al/async/FunctorV0M.h"

namespace al {

FunctorBase::~FunctorBase() = default;

template <class T, class F>
void FunctorV0M<T, F>::operator()() const {
    (mObjPointer->*mFunctor)();
}

template <class T, class F>
FunctorV0M<T, F>* FunctorV0M<T, F>::clone() const {
    return new FunctorV0M<T, F>(*this);
}

template <class T, class F>
FunctorV0M<T, F>::~FunctorV0M<T, F>() = default;

}  // namespace al
