#pragma once

#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {

bool isNearZero(f32, f32);
bool isNearZero(const sead::Vector2f&, f32);
bool isNearZero(const sead::Vector3f&, f32);
void verticalizeVec(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&);
bool tryNormalizeOrZero(sead::Vector3f*);
bool normalize(sead::Vector2f* value);
void normalize(sead::Vector3f*);
void turnVecToVecDegree(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&, f32);
void turnVecToVecRate(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&, f32);

void calcQuatFront(sead::Vector3f*, const sead::Quatf&);
void calcQuatUp(sead::Vector3f*, const sead::Quatf&);
void makeQuatFrontUp(sead::Quatf*, const sead::Vector3f&, const sead::Vector3f&);

void makeMtxFrontUpPos(sead::Matrix34f*, const sead::Vector3f&, const sead::Vector3f&, const sead::Vector3f&);
void makeMtxUpFrontPos(sead::Matrix34f*, const sead::Vector3f&, const sead::Vector3f&, const sead::Vector3f&);
void makeMtxRotateTrans(sead::Matrix34f*, const sead::Vector3f&, const sead::Vector3f&);

}  // namespace al
