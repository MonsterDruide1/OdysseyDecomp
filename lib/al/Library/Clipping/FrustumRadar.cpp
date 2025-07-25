#include "Library/Clipping/FrustumRadar.h"

#include <algorithm>

#include <math/seadMathCalcCommon.h>

#include "Library/Math/MathUtil.h"
#include "Library/Matrix/MatrixUtil.h"

namespace al {

FrustumRadar::FrustumRadar() {


}

void FrustumRadar::calcFrustumArea(const sead::Matrix34f& orthoMtx, f32 f1, f32 f2, f32 f3, f32 f4) {
    setLocalAxis(orthoMtx);

    setFactor(f1, f2);

    _54 = f3;
    _58 = f4;
}


void FrustumRadar::setLocalAxis(const sead::Matrix34f& orthoMtx) {
    sead::Matrix34f mtxInvertOrtho;
    calcMxtInvertOrtho(&mtxInvertOrtho, orthoMtx);

    mOrthoSide.x = mtxInvertOrtho.m[0][0];
    mOrthoSide.y = mtxInvertOrtho.m[1][0];
    mOrthoSide.z = mtxInvertOrtho.m[2][0];
    mOrthoUp.x = mtxInvertOrtho.m[0][1];
    mOrthoUp.y = mtxInvertOrtho.m[1][1];
    mOrthoUp.z = mtxInvertOrtho.m[2][1];
    const sead::Vector3f& base2=mtxInvertOrtho.getBase(2);
    mOrthoFront.x = -base2.x;
    mOrthoFront.y = -base2.y;
    mOrthoFront.z = -base2.z;
    mOrthoTrans.x = mtxInvertOrtho.m[0][3];
    mOrthoTrans.y = mtxInvertOrtho.m[1][3];
    mOrthoTrans.z = mtxInvertOrtho.m[2][3];
    _40 = 0.0f;

}

void FrustumRadar::setFactor(f32 f1, f32 f2) {
    _38 = sead::Mathf::tan(sead::Mathf::deg2rad(f1 * 0.5f));
    _3c = sead::Mathf::sqrt(_38*_38+1.0f);

    _30 = _38 * f2;
    _34 = sead::Mathf::sqrt(_30*_30+1.0f);
}

void FrustumRadar::calcFrustumArea(const sead::Matrix34f& orthoMtx, const sead::Matrix44f& factorMtx, f32 f1, f32 f2) {
    setLocalAxis(orthoMtx);
    setFactor(factorMtx);
    _54 = f1;
    _58 = f2;
}

void FrustumRadar::setFactor(const sead::Matrix44f& mtx) {
    _38 = 1.0 / mtx(1, 1);
    _3c = sead::Mathf::sqrt(_38*_38+1);

    _30 = 1.0 / mtx(0,0);
    _34 = sead::Mathf::sqrt(_30*_30+1);
}

void FrustumRadar::calcFrustumAreaStereo(const sead::Matrix34f& orthoMtxLeft, const sead::Matrix34f& OrthoMtxRight,
                                         const sead::Matrix44f& mtx, f32 f1, f32 f2) {
    setLocalAxisStereo(orthoMtxLeft, OrthoMtxRight);
    setFactorStereo(mtx);
    _54 = f1;
    _58 = f2;


}

void FrustumRadar::setLocalAxisStereo(const sead::Matrix34f& orthoMtxLeft, const sead::Matrix34f& orthoMtxRight) {
    sead::Matrix34f mtxInvertOrthoLeft;
    sead::Matrix34f mtxInvertRight;

    calcMxtInvertOrtho(&mtxInvertOrthoLeft, orthoMtxLeft);

    mOrthoSide.x = mtxInvertOrthoLeft.m[0][0];
    mOrthoSide.y = mtxInvertOrthoLeft.m[1][0];
    mOrthoSide.z = mtxInvertOrthoLeft.m[2][0];
    mOrthoUp.x = mtxInvertOrthoLeft.m[0][1];
    mOrthoUp.y = mtxInvertOrthoLeft.m[1][1];
    mOrthoUp.z = mtxInvertOrthoLeft.m[2][1];
    const sead::Vector3f& base2 = mtxInvertOrthoLeft.getBase(2);
    mOrthoFront.x = -base2.x;
    mOrthoFront.y = -base2.y;
    mOrthoFront.z = -base2.z;

    calcMxtInvertOrtho(&mtxInvertRight, orthoMtxRight);
    mOrthoTrans = (mtxInvertOrthoLeft.getTranslation() + mtxInvertRight.getTranslation()) * 0.5f;

    _40 = mOrthoSide.dot(mOrthoTrans - mtxInvertOrthoLeft.getTranslation());
}

void FrustumRadar::setFactorStereo(const sead::Matrix44f& mtx) {

    setFactor(mtx);
    f32 f2 = mtx(0,2);


    _44 = _30 * (1.0f - f2);
    _48 = sead::Mathf::sqrt(_44 * _44 + 1.0f);
    _4c = _30 * (f2 + 1.0f) ;
    _50 = sead::Mathf::sqrt(_4c * _4c + 1.0f);
}

bool FrustumRadar::judgeInLeft(const sead::Vector3f& pos, f32 f) const {
    f32 dot1 = mOrthoFront.dot(pos - mOrthoTrans);
    f32 dot2 = mOrthoSide.dot(pos - mOrthoTrans);

    return !(dot2 < -(dot1 * _30 + _34 * f));
}

bool FrustumRadar::judgeInRight(const sead::Vector3f& pos, f32 f) const {
    f32 dot1 = mOrthoFront.dot(pos - mOrthoTrans);
    f32 dot2 = mOrthoSide.dot(pos - mOrthoTrans);

    return !(dot1 * _30 + _34 * f < dot2);
}

bool FrustumRadar::judgeInTop(const sead::Vector3f& pos, f32 f) const {
    f32 dot1 = mOrthoFront.dot(pos - mOrthoTrans);
    f32 dot2 = mOrthoUp.dot(pos - mOrthoTrans);

    return !(dot1 * _38 + _3c * f < dot2);
}

bool FrustumRadar::judgeInBottom(const sead::Vector3f& pos, f32 f) const {
    f32 dot1 = mOrthoFront.dot(pos - mOrthoTrans);
    f32 dot2 = mOrthoUp.dot(pos - mOrthoTrans);

    return !(-(dot1 * _38 + _3c * f) > dot2);

}

bool FrustumRadar::judgeInArea(const sead::Vector3f& pos, f32 f1, f32 f2, f32 f3) const {
    f32 dot;
    f32 f4;
    f32 fVar2;

    dot = mOrthoFront.dot(pos - mOrthoTrans);
    if (dot < f2 - f1)
        return false;

    if (0.0 < f3 && f1 + f3 < dot)
        return false;

    f4 = sead::Mathf::abs(mOrthoUp.dot(pos - mOrthoTrans));
    if (dot * _38 + _3c * f1 < f4)
        return false;

    fVar2 = mOrthoSide.dot(pos - mOrthoTrans);
    if (isNearZero(_40, 0.001)) {
        if (dot * _30 + _34 * f1 < sead::Mathf::abs(fVar2)) {
            return false;
        }
    }
    else {
        f32 fVar3 = dot * _4c + _50 * f1;
        dot = dot * _44 + _48 * f1;
        f32 fVar4 = fVar2 - _40;
        fVar2 = fVar2 + _40;

        if (fVar4 > dot && fVar2 > fVar3)
            return false;

        f32 neg_fVar3 = -fVar3;
        f32 neg_dot = -dot;

        if (fVar4 < neg_fVar3 && fVar2 < neg_dot) {
            return false;
        }
    }
    return true;
}

bool FrustumRadar::judgeInArea(const sead::Vector3f& pos, f32 f1, f32 f2) const {
    return judgeInArea(pos, f1, f2, _58);
}

bool FrustumRadar::judgeInArea(const sead::Vector3f& pos, f32 f1) const {
    return judgeInArea(pos, f1, _54, _58);
}

bool FrustumRadar::judgeInAreaNoFar(const sead::Vector3f& pos, f32 f1) const {
    return judgeInArea(pos, f1, _54, -1.0f);
}

bool FrustumRadar::judgePointFlag(const sead::Vector3f& pos, f32 f1, f32 f2) const {
    bool bVar4;
    bool bVar3;

    sead::Vector3f transDotVec = pos - (mOrthoTrans);


    float transDot = transDotVec.dot(mOrthoFront);

    bool bVar1 = transDot < f1 | 2;
    if (transDot <= f2 || f2 == 0.0 || (f2 < transDot && f2 < 0.0)) {
        bVar1 = transDot < f1;
    }

    float dotUp = transDotVec.dot(mOrthoUp);

    bool bVar2 = bVar1 | 4;
    if (-(transDot * _38) <= dotUp) {
        bVar2 = bVar1;
    }
    bool bVar5 = bVar2 | 8;
    if (dotUp <= transDot * _38) {
        bVar5 = bVar2;
    }
    transDotVec.x = transDotVec.x * mOrthoSide.x + transDotVec.y * mOrthoSide.y +
                transDotVec.z * mOrthoSide.z;
    bool bVar6 = isNearZero(_40, 0.001);
    if (bVar6) {
        bVar4 = bVar5 | 0x10;
        if (-(transDot * _30) <= transDotVec.x) {
            bVar4 = bVar5;
        }
        bVar6 = transDot * _30 < transDotVec.x;
        bVar3 = bVar4 | 0x20;
    }
    else {
        transDotVec.y = transDot * _44;
        transDot = transDot * _4c;
        transDotVec.z = transDotVec.x - _40;
        transDotVec.x = transDotVec.x + _40;
        bVar4 = bVar5 | 0x20;
        if (transDotVec.x <= transDot || transDotVec.z == transDotVec.y ||
            (transDot < transDotVec.x && transDotVec.z < transDotVec.y)) {
            bVar4 = bVar5;
        }

        bVar3 = bVar4 | 0x10;
        bVar6 = false;
    }
    if (!bVar6) {
        bVar3 = bVar4;
    }
    return bVar3;
}



} // namespace al
