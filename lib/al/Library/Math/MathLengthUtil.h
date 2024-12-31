#pragma once

#include <gfx/seadColor.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {
bool isNear(f32 value, f32 target, f32 tolerance);
bool isNear(const sead::Vector2f& value, const sead::Vector2f& target, f32 tolerance);
bool isNear(const sead::Vector3f& value, const sead::Vector3f& target, f32 tolerance);
bool isNear(const sead::Color4f& value, const sead::Color4f& target, f32 tolerance);
bool isNearZero(f32 value, f32 tolerance);
bool isNearZero(const sead::Matrix34f& value, f32 tolerance);
bool isNearZeroOrGreater(f32 value, f32 tolerance);
bool isNearZeroOrLess(f32 value, f32 tolerance);
bool isExistNearZeroVal(const sead::Vector3f&, f32);

bool isNormalize(const sead::Vector3f&, f32);
bool isNormalize(const sead::Matrix34f&);
bool isParallelDirection(const sead::Vector2f&, const sead::Vector2f&, f32);
bool isParallelDirection(const sead::Vector3f&, const sead::Vector3f&, f32);
bool isReverseDirection(const sead::Vector3f&, const sead::Vector3f&, f32);
bool isNearDirection(const sead::Vector2f&, const sead::Vector2f&, f32);
bool isNearDirection(const sead::Vector3f&, const sead::Vector3f&, f32);
bool isInRange(s32, s32, s32);
bool isInRange(f32, f32, f32);

void normalize(sead::Vector2f*);
void normalize(sead::Vector3f*);
void normalize(sead::Matrix33f*);
void normalize(sead::Matrix34f*);
bool tryNormalizeOrZero(sead::Vector2f*);
bool tryNormalizeOrZero(sead::Vector2f*, const sead::Vector2f&);
bool tryNormalizeOrDirZ(sead::Vector3f*);
bool tryNormalizeOrDirZ(sead::Vector3f*, const sead::Vector3f&);
void normalizeComplement(sead::Matrix34f*);

f32 normalizeAbs(f32, f32, f32);
f32 normalize(f32, f32, f32);
f32 normalize(s32, s32, s32);
bool limitLength(sead::Vector2f*, const sead::Vector2f&, f32);
bool limitLength(sead::Vector3f*, const sead::Vector3f&, f32);
bool limitLength(sead::Vector2f*, f32);
bool limitLength(sead::Vector3f*, f32);

u32 getMaxAbsElementIndex(const sead::Vector3f&);
void setLength(sead::Vector3f*, f32);
void setProjectionLength(sead::Vector3f*, const sead::Vector3f&, f32);

f32 sign(f32);
s32 sign(s32);
f32 cubeRoot(f32);

void clampV3f(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&);
void clampV2f(sead::Vector2f*, const sead::Vector2f&, const sead::Vector2f&);
}  // namespace al
