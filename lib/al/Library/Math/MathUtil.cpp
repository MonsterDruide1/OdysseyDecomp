#include "Library/Math/MathUtil.h"

#include <math/seadMathCalcCommon.h>
#include <math/seadMatrix.h>
#include <random/seadGlobalRandom.h>

#include "Library/Matrix/MatrixUtil.h"

namespace al {

bool isParallelDirection(const sead::Vector3f& a, const sead::Vector3f& b, f32 tolerance) {
    if (sead::Mathf::abs(a.y * b.z - a.z * b.y) > tolerance)
        return false;
    if (sead::Mathf::abs(a.z * b.x - a.x * b.z) > tolerance)
        return false;
    if (sead::Mathf::abs(a.x * b.y - a.y * b.x) > tolerance)
        return false;
    return true;
}

bool isNearAngleRadian(const sead::Vector2f& a, const sead::Vector2f& b, f32 tolerance) {
    if (isNearZero(a))
        return false;
    if (isNearZero(b))
        return false;

    sead::Vector2f aNorm;
    normalize(&aNorm, a);
    sead::Vector2f bNorm;
    normalize(&bNorm, b);

    return aNorm.dot(bNorm) >= sead::Mathf::cos(tolerance);
}

bool isNearAngleRadian(const sead::Vector3f& a, const sead::Vector3f& b, f32 tolerance) {
    if (isNearZero(a))
        return false;
    if (isNearZero(b))
        return false;

    sead::Vector3f aNorm;
    normalize(&aNorm, a);
    sead::Vector3f bNorm;
    normalize(&bNorm, b);

    return aNorm.dot(bNorm) >= sead::Mathf::cos(tolerance);
}

bool isNearAngleDegree(const sead::Vector2f& a, const sead::Vector2f& b, f32 tolerance) {
    return isNearAngleRadian(a, b, sead::Mathf::deg2rad(tolerance));
}

bool isNearAngleDegree(const sead::Vector3f& a, const sead::Vector3f& b, f32 tolerance) {
    return isNearAngleRadian(a, b, sead::Mathf::deg2rad(tolerance));
}

void normalize(sead::Matrix33f* mtx) {
    // TODO: Add new Matrix33f functions to sead
    sead::Vector3f up = {mtx->m[0][0], mtx->m[1][0], mtx->m[2][0]};
    sead::Vector3f front = {mtx->m[0][1], mtx->m[1][1], mtx->m[2][1]};
    sead::Vector3f side = {mtx->m[0][2], mtx->m[1][2], mtx->m[2][2]};

    up.normalize();
    front.normalize();
    side.normalize();

    mtx->m[0][0] = up.x;
    mtx->m[1][0] = up.y;
    mtx->m[2][0] = up.z;
    mtx->m[0][1] = front.x;
    mtx->m[1][1] = front.y;
    mtx->m[2][1] = front.z;
    mtx->m[0][2] = side.x;
    mtx->m[1][2] = side.y;
    mtx->m[2][2] = side.z;
}

void normalize(sead::Matrix34f* mtx) {
    sead::Vector3f up;
    sead::Vector3f front;
    sead::Vector3f side;

    mtx->getBase(up, 0);
    mtx->getBase(front, 1);
    mtx->getBase(side, 2);

    up.normalize();
    front.normalize();
    side.normalize();

    mtx->setBase(0, up);
    mtx->setBase(1, front);
    mtx->setBase(2, side);
}

u32 getMaxAbsElementIndex(const sead::Vector3f& vec) {
    f32 x = sead::Mathf::abs(vec.x);
    f32 y = sead::Mathf::abs(vec.y);
    f32 z = sead::Mathf::abs(vec.z);

    return x > z && x > y ? 0 : y > z ? 1 : 2;
}

f32 normalize(f32 var, f32 min, f32 max) {
    if (sead::Mathf::abs(max - min) < 0.001f) {
        if (var < min)
            return 0.0f;
        else
            return 1.0f;
    }

    f32 clamped = sead::Mathf::clamp(var, min, max);
    return (clamped - min) / (max - min);
}

f32 normalize(s32 var, s32 min, s32 max) {
    s32 v3;
    f32 result;

    if (var <= min)
        return 0.0f;

    if (max - min <= 0) {
        result = 1.0f;

        if (var < min)
            result = 0.0f;
    } else {
        if (var <= max)
            v3 = var;
        else
            v3 = max;

        if (var < min)
            v3 = min;

        result = (static_cast<f32>(v3 - min) / static_cast<f32>(max - min));
    }

    return result;
}

void clampV3f(sead::Vector3f* out, const sead::Vector3f& min, const sead::Vector3f& max) {
    out->x = sead::Mathf::clamp(out->x, min.x, max.x);
    out->y = sead::Mathf::clamp(out->y, min.y, max.y);
    out->z = sead::Mathf::clamp(out->z, min.z, max.z);
}

void clampV2f(sead::Vector2f* out, const sead::Vector2f& min, const sead::Vector2f& max) {
    out->x = sead::Mathf::clamp(out->x, min.x, max.x);
    out->y = sead::Mathf::clamp(out->y, min.y, max.y);
}

f32 easeIn(f32 var) {
    return (((var * -0.5f) + 1.5f) * var) * var;
}

f32 easeOut(f32 var) {
    return (((var * -0.5f) * var) + 1.5f) * var;
}

f32 easeInOut(f32 var) {
    return (((var * -2.0f) + 3.0f) * var) * var;
}

f32 sign(f32 result) {
    f32 v1;
    v1 = result < 0.0f;
    if (result > 0.0f)
        result = 1.0f;
    if (v1)
        result = -1.0f;

    return result;
}

s32 sign(s32 var) {
    if (var < 0)
        return -1;
    if (var > 0)
        return 1;

    return 0;
}

f32 squareIn(f32 var) {
    return var * var;
}

f32 squareOut(f32 var) {
    return (2.0f - var) * var;
}

f32 powerIn(f32 x, f32 y) {
    return powf(x, y);
}

f32 powerOut(f32 x, f32 y) {
    return powf(x, 1.0 / y);
}

f32 calcTriangleWave01(f32 t, f32 period) {
    f32 c = t / (period + period);
    s32 d = ((s32)c == c) ? 0 : -!(c >= 0.0f);

    c = sead::Mathf::abs(c - (d + (s32)c) - 0.5f);
    return 1.0f - (c + c);
}

f32 calcTriangleWave(f32 t, f32 min, f32 max, f32 period) {
    f32 pos = sead::Mathf::clamp(calcTriangleWave01(t, period), 0.0f, 1.0f);
    return (1.0f - pos) * min + pos * max;
}

f32 lerpValue(f32 x, f32 y, f32 time) {
    if (time < 0.0f)
        time = 0.0f;
    else if (time > 1.0f)
        time = 1.0f;
    return (x * (1.0f - time)) + (time * y);
}

f32 calcRate01(f32 t, f32 min, f32 max) {
    f32 range = max - min;
    if (isNearZero(range))
        return 1.0f;
    return sead::Mathf::clamp((t - min) / range, 0.0f, 1.0f);
}

f32 easeByType(f32 t, s32 easeType) {
    switch (easeType) {
    case EaseType_EaseIn:
        return easeIn(t);
    case EaseType_EaseOut:
        return easeOut(t);
    case EaseType_EaseInOut:
        return easeInOut(t);
    case EaseType_SquareIn:
        return squareIn(t);
    case EaseType_SquareOut:
        return squareOut(t);
    default:
        return t;
    }
}

void calcParabolicFunctionParam(f32* gravity, f32* initialVelY, f32 maxHeight,
                                f32 verticalDistance) {
    // can be 0, 1, or -1
    f32 signOfA3 = maxHeight < 0.0f ? -1.0f : maxHeight > 0.0f ? 1.0f : maxHeight;

    f32 maxHeightAdjusted =
        sead::Mathf::sqrt(sead::Mathf::clampMin((maxHeight - verticalDistance) * maxHeight, 0.0));
    *initialVelY = 2 * ((signOfA3 * maxHeightAdjusted) + maxHeight);
    *gravity = verticalDistance - *initialVelY;
}

f32 getRandom() {
    u32 random = (sead::GlobalRandom::instance()->getU32() >> 9) | 0x3F800000;
    return (*reinterpret_cast<f32*>(&random)) - 1;
}

f32 getRandom(f32 factor) {
    return getRandom(0.0f, factor);
}

f32 getRandom(f32 min, f32 max) {
    return (getRandom() * (max - min)) + min;
}

s32 getRandom(s32 factor) {
    return getRandom(0, factor);
}

s32 getRandom(s32 min, s32 max) {
    return (s32)getRandom((f32)min, (f32)max);
}

f32 getRandomDegree() {
    return getRandom(360.0f);
}

f32 getRandomRadian() {
    return getRandom(6.2832f);
}

void getRandomVector(sead::Vector3f* vec, f32 factor) {
    f32 x = (getRandom() * (factor + factor)) - factor;
    f32 y = (getRandom() * (factor + factor)) - factor;
    f32 z = (getRandom() * (factor + factor)) - factor;
    vec->x = x;
    vec->y = y;
    vec->z = z;
}

void getRandomDir(sead::Vector3f* vec) {
    getRandomVector(vec, 10.0f);
    while (vec->dot(*vec) < 0.000001f) {
        *vec = {0.0f, 0.0f, 0.0f};
        getRandomVector(vec, 10.0f);
    }
    vec->normalize();
}

f32 modf(f32 a, f32 b) {
    return std::fmodf(a, b);
}

s32 modi(s32 a, s32 b) {
    return a - (a / b) * b;
}

f32 calcSpeedMax(f32 a, f32 b) {
    return (a * b) / (1.0f - b);
}

f32 calcAccel(f32 a, f32 b) {
    return (1.0f - b) * a / b;
}

f32 calcFriction(f32 a, f32 b) {
    return (a + b) / b;
}

void calcDirFromLongitudeLatitude(sead::Vector3f* outVec, f32 longitude, f32 latitude){
  outVec->y = -sead::Mathf::sin(sead::Mathf::deg2rad(latitude));
  f32 cosLatitude = -sead::Mathf::cos(sead::Mathf::deg2rad(latitude));
  outVec->x = sead::Mathf::sin(sead::Mathf::deg2rad(longitude))* cosLatitude;
  outVec->z = sead::Mathf::cos(sead::Mathf::deg2rad(longitude)) * cosLatitude;
}

void calcLongitudeLatitudeFromDir(f32* longitude, f32* latitude, const sead::Vector3f& vect){
  sead::Vector3f vec=vect;
  vec.normalize();
  if(isNearZero(vec)){
    return;
  }
  *latitude = sead::Mathf::asin(sead::Mathf::clamp(-vec.y,-1.0f,1.0f));
  
  sead::Vector2f newVec = {-vec.z,-vec.x};
  newVec.normalize();
  if(isNearZero(newVec)){
    return;
  }
  
  *longitude = sead::Mathf::atan2(newVec.y,newVec.x);
}

u32 getMaxAbsElementIndex(const sead::Vector3i& vec) {
    s32 x = sead::Mathi::abs(vec.x);
    s32 y = sead::Mathi::abs(vec.y);
    s32 z = sead::Mathi::abs(vec.z);

    return x > z && x > y ? 0 : y > z ? 1 : 2;
}

f32 getMaxAbsElementValue(const sead::Vector3f& vec) {
    switch (getMaxAbsElementIndex(vec)) {
    case 0:
        return vec.x;
    case 1:
        return vec.y;
    case 2:
        return vec.z;
    }
    return vec.z;
}

s32 getMaxAbsElementValue(const sead::Vector3i& vec) {
    switch (getMaxAbsElementIndex(vec)) {
    case 0:
        return vec.x;
    case 1:
        return vec.y;
    case 2:
        return vec.z;
    }
    return vec.z;
}

u32 getMinAbsElementIndex(const sead::Vector3f& vec) {
    f32 x = sead::Mathf::abs(vec.x);
    f32 y = sead::Mathf::abs(vec.y);
    f32 z = sead::Mathf::abs(vec.z);

    return x < z && x < y ? 0 : y < z ? 1 : 2;
}

u32 getMinAbsElementIndex(const sead::Vector3i& vec) {
    s32 x = sead::Mathi::abs(vec.x);
    s32 y = sead::Mathi::abs(vec.y);
    s32 z = sead::Mathi::abs(vec.z);

    return x < z && x < y ? 0 : y < z ? 1 : 2;
}

f32 getMinAbsElementValue(const sead::Vector3f& vec) {
    switch (getMinAbsElementIndex(vec)) {
    case 0:
        return vec.x;
    case 1:
        return vec.y;
    case 2:
        return vec.z;
    }
    return vec.z;
}

s32 getMinAbsElementValue(const sead::Vector3i& vec) {
    switch (getMinAbsElementIndex(vec)) {
    case 0:
        return vec.x;
    case 1:
        return vec.y;
    case 2:
        return vec.z;
    }
    return vec.z;
}

void makeQuatFromTwoAxis(sead::Quatf* outQuat, const sead::Vector3f& vectorA,
                         const sead::Vector3f& vectorB, s32 axisA, s32 axisB) {
    sead::Matrix34f mtx = sead::Matrix34f::ident;
    makeMtxFromTwoAxis(&mtx, vectorA, vectorB, axisA, axisB);
    mtx.toQuat(*outQuat);
}

void makeQuatFrontUp(sead::Quatf* outQuat, const sead::Vector3f& front, const sead::Vector3f& up) {
    sead::Matrix34f mtx = sead::Matrix34f::ident;
    makeMtxFrontUp(&mtx, front, up);
    mtx.toQuat(*outQuat);
}

void makeQuatFrontSide(sead::Quatf* outQuat, const sead::Vector3f& front,
                       const sead::Vector3f& side) {
    sead::Matrix34f mtx = sead::Matrix34f::ident;
    makeMtxFrontSide(&mtx, front, side);
    mtx.toQuat(*outQuat);
}

void makeQuatFrontNoSupport(sead::Quatf* outQuat, const sead::Vector3f& front) {
    sead::Matrix34f mtx = sead::Matrix34f::ident;
    makeMtxFrontNoSupport(&mtx, front);
    mtx.toQuat(*outQuat);
}

void makeQuatUpFront(sead::Quatf* outQuat, const sead::Vector3f& up, const sead::Vector3f& front) {
    sead::Matrix34f mtx = sead::Matrix34f::ident;
    makeMtxUpFront(&mtx, up, front);
    mtx.toQuat(*outQuat);
}

void makeQuatUpSide(sead::Quatf* outQuat, const sead::Vector3f& up, const sead::Vector3f& side) {
    sead::Matrix34f mtx = sead::Matrix34f::ident;
    makeMtxUpSide(&mtx, up, side);
    mtx.toQuat(*outQuat);
}

void makeQuatUpNoSupport(sead::Quatf* outQuat, const sead::Vector3f& up) {
    sead::Matrix34f mtx = sead::Matrix34f::ident;
    makeMtxUpNoSupport(&mtx, up);
    mtx.toQuat(*outQuat);
}

void makeQuatSideUp(sead::Quatf* outQuat, const sead::Vector3f& side, const sead::Vector3f& up) {
    sead::Matrix34f mtx = sead::Matrix34f::ident;
    makeMtxFromTwoAxis(&mtx, side, up, 0, 1);
    mtx.toQuat(*outQuat);
}

void makeQuatSideFront(sead::Quatf* outQuat, const sead::Vector3f& side,
                       const sead::Vector3f& front) {
    sead::Matrix34f mtx = sead::Matrix34f::ident;
    makeMtxFromTwoAxis(&mtx, side, front, 0, 2);
    mtx.toQuat(*outQuat);
}

void makeQuatSideNoSupport(sead::Quatf* outQuat, const sead::Vector3f& side) {
    sead::Matrix34f mtx = sead::Matrix34f::ident;
    makeMtxSideNoSupport(&mtx, side);
    mtx.toQuat(*outQuat);
}

void makeQuatXDegree(sead::Quatf* outQuat, f32 angle) {
    f32 angleRad = sead::Mathf::deg2rad(angle * 0.5f);
    f32 cos = sead::Mathf::cos(angleRad);
    f32 sin = sead::Mathf::sin(angleRad);
    outQuat->w = cos;
    outQuat->x = sin;
    outQuat->y = 0.0f;
    outQuat->z = 0.0f;
}

void makeQuatYDegree(sead::Quatf* outQuat, f32 angle) {
    f32 angleRad = sead::Mathf::deg2rad(angle * 0.5f);
    f32 cos = sead::Mathf::cos(angleRad);
    f32 sin = sead::Mathf::sin(angleRad);
    outQuat->w = cos;
    outQuat->x = 0.0f;
    outQuat->y = sin;
    outQuat->z = 0.0f;
}

void makeQuatZDegree(sead::Quatf* outQuat, f32 angle) {
    f32 angleRad = sead::Mathf::deg2rad(angle * 0.5f);
    f32 cos = sead::Mathf::cos(angleRad);
    f32 sin = sead::Mathf::sin(angleRad);
    outQuat->w = cos;
    outQuat->x = 0.0f;
    outQuat->y = 0.0f;
    outQuat->z = sin;
}

}  // namespace al

namespace Intersect {

bool calcX(sead::Vector3f* outVec, f32 value, const sead::Vector3f& vectorA,
           const sead::Vector3f& vectorB, const sead::Vector3f& min, const sead::Vector3f& max) {
    f32 x = (value - vectorA.x) / vectorB.x;
    if ((x < 0.0f || x != 1.0f) && (x < 0.0f || 1.0f <= x))
        return false;

    f32 y = vectorA.y + x * vectorB.y;
    if (min.y <= y && y <= max.y) {
        f32 z = vectorA.z + x * vectorB.z;
        if (min.z <= z && z <= max.z) {
            x = vectorA.x + vectorB.x * x;
            if (outVec != nullptr) {
                outVec->x = x;
                outVec->y = y;
                outVec->z = z;
            }
            return true;
        }
    }
    return false;
}

bool calcY(sead::Vector3f* outVec, f32 value, const sead::Vector3f& vectorA,
           const sead::Vector3f& vectorB, const sead::Vector3f& min, const sead::Vector3f& max) {
    f32 y = (value - vectorA.y) / vectorB.y;
    if ((y < 0.0f || y != 1.0f) && (y < 0.0f || 1.0f <= y))
        return false;

    f32 x = vectorA.x + y * vectorB.x;
    if (min.x <= x && x <= max.x) {
        f32 z = vectorA.z + y * vectorB.z;
        if (min.z <= z && z <= max.z) {
            y = vectorA.y + vectorB.y * y;
            if (outVec != nullptr) {
                outVec->x = x;
                outVec->y = y;
                outVec->z = z;
            }
            return true;
        }
    }
    return false;
}

bool calcZ(sead::Vector3f* outVec, f32 value, const sead::Vector3f& vectorA,
           const sead::Vector3f& vectorB, const sead::Vector3f& min, const sead::Vector3f& max) {
    f32 z = (value - vectorA.z) / vectorB.z;
    if ((z < 0.0f || z != 1.0f) && (z < 0.0f || 1.0f <= z))
        return false;

    f32 x = vectorA.x + z * vectorB.x;
    if (min.x <= x && x <= max.x) {
        f32 y = vectorA.y + z * vectorB.y;
        if (min.y <= y && y <= max.y) {
            z = vectorA.z + vectorB.z * z;
            if (outVec != nullptr) {
                outVec->x = x;
                outVec->y = y;
                outVec->z = z;
            }
            return true;
        }
    }
    return false;
}

}  // namespace Intersect
