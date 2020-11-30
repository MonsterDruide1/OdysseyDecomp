#pragma once

#include <basis/seadTypes.h>

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

    static T acos(T t);
    static T asin(T t);
    static T atan(T t);
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
}  // namespace sead

#define SEAD_MATH_MATH_CALC_COMMON_H_
#include <math/seadMathCalcCommon.hpp>
#undef SEAD_MATH_MATH_CALC_COMMON_H_
