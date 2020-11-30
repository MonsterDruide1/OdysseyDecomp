#pragma once

#include <math/seadVector.h>

namespace sead
{
template <typename T>
struct BoundBox2
{
    Vector2<T> mMin;
    Vector2<T> mMax;

    static const BoundBox2<T> cUndefined;
};

template <typename T>
struct BoundBox3
{
    Vector3<T> mMin;
    Vector3<T> mMax;

    static const BoundBox3<T> cUndefined;
};

typedef BoundBox2<f32> BoundBox2f;
typedef BoundBox3<f32> BoundBox3f;

}  // namespace sead
