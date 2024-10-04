#pragma once

#include <math/seadVector.h>

namespace al {
// TODO: Fix return types
class IUseFluidSurface {
public:
    virtual void calcDisplacementPos(sead::Vector3f*, const sead::Vector3f&) const = 0;
    virtual void setFieldScale(f32) = 0;
    virtual void calcPosFlat(sead::Vector3f*, const sead::Vector3f&) const = 0;
    virtual void tryAddRipple(const sead::Vector3f&, f32, f32) = 0;
    virtual void tryAddRippleWithRange(const sead::Vector3f&, f32, f32, f32, f32) = 0;
    virtual void tryAddQuadRipple(const sead::Vector3f&, const sead::Vector3f&,
                                  const sead::Vector3f&, const sead::Vector3f&, f32) = 0;
};
}  // namespace al
