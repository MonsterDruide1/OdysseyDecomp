#pragma once

#include <math/seadVector.h>

namespace al {
void verticalizeVec(sead::Vector3f* out, const sead::Vector3f& vertical, const sead::Vector3f& vec);

f32 calcAngleRadian(const sead::Vector3f&, const sead::Vector3f&);
f32 calcAngleDegree(const sead::Vector3f&, const sead::Vector3f&);
f32 calcAngleDegree(const sead::Vector2f&, const sead::Vector2f&);
f32 calcAngleOnPlaneRadian(const sead::Vector3f&, const sead::Vector3f&, const sead::Vector3f&);
f32 calcAngleOnPlaneDegree(const sead::Vector3f&, const sead::Vector3f&, const sead::Vector3f&);
f32 calcAngleOnPlaneDegreeOrZero(const sead::Vector3f&, const sead::Vector3f&,
                                 const sead::Vector3f&);
s32 calcAngleSignOnPlane(const sead::Vector3f&, const sead::Vector3f&, const sead::Vector3f&);

bool tryCalcAngleDegree(f32*, const sead::Vector3f&, const sead::Vector3f&);
bool tryCalcAngleOnPlaneDegree(f32*, const sead::Vector3f&, const sead::Vector3f&,
                               const sead::Vector3f&);

bool isNearZero(const sead::Vector2f& vec, f32 tolerance);
bool isNearZero(const sead::Vector3f& vec, f32 tolerance);
bool isParallelDirection(const sead::Vector3f&, const sead::Vector3f&, f32);
bool isNearAngleRadian(const sead::Vector2f&, const sead::Vector2f&, f32);
bool isNearAngleRadian(const sead::Vector3f&, const sead::Vector3f&, f32);
bool isNearAngleDegree(const sead::Vector2f&, const sead::Vector2f&, f32);
bool isNearAngleDegree(const sead::Vector3f&, const sead::Vector3f&, f32);
bool isNearAngleRadianHV(const sead::Vector3f&, const sead::Vector3f&, const sead::Vector3f&, f32,
                         f32);
bool isNearAngleDegreeHV(const sead::Vector3f&, const sead::Vector3f&, const sead::Vector3f&, f32,
                         f32);
bool isInAngleOnPlaneDegreeHV(const sead::Vector3f&, const sead::Vector3f&, const sead::Vector3f&,
                              f32, f32, f32, f32);

void normalize(sead::Vector2f*, const sead::Vector2f&);
void normalize(sead::Vector3f*, const sead::Vector3f&);
bool tryNormalizeOrZero(sead::Vector3f*, const sead::Vector3f&);
bool tryNormalizeOrZero(sead::Vector3f*);
}  // namespace al
