#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {
f32 easeIn(f32);
f32 easeOut(f32);
f32 easeInOut(f32);

f32 squareIn(f32);
f32 squareOut(f32);

f32 powerIn(f32, f32);
f32 powerOut(f32, f32);

f32 lerpValue(f32, f32, f32);
f32 lerpValue(f32, f32, f32, f32, f32);

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
s32 modi(s32 a, s32 b);

bool normalize(f32 value);
void normalize(sead::Matrix33f*);
void normalize(sead::Matrix34f*);
f32 normalize(f32 value, f32, f32);

bool isNear(const sead::Matrix34f& value, const sead::Matrix34f& target, f32 tolerance);

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

void lerpVec(sead::Vector2f*, const sead::Vector2f&, const sead::Vector2f&, f32);
void lerpVec(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&, f32);
void lerpVecHV(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&, const sead::Vector3f&,
               f32, f32);

void rotateVectorDegree(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&, f32);

f32 calcRate01(f32, f32, f32);

f32 slerpQuat(sead::Quatf*, const sead::Quatf&, const sead::Quatf&, f32);

bool checkHitSegmentSphere(const sead::Vector3f&, const sead::Vector3f&, const sead::Vector3f&, f32,
                           sead::Vector3f*, sead::Vector3f*);

s32 converge(s32, s32, s32);
f32 converge(f32, f32, f32);
f32 convergeDegree(f32, f32, f32);
f32 convergeRadian(f32, f32, f32);
void convergeVec(sead::Vector2f*, const sead::Vector2f&, const sead::Vector2f&, f32);
void convergeVec(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&, f32);

void separateScalarAndDirection(f32*, sead::Vector3f*, const sead::Vector3f&);

void calcParabolicFunctionParam(f32*, f32*, f32, f32 verticalDistance);

f32 hermiteRate(f32, f32, f32);
void calcDirVerticalAny(sead::Vector3f*, const sead::Vector3f&);

void makeQuatRotationRate(sead::Quatf*, const sead::Vector3f&, const sead::Vector3f&, f32);

}  // namespace al
