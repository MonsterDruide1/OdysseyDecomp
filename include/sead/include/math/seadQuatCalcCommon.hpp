#pragma once

#ifndef SEAD_MATH_QUAT_CALC_COMMON_H_
#include "math/seadQuatCalcCommon.h"
#endif

#include <limits>
#include "math/seadMathCalcCommon.h"

namespace sead
{
template <typename T>
inline void QuatCalcCommon<T>::slerpTo(BaseQuat<T>& out, const BaseQuat<T>& q1,
                                       const BaseQuat<T>& q2, f32 t)
{
    T dot = (q1.x * q2.x) + (q1.y * q2.y) + (q1.z * q2.z) + (q1.w * q2.w);
    if (dot > 1)
        dot = 1;
    else if (dot < -1)
        dot = -1;

    const T dot_0 = dot;
    if (dot < 0)
        dot = -dot;

    T a, b;
    const T theta_0 = MathCalcCommon<T>::acos(dot);
    const T sin_theta_0 = std::sin(theta_0);
    if (sead::absf(sin_theta_0) < std::numeric_limits<T>::epsilon())
    {
        a = 1.0f - t;
        b = t;
    }
    else
    {
        const T theta = theta_0 * t;
        const T sin_theta_0_inv = 1.0f / sin_theta_0;
        a = sin_theta_0_inv * std::sin(theta_0 - theta);
        b = sin_theta_0_inv * std::sin(theta);
    }

    if (dot_0 < 0)
        b = -b;

    out.x = a * q1.x + b * q2.x;
    out.y = a * q1.y + b * q2.y;
    out.z = a * q1.z + b * q2.z;
    out.w = a * q1.w + b * q2.w;
}
}  // namespace sead
