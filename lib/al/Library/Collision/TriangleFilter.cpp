#include "Library/Collision/TriangleFilter.h"

#include "Library/Collision/Angle.h"
#include "Library/Collision/HitInfo.h"

namespace al {

bool TriangleFilterIgnoreGround::isInvalidTriangle(const Triangle& triangle) const {
    return isFloorPolygon(triangle.getFaceNormal(), mGravity);
}

bool TriangleFilterGroundOnly::isInvalidTriangle(const Triangle& triangle) const {
    return !isFloorPolygon(triangle.getFaceNormal(), mGravity);
}

bool TriangleFilterWallOnly::isInvalidTriangle(const Triangle& triangle) const {
    return !isWallPolygon(triangle.getFaceNormal(), mGravity);
}

}  // namespace al
