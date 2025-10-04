#include "Library/Math/MathUtil.h"

#include <math/seadMathCalcCommon.h>
#include <math/seadMatrix.h>
#include <nn/os.h>
#include <prim/seadBitUtil.h>
#include <random/seadGlobalRandom.h>

#include "Library/Base/HashCodeUtil.h"
#include "Library/Matrix/MatrixUtil.h"

namespace al {
f32 calcAngleRadian(const sead::Vector3f& a, const sead::Vector3f& b) {
    f32 dot = a.dot(b);
    sead::Vector3f cross;
    cross.setCross(a, b);
    return sead::Mathf::atan2(cross.length(), dot);
}

f32 calcAngleDegree(const sead::Vector3f& a, const sead::Vector3f& b) {
    return sead::Mathf::rad2deg(calcAngleRadian(a, b));
}

f32 calcAngleDegree(const sead::Vector2f& a, const sead::Vector2f& b) {
    if (isNearZero(a) || isNearZero(b))
        return 0.0f;

    return sead::Mathf::rad2deg(sead::Mathf::atan2(a.cross(b), a.dot(b)));
}

bool isNearZero(const sead::Vector2f& vec, f32 tolerance) {
    return vec.squaredLength() < tolerance * tolerance;
}

bool tryCalcAngleDegree(f32* out, const sead::Vector3f& a, const sead::Vector3f& b) {
    if (isNearZero(a) || isNearZero(b))
        return false;

    *out = calcAngleDegree(a, b);
    return true;
}

bool isNearZero(const sead::Vector3f& vec, f32 tolerance) {
    return vec.squaredLength() < tolerance * tolerance;
}

f32 calcAngleOnPlaneRadian(const sead::Vector3f& a, const sead::Vector3f& b,
                           const sead::Vector3f& vertical) {
    sead::Vector3f planeA;
    verticalizeVec(&planeA, vertical, a);
    sead::Vector3f planeB;
    verticalizeVec(&planeB, vertical, b);

    f32 dot = planeA.dot(planeB);
    sead::Vector3f cross;
    cross.setCross(planeA, planeB);
    f32 angle = sead::Mathf::atan2(cross.length(), dot);

    return vertical.dot(cross) < 0.0f ? -angle : angle;
}

/**
 * Takes the plane perpendicular to unit vector `vertical`, projects `vec` onto it, and
 * stores the result in `out`. The effect is that `vec` and `out` will look equal
 * if looking in the direction of `vertical`.
 */
void verticalizeVec(sead::Vector3f* out, const sead::Vector3f& vertical,
                    const sead::Vector3f& vec) {
    out->setScaleAdd(-vertical.dot(vec), vertical, vec);
}

f32 calcAngleOnPlaneDegree(const sead::Vector3f& a, const sead::Vector3f& b,
                           const sead::Vector3f& vertical) {
    return sead::Mathf::rad2deg(calcAngleOnPlaneRadian(a, b, vertical));
}

f32 calcAngleOnPlaneDegreeOrZero(const sead::Vector3f& a, const sead::Vector3f& b,
                                 const sead::Vector3f& vertical) {
    f32 angle = 0.0f;
    if (!tryCalcAngleOnPlaneDegree(&angle, a, b, vertical))
        return 0.0f;

    return angle;
}

bool isParallelDirection(const sead::Vector3f& a, const sead::Vector3f& b, f32 tolerance) {
    if (sead::Mathf::abs(a.y * b.z - a.z * b.y) > tolerance)
        return false;
    if (sead::Mathf::abs(a.z * b.x - a.x * b.z) > tolerance)
        return false;
    if (sead::Mathf::abs(a.x * b.y - a.y * b.x) > tolerance)
        return false;
    return true;
}

s32 calcAngleSignOnPlane(const sead::Vector3f& a, const sead::Vector3f& b,
                         const sead::Vector3f& vertical) {
    sead::Vector3f planeA;
    verticalizeVec(&planeA, vertical, a);
    sead::Vector3f planeB;
    verticalizeVec(&planeB, vertical, b);

    sead::Vector3f cross;
    cross.setCross(planeA, planeB);
    const f32 angle = vertical.dot(cross);

    if (angle > 0.0f)
        return 1;
    if (angle < 0.0f)
        return -1;

    return 0;
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

void normalize(sead::Vector2f* out, const sead::Vector2f& vec) {
    *out = vec;
    normalize(out);
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

void normalize(sead::Vector3f* out, const sead::Vector3f& vec) {
    *out = vec;
    normalize(out);
}

bool isNearAngleDegree(const sead::Vector2f& a, const sead::Vector2f& b, f32 tolerance) {
    return isNearAngleRadian(a, b, sead::Mathf::deg2rad(tolerance));
}

bool isNearAngleDegree(const sead::Vector3f& a, const sead::Vector3f& b, f32 tolerance) {
    return isNearAngleRadian(a, b, sead::Mathf::deg2rad(tolerance));
}

bool tryNormalizeOrZero(sead::Vector3f* vec) {
    if (isNearZero(*vec)) {
        *vec = {0.0f, 0.0f, 0.0f};
        return false;
    }

    normalize(vec);
    return true;
}

bool tryNormalizeOrZero(sead::Vector3f* out, const sead::Vector3f& vec) {
    *out = vec;
    return tryNormalizeOrZero(out);
}

// TODO: Rename parameters here and in header
bool isNearAngleDegreeHV(const sead::Vector3f& a, const sead::Vector3f& b, const sead::Vector3f& c,
                         f32 d, f32 e) {
    return isNearAngleRadianHV(a, b, c, sead::Mathf::deg2rad(d), sead::Mathf::deg2rad(e));
}

bool isNear(f32 value, f32 target, f32 tolerance) {
    return sead::Mathf::abs(value - target) < sead::Mathf::abs(tolerance);
}

bool isNear(const sead::Vector2f& value, const sead::Vector2f& target, f32 tolerance) {
    return (value - target).length() <= tolerance;
}

bool isNear(const sead::Vector3f& value, const sead::Vector3f& target, f32 tolerance) {
    return (value - target).length() <= tolerance;
}

bool isNear(const sead::Color4f& value, const sead::Color4f& target, f32 tolerance) {
    return sead::Mathf::abs(value.r - target.r) < tolerance &&
           sead::Mathf::abs(value.g - target.g) < tolerance &&
           sead::Mathf::abs(value.b - target.b) < tolerance &&
           sead::Mathf::abs(value.a - target.a) < tolerance;
}

bool isNearZero(f32 value, f32 tolerance) {
    return sead::Mathf::abs(value) < tolerance;
}

bool isNearZero(const sead::Matrix34f& value, f32 tolerance) {
    sead::Vector3f vec;

    value.getBase(vec, 0);
    if (isNearZero(vec, tolerance))
        return true;
    value.getBase(vec, 1);
    if (isNearZero(vec, tolerance))
        return true;
    value.getBase(vec, 2);
    if (isNearZero(vec, tolerance))
        return true;

    return false;
}

bool isNearZeroOrGreater(f32 value, f32 tolerance) {
    return value >= 0.0f || isNearZero(value, tolerance);
}

bool isNearZeroOrLess(f32 value, f32 tolerance) {
    return value <= 0.0f || isNearZero(value, tolerance);
}

bool isExistNearZeroVal(const sead::Vector3f& vec, f32 tolerance) {
    return isNearZero(vec.x, tolerance) || isNearZero(vec.y, tolerance) ||
           isNearZero(vec.z, tolerance);
}

bool isNormalize(const sead::Vector3f& vec, f32 tolerance) {
    return sead::Mathf::abs(1.0f - vec.length()) <= tolerance;
}

bool isParallelDirection(const sead::Vector2f& a, const sead::Vector2f& b, f32 tolerance) {
    return !(sead::Mathf::abs(a.cross(b)) > tolerance);
}

bool isNearDirection(const sead::Vector2f& a, const sead::Vector2f& b, f32 tolerance) {
    if (a.dot(b) < 0.0f)
        return false;

    return isParallelDirection(a, b, tolerance);
}

bool isInRange(s32 x, s32 a, s32 b) {
    return (b < a) ? (a >= x && x >= b) : (b >= x && x >= a);
}

bool isInRange(f32 x, f32 a, f32 b) {
    if (b < a) {
        if (x < b || a < x)
            return false;
        return true;
    } else {
        if (x < a || b < x)
            return false;
        return true;
    }
}

void normalize(sead::Vector2f* vec) {
    vec->normalize();
}

void normalize(sead::Vector3f* vec) {
    vec->normalize();
}

void normalize(sead::Matrix33f* mtx) {
    sead::Vector3f up = mtx->getBase(0);
    sead::Vector3f front = mtx->getBase(1);
    sead::Vector3f side = mtx->getBase(2);

    up.normalize();
    front.normalize();
    side.normalize();

    mtx->setBase(0, up);
    mtx->setBase(1, front);
    mtx->setBase(2, side);
}

void normalize(sead::Matrix34f* mtx) {
    sead::Vector3f up = mtx->getBase(0);
    sead::Vector3f front = mtx->getBase(1);
    sead::Vector3f side = mtx->getBase(2);

    up.normalize();
    front.normalize();
    side.normalize();

    mtx->setBase(0, up);
    mtx->setBase(1, front);
    mtx->setBase(2, side);
}

bool tryNormalizeOrZero(sead::Vector2f* vec) {
    if (isNearZero(*vec)) {
        *vec = {0.0f, 0.0f};
        return false;
    }

    normalize(vec);
    return true;
}

bool tryNormalizeOrZero(sead::Vector2f* out, const sead::Vector2f& vec) {
    *out = vec;
    return tryNormalizeOrZero(out);
}

bool tryNormalizeOrDirZ(sead::Vector3f* vec) {
    if (isNearZero(*vec)) {
        // mismatches if this isn't set twice
        vec->set(0.0f, 0.0f, 0.0f);
        vec->set(sead::Vector3f::ez);
        return false;
    }

    normalize(vec);
    return true;
}

bool tryNormalizeOrDirZ(sead::Vector3f* out, const sead::Vector3f& vec) {
    *out = vec;
    return tryNormalizeOrDirZ(out);
}

u32 getMaxAbsElementIndex(const sead::Vector3f& vec) {
    f32 x = sead::Mathf::abs(vec.x);
    f32 y = sead::Mathf::abs(vec.y);
    f32 z = sead::Mathf::abs(vec.z);

    return x > z && x > y ? 0 : y > z ? 1 : 2;
}

void setLength(sead::Vector3f* vec, f32 length) {
    f32 curLen = vec->length();
    if (curLen > 0.0f) {
        f32 scale = length / curLen;
        *vec *= scale;
    }
}

void setProjectionLength(sead::Vector3f* out, const sead::Vector3f& vec, f32 length) {
    f32 scale = length / sead::Mathf::abs(vec.dot(*out));
    *out *= scale;
}

bool limitLength(sead::Vector2f* out, const sead::Vector2f& vec, f32 limit) {
    f32 len = vec.length();
    if (len > limit) {
        f32 invLen = limit / len;
        out->setScale(vec, invLen);
        return true;
    } else {
        out->set(vec);
        return false;
    }
}

bool limitLength(sead::Vector3f* out, const sead::Vector3f& vec, f32 limit) {
    f32 len = vec.length();
    if (len > limit) {
        f32 invLen = limit / len;
        out->setScale(vec, invLen);
        return true;
    } else {
        out->set(vec);
        return false;
    }
}

f32 normalizeAbs(f32 x, f32 min, f32 max) {
    if (x >= 0)
        return normalize(x, min, max);
    else
        return -normalize(-x, min, max);
}

f32 normalize(f32 x, f32 min, f32 max) {
    if (sead::Mathf::abs(max - min) < 0.001f) {
        if (x < min)
            return 0.0f;
        else
            return 1.0f;
    }

    f32 clamped = sead::Mathf::clamp(x, min, max);
    return (clamped - min) / (max - min);
}

f32 normalize(s32 x, s32 min, s32 max) {
    if (x <= min)
        return 0.0f;
    if (max - min <= 0) {
        if (x < min)
            return 0.0f;
        else
            return 1.0f;
    }

    return (static_cast<f32>(clamp(x, min, max) - min) / static_cast<f32>(max - min));
}

f32 sign(f32 x) {
    if (x < 0.0f)
        return -1.0f;
    if (x > 0.0f)
        return 1.0f;
    return x;
}

s32 sign(s32 x) {
    if (x < 0)
        return -1;
    if (x > 0)
        return 1;
    return x;
}

f32 cubeRoot(f32 x) {
    f32 onethird = 1.0f / 3.0f;

    u32 i = 0x54a0fc86 - sead::BitUtil::bitCast<u32>(x) / 3;
    f32 y = sead::BitUtil::bitCast<f32>(i);

    y = y * onethird * (4.0f - x * y * y * y);
    y = y * onethird * (4.0f - x * y * y * y);
    y = y * onethird * (4.0f - x * y * y * y);
    return x * y * y;
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

f32 easeIn(f32 t) {
    return (((t * -0.5f) + 1.5f) * t) * t;
}

f32 easeOut(f32 t) {
    return (((t * -0.5f) * t) + 1.5f) * t;
}

f32 easeInOut(f32 t) {
    return (((t * -2.0f) + 3.0f) * t) * t;
}

f32 squareIn(f32 t) {
    return t * t;
}

f32 squareOut(f32 t) {
    return (2.0f - t) * t;
}

f32 powerIn(f32 t, f32 exp) {
    return sead::Mathf::pow(t, exp);
}

f32 powerOut(f32 t, f32 exp) {
    return sead::Mathf::pow(t, 1.0 / exp);
}

f32 logarithmIn(f32 t, f32 base) {
    f32 base1 = base + sead::Mathf::epsilon();
    f32 a = powf(base1 + 0.0f, 1.0 - t);
    f32 b = powf(base1 + 1.0f, t);
    return a * b - base1;
}

f32 logarithmOut(f32 t, f32 base) {
    return 1.0f - logarithmIn(1.0f - t, base);
}

f32 exponentIn(f32 t, f32 exp) {
    return t * exp2f(exp * (t - 1.0f));
}

f32 exponentOut(f32 t, f32 exp) {
    return 1.0f - exponentIn(1.0f - t, exp);
}

f32 hermiteRate(f32 t, f32 m0, f32 m1) {
    return hermite(0.0f, m0, 1.0f, m1, t);
}

f32 calcFourthOrderRate(f32 t, f32 scale) {
    return ((scale + -3.0f) * t * t + (scale * -2.0f + 4.0f) * t + scale) * t * t;
}

f32 calcTriangleWave01(f32 t, f32 period) {
    f32 val = t / (2 * period);
    s32 rounded = (s32)val + (((s32)val == val) ? 0 : -!(val >= 0.0f));

    val = sead::Mathf::abs(val - rounded - 0.5f);
    return 1.0f - (2 * val);
}

f32 calcTriangleWave(f32 t, f32 min, f32 max, f32 period) {
    f32 pos = sead::Mathf::clamp(calcTriangleWave01(t, period), 0.0f, 1.0f);
    return (1.0f - pos) * min + pos * max;
}

f32 lerpValue(f32 a, f32 b, f32 t) {
    t = sead::Mathf::clamp(t, 0.0f, 1.0f);
    return a * (1.0f - t) + t * b;
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

inline f32 clamp(f32 value, f32 low, f32 high) {
    f32 result = high;
    if (value < low)
        result = low;
    else if (!(value > high))
        result = value;
    return result;
}

f32 lerpValue(f32 a, f32 b, f32 t, f32 clampA, f32 clampB) {
    if (sead::Mathf::abs(t - b) < 0.001f)
        return a <= b ? clampA : clampB;

    f32 rate = (a - b) / (t - b);
    f32 t2 = clamp(rate, 0.0f, 1.0f);

    return clampA * (1.0f - t2) + t2 * clampB;
}

f32 lerpDegree(f32 a, f32 b, f32 t) {
    a = modf(a + 360.0f, 360.0f) + 0.0f;
    b = modf(b + 360.0f, 360.0f) + 0.0f;

    f32 aa = b - a > 180.0f ? a + 360.0f : a;
    f32 bb = b - a < -180.0f ? b + 360.0f : b;

    return modf(lerpValue(aa, bb, t) + 360.0f, 360.0f) + 0.0f;
}

f32 lerpRadian(f32 a, f32 b, f32 t) {
    a = modf(a + sead::Mathf::pi2(), sead::Mathf::pi2()) + 0.0f;
    b = modf(b + sead::Mathf::pi2(), sead::Mathf::pi2()) + 0.0f;

    f32 aa = b - a > sead::Mathf::pi() ? a + sead::Mathf::pi2() : a;
    f32 bb = b - a < -sead::Mathf::pi() ? b + sead::Mathf::pi2() : b;

    return modf(lerpValue(aa, bb, t) + sead::Mathf::pi2(), sead::Mathf::pi2()) + 0.0f;
}

void lerpVec(sead::Vector2f* outVec, const sead::Vector2f& a, const sead::Vector2f& b, f32 t) {
    outVec->x = a.x + (b.x - a.x) * t;
    outVec->y = a.y + (b.y - a.y) * t;
}

void lerpVec(sead::Vector3f* outVec, const sead::Vector3f& a, const sead::Vector3f& b, f32 t) {
    outVec->x = a.x + (b.x - a.x) * t;
    outVec->y = a.y + (b.y - a.y) * t;
    outVec->z = a.z + (b.z - a.z) * t;
}

void lerpVecHV(sead::Vector3f* outVec, const sead::Vector3f& a, const sead::Vector3f& b,
               const sead::Vector3f& c, f32 tH, f32 tV) {
    sead::Vector3f ba = b - a;
    sead::Vector3f totalV = c * ba.dot(c);

    outVec->x = a.x + (ba.x - totalV.x) * tH;
    outVec->y = a.y + (ba.y - totalV.y) * tH;
    outVec->z = a.z + (ba.z - totalV.z) * tH;

    outVec->x += totalV.x * tV;
    outVec->y += totalV.y * tV;
    outVec->z += totalV.z * tV;
}

void separateVectorHV(sead::Vector3f* outH, sead::Vector3f* outV, const sead::Vector3f& a,
                      const sead::Vector3f& b) {
    f32 dot = a.dot(b);

    outV->x = a.x * dot;
    outV->y = a.y * dot;
    outV->z = a.z * dot;

    outH->x = b.x - outV->x;
    outH->y = b.y - outV->y;
    outH->z = b.z - outV->z;
}

void lerpColor(sead::Color4f* outColor, const sead::Color4f& a, const sead::Color4f& b, f32 t) {
    outColor->setLerp(a, b, t);
}

f32 lerpLogValueEaseIn(f32 a, f32 b, f32 max, f32 min) {
    return lerpValue(a, b, logarithmIn(max, min));
}

f32 lerpLogValueEaseOut(f32 a, f32 b, f32 max, f32 min) {
    return lerpValue(a, b, logarithmOut(max, min));
}

void lerpLogVecEaseIn(sead::Vector3f* outVec, const sead::Vector3f& a, const sead::Vector3f& b,
                      f32 max, f32 min) {
    f32 x = lerpLogValueEaseIn(a.x, b.x, max, min);
    f32 y = lerpLogValueEaseIn(a.y, b.y, max, min);
    f32 z = lerpLogValueEaseIn(a.z, b.z, max, min);
    outVec->x = x;
    outVec->y = y;
    outVec->z = z;
}

void lerpLogVecEaseOut(sead::Vector3f* outVec, const sead::Vector3f& a, const sead::Vector3f& b,
                       f32 max, f32 min) {
    f32 x = lerpLogValueEaseOut(a.x, b.x, max, min);
    f32 y = lerpLogValueEaseOut(a.y, b.y, max, min);
    f32 z = lerpLogValueEaseOut(a.z, b.z, max, min);
    outVec->x = x;
    outVec->y = y;
    outVec->z = z;
}

f32 lerpExponentValueEaseIn(f32 a, f32 b, f32 max, f32 min) {
    return lerpValue(a, b, exponentIn(max, min));
}

f32 lerpExponentValueEaseOut(f32 a, f32 b, f32 max, f32 min) {
    return lerpValue(a, b, exponentOut(max, min));
}

void lerpExponentVecEaseIn(sead::Vector3f* outVec, const sead::Vector3f& a, const sead::Vector3f& b,
                           f32 max, f32 min) {
    f32 x = lerpExponentValueEaseIn(a.x, b.x, max, min);
    f32 y = lerpExponentValueEaseIn(a.y, b.y, max, min);
    f32 z = lerpExponentValueEaseIn(a.z, b.z, max, min);
    outVec->x = x;
    outVec->y = y;
    outVec->z = z;
}

void lerpExponentVecEaseOut(sead::Vector3f* outVec, const sead::Vector3f& a,
                            const sead::Vector3f& b, f32 max, f32 min) {
    f32 x = lerpExponentValueEaseOut(a.x, b.x, max, min);
    f32 y = lerpExponentValueEaseOut(a.y, b.y, max, min);
    f32 z = lerpExponentValueEaseOut(a.z, b.z, max, min);
    outVec->x = x;
    outVec->y = y;
    outVec->z = z;
}

// BUG: should've been called clampLerpMinAbs
f32 clampLeapMinAbs(f32 t, f32 beforeLerp, f32 startLerp, f32 endLerp) {
    if (sead::Mathf::abs(t) < sead::Mathf::abs(startLerp))
        return sign(t) * sead::Mathf::abs(beforeLerp);

    if (sead::Mathf::abs(t) > sead::Mathf::abs(endLerp))
        return t;

    return sign(t) * lerpValue(sead::Mathf::abs(t), sead::Mathf::abs(startLerp),
                               sead::Mathf::abs(endLerp), sead::Mathf::abs(beforeLerp),
                               sead::Mathf::abs(endLerp));
}

/**
 * Interpolates between `y0` and `y1` as `t` goes from 0.0 to 1.0. This interpolation is defined by
 * `m0` and `m1`, which are the rates of change of `t` at the points `y0` and `y1` respectively.
 */
f32 hermite(f32 y0, f32 m0, f32 y1, f32 m1, f32 t) {
    f32 coef_m1 = t * (t * t - t);
    f32 coef_y1 = t * t + -2.0f * coef_m1;
    f32 coef_m0 = coef_m1 - (t * t - t);
    return y0 - coef_y1 * y0 + coef_y1 * y1 + coef_m0 * m0 + coef_m1 * m1;
}

f32 hermite(f32 y0, f32 m0, f32 y1, f32 m1, f32 t, f32 width) {
    t *= 1.0f / width;
    f32 a1 = y0 - y1;
    f32 a2 = t - 1.0f;
    f32 a3 = t + t - 3.0f;
    return y0 + (a1 * a3) * t * t + (t * a2) * (t * m1 + a2 * m0);
}

void hermiteVec(sead::Vector3f* outVec, const sead::Vector3f& y0, const sead::Vector3f& m0,
                const sead::Vector3f& y1, const sead::Vector3f& m1, f32 t) {
    outVec->x = hermite(y0.x, m0.x, y1.x, m1.x, t);
    outVec->y = hermite(y0.y, m0.y, y1.y, m1.y, t);
    outVec->z = hermite(y0.z, m0.z, y1.z, m1.z, t);
}

s32 converge(s32 current, s32 target, s32 step) {
    s32 result = current;

    if (current < target) {
        result += step;
        if (result > target)
            result = target;
    } else {
        result -= step;
        if (result < target)
            result = target;
    }

    return result;
}

f32 converge(f32 current, f32 target, f32 step) {
    f32 result = current;

    if (current < target) {
        result += step;
        if (result > target)
            result = target;
    } else {
        result -= step;
        if (result < target)
            result = target;
    }

    return result;
}

f32 convergeDegree(f32 current, f32 target, f32 step) {
    if ((target + 360.0f) - current < 180.0f)
        target += 360.0f;
    else if (current - (target - 360.0f) < 180.0f)
        target -= 360.0f;

    return fmod(converge(current, target, step) + 360.0f, 360.0f) + 0.0f;
}

f32 convergeRadian(f32 current, f32 target, f32 step) {
    // BUG: N's mistake here. Correct comparison: (target + pi2()) - current < pi()
    if ((target + sead::Mathf::pi2()) - current < sead::Mathf::pi2())
        target += sead::Mathf::pi2();
    else if (current - (target - sead::Mathf::pi2()) < sead::Mathf::pi())
        target -= sead::Mathf::pi2();

    return fmod(converge(current, target, step) + sead::Mathf::pi2(), sead::Mathf::pi2()) + 0.0f;
}

bool convergeVec(sead::Vector2f* outVec, const sead::Vector2f& current,
                 const sead::Vector2f& target, f32 step) {
    sead::Vector2f dir = target - current;

    bool isReachedTarget = true;
    f32 length = dir.length();
    if (length > step) {
        dir *= step / length;
        isReachedTarget = false;
    }

    // TODO: Replace with outVec->setAdd(current, dir);
    outVec->x = dir.x + current.x;
    outVec->y = dir.y + current.y;
    return isReachedTarget;
}

bool convergeVec(sead::Vector3f* outVec, const sead::Vector3f& current,
                 const sead::Vector3f& target, f32 step) {
    sead::Vector3f dir = target - current;

    bool isReachedTarget = true;
    f32 length = dir.length();
    if (length > step) {
        dir *= step / length;
        isReachedTarget = false;
    }

    outVec->setAdd(current, dir);
    return isReachedTarget;
}

f32 diffNearAngleDegree(f32 a, f32 b) {
    f32 angle = modf(b - a + 360.0f, 360.0f) + 0.0f;
    if (angle >= 180.0f)
        angle -= 360.0f;
    return angle;
}

bool isSameSign(f32 a, f32 b) {
    return a * b > 0.0f;
}

u8 reverseBit8(u8 x) {
    x = ((x & 0x55) << 1) | ((x >> 1) & 0x55);  // 0101...
    x = ((x & 0x33) << 2) | ((x >> 2) & 0x33);  // 0011...
    return x >> 4 | x << 4;
}

u16 reverseBit16(u16 x) {
    x = ((x & 0x5555) << 1) | ((x >> 1) & 0x5555);  // 01010101...
    x = ((x & 0x3333) << 2) | ((x >> 2) & 0x3333);  // 00110011...
    x = ((x & 0xf0f) << 4) | ((x >> 4) & 0xf0f);    // 11110000..
    return x >> 8 | x << 8;
}

u32 reverseBit32(u32 x) {
    x = ((x & 0x55555555) << 1) | ((x >> 1) & 0x55555555);  // 0101010101010101...
    x = ((x & 0x33333333) << 2) | ((x >> 2) & 0x33333333);  // 0011001100110011...
    x = ((x & 0xf0f0f0f) << 4) | ((x >> 4) & 0xf0f0f0f);    // 1111000011110000...
    x = ((x & 0xff00ff) << 8) | ((x >> 8) & 0xff00ff);      // 1111111100000000...
    return x >> 16 | x << 16;
}

f32 calcVanDerCorput(u32 x) {
    return reverseBit32(x) * (f32)0x1p-32;
}

void calcHammersleyPoint(sead::Vector2f* outPoint, u32 i, u32 num) {
    outPoint->x = (f32)i / (f32)num;
    outPoint->y = calcVanDerCorput(i);
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

void getRandomOnCircle(sead::Vector2f* outPos, f32 radius) {
    f32 angle = getRandom(sead::Mathf::pi2());

    outPos->x = sead::Mathf::cos(angle) * radius;
    outPos->y = sead::Mathf::sin(angle) * radius;
}

void getRandomInCircle(sead::Vector2f* outPos, f32 maxRadius) {
    f32 angle = getRandom(sead::Mathf::pi2());
    f32 radius = sead::Mathf::sqrt(getRandom()) * maxRadius;

    outPos->x = radius * sead::Mathf::cos(angle);
    outPos->y = radius * sead::Mathf::sin(angle);
}

void getRandomInCircleMinMaxRadius(sead::Vector2f* outPos, f32 minRadius, f32 maxRadius) {
    f32 angle = getRandom(sead::Mathf::pi2());
    f32 range = sead::Mathf::square(minRadius / maxRadius);
    f32 radius = sead::Mathf::sqrt(range + getRandom() * (1.0f - range)) * maxRadius;

    outPos->x = radius * sead::Mathf::cos(angle);
    outPos->y = radius * sead::Mathf::sin(angle);
}

void getRandomInCircle(sead::Vector3f* outPos, const sead::Vector3f& pos,
                       const sead::Vector3f& front, f32 maxRadius) {
    sead::Matrix34f mtx;
    makeMtxFrontNoSupportPos(&mtx, front, pos);

    sead::Vector2f pos2D;
    getRandomInCircle(&pos2D, maxRadius);

    outPos->setMul(mtx, {pos2D.x, pos2D.y, 0.0f});
}

void getRandomOnSphere(sead::Vector3f* outPos, f32 radius) {
    f32 angle = getRandom(sead::Mathf::pi2());
    f32 zPos = 2.0f * getRandom() - 1.0f;
    f32 radiusXY = sead::Mathf::sqrt(1.0f - sead::Mathf::square(zPos)) * radius;

    outPos->x = sead::Mathf::cos(angle) * radiusXY;
    outPos->y = sead::Mathf::sin(angle) * radiusXY;
    outPos->z = zPos * radius;
}

void initRandomSeed(u32 seed) {
    sead::GlobalRandom::instance()->init(seed);
}

void initRandomSeedByTick() {
    initRandomSeed(nn::os::GetSystemTick().value);
}

void initRandomSeedByString(const char* name) {
    initRandomSeed(calcHashCode(name));
}

bool isHalfProbability() {
    return getRandom() < 0.5f;
}

bool isPercentProbability(f32 threshold) {
    return getRandom() * 100.0f < threshold;
}

void getRandomContext(u32* xSeed, u32* ySeed, u32* zSeed, u32* wSeed) {
    sead::GlobalRandom::instance()->getContext(xSeed, ySeed, zSeed, wSeed);
}

void setRandomContext(u32 xSeed, u32 ySeed, u32 zSeed, u32 wSeed) {
    sead::GlobalRandom::instance()->init(xSeed, ySeed, zSeed, wSeed);
}

void makeRandomDirXZ(sead::Vector3f* outDir) {
    f32 angle = getRandom(sead::Mathf::pi2());

    outDir->set({sead::Mathf::sin(angle), 0.0f, sead::Mathf::cos(angle)});
}

f32 calcBoxMullerRandomGauss() {
    sead::Vector2f box;
    makeBoxMullerRandomGauss(&box, getRandom(), getRandom());

    return box.x;
}

void makeBoxMullerRandomGauss(sead::Vector2f* outBox, f32 randA, f32 randB) {
    f32 valX = sead::Mathf::sqrt(sead::Mathf::log(randA) * -2.0f) *
               sead::Mathf::cos(sead::Mathf::pi2() * randB);
    f32 valY = sead::Mathf::sqrt(sead::Mathf::log(randB) * -2.0f) *
               sead::Mathf::sin(sead::Mathf::pi2() * randA);
    outBox->set({valX, valY});
}

f32 modf(f32 a, f32 b) {
    return std::fmodf(a, b);
}

s32 modi(s32 a, s32 b) {
    return a - (a / b) * b;
}

f32 calcSpeedMax(f32 accel, f32 friction) {
    return (accel * friction) / (1.0f - friction);
}

f32 calcAccel(f32 speed, f32 friction) {
    return (1.0f - friction) * speed / friction;
}

f32 calcFriction(f32 accel, f32 speed) {
    // BUG: N's mistake here. Correct function: friction = speed / (speed + accel)
    return (accel + speed) / speed;
}

inline f32 round(f32 v) {
    return (s32)(v + (v >= 0.0f ? 0.5f : -0.5f));
}

void roundOffVec(sead::Vector3f* outVec, const sead::Vector3f& vec) {
    outVec->x = round(vec.x);
    outVec->y = round(vec.y);
    outVec->z = round(vec.z);
}

void roundOffVec(sead::Vector3f* vec) {
    roundOffVec(vec, *vec);
}

void roundOffVec(sead::Vector2f* outVec, const sead::Vector2f& vec) {
    outVec->x = round(vec.x);
    outVec->y = round(vec.y);
}

void roundOffVec(sead::Vector2f* vec) {
    roundOffVec(vec, *vec);
}

f32 snapToGrid(f32 val, f32 gridSize, f32 offset) {
    return round((val - offset) / gridSize) * gridSize + offset;
}

void snapVecToGrid(sead::Vector3f* outVec, const sead::Vector3f& vec, f32 gridSize,
                   const sead::Vector3f& offset) {
    outVec->x = snapToGrid(vec.x, gridSize, offset.x);
    outVec->y = snapToGrid(vec.y, gridSize, offset.y);
    outVec->z = snapToGrid(vec.z, gridSize, offset.z);
}

void snapVecToGrid(sead::Vector3f* outVec, const sead::Vector3f& vec,
                   const sead::Vector3f& gridSize, const sead::Vector3f& offset) {
    outVec->x = snapToGrid(vec.x, gridSize.x, offset.x);
    outVec->y = snapToGrid(vec.y, gridSize.y, offset.y);
    outVec->z = snapToGrid(vec.z, gridSize.z, offset.z);
}

void limitVectorOppositeDir(sead::Vector3f* outVec, const sead::Vector3f& inVec,
                            const sead::Vector3f& dir, f32 scale) {
    scale = sead::Mathf::clamp(dir.dot(inVec), -scale, 0.0f);
    outVec->setScaleAdd(-scale, inVec, dir);
}

void scaleVectorDirection(sead::Vector3f* outVec, const sead::Vector3f& inVec,
                          const sead::Vector3f& dir, f32 scale) {
    sead::Vector3f direction = inVec * inVec.dot(dir);
    outVec->setAdd(dir - direction, direction * scale);
}

void scaleVectorExceptDirection(sead::Vector3f* outVec, const sead::Vector3f& inVec,
                                const sead::Vector3f& dir, f32 scale) {
    sead::Vector3f direction = inVec * inVec.dot(dir);
    outVec->setScaleAdd(scale, dir - direction, direction);
}

bool calcDir(sead::Vector3f* outVec, const sead::Vector3f& vecA, const sead::Vector3f& vecB) {
    outVec->setSub(vecB, vecA);
    return !tryNormalizeOrZero(outVec);
}

bool calcDirH(sead::Vector3f* outVec, const sead::Vector3f& vecA, const sead::Vector3f& vecB) {
    return calcDirOnPlane(outVec, vecA, vecB, sead::Vector3f::ey);
}

bool calcDirOnPlane(sead::Vector3f* outVec, const sead::Vector3f& vecA, const sead::Vector3f& vecB,
                    const sead::Vector3f& plane) {
    outVec->setSub(vecB, vecA);
    outVec->setScaleAdd(-plane.dot(*outVec), plane, *outVec);
    return !tryNormalizeOrZero(outVec);
}

f32 mapRangeLogarithmic(f32 x, f32 min, f32 max, f32 start, f32 end, f32 exponent) {
    f32 base = sead::Mathf::pow(10.0f, -exponent);
    f32 val = (x - min) / (max - min);
    f32 range = sead::Mathf::abs(end - start);
    f32 direction = end > start ? 1.0f : -1.0f;

    return sead::Mathf::pow(val, base) * direction * range + start;
}

void calcDirFromLongitudeLatitude(sead::Vector3f* outVec, f32 longitude, f32 latitude) {
    outVec->y = -sead::Mathf::sin(sead::Mathf::deg2rad(latitude));
    f32 cosLatitude = -sead::Mathf::cos(sead::Mathf::deg2rad(latitude));
    outVec->x = sead::Mathf::sin(sead::Mathf::deg2rad(longitude)) * cosLatitude;
    outVec->z = sead::Mathf::cos(sead::Mathf::deg2rad(longitude)) * cosLatitude;
}

void calcLongitudeLatitudeFromDir(f32* longitude, f32* latitude, const sead::Vector3f& dir) {
    sead::Vector3f dirNormalized = dir;
    dirNormalized.normalize();
    if (isNearZero(dirNormalized))
        return;
    *latitude = sead::Mathf::asin(sead::Mathf::clamp(-dirNormalized.y, -1.0f, 1.0f));

    sead::Vector2f newVec = {-dirNormalized.z, -dirNormalized.x};
    newVec.normalize();
    if (isNearZero(newVec))
        return;

    *longitude = sead::Mathf::atan2(newVec.y, newVec.x);
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

// https://decomp.me/scratch/MqKUQ
//  NON_MATCHING: Regswap on Mult and Add
void rotateQuatRadian(sead::Quatf* outQuat, const sead::Quatf& quat, const sead::Vector3f& vec,
                      f32 angle) {
    f32 cos = sead::Mathf::cos(angle * 0.5f);
    f32 sin = sead::Mathf::sin(angle * 0.5f);

    sead::Quatf rotation;
    rotation.w = cos;
    rotation.x = sin * vec.x;
    rotation.y = sin * vec.y;
    rotation.z = sin * vec.z;

    *outQuat = rotation * quat;
    outQuat->normalize();
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

// https://decomp.me/scratch/utMuy
//  NON_MATCHING: Regswap on Add
void rotateQuatXDirDegree(sead::Quatf* outQuat, const sead::Quatf& quat, f32 angle) {
    sead::Quatf rotation;
    makeQuatXDegree(&rotation, angle);
    *outQuat = quat * rotation;
    outQuat->normalize();
}

// https://decomp.me/scratch/DEZoH
//  NON_MATCHING: Regswap on Add
void rotateQuatYDirDegree(sead::Quatf* outQuat, const sead::Quatf& quat, f32 angle) {
    sead::Quatf rotation;
    makeQuatYDegree(&rotation, angle);
    *outQuat = quat * rotation;
    outQuat->normalize();
}

// https://decomp.me/scratch/iJBbn
//  NON_MATCHING: Regswap on Add
void rotateQuatZDirDegree(sead::Quatf* outQuat, const sead::Quatf& quat, f32 angle) {
    sead::Quatf rotation;
    makeQuatZDegree(&rotation, angle);
    *outQuat = quat * rotation;
    outQuat->normalize();
}

void rotateQuatLocalDirDegree(sead::Quatf* outQuat, const sead::Quatf& quat, s32 axis, f32 angle) {
    sead::Vector3f vec;
    switch (axis) {
    case 0:
        vec.setRotated(quat, sead::Vector3f::ex);
        break;
    case 1:
        vec.setRotated(quat, sead::Vector3f::ey);
        break;
    case 2:
        vec.setRotated(quat, sead::Vector3f::ez);
        break;
    default:
        return;
    }

    rotateQuatRadian(outQuat, quat, vec, sead::Mathf::deg2rad(angle));
}

// https://decomp.me/scratch/nV2zl
// NON_MATCHING: Multiple issues
void rotateQuatMoment(sead::Quatf* outQuat, const sead::Quatf& quat, const sead::Vector3f& vec) {
    sead::Vector3f vecNorm;
    tryNormalizeOrZero(&vecNorm, vec);

    f32 angle = vec.length() * 0.5f;
    f32 cos = sead::Mathf::cos(angle);
    f32 sin = sead::Mathf::sin(angle);

    sead::Quatf rotation;
    rotation.w = cos;
    rotation.x = sin * vecNorm.x;
    rotation.y = sin * vecNorm.y;
    rotation.z = sin * vecNorm.z;
    *outQuat = rotation * quat;
    outQuat->normalize();
}

// https://decomp.me/scratch/l1K35
// NON_MATCHING: Multiple issues
void rotateQuatMomentDegree(sead::Quatf* outQuat, const sead::Quatf& quat,
                            const sead::Vector3f& vec) {
    sead::Vector3f vecNorm;
    tryNormalizeOrZero(&vecNorm, vec);

    f32 angle = sead::Mathf::deg2rad(vec.length()) * 0.5f;
    f32 cos = sead::Mathf::cos(angle);
    f32 sin = sead::Mathf::sin(angle);

    sead::Quatf rotation;
    rotation.w = cos;
    rotation.x = sin * vecNorm.x;
    rotation.y = sin * vecNorm.y;
    rotation.z = sin * vecNorm.z;
    *outQuat = rotation * quat;
    outQuat->normalize();
}

void rotateQuatRollBall(sead::Quatf* outQuat, const sead::Quatf& quat, const sead::Vector3f& vecA,
                        const sead::Vector3f& vecB, f32 scale) {
    sead::Vector3f vecNorm;
    calcMomentRollBall(&vecNorm, vecA, vecB, scale);
    rotateQuatMoment(outQuat, quat, vecNorm);
}

void calcMomentRollBall(sead::Vector3f* outVec, const sead::Vector3f& vecA,
                        const sead::Vector3f& vecB, f32 scale) {
    sead::Vector3f vecNorm = vecB;
    if (!tryNormalizeOrZero(&vecNorm)) {
        *outVec = vecNorm;
        return;
    }

    vecNorm.setCross(vecNorm, vecA);
    scale = 1.0f / scale;
    *outVec = scale * vecNorm;
}

void calcCirclePointPicking(sead::Vector2f* outPoint, f32 x, f32 y) {
    f32 invLength = 1.0 / (x * x + y * y);
    outPoint->x = (x * x - y * y) * invLength;
    outPoint->y = 2 * x * y * invLength;
}

void pickUniformPointsOnCircleHammersley(sead::Vector2f* outPoint, u32 x, u32 y) {
    sead::Vector2f hammersleyPoint;
    calcHammersleyPoint(&hammersleyPoint, x, y);
    calcCirclePointPicking(outPoint, hammersleyPoint.x, hammersleyPoint.y);
}

void calcDiskPointPicking(sead::Vector2f* outPoint, f32 radius, f32 angle) {
    radius = sead::Mathf::sqrt(radius);
    f32 cos = sead::Mathf::cos(angle * sead::Mathf::pi2());
    f32 sin = sead::Mathf::sin(angle * sead::Mathf::pi2());
    outPoint->x = radius * cos;
    outPoint->y = radius * sin;
}

void pickUniformPointsOnDiskHammersley(sead::Vector2f* outPoint, u32 x, u32 y) {
    sead::Vector2f hammersleyPoint;
    calcHammersleyPoint(&hammersleyPoint, x, y);
    calcDiskPointPicking(outPoint, hammersleyPoint.x, hammersleyPoint.y);
}

void pickUniformPointOnDisk(sead::Vector2f* outPoint) {
    calcDiskPointPicking(outPoint, getRandom(), getRandom());
}

void calcSpherePointPicking(sead::Vector3f* outPoint, f32 x, f32 y) {
    f32 xx = 2 * x - 1.0f;
    f32 angle = y * sead::Mathf::pi() * 2;
    f32 radius = sead::Mathf::sqrt(1.0f - xx * xx);
    f32 cos = radius * sead::Mathf::cos(angle);
    f32 sin = radius * sead::Mathf::sin(angle);
    outPoint->x = cos;
    outPoint->y = sin;
    outPoint->z = xx;
}

void pickUniformPointOnSphere(sead::Vector3f* outPoint) {
    calcSpherePointPicking(outPoint, getRandom(), getRandom());
}

void calcParabolicFunctionParam(f32* gravity, f32* initialVelY, f32 maxHeight,
                                f32 verticalDistance) {
    f32 maxHeightSign = sign(maxHeight);

    f32 maxHeightAdjusted =
        sead::Mathf::sqrt(sead::Mathf::clampMin((maxHeight - verticalDistance) * maxHeight, 0.0));
    *initialVelY = 2 * ((maxHeightSign * maxHeightAdjusted) + maxHeight);
    *gravity = verticalDistance - *initialVelY;
}

}  // namespace al

namespace Intersect {

bool calcX(sead::Vector3f* outVec, f32 value, const sead::Vector3f& vectorA,
           const sead::Vector3f& vectorB, const sead::Vector3f& min, const sead::Vector3f& max) {
    f32 x = (value - vectorA.x) / vectorB.x;
    if ((x < 0.0f || x != 1.0f) && (x < 0.0f || 1.0f <= x))
        return false;

    f32 y = vectorA.y + x * vectorB.y;
    if (!(min.y <= y && y <= max.y))
        return false;

    f32 z = vectorA.z + x * vectorB.z;
    if (!(min.z <= z && z <= max.z))
        return false;

    x = vectorA.x + vectorB.x * x;
    if (outVec) {
        outVec->x = x;
        outVec->y = y;
        outVec->z = z;
    }
    return true;
}

bool calcY(sead::Vector3f* outVec, f32 value, const sead::Vector3f& vectorA,
           const sead::Vector3f& vectorB, const sead::Vector3f& min, const sead::Vector3f& max) {
    f32 y = (value - vectorA.y) / vectorB.y;
    if ((y < 0.0f || y != 1.0f) && (y < 0.0f || 1.0f <= y))
        return false;

    f32 x = vectorA.x + y * vectorB.x;
    if (!(min.x <= x && x <= max.x))
        return false;

    f32 z = vectorA.z + y * vectorB.z;
    if (!(min.z <= z && z <= max.z))
        return false;
    y = vectorA.y + vectorB.y * y;
    if (outVec) {
        outVec->x = x;
        outVec->y = y;
        outVec->z = z;
    }
    return true;
}

bool calcZ(sead::Vector3f* outVec, f32 value, const sead::Vector3f& vectorA,
           const sead::Vector3f& vectorB, const sead::Vector3f& min, const sead::Vector3f& max) {
    f32 z = (value - vectorA.z) / vectorB.z;
    if ((z < 0.0f || z != 1.0f) && (z < 0.0f || 1.0f <= z))
        return false;

    f32 x = vectorA.x + z * vectorB.x;
    if (!(min.x <= x && x <= max.x))
        return false;
    f32 y = vectorA.y + z * vectorB.y;
    if (!(min.y <= y && y <= max.y))
        return false;

    z = vectorA.z + vectorB.z * z;
    if (outVec) {
        outVec->x = x;
        outVec->y = y;
        outVec->z = z;
    }
    return true;
}

}  // namespace Intersect
