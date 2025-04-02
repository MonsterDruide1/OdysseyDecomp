#include "Player/PlayerActionFunction.h"

#include "Library/Math/MathUtil.h"

namespace PlayerActionFunction {

f32 accel(f32 speed, f32 speedMax, f32 accel) {
    if (speed > speedMax)
        return speed;
    return sead::Mathf::clampMax(speed + accel, speedMax);
}

f32 brake(f32 speed, u32 frames, f32 speedMax) {
    if (speed == 0.0f)
        return speed;
    f32 decel = speedMax / frames;
    if (speed < 0.0f) {
        speed += decel;
        if (speed > 0.0f)
            return 0.0f;
    } else {
        speed -= decel;
        if (speed < 0.0f)
            return 0.0f;
    }
    return speed;
}

f32 brakeLimit(f32 speed, u32 frames, f32 speedMax, f32 speedMin) {
    return sead::Mathf::clampMin(speed - (speedMax - speedMin) / frames, speedMin);
}

f32 calcJumpSpeed(f32 speedFront, f32 speedMin, f32 speedMax, f32 jumpPowerMin, f32 jumpPowerMax) {
    if (al::isNearZero(jumpPowerMax - jumpPowerMin))
        return jumpPowerMax;

    f32 clampedSpeedFront = sead::Mathf::clamp(speedFront, speedMin, speedMax);
    f32 jumpFactor =
        sead::Mathf::clamp((clampedSpeedFront - speedMin) / (speedMax - speedMin), 0.0f, 1.0f);
    return ((jumpPowerMax - jumpPowerMin) * jumpFactor + jumpPowerMin);
}

bool isOppositeDir(const sead::Vector3f& a, const sead::Vector3f& b) {
    return b.dot(a) <= -0.087156f;  // sin(5°)
}

bool isOppositeVec(const sead::Vector3f& a, const sead::Vector3f& b) {
    sead::Vector3f normalizedA = {0.0f, 0.0f, 0.0f};
    if (!al::tryNormalizeOrZero(&normalizedA, a))
        return false;
    sead::Vector3f normalizedB = {0.0f, 0.0f, 0.0f};
    if (!al::tryNormalizeOrZero(&normalizedB, b))
        return false;
    return isOppositeDir(normalizedA, normalizedB);
}

f32 calcStickPow(f32 pow) {
    return sead::Mathf::sign(pow) *
           sead::Mathf::clamp((sead::Mathf::sign(pow) * pow - 0.1f) / 0.9f, 0.0f, 1.0f);
}

}  // namespace PlayerActionFunction
