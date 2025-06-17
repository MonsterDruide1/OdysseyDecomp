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

}  // namespace al
