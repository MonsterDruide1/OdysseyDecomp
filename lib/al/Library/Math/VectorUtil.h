#pragma once

#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {

void turnVecToVecDegree(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&, f32);
void turnVecToVecRate(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&, f32);
void verticalizeVec(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&);

void rotateQuatRadian(sead::Quatf*, const sead::Quatf&, const sead::Vector3f&, f32);
void calcQuatFront(sead::Vector3f*, const sead::Quatf&);
void calcQuatUp(sead::Vector3f*, const sead::Quatf&);
void makeQuatFrontUp(sead::Quatf*, const sead::Vector3f&, const sead::Vector3f&);
void makeQuatUpFront(sead::Quatf*, const sead::Vector3f&, const sead::Vector3f&);

void rotateVectorDegreeX(sead::Vector3f*, f32);
void rotateVectorDegreeY(sead::Vector3f*, f32);
void rotateVectorDegreeZ(sead::Vector3f*, f32);

void makeMtxFrontUpPos(sead::Matrix34f*, const sead::Vector3f&, const sead::Vector3f&,
                       const sead::Vector3f&);
void makeMtxUpFrontPos(sead::Matrix34f*, const sead::Vector3f&, const sead::Vector3f&,
                       const sead::Vector3f&);
void makeMtxRotateTrans(sead::Matrix34f*, const sead::Vector3f&, const sead::Vector3f&);

void turnQuatYDirRate(sead::Quatf*, const sead::Quatf&, const sead::Vector3f&, f32);

}  // namespace al
