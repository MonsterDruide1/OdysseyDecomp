#pragma once

#include <math/seadVector.h>

namespace al {
// TODO: Fix return types
class IUseFluidSurface {
public:
    virtual void calcIsInArea(const sead::Vector3f&) const = 0;  // based on RippleCtrl vtable
    virtual void calcPos(sead::Vector3f*,
                         const sead::Vector3f&) const = 0;  // based on RippleCtrl vtable
    virtual void calcPosFlat(sead::Vector3f*, const sead::Vector3f&) const;
    virtual void calcDisplacementPos(sead::Vector3f*, const sead::Vector3f&) const;
    virtual void calcNormal(sead::Vector3f*,
                            const sead::Vector3f&) const = 0;  // based on RippleCtrl vtable
    virtual void tryAddRipple(const sead::Vector3f&, f32, f32);
    virtual void tryAddRippleWithRange(const sead::Vector3f&, f32, f32, f32, f32);
    virtual void tryAddQuadRipple(const sead::Vector3f&, const sead::Vector3f&,
                                  const sead::Vector3f&, const sead::Vector3f&, f32);
    virtual void getTypeName() const = 0;  // based on RippleCtrl vtable
    virtual void setFieldScale(f32);
};
}  // namespace al
