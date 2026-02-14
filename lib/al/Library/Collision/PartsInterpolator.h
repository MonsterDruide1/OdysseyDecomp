#pragma once

#include <math/seadVector.h>

namespace al {
class Triangle;

class TriangleFilterBase {
public:
    virtual bool isInvalidTriangle(const Triangle& triangle) const = 0;
};

class TriangleFilterWallOnly : public TriangleFilterBase {
public:
    TriangleFilterWallOnly(const sead::Vector3f& down) : mDown(down) {}

    bool isInvalidTriangle(const Triangle& triangle) const override;

private:
    const sead::Vector3f& mDown;
};

template <typename T>
class TriangleFilterDelegator : public TriangleFilterBase {
public:
    using DelegateFilter = bool (T::*)(const Triangle&) const;

    TriangleFilterDelegator(T* parent, DelegateFilter filter) : mParent(parent), mFunc(filter) {}

    bool isInvalidTriangle(const Triangle& triangle) const override {
        return (mParent->*mFunc)(triangle);
    }

private:
    T* mParent;
    DelegateFilter mFunc;
};

}  // namespace al
