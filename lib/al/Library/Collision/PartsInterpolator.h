#pragma once

#include "math/seadVectorFwd.h"
namespace al {
class Triangle;

class TriangleFilterBase {
public:
    virtual bool isInvalidTriangle(const al::Triangle& triangle) const = 0;
};

class TriangleFilterWallOnly : public al::TriangleFilterBase {
public:
    TriangleFilterWallOnly(const sead::Vector3f& down) : mDown(down) {}
    bool isInvalidTriangle(const al::Triangle& triangle) const override;

private:
    const sead::Vector3f& mDown;
};

}
