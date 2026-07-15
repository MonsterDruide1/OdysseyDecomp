#include "Project/Fluid/FluidSurfaceHolder.h"

#include "Library/Base/StringUtil.h"
#include "Library/Fluid/IUseFluidSurface.h"

namespace al {

FluidSurfaceHolder::FluidSurfaceHolder() {
    mSurfaces.allocBuffer(40, nullptr);
}

void FluidSurfaceHolder::registerFluidSurface(IUseFluidSurface* fluidSurface) {
    mSurfaces.pushBack(fluidSurface);
}

void FluidSurfaceHolder::setWaterRippleFieldScale(f32 fieldScale) {
    mRippleFieldScale = fieldScale;
    for (s32 i = 0; i < mSurfaces.size(); i++)
        mSurfaces[i]->setFieldScale(mRippleFieldScale);
}

bool FluidSurfaceHolder::calcIsInFluid(const sead::Vector3f& trans, const char* type) {
    IUseFluidSurface* surface = tryFindFluidSurface(trans, type);
    if (!surface)
        return false;

    mCurSurfaceIn = surface;
    return true;
}

// NON_MATCHING: https://decomp.me/scratch/YGKVO
IUseFluidSurface* FluidSurfaceHolder::tryFindFluidSurface(const sead::Vector3f& trans,
                                                          const char* type) {
    IUseFluidSurface* out = nullptr;

    if (isEqualString(type, "Anything")) {
        f32 minDist = 100000.0f;
        for (s32 i = 0; i < mSurfaces.size(); i++) {
            if (!mSurfaces[i]->calcIsInArea(trans))
                continue;

            sead::Vector3f posFlat = {0.0f, 0.0f, 0.0f};
            mSurfaces[i]->calcPosFlat(&posFlat, trans);

            f32 dist = (posFlat - trans).length();
            if (dist < minDist) {
                minDist = dist;
                out = mSurfaces[i];
            }
        }
    } else {
        f32 minDist = 100000.0f;
        for (s32 i = 0; i < mSurfaces.size(); i++) {
            if (!isEqualSubString(mSurfaces[i]->getTypeName(), type))
                continue;

            if (!mSurfaces[i]->calcIsInArea(trans))
                continue;

            sead::Vector3f posFlat = {0.0f, 0.0f, 0.0f};
            mSurfaces[i]->calcPosFlat(&posFlat, trans);

            f32 dist = (posFlat - trans).length();
            if (dist < minDist) {
                minDist = dist;
                out = mSurfaces[i];
            }
        }
    }

    return out;
}

bool FluidSurfaceHolder::tryCalcFluidPos(sead::Vector3f* fluidPos, const sead::Vector3f& trans,
                                         const char* type) {
    IUseFluidSurface* surface = tryFindFluidSurface(trans, type);
    if (!surface)
        return false;

    mCurSurfaceIn = surface;
    mCurSurfaceIn->calcPos(fluidPos, trans);
    return true;
}

bool FluidSurfaceHolder::tryCalcFluidPosFlat(sead::Vector3f* fluidPosFlat,
                                             const sead::Vector3f& trans, const char* type) {
    IUseFluidSurface* surface = tryFindFluidSurface(trans, type);
    if (!surface)
        return false;

    mCurSurfaceIn = surface;
    mCurSurfaceIn->calcPosFlat(fluidPosFlat, trans);
    return true;
}

bool FluidSurfaceHolder::tryCalcFluidDisplacement(sead::Vector3f* fluidDisplacement,
                                                  const sead::Vector3f& trans, const char* type) {
    IUseFluidSurface* surface = tryFindFluidSurface(trans, type);
    if (!surface)
        return false;

    mCurSurfaceIn = surface;
    mCurSurfaceIn->calcDisplacementPos(fluidDisplacement, trans);
    return true;
}

bool FluidSurfaceHolder::tryCalcFluidNormal(sead::Vector3f* fluidNormal,
                                            const sead::Vector3f& trans, const char* type) {
    IUseFluidSurface* surface = tryFindFluidSurface(trans, type);
    if (!surface)
        return false;

    mCurSurfaceIn = surface;
    mCurSurfaceIn->calcNormal(fluidNormal, trans);
    return true;
}

bool FluidSurfaceHolder::tryAddRipple(const sead::Vector3f& trans, f32 a, f32 b, const char* type) {
    IUseFluidSurface* surface = tryFindFluidSurface(trans, type);
    if (!surface)
        return false;

    mCurSurfaceIn = surface;
    return mCurSurfaceIn->tryAddRipple(trans, a, b);
}

bool FluidSurfaceHolder::tryAddRippleAll(const sead::Vector3f& trans, f32 a, f32 b) {
    bool out = false;
    for (s32 i = 0; i < mSurfaces.size(); i++)
        out |= mSurfaces[i]->tryAddRipple(trans, a, b);
    return out;
}

bool FluidSurfaceHolder::tryAddRippleAllWithRange(const sead::Vector3f& trans, f32 a, f32 b, f32 c,
                                                  f32 d) {
    bool out = false;
    for (s32 i = 0; i < mSurfaces.size(); i++)
        out |= mSurfaces[i]->tryAddRippleWithRange(trans, a, b, c, d);
    return out;
}

bool FluidSurfaceHolder::tryAddQuadRippleAll(const sead::Vector3f& a, const sead::Vector3f& b,
                                             const sead::Vector3f& c, const sead::Vector3f& d,
                                             f32 e) {
    bool out = false;
    for (s32 i = 0; i < mSurfaces.size(); i++)
        out |= mSurfaces[i]->tryAddQuadRipple(a, b, c, d, e);
    return out;
}

}  // namespace al
