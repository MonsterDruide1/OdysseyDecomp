#pragma once

#include "math/seadVector.h"

namespace PlayerActionFunction {

float accel(float, float, float);
float brake(float, unsigned int, float);
float brakeLimit(float, unsigned int, float, float);
float calcJumpSpeed(float, float, float, float, float);
bool isOppositeDir(const sead::Vector3f&, const sead::Vector3f&);
bool isOppositeVec(const sead::Vector3f&, const sead::Vector3f&);
float calcStickPow(float);

}
