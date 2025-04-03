#include "Library/Math/MathUtil.h"

#include <math/seadMathCalcCommon.h>
#include <prim/seadBitUtil.h>
#include <random/seadGlobalRandom.h>

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
    if (isNearZero(a, 0.001) || isNearZero(b, 0.001))
        return 0.0f;

    return sead::Mathf::rad2deg(sead::Mathf::atan2(a.cross(b), a.dot(b)));
}

bool isNearZero(const sead::Vector2f& vec, f32 tolerance) {
    return vec.squaredLength() < tolerance * tolerance;
}

bool tryCalcAngleDegree(f32* out, const sead::Vector3f& a, const sead::Vector3f& b) {
    if (isNearZero(a, 0.001) || isNearZero(b, 0.001))
        return false;

    *out = calcAngleDegree(a, b);
    return true;
}

bool isNearZero(const sead::Vector3f& vec, f32 tolerance) {
    return vec.squaredLength() < tolerance * tolerance;
}

f32 calcAngleOnPlaneRadian(const sead::Vector3f& a, const sead::Vector3f& b,
                           const sead::Vector3f& vertical) {
    sead::Vector3f v1;
    verticalizeVec(&v1, vertical, a);
    sead::Vector3f v2;
    verticalizeVec(&v2, vertical, b);

    f32 dot = v1.dot(v2);
    sead::Vector3f cross;
    cross.setCross(v1, v2);
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

s32 calcAngleSignOnPlane(const sead::Vector3f& a, const sead::Vector3f& b,
                         const sead::Vector3f& vertical) {
    sead::Vector3f v1;
    verticalizeVec(&v1, vertical, a);
    sead::Vector3f v2;
    verticalizeVec(&v2, vertical, b);

    sead::Vector3f cross;
    cross.setCross(v1, v2);
    const f32 angle = vertical.dot(cross);

    if (angle > 0.0f)
        return 1;
    if (angle < 0.0f)
        return -1;

    return 0;
}

void normalize(sead::Vector2f* out, const sead::Vector2f& vec) {
    *out = vec;
    normalize(out);
}

void normalize(sead::Vector3f* out, const sead::Vector3f& vec) {
    *out = vec;
    normalize(out);
}

bool tryNormalizeOrZero(sead::Vector3f* vec) {
    if (isNearZero(*vec, 0.001f)) {
        *vec = {0.0f, 0.0f, 0.0f};
        return false;
    } else {
        normalize(vec);
        return true;
    }
}

bool tryNormalizeOrZero(sead::Vector3f* out, const sead::Vector3f& vec) {
    *out = vec;
    return tryNormalizeOrZero(out);
}

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
    if (vec.squaredLength() < tolerance * tolerance)
        return true;
    value.getBase(vec, 1);
    if (vec.squaredLength() < tolerance * tolerance)
        return true;
    value.getBase(vec, 2);
    if (vec.squaredLength() < tolerance * tolerance)
        return true;

    return false;
}

bool isNearZeroOrGreater(f32 value, f32 tolerance) {
    return value >= 0.0f || sead::Mathf::abs(value) < tolerance;
}

bool isNearZeroOrLess(f32 value, f32 tolerance) {
    return value <= 0.0f || sead::Mathf::abs(value) < tolerance;
}

bool isExistNearZeroVal(const sead::Vector3f& vec, f32 tolerance) {
    return sead::Mathf::abs(vec.x) < tolerance || sead::Mathf::abs(vec.y) < tolerance ||
           sead::Mathf::abs(vec.z) < tolerance;
}

bool isNormalize(const sead::Vector3f& vec, f32 tolerance) {
    return sead::Mathf::abs(1.0f - vec.length()) <= tolerance;
}

bool isParallelDirection(const sead::Vector2f& a, const sead::Vector2f& b, f32 tolerance) {
    // looks awkward but mismatches without this
    return !(tolerance < sead::Mathf::abs(a.cross(b)));
}

bool isNearDirection(const sead::Vector2f& a, const sead::Vector2f& b, f32 tolerance) {
    if (a.dot(b) < 0.0f)
        return false;

    return !(sead::Mathf::abs(a.cross(b)) > tolerance);
}

bool isInRange(s32 x, s32 a, s32 b) {
    return (b < a) ? (a >= x && x >= b) : (b >= x && x >= a);
}

// NON_MATCHING
bool isInRange(f32 x, f32 a, f32 b) {
    return (b < a) ? (a >= x && x >= b) : (b >= x && x >= a);
}

void normalize(sead::Vector2f* vec) {
    f32 len = vec->length();
    if (len > 0.0f) {
        f32 invLen = 1.0f / len;
        *vec *= invLen;
    }
}

void normalize(sead::Vector3f* vec) {
    vec->normalize();
}

bool tryNormalizeOrZero(sead::Vector2f* vec) {
    if (isNearZero(*vec, 0.001f)) {
        *vec = {0.0f, 0.0f};
        return false;
    } else {
        normalize(vec);
        return true;
    }
}

bool tryNormalizeOrZero(sead::Vector2f* out, const sead::Vector2f& vec) {
    *out = vec;
    return tryNormalizeOrZero(out);
}

bool tryNormalizeOrDirZ(sead::Vector3f* vec) {
    if (isNearZero(*vec, 0.001f)) {
        // mismatches if this isn't set twice
        vec->set(0.0f, 0.0f, 0.0f);
        vec->set(sead::Vector3f::ez);
        return false;
    } else {
        normalize(vec);
        return true;
    }
}

bool tryNormalizeOrDirZ(sead::Vector3f* out, const sead::Vector3f& vec) {
    *out = vec;
    return tryNormalizeOrDirZ(out);
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
        out->x = invLen * vec.x;
        out->y = invLen * vec.y;
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
        out->x = invLen * vec.x;
        out->y = invLen * vec.y;
        out->z = invLen * vec.z;
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
    s32 v3;
    f32 result;

    if (x <= min)
        return 0.0f;

    if (max - min <= 0) {
        result = 1.0f;

        if (x < min)
            result = 0.0f;
    } else {
        if (x <= max)
            v3 = x;
        else
            v3 = max;

        if (x < min)
            v3 = min;

        result = (static_cast<f32>(v3 - min) / static_cast<f32>(max - min));
    }

    return result;
}

f32 sign(f32 x) {
    f32 v1;
    v1 = x < 0.0f;
    if (x > 0.0f)
        x = 1.0f;
    if (v1)
        x = -1.0f;

    return x;
}

s32 sign(s32 x) {
    if (x < 0)
        return -1;
    if (x > 0)
        return 1;

    return 0;
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

f32 lerpValue(f32 a, f32 b, f32 t) {
    if (t < 0.0f)
        t = 0.0f;
    else if (t > 1.0f)
        t = 1.0f;
    return (a * (1.0f - t)) + (t * b);
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

// NON_MATCHING: regswap; this function computes the same thing as the one above it, just in a
// different way (and the width scaling)
f32 hermite(f32 y0, f32 m0, f32 y1, f32 m1, f32 t, f32 width) {
    t *= 1.0f / width;
    f32 a1 = y0 - y1;
    f32 a2 = t + -1.0f;
    f32 a3 = t + t + -3.0f;
    f32 a4 = a2 * m0 + t * m1;
    return a1 * a3 * t * t + y0 + t * a2 * a4;
}

f32 calcFourthOrderRate(f32 t, f32 scale) {
    return ((scale + -3.0f) * t * t + (scale * -2.0f + 4.0f) * t + scale) * t * t;
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

void calcParabolicFunctionParam(f32* gravity, f32* initialVelY, f32 maxHeight,
                                f32 verticalDistance) {
    // can be 0, 1, or -1
    f32 signOfA3 = maxHeight < 0.0f ? -1.0f : maxHeight > 0.0f ? 1.0f : maxHeight;

    f32 maxHeightAdjusted =
        sead::Mathf::sqrt(sead::Mathf::clampMin((maxHeight - verticalDistance) * maxHeight, 0.0));
    *initialVelY = 2 * ((signOfA3 * maxHeightAdjusted) + maxHeight);
    *gravity = verticalDistance - *initialVelY;
}

}  // namespace al
