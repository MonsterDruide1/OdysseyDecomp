#pragma once

#include "math/seadQuat.h"

namespace sead
{
template <typename T>
struct QuatCalcCommon
{
    static void slerpTo(BaseQuat<T>& out, const BaseQuat<T>& q1, const BaseQuat<T>& q2, f32 t);
};
}  // namespace sead

#define SEAD_MATH_QUAT_CALC_COMMON_H_
#include "math/seadQuatCalcCommon.hpp"
#undef SEAD_MATH_QUAT_CALC_COMMON_H_
