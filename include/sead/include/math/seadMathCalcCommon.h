#pragma once

#include <basis/seadTypes.h>
#include <cmath>
#include <limits>
#include <type_traits>

namespace sead
{
template <typename T>
class MathCalcCommon
{
public:
    static T roundUp(T x, s32 multNumber);

    static s32 roundUpPow2(T x, s32 y);
    static s32 roundUpPow2Positive(T x, s32 y);

    static s32 roundDownPow2(T x, s32 y);
    static T gcd(T x, T y);
    static T lcm(T x, T y);

    static T max(T a, T b);
    static T min(T a, T b);

    /// Returns -1 for strictly negative values and 1 otherwise.
    static s32 sign(T value);

    static T fitSign(T value, T sign_value) { return value * sign(sign_value); }

    static T epsilon() { return std::numeric_limits<T>::epsilon(); }

    static bool equalsEpsilon(T lhs, T rhs, T eps = epsilon())
    {
        return std::abs(lhs - rhs) <= eps;
    }

    static T acos(T t);
    static T asin(T t);
    static T atan(T t);

    static T deg2rad(T deg);
    static T rad2deg(T rad);

    /// Adds or subtracts `step` from `value` towards `target`.
    /// Returns whether the new value is equal to the target.
    static bool chase(T* value, T target, T step);
};

typedef MathCalcCommon<s32> Mathi;
typedef MathCalcCommon<u32> Mathu;
typedef MathCalcCommon<f32> Mathf;
typedef MathCalcCommon<size_t> MathSizeT;

template <typename T, typename T2 = float>
T lerp(T a, T b, T2 t);

template <typename T>
T clamp(T value, T low, T high);

/// Branchless version of abs for signed 32-bit integers
constexpr s32 abs(s32 x)
{
    return (x ^ (x >> 31)) - (x >> 31);
}

constexpr f32 absf(f32 x)
{
    return x > 0 ? x : -x;
}

template <typename T>
constexpr T log2(T n)
{
    static_assert(std::is_integral<T>(), "T must be an integral type");
    return n <= 1 ? 0 : 1 + log2(n >> 1);
}
}  // namespace sead

#define SEAD_MATH_MATH_CALC_COMMON_H_
#include <math/seadMathCalcCommon.hpp>
#undef SEAD_MATH_MATH_CALC_COMMON_H_
