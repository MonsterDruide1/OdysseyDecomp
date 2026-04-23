#include "Library/Clipping/FrustumRadar.h"

#include <math/seadMathCalcCommon.h>

#include "Library/Math/MathUtil.h"
#include "Library/Matrix/MatrixUtil.h"

namespace al {

FrustumRadar::FrustumRadar() = default;

void FrustumRadar::calcFrustumArea(const sead::Matrix34f& orthoMtx, f32 fovyAngle, f32 aspectRatio,
                                   f32 near, f32 far) {
    setLocalAxis(orthoMtx);

    setFactor(fovyAngle, aspectRatio);

    mNear = near;
    mFar = far;
}

void FrustumRadar::setLocalAxis(const sead::Matrix34f& orthoMtx) {
    sead::Matrix34f mtxInvertOrtho;
    calcMxtInvertOrtho(&mtxInvertOrtho, orthoMtx);

    mtxInvertOrtho.getBase(mOrthoSide, 0);
    mtxInvertOrtho.getBase(mOrthoUp, 1);
    mtxInvertOrtho.getBase(mOrthoFront, 2);
    mOrthoFront.negate();
    mtxInvertOrtho.getTranslation(mOrthoTrans);

    mStereoEyeOffset = 0.0f;
}

void FrustumRadar::setFactor(f32 fovyAngle, f32 aspectRatio) {
    mVerticalSlope = sead::Mathf::tan(sead::Mathf::deg2rad(fovyAngle * 0.5f));
    mVerticalNormFactor = sead::Mathf::sqrt(mVerticalSlope * mVerticalSlope + 1.0f);

    mHorizontalSlope = mVerticalSlope * aspectRatio;
    mHorizontalNormFactor = sead::Mathf::sqrt(mHorizontalSlope * mHorizontalSlope + 1.0f);
}

void FrustumRadar::calcFrustumArea(const sead::Matrix34f& orthoMtx,
                                   const sead::Matrix44f& projectionMtx, f32 near, f32 far) {
    setLocalAxis(orthoMtx);
    setFactor(projectionMtx);
    mNear = near;
    mFar = far;
}

void FrustumRadar::setFactor(const sead::Matrix44f& projectionMtx) {
    mVerticalSlope = 1.0f / projectionMtx(1, 1);
    mVerticalNormFactor = sead::Mathf::sqrt(mVerticalSlope * mVerticalSlope + 1.0f);

    mHorizontalSlope = 1.0f / projectionMtx(0, 0);
    mHorizontalNormFactor = sead::Mathf::sqrt(mHorizontalSlope * mHorizontalSlope + 1.0f);
}

void FrustumRadar::calcFrustumAreaStereo(const sead::Matrix34f& orthoMtxLeft,
                                         const sead::Matrix34f& orthoMtxRight,
                                         const sead::Matrix44f& projectionMtx, f32 near, f32 far) {
    setLocalAxisStereo(orthoMtxLeft, orthoMtxRight);
    setFactorStereo(projectionMtx);
    mNear = near;
    mFar = far;
}

void FrustumRadar::setLocalAxisStereo(const sead::Matrix34f& orthoMtxLeft,
                                      const sead::Matrix34f& orthoMtxRight) {
    sead::Matrix34f mtxInvertOrthoLeft;
    sead::Matrix34f mtxInvertRight;
    calcMxtInvertOrtho(&mtxInvertOrthoLeft, orthoMtxLeft);

    mtxInvertOrthoLeft.getBase(mOrthoSide, 0);
    mtxInvertOrthoLeft.getBase(mOrthoUp, 1);
    mtxInvertOrthoLeft.getBase(mOrthoFront, 2);
    mOrthoFront.negate();

    calcMxtInvertOrtho(&mtxInvertRight, orthoMtxRight);
    mOrthoTrans = (mtxInvertOrthoLeft.getTranslation() + mtxInvertRight.getTranslation()) * 0.5f;

    mStereoEyeOffset = mOrthoSide.dot(mOrthoTrans - mtxInvertOrthoLeft.getTranslation());
}

void FrustumRadar::setFactorStereo(const sead::Matrix44f& projectionMtx) {
    setFactor(projectionMtx);
    f32 centerOffset = projectionMtx(0, 2);

    mStereoSlopeLeft = mHorizontalSlope * (1.0f - centerOffset);
    mStereoNormFactorLeft = sead::Mathf::sqrt(mStereoSlopeLeft * mStereoSlopeLeft + 1.0f);

    mStereoSlopeRight = mHorizontalSlope * (1.0f + centerOffset);
    mStereoNormFactorRight = sead::Mathf::sqrt(mStereoSlopeRight * mStereoSlopeRight + 1.0f);
}

bool FrustumRadar::judgeInLeft(const sead::Vector3f& pos, f32 radius) const {
    f32 dotFront = mOrthoFront.dot(pos - mOrthoTrans);
    f32 dotSide = mOrthoSide.dot(pos - mOrthoTrans);

    return !(dotSide < -(dotFront * mHorizontalSlope + mHorizontalNormFactor * radius));
}

bool FrustumRadar::judgeInRight(const sead::Vector3f& pos, f32 radius) const {
    f32 dotFront = mOrthoFront.dot(pos - mOrthoTrans);
    f32 dotSide = mOrthoSide.dot(pos - mOrthoTrans);

    return !(dotFront * mHorizontalSlope + mHorizontalNormFactor * radius < dotSide);
}

bool FrustumRadar::judgeInTop(const sead::Vector3f& pos, f32 radius) const {
    f32 dotFront = mOrthoFront.dot(pos - mOrthoTrans);
    f32 dotUp = mOrthoUp.dot(pos - mOrthoTrans);

    return !(dotFront * mVerticalSlope + mVerticalNormFactor * radius < dotUp);
}

bool FrustumRadar::judgeInBottom(const sead::Vector3f& pos, f32 radius) const {
    f32 dotFront = mOrthoFront.dot(pos - mOrthoTrans);
    f32 dotUp = mOrthoUp.dot(pos - mOrthoTrans);

    return !(dotUp < -(dotFront * mVerticalSlope + mVerticalNormFactor * radius));
}

bool FrustumRadar::judgeInArea(const sead::Vector3f& pos, f32 radius, f32 near, f32 far) const {
    f32 dotFront = mOrthoFront.dot(pos - mOrthoTrans);

    if (dotFront < near - radius)
        return false;

    if (far > 0.0f && radius + far < dotFront)
        return false;

    f32 dotUpAbs = sead::Mathf::abs(mOrthoUp.dot(pos - mOrthoTrans));
    if (dotFront * mVerticalSlope + mVerticalNormFactor * radius < dotUpAbs)
        return false;

    f32 dotSide = mOrthoSide.dot(pos - mOrthoTrans);
    if (isNearZero(mStereoEyeOffset)) {
        if (dotFront * mHorizontalSlope + mHorizontalNormFactor * radius <
            sead::Mathf::abs(dotSide))
            return false;
    } else {
        f32 limitRight = dotFront * mStereoSlopeRight + mStereoNormFactorRight * radius;
        f32 limitLeft = dotFront * mStereoSlopeLeft + mStereoNormFactorLeft * radius;

        f32 relSideLeft = dotSide - mStereoEyeOffset;
        f32 relSideRight = dotSide + mStereoEyeOffset;

        if (relSideLeft > limitLeft && relSideRight > limitRight)
            return false;

        if (relSideLeft < -limitRight && relSideRight < -limitLeft)
            return false;
    }
    return true;
}

bool FrustumRadar::judgeInArea(const sead::Vector3f& pos, f32 radius, f32 near) const {
    return judgeInArea(pos, radius, near, mFar);
}

bool FrustumRadar::judgeInArea(const sead::Vector3f& pos, f32 radius) const {
    return judgeInArea(pos, radius, mNear, mFar);
}

bool FrustumRadar::judgeInAreaNoFar(const sead::Vector3f& pos, f32 radius) const {
    return judgeInArea(pos, radius, mNear, -1.0f);
}

FrustumRadar::PointFlag FrustumRadar::judgePointFlag(const sead::Vector3f& pos, f32 near,
                                                     f32 far) const {
    u32 flag = PointFlag::None;

    sead::Vector3f relPos = pos - mOrthoTrans;
    f32 dotFront = mOrthoFront.dot(relPos);

    if (dotFront < near)
        flag |= PointFlag::Near;

    if (far > 0.0f && dotFront > far)
        flag |= PointFlag::Far;

    f32 dotUp = mOrthoUp.dot(relPos);
    if (dotUp < -(dotFront * mVerticalSlope))
        flag |= PointFlag::Bottom;

    if (dotFront * mVerticalSlope < dotUp)
        flag |= PointFlag::Top;

    f32 dotSide = mOrthoSide.dot(relPos);
    if (isNearZero(mStereoEyeOffset)) {
        if (dotSide < -(dotFront * mHorizontalSlope))
            flag |= PointFlag::Left;
        if (dotFront * mHorizontalSlope < dotSide)
            flag |= PointFlag::Right;
        return (PointFlag)flag;
    }

    f32 limitRight = dotFront * mStereoSlopeRight;
    f32 limitLeft = dotFront * mStereoSlopeLeft;
    f32 distSideLeftEye = dotSide - mStereoEyeOffset;
    f32 distSideRightEye = dotSide + mStereoEyeOffset;

    if (distSideLeftEye > limitLeft && distSideRightEye > limitRight)
        flag |= PointFlag::Right;

    if (distSideLeftEye < -limitRight && distSideRightEye < -limitLeft)
        flag |= PointFlag::Left;

    return (PointFlag)flag;
}

bool FrustumRadar::judgeInAreaObb(const sead::Matrix34f* mtx, const sead::BoundBox3f& boundBox,
                                  f32 near, f32 far) const {
    sead::Vector3f corners[8];
    calcObbCorners(corners, *mtx, boundBox);

    s32 mask = ~PointFlag::None;
    for (s32 i = 0; i < 8; ++i) {
        s32 pointFlags = judgePointFlag(corners[i], near, far);
        if (pointFlags == PointFlag::None)
            return true;

        mask &= pointFlags;
    }

    return mask == PointFlag::None;
}

bool FrustumRadar::judgeInAreaObb(const sead::Matrix34f* mtx, const sead::BoundBox3f& boundBox,
                                  f32 near) const {
    return judgeInAreaObb(mtx, boundBox, near, mFar);
}

bool FrustumRadar::judgeInAreaObb(const sead::Matrix34f* mtx,
                                  const sead::BoundBox3f& boundBox) const {
    return judgeInAreaObb(mtx, boundBox, mNear, mFar);
}

bool FrustumRadar::judgeInAreaObbNoFar(const sead::Matrix34f* mtx,
                                       const sead::BoundBox3f& boundBox) const {
    return judgeInAreaObb(mtx, boundBox, mNear, -1.0f);
}

}  // namespace al
