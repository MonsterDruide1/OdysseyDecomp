#pragma once

#include "Library/Collision/TriangleFilter.h"

namespace al {

class CameraTriangleFilter : public TriangleFilterBase {
public:
    CameraTriangleFilter() {}

    bool isInvalidTriangle(const Triangle& triangle) const override;
};

class CameraTriangleFilterOnlyCeiling : public CameraTriangleFilter {
public:
    CameraTriangleFilterOnlyCeiling() {}

    bool isInvalidTriangle(const Triangle& triangle) const override;
};

class SubjectiveCameraTriangleFilter : public TriangleFilterBase {
public:
    SubjectiveCameraTriangleFilter() {}

    bool isInvalidTriangle(const Triangle& triangle) const override;
};

}  // namespace al
