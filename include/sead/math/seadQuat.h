#pragma once

#include <basis/seadTypes.h>
#include <math/seadMathPolicies.h>

namespace sead
{
template <typename T>
struct Quat : public Policies<T>::QuatBase
{
    Quat() {}
    Quat(T x, T y, T z, T w)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    static const Quat unit;
};

typedef Quat<f32> Quatf;

}  // namespace sead
