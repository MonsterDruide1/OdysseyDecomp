#pragma once

#include "Library/Math/MathAngleUtil.h"
#include "Library/Math/SegmentUtil.h"
#include "Library/Matrix/MatrixUtil.h"
#include "math/seadMatrix.h"
#include "math/seadQuatCalcCommon.h"
#include "math/seadVectorFwd.h"

namespace al {

void makeMtxRotateTrans(sead::Matrix34f* out, const sead::Vector3f& rotate, const sead::Vector3f& trans) {
    out->makeRT(sead::Vector3f(sead::Mathf::deg2rad(rotate.x), sead::Mathf::deg2rad(rotate.y), sead::Mathf::deg2rad(rotate.z)), trans);
}

void makeMtxFrontUpPos(sead::Matrix34f* out, const sead::Vector3f& front, const sead::Vector3f& up, const sead::Vector3f& pos) {
    sead::Vector3f frontNorm = front;
    al::normalize(&frontNorm);

    sead::Vector3f v18;
    v18.setCross(up, frontNorm);
    al::normalize(&v18);

    sead::Vector3f unk;
    unk.setCross(frontNorm, v18);

    out->m[0][0] = v18.x;
    out->m[0][1] = unk.x;
    out->m[1][0] = v18.y;
    out->m[1][1] = unk.y;
    out->m[0][2] = frontNorm.x;
    out->m[1][2] = frontNorm.y;
    out->m[2][0] = v18.z;
    out->m[2][1] = unk.z;
    out->m[2][2] = frontNorm.z;

    out->setTranslation(pos);
}

void makeMtxUpFrontPos(sead::Matrix34f* out, const sead::Vector3f& up, const sead::Vector3f& front, const sead::Vector3f& pos) {
    makeMtxFrontUpPos(out, front, up, pos);
}

void normalize(sead::Vector3f* vec) {
    sead::Vector3f tmp = *vec;
    al::normalize(vec, tmp);
}

void normalize(sead::Vector3f* out, const sead::Vector3f& vec) {
    *out = vec;
    out->normalize();
}

void calcQuatUp(sead::Vector3f* out, const sead::Quatf& quat) {
    out->x = 2.0f * (quat.x * quat.y - quat.w * quat.z);
    out->y = (1 - (quat.x * quat.x)) - 2.0f * (quat.z * quat.z);
    out->z = 2.0f * (quat.w * quat.x + quat.y * quat.z);
}

void calcQuatFront(sead::Vector3f* out, const sead::Quatf& quat) {
    out->x = 2.0f * (quat.x * quat.z + quat.w * quat.y);
    out->y = 2.0f * (quat.y * quat.z - quat.w * quat.x);
    out->z = (1 - (quat.x * quat.x)) - 2.0f * (quat.y * quat.y);
}

f32 calcRate01(f32 a2, f32 a3, f32 a4) {
    float v3; // s2
    float v4; // s3
    float result; // s0

    v3 = a4 - a3;
    v4 = -v3;
    if ( v3 > 0.0 )
        v4 = v3;

    if ( v4 < 0.001 )
        return 1.0;

    result = (float)(a2 - a3) / v3;
    if ( result < 0.0 )
        return 0.0;

    if ( result > 1.0 )
        return 1.0;

    return result;
}

void slerpQuat(sead::Quatf* out, const sead::Quatf& a, const sead::Quatf& b, float val) {
    sead::QuatCalcCommon<f32>::slerpTo(*out, a, b, val);
}

f32 calcCylinderRadiusDot(const sead::Vector3f& a1, const sead::Vector3f& a2, f32 a3) {
    float v6; // s0
    float v7; // s1
    float v8; // s0
    float v9; // s0

    v6 = a2.dot(a1);
    v7 = -v6;
    if ( v6 > 0.0 )
        v7 = a2.dot(a1);

    v8 = -1.0;
    if ( v7 >= -1.0 )
    {
        v8 = v7;
        if ( v7 > 1.0 )
        v8 = 1.0;
    }

    v9 = sead::Mathf::cos(v8);
    return sead::Mathf::sin(v9) * a3;
}

bool isParallelDirection(const sead::Vector3f& a1, const sead::Vector3f& a2, f32 a3) {
    float v7; // s3
    float v9; // s4
    float v10; // s1

    v7 = (float)(a1.y * a2.z) - (float)(a1.z * a2.y);
    if ( v7 <= 0.0 )
        v7 = -v7;

    if ( v7 > a3 )
        return 0LL;

    v9 = (float)(a1.z * a2.x) - (float)(a2.z * a1.x);
    if ( v9 <= 0.0 )
        v9 = -v9;

    if ( v9 > a3 )
        return 0LL;

    v10 = (float)(a2.y * a1.x) - (float)(a1.y * a2.x);
    if ( v10 <= 0.0 )
        v10 = -v10;

    return v10 <= a3;
}

bool isNearZero(f32 val, f32 epsilon) {
    return sead::Mathf::abs(val) < epsilon;
}

bool isNearZero(const sead::Vector3f& vec, f32 epsilon) {
    return vec.squaredLength() < epsilon*epsilon;
}

bool tryNormalizeOrZero(sead::Vector3f* vec) {
    f32 squaredLength = vec->squaredLength();
    if (squaredLength < 0.000001f) {
        *vec = sead::Vector3f::zero;
        return false;
    }

    vec->normalize();
    return true;
}

void preScaleMtx(sead::Matrix34f * mtx, const sead::Vector3f & vec) {
    mtx->scaleBases(vec.x, vec.y, vec.z);
}

void calcMtxScale(sead::Vector3f* scale, const sead::Matrix34f& mtx) {
    sead::Vector3f x,y,z;
    mtx.getBase(x, 0);
    mtx.getBase(y, 1);
    mtx.getBase(z, 2);

    scale->x = x.length();
    scale->y = y.length();
    scale->z = z.length();
}

}
