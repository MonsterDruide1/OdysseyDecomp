#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {
float normalize(float, float, float);
float normalize(signed int, signed int, signed int);

float easeIn(float);
float easeOut(float);
float easeInOut(float);

float sign(float);
int sign(int);

float squareIn(float);
float squareOut(float);

float powerIn(float, float);
float powerOut(float, float);

float lerpValue(float, float, float);

bool isNearZero(float, float);

template <typename T>
inline T clamp(T value, T min, T max) {
    if (value < min)
        return min;
    if (value > max)
        return max;
    return value;
}

void verticalizeVec(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&);
void parallelizeVec(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&);

f32 modf(f32 a, f32 b);

bool normalize(f32 value);
bool normalize(sead::Matrix33f* value);
bool normalize(sead::Matrix34f* value);

bool isNear(f32 value, f32 target, f32 tolerance);
bool isNear(const sead::Vector2f& value, const sead::Vector2f& target, f32 tolerance);
bool isNear(const sead::Vector3f& value, const sead::Vector3f& target, f32 tolerance);
bool isNear(const sead::Matrix34f& value, const sead::Matrix34f& target, f32 tolerance);
bool isNearZero(f32 value, f32 tolerance);
bool isNearZero(const sead::Vector2f& value, f32 tolerance);
bool isNearZero(const sead::Vector3f& value, f32 tolerance);
bool isNearZero(const sead::Matrix34f& value, f32 tolerance);
bool isNearZeroOrGreater(f32 value, f32 tolerance);
bool isNearZeroOrLess(f32 value, f32 tolerance);

void calcQuatSide(sead::Vector3f* out, const sead::Quatf& quat);
void calcQuatUp(sead::Vector3f* out, const sead::Quatf& quat);
void calcQuatGravity(sead::Vector3f* out, const sead::Quatf& quat);
void calcQuatFront(sead::Vector3f* out, const sead::Quatf& quat);

void makeMtxRotateTrans(sead::Matrix34f* out, const sead::Vector3f& rotate,
                        const sead::Vector3f& trans);
void makeMtxFrontUpPos(sead::Matrix34f* out, const sead::Vector3f& front, const sead::Vector3f& up,
                       const sead::Vector3f& pos);
void makeMtxUpFrontPos(sead::Matrix34f* out, const sead::Vector3f& up, const sead::Vector3f& front,
                       const sead::Vector3f& pos);

void separateVectorHV(sead::Vector3f*, sead::Vector3f*, const sead::Vector3f&,
                      const sead::Vector3f&);

void lerpVec(sead::Vector2f*, const sead::Vector2f&, const sead::Vector2f&, float);
void lerpVec(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&, float);
void lerpVecHV(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&, const sead::Vector3f&,
               float, float);

}  // namespace al
