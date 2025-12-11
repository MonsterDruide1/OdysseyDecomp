#pragma once

#include <gfx/seadColor.h>
#include <math/seadVector.h>

namespace al {

enum class PolygonType : s32 { /* TODO: MISSING */ };

bool isWallPolygon(const sead::Vector3f&, const sead::Vector3f&);
bool isFloorPolygon(const sead::Vector3f&, const sead::Vector3f&);
bool isFloorPolygonCos(const sead::Vector3f&, const sead::Vector3f&, f32);
bool isCeilingPolygon(const sead::Vector3f&, const sead::Vector3f&);
PolygonType calcPolygonType(const sead::Vector3f&, const sead::Vector3f&);
void calcTriangleColorByAngle(sead::Color4f*, f32*, const sead::Vector3f&);

}  // namespace al
