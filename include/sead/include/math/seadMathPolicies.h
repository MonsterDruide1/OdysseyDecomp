#ifndef SEAD_POLICIES_H_
#define SEAD_POLICIES_H_

#include "math/seadMathBase.h"

namespace sead
{
template <typename T>
class Policies
{
public:
    using Vec2Base = BaseVec2<T>;
    using Vec3Base = BaseVec3<T>;
    using Vec4Base = BaseVec4<T>;
    using QuatBase = BaseQuat<T>;
    using Mtx34Base = BaseMtx34<T>;
    using Mtx44Base = BaseMtx44<T>;
};

}  // namespace sead

#endif  // SEAD_POLICIES_H_
