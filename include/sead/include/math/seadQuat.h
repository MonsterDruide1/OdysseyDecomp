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

    constexpr Quat& operator=(const Quat& other)
    {
        this->x = other.x;
        this->y = other.y;
        this->z = other.z;
        this->w = other.w;
        return *this;
    }

    static const Quat unit;
};

typedef Quat<f32> Quatf;

template <>
const Quatf Quatf::unit;

}  // namespace sead
