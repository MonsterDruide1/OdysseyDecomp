#include <math/seadMatrix.h>

namespace sead
{
template <>
const Matrix34<f32> Matrix34<f32>::zero(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                        0.0f, 0.0f);

template <>
const Matrix34<f32> Matrix34<f32>::ident(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                         1.0f, 0.0f);

template <>
const Matrix44<f32> Matrix44<f32>::zero(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

template <>
const Matrix44<f32> Matrix44<f32>::ident(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                         1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

}  // namespace sead
