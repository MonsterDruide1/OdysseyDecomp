#pragma once

#include <math/seadVector.h>

namespace al {
class Triangle;

class TriangleFilterBase {
public:
    virtual bool isInvalidTriangle(const Triangle& triangle) const = 0;
};

class TriangleFilterIgnoreGround : public TriangleFilterBase {
public:
    TriangleFilterIgnoreGround(const sead::Vector3f& gravity) : mGravity(gravity) {}

    bool isInvalidTriangle(const Triangle& triangle) const override;

private:
    const sead::Vector3f& mGravity;
};

class TriangleFilterGroundOnly : public TriangleFilterBase {
public:
    TriangleFilterGroundOnly(const sead::Vector3f& gravity) : mGravity(gravity) {}

    bool isInvalidTriangle(const Triangle& triangle) const override;

private:
    const sead::Vector3f& mGravity;
};

class TriangleFilterWallOnly : public TriangleFilterBase {
public:
    TriangleFilterWallOnly(const sead::Vector3f& gravity) : mGravity(gravity) {}

    bool isInvalidTriangle(const Triangle& triangle) const override;

private:
    const sead::Vector3f& mGravity;
};

class TriangleFilterFunc : public TriangleFilterBase {
public:
    using DelegateFilter = bool (*)(const Triangle& triangle);

    TriangleFilterFunc(DelegateFilter filter) : mFilter(filter) {}

    bool isInvalidTriangle(const Triangle& triangle) const override { return mFilter(triangle); }

private:
    DelegateFilter mFilter;
};

template <typename T>
class TriangleFilterDelegator : public TriangleFilterBase {
public:
    using DelegateFilter = bool (T::*)(const Triangle& triangle) const;

    TriangleFilterDelegator(T* parent, DelegateFilter filter) : mParent(parent), mFilter(filter) {}

    bool isInvalidTriangle(const Triangle& triangle) const override {
        return (mParent->*mFilter)(triangle);
    }

private:
    T* mParent;
    DelegateFilter mFilter;
};

}  // namespace al
