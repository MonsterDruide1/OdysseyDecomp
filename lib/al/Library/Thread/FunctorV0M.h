#pragma once

namespace al {
class FunctorBase {
public:
    inline FunctorBase() = default;
    inline FunctorBase(const FunctorBase& copy) = default;
    virtual void operator()() const = 0;
    virtual FunctorBase* clone() const = 0;

    virtual ~FunctorBase() {}
};

template <class T, class F>
class FunctorV0M : public FunctorBase {
public:
    inline FunctorV0M() = default;
    inline FunctorV0M(T objPointer, F functPointer)
        : mObjPointer(objPointer), mFunctPointer(functPointer) {};
    inline FunctorV0M(const FunctorV0M<T, F>& copy) = default;

    void operator()() const override { (mObjPointer->*mFunctPointer)(); }

    FunctorV0M<T, F>* clone() const override { return new FunctorV0M<T, F>(*this); }

private:
    T mObjPointer = nullptr;
    F mFunctPointer = nullptr;
};
}  // namespace al
