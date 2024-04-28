#pragma once

#include "Library/Math/MathAngleUtil.h"
#include "Library/Matrix/MatrixUtil.h"

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

}
