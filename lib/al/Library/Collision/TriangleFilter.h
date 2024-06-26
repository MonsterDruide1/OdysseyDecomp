#pragma once

#include "Library/Collision/KTriangle.h"
#include "Library/stuff.h"

namespace al {
class Triangle;

class TriangleFilterBase {
public:
    virtual bool isInvalidTriangle(const al::Triangle&) const = 0;
};

class TriangleFilterIgnoreGround : public al::TriangleFilterBase {
public:
    bool isInvalidTriangle(const al::Triangle& triangle) const override {
        return al::isFloorPolygon(triangle.getFaceNormal(), *_8);
    }

public:
    const sead::Vector3f* _8;
};

class TriangleFilterGroundOnly : public al::TriangleFilterBase {
public:
    bool isInvalidTriangle(const al::Triangle& triangle) const override {
        return !al::isFloorPolygon(triangle.getFaceNormal(), *_8);
    }

public:
    const sead::Vector3f* _8;
};

}
