#pragma once

#include <math/seadVector.h>

namespace PlayerActionFunction {

bool isOppositeDir(const sead::Vector3f&, const sead::Vector3f&);

f32 accel(f32, f32, f32);
f32 brake(f32, u32, f32);

}
