#pragma once

#include <gfx/seadColor.h>
#include <math/seadVector.h>

namespace al {

enum class PolygonType {
    None = 0,
    Wall = 1,
    Floor = 2,
    Ceiling = 3,
};

bool isWallPolygon(const sead::Vector3f& normal, const sead::Vector3f& gravity);
bool isFloorPolygon(const sead::Vector3f& normal, const sead::Vector3f& gravity);
bool isFloorPolygonCos(const sead::Vector3f& normal, const sead::Vector3f& gravity, f32 maxCos);
bool isCeilingPolygon(const sead::Vector3f& normal, const sead::Vector3f& gravity);

PolygonType calcPolygonType(const sead::Vector3f& normal, const sead::Vector3f& gravity);
void calcTriangleColorByAngle(sead::Color4f* outColor, f32* outAngle, const sead::Vector3f& normal);

}  // namespace al
