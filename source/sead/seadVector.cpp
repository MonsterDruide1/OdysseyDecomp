#include "sead/seadVector.h"

namespace sead
{
    template <>
    const Vector3<float> Vector3<float>::zero(0.0f, 0.0f, 0.0f);

    template <>
    const Vector3<float> Vector3<float>::ex(1.0f, 0.0f, 0.0f);

    template <>
    const Vector3<float> Vector3<float>::ey(0.0f, 1.0f, 0.0f);

    template <>
    const Vector3<float> Vector3<float>::ez(0.0f, 0.0f, 1.0f);

    template <>
    const Vector3<float> Vector3<float>::ones(1.0f, 1.0f, 1.0f);
};