#pragma once

#include <math/seadBoundBox.h>
#include <math/seadMatrix.h>

#include "Library/Matrix/MatrixUtil.h"

namespace al {

class FrustumRadar {
public:
    enum PointFlag : s32 {
        Invalid = -1,
        None = 0,
        Near = 1,
        Far = 2,
        Bottom = 4,
        Top = 8,
        Left = 16,
        Right = 32,
    };

    FrustumRadar();
    void calcFrustumArea(const sead::Matrix34f& orthoMtx, f32 vFovAngle, f32 aspectRatio,
                         f32 areaMin, f32 areaMax);
    void setLocalAxis(const sead::Matrix34f& orthoMtx);
    void setFactor(f32 vFovAngle, f32 aspectRatio);
    void calcFrustumArea(const sead::Matrix34f& orthoMtx, const sead::Matrix44f& projectionMtx,
                         f32 areaMin, f32 areaMax);
    void setFactor(const sead::Matrix44f& projectionMtx);
    void calcFrustumAreaStereo(const sead::Matrix34f& orthoMtxLeft,
                               const sead::Matrix34f& OrthoMtxRight,
                               const sead::Matrix44f& projectionMtx, f32 areaMin, f32 areaMax);
    void setLocalAxisStereo(const sead::Matrix34f& orthoMtxLeft,
                            const sead::Matrix34f& orthoMtxRight);
    void setFactorStereo(const sead::Matrix44f& projectionMtx);
    bool judgeInLeft(const sead::Vector3f& pos, f32 radius) const;
    bool judgeInRight(const sead::Vector3f& pos, f32 radius) const;
    bool judgeInTop(const sead::Vector3f& pos, f32 radius) const;
    bool judgeInBottom(const sead::Vector3f& pos, f32 radius) const;
    bool judgeInArea(const sead::Vector3f& pos, f32 radius, f32 areaMin, f32 areaMax) const;
    bool judgeInArea(const sead::Vector3f& pos, f32 radius, f32 areaMin) const;
    bool judgeInArea(const sead::Vector3f& pos, f32 radius) const;
    bool judgeInAreaNoFar(const sead::Vector3f& pos, f32 radius) const;
    PointFlag judgePointFlag(const sead::Vector3f& pos, f32 areaMin, f32 areaMax) const;
    bool judgeInAreaObb(const sead::Matrix34f* mtx, const sead::BoundBox3f& boundBox, f32 areaMin,
                        f32 areaMax) const;
    bool judgeInAreaObb(const sead::Matrix34f* mtx, const sead::BoundBox3f& boundBox,
                        f32 areaMin) const;
    bool judgeInAreaObb(const sead::Matrix34f* mtx, const sead::BoundBox3f& boundBox) const;
    bool judgeInAreaObbNoFar(const sead::Matrix34f* mtx, const sead::BoundBox3f& boundBox) const;

private:
    sead::Vector3f mOrthoSide = sead::Vector3f::ex;
    sead::Vector3f mOrthoUp = sead::Vector3f::ey;
    sead::Vector3f mOrthoFront = sead::Vector3f::ez;
    sead::Vector3f mOrthoTrans = sead::Vector3f::zero;
    f32 mHorizontalSlope = 0.3f;
    f32 mHorizontalNormFactor = 1.04403f;
    f32 mVerticalSlope = 0.2f;
    f32 mVerticalNormFactor = 1.0098f;
    f32 mStereoEyeOffset = 0.0f;
    f32 mStereoSlopeLeft = 0.3f;
    f32 mStereoNormFactorLeft = 1.04403f;
    f32 mStereoSlopeRight = 0.3f;
    f32 mStereoNormFactorRight = 1.04403f;
    f32 mAreaMin = 100.0f;
    f32 mAreaMax = 10000.0f;
};

}  // namespace al
