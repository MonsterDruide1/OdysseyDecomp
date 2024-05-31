#include "Player/PlayerActionFunction.h"

#include "Library/Math/MathAngleUtil.h"
#include "Library/Math/MathLengthUtil.h"

f32 PlayerActionFunction::accel(f32 val, f32 max, f32 step) {
    if (val > max)
        return val;
    return sead::Mathf::clampMax(val + step, max);
}

f32 PlayerActionFunction::brake(f32 val, u32 frames, f32 max) {
    if (val == 0.0f)
        return val;
    f32 step = max / frames;
    if (val < 0.0f) {
        val += step;
        if (val > 0.0f)
            return 0.0f;
    } else {
        val -= step;
        if (val < 0.0f)
            return 0.0f;
    }
    return val;
}

f32 PlayerActionFunction::brakeLimit(f32 val, u32 frames, f32 max, f32 min) {
    return sead::Mathf::clampMin(val - (max - min) / frames, min);
}

f32 PlayerActionFunction::calcJumpSpeed(f32 jumpPowerMax, f32 velocityFront, f32 speedMin,
                                        f32 speedMax, f32 jumpPowerMin) {
    if (al::isNearZero(jumpPowerMin - speedMax, 0.001f))
        return jumpPowerMin;

    f32 v12 = sead::Mathf::clamp(jumpPowerMax, velocityFront, speedMin);
    f32 v13 = sead::Mathf::clamp((v12 - velocityFront) / (speedMin - velocityFront), 0.0f, 1.0f);
    return ((jumpPowerMin - speedMax) * v13 + speedMax);
}

bool PlayerActionFunction::isOppositeDir(const sead::Vector3f& a, const sead::Vector3f& b) {
    return b.dot(a) <= -0.087156f;  // sin(5°)
}

bool PlayerActionFunction::isOppositeVec(const sead::Vector3f& a, const sead::Vector3f& b) {
    sead::Vector3f normalizedA = {0.0f, 0.0f, 0.0f};
    if (!al::tryNormalizeOrZero(&normalizedA, a))
        return false;
    sead::Vector3f normalizedB = {0.0f, 0.0f, 0.0f};
    if (!al::tryNormalizeOrZero(&normalizedB, b))
        return false;
    return isOppositeDir(normalizedA, normalizedB);
}

f32 PlayerActionFunction::calcStickPow(f32 pow) {
    return sead::Mathf::sign(pow) *
           sead::Mathf::clamp((sead::Mathf::sign(pow) * pow - 0.1f) / 0.9f, 0.0f, 1.0f);
}
