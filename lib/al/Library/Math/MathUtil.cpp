#include "Library/Math/MathUtil.h"

#include <math/seadMathCalcCommon.h>
#include <random/seadGlobalRandom.h>
#include "prim/seadBitUtil.h"

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
                           const sead::Vector3f& c) {
    return sead::Mathf::rad2deg(calcAngleOnPlaneRadian(a, b, c));
}

f32 calcAngleOnPlaneDegreeOrZero(const sead::Vector3f& a, const sead::Vector3f& b,
                                 const sead::Vector3f& c) {
    f32 angle = 0.0f;
    if (!tryCalcAngleOnPlaneDegree(&angle, a, b, c))
        return 0.0f;

    return angle;
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

void setLength(sead::Vector3f* vec, f32 lenSet) {
    f32 len = vec->length();
    if (len > 0.0f) {
        f32 scale = lenSet / len;
        *vec *= scale;
    }
}

void setProjectionLength(sead::Vector3f* out, const sead::Vector3f& vec, f32 len) {
    f32 scale = len / sead::Mathf::abs(vec.dot(*out));
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

f32 cubeRoot(f32 x) {
    f32 onethird = 1.0f / 3.0f;

    u32 i = 0x54a0fc86 - sead::BitUtil::bitCast<u32>(x) / 3;
    f32 y = sead::BitUtil::bitCast<f32>(i);
    y = y * onethird * (4.0f - x * y * y * y);
    y = y * onethird * (4.0f - x * y * y * y);
    y = y * onethird * (4.0f - x * y * y * y);
    return x * y * y;
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

f32 lerpValue(f32 x, f32 y, f32 time) {
    if (time < 0.0f)
        time = 0.0f;
    else if (time > 1.0f)
        time = 1.0f;
    return (x * (1.0f - time)) + (time * y);
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

}  // namespace al
