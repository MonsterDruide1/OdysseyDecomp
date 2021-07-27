#pragma once

#include <basis/seadRawPrint.h>
#include <cmath>
#ifndef SEAD_MATH_MATH_CALC_COMMON_H_
#include <math/seadMathCalcCommon.h>
#endif
#include <math/seadMathNumbers.h>

namespace sead
{
template <typename T>
inline T MathCalcCommon<T>::roundUp(T x, s32 multNumber)
{
    SEAD_ASSERT(multNumber > 0);
    return (x + multNumber - 1) / multNumber * multNumber;
}

template <typename T>
inline s32 MathCalcCommon<T>::roundUpPow2(T val, s32 base)
{
    SEAD_ASSERT_MSG((u32(base - 1) & u32(base)) == 0, "illegal param[val:%d, base:%d]", val, base);
    return val + base - 1 & (u32)-base;
}

template <typename T>
inline s32 MathCalcCommon<T>::roundUpPow2Positive(T val, s32 base)
{
    SEAD_ASSERT_MSG(val >= 0 && (u32(base - 1) & u32(base)) == 0, "illegal param[val:%d, base:%d]",
                    val, base);
    return val + base - 1 & (u32)-base;
}

template <typename T>
inline T MathCalcCommon<T>::acos(T t)
{
    if constexpr (std::is_floating_point<T>())
        SEAD_ASSERT_MSG(-1.0 <= t && t <= 1.0, "t(%f) is not in [-1, 1].", t);
    return std::acos(t);
}

template <typename T>
inline T MathCalcCommon<T>::asin(T t)
{
    if constexpr (std::is_floating_point<T>())
        SEAD_ASSERT_MSG(-1.0 <= t && t <= 1.0, "t(%f) is not in [-1, 1].", t);
    return std::asin(t);
}

template <typename T>
inline T MathCalcCommon<T>::atan(T t)
{
    return std::atan(t);
}

template <typename T>
inline T MathCalcCommon<T>::deg2rad(T deg)
{
    return deg * (numbers::pi_v<T> / static_cast<T>(180));
}

template <typename T>
inline T MathCalcCommon<T>::rad2deg(T rad)
{
    return rad * (static_cast<T>(180) / numbers::pi_v<T>);
}

template <typename T>
inline T MathCalcCommon<T>::max(T a, T b)
{
    return a < b ? b : a;
}

template <typename T>
inline T MathCalcCommon<T>::min(T a, T b)
{
    return a < b ? a : b;
}

template <typename T>
inline s32 MathCalcCommon<T>::sign(T value)
{
    return value < 0 ? -1 : 1;
}

template <typename T>
inline bool MathCalcCommon<T>::chase(T* value, T target, T step)
{
    const T current = *value;

    if (current < target)
    {
        const T new_value = current + step;
        if (target <= new_value || new_value < current)
        {
            *value = target;
            return true;
        }
        *value = new_value;
        return false;
    }

    if (current > target)
    {
        const T new_value = current - step;
        if (target >= new_value || current < new_value)
        {
            *value = target;
            return true;
        }
        *value = new_value;
        return false;
    }

    return true;
}

template <typename T, typename T2>
inline T lerp(T a, T b, T2 t)
{
    return a + (b - a) * t;
}

template <typename T>
inline T clamp(T value, T low, T high)
{
    if (value < low)
        value = low;
    else if (value > high)
        value = high;
    return value;
}
}  // namespace sead
