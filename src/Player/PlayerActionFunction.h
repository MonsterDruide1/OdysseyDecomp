#pragma once

#include <math/seadVector.h>

namespace PlayerActionFunction {

f32 accel(f32 speed, f32 speedMax, f32 accel);
f32 brake(f32 speed, u32 frames, f32 speedMax);
f32 brakeLimit(f32 speed, u32 frames, f32 speedMax, f32 speedMin);
f32 calcJumpSpeed(f32 speedFront, f32 speedMin, f32 speedMax, f32 jumpPowerMin, f32 jumpPowerMax);
bool isOppositeDir(const sead::Vector3f& a, const sead::Vector3f& b);
bool isOppositeVec(const sead::Vector3f& a, const sead::Vector3f& b);
f32 calcStickPow(f32 pow);

}  // namespace PlayerActionFunction
