#pragma once

#include <math/seadMathPolicies.h>

namespace sead
{
template <typename T>
class Matrix44CalcCommon
{
public:
    static void multiply(typename Policies<T>::Mtx44Base& out,
                         const typename Policies<T>::Mtx44Base& b,
                         const typename Policies<T>::Mtx34Base& a);
};

}  // namespace sead

#define SEAD_MATH_MATRIX_CALC_COMMON_H_
#include <math/seadMatrixCalcCommon.hpp>
#undef SEAD_MATH_MATRIX_CALC_COMMON_H_
