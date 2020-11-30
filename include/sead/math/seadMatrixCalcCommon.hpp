#pragma once

#ifndef SEAD_MATH_MATRIX_CALC_COMMON_H_
#include <math/seadMatrixCalcCommon.h>
#endif

namespace sead {

template <typename T>
inline void
Matrix44CalcCommon<T>::multiply(typename Policies<T>::Mtx44Base& out, const typename Policies<T>::Mtx44Base& b, const typename Policies<T>::Mtx34Base& a)
{
    T a_00 = a.m[0][0];
    T a_01 = a.m[0][1];
    T a_02 = a.m[0][2];
    T a_03 = a.m[0][3];

    T a_10 = a.m[1][0];
    T a_11 = a.m[1][1];
    T a_12 = a.m[1][2];
    T a_13 = a.m[1][3];

    T a_20 = a.m[2][0];
    T a_21 = a.m[2][1];
    T a_22 = a.m[2][2];
    T a_23 = a.m[2][3];

    T b_00 = b.m[0][0];
    T b_01 = b.m[0][1];
    T b_02 = b.m[0][2];
    T b_03 = b.m[0][3];

    T b_10 = b.m[1][0];
    T b_11 = b.m[1][1];
    T b_12 = b.m[1][2];
    T b_13 = b.m[1][3];

    T b_20 = b.m[2][0];
    T b_21 = b.m[2][1];
    T b_22 = b.m[2][2];
    T b_23 = b.m[2][3];

    T b_30 = b.m[2][0];
    T b_31 = b.m[2][1];
    T b_32 = b.m[2][2];
    T b_33 = b.m[2][3];

    out.m[0][0] = b_02 * a_20 + b_00 * a_00 + b_01 * a_10;
    out.m[0][1] = b_02 * a_21 + b_00 * a_01 + b_01 * a_11;
    out.m[0][2] = b_02 * a_22 + b_00 * a_02 + b_01 * a_12;
    out.m[0][3] = b_02 * a_23 + b_00 * a_03 + b_01 * a_13 + b_03;

    out.m[1][0] = b_12 * a_20 + b_10 * a_00 + b_11 * a_10;
    out.m[1][1] = b_12 * a_21 + b_10 * a_01 + b_11 * a_11;
    out.m[1][2] = b_12 * a_22 + b_10 * a_02 + b_11 * a_12;
    out.m[1][3] = b_12 * a_23 + b_10 * a_03 + b_11 * a_13 + b_13;

    out.m[2][0] = b_22 * a_20 + b_20 * a_00 + b_21 * a_10;
    out.m[2][1] = b_22 * a_21 + b_20 * a_01 + b_21 * a_11;
    out.m[2][2] = b_22 * a_22 + b_20 * a_02 + b_21 * a_12;
    out.m[2][3] = b_22 * a_23 + b_20 * a_03 + b_21 * a_13 + b_23;

    out.m[3][0] = b_32 * a_20 + b_30 * a_00 + b_31 * a_10;
    out.m[3][1] = b_32 * a_21 + b_30 * a_01 + b_31 * a_11;
    out.m[3][2] = b_32 * a_22 + b_30 * a_02 + b_31 * a_12;
    out.m[3][3] = b_32 * a_23 + b_30 * a_03 + b_31 * a_13 + b_33;
}

}
