#pragma once

#include <math/seadVector.h>

class PlayerActionFunction {
public:
    static f32 accel(f32 val, f32 max, f32 step);
    static f32 brake(f32 val, u32 frames, f32 max);
    static f32 brakeLimit(f32 val, u32 frames, f32 max, f32 min);
    static f32 calcJumpSpeed(f32 jumpPowerMax, f32 velocityFront, f32 speedMin, f32 speedMax,
                             f32 jumpPowerMin);
    static bool isOppositeDir(const sead::Vector3f& a, const sead::Vector3f& b);
    static bool isOppositeVec(const sead::Vector3f& a, const sead::Vector3f& b);
    static f32 calcStickPow(f32);
};
