#pragma once

#include <container/seadPtrArray.h>
#include <math/seadVector.h>

namespace al {

class IUseFluidSurface;

class FluidSurfaceHolder {
public:
    FluidSurfaceHolder();

    void registerFluidSurface(IUseFluidSurface* fluidSurface);
    void setWaterRippleFieldScale(f32 fieldScale);
    bool calcIsInFluid(const sead::Vector3f& trans, const char* type);
    IUseFluidSurface* tryFindFluidSurface(const sead::Vector3f& trans, const char* type);
    bool tryCalcFluidPos(sead::Vector3f* fluidPos, const sead::Vector3f& trans, const char* type);
    bool tryCalcFluidPosFlat(sead::Vector3f* fluidPosFlat, const sead::Vector3f& trans,
                             const char* type);
    bool tryCalcFluidDisplacement(sead::Vector3f* fluidDisplacement, const sead::Vector3f& trans,
                                  const char* type);
    bool tryCalcFluidNormal(sead::Vector3f* fluidNormal, const sead::Vector3f& trans,
                            const char* type);
    // TODO: add parameter names for all four functions below
    bool tryAddRipple(const sead::Vector3f& trans, f32, f32, const char* type);
    bool tryAddRippleAll(const sead::Vector3f& trans, f32, f32);
    bool tryAddRippleAllWithRange(const sead::Vector3f& trans, f32, f32, f32, f32);
    bool tryAddQuadRippleAll(const sead::Vector3f&, const sead::Vector3f&, const sead::Vector3f&,
                             const sead::Vector3f&, f32);

private:
    sead::PtrArray<IUseFluidSurface> mSurfaces;
    IUseFluidSurface* mCurSurfaceIn = nullptr;
    f32 mRippleFieldScale = 1.0f;
};

static_assert(sizeof(FluidSurfaceHolder) == 0x20);

}  // namespace al
