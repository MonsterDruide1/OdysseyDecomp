#pragma once

#include <math/seadVector.h>

namespace al {
class IUseFluidSurface {
public:
    virtual bool calcIsInArea(const sead::Vector3f&) const = 0;
    virtual void calcPos(sead::Vector3f*, const sead::Vector3f&) const = 0;
    virtual void calcPosFlat(sead::Vector3f*, const sead::Vector3f&) const;
    virtual void calcDisplacementPos(sead::Vector3f*, const sead::Vector3f&) const;
    virtual void calcNormal(sead::Vector3f*, const sead::Vector3f&) const = 0;
    virtual bool tryAddRipple(const sead::Vector3f&, f32, f32);
    virtual bool tryAddRippleWithRange(const sead::Vector3f&, f32, f32, f32, f32);
    virtual bool tryAddQuadRipple(const sead::Vector3f&, const sead::Vector3f&,
                                  const sead::Vector3f&, const sead::Vector3f&, f32);
    virtual const char* getTypeName() const = 0;
    virtual void setFieldScale(f32);
};
}  // namespace al
