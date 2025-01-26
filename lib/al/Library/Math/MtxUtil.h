#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {

void makeMtxRotateTrans(sead::Matrix34f* out, const sead::Vector3f& rotate,
                        const sead::Vector3f& trans);
void makeMtxFrontUpPos(sead::Matrix34f* out, const sead::Vector3f& front, const sead::Vector3f& up,
                       const sead::Vector3f& pos);
void makeMtxUpFrontPos(sead::Matrix34f* out, const sead::Vector3f& up, const sead::Vector3f& front,
                       const sead::Vector3f& pos);

}  // namespace al
