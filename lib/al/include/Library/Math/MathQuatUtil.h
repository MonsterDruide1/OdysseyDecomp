#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {

void makeQuatFrontUp(sead::Quatf*,const sead::Vector3f&,const sead::Vector3f&);
void makeQuatRotationLimit(sead::Quatf*, const sead::Vector3f&, const sead::Vector3f&, float);

void calcQuatFront(sead::Vector3f*, const sead::Quatf&);
void calcQuatUp(sead::Vector3f*, const sead::Quatf&);

void turnVecToVecDegree(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&, float);
void turnVecToVecRate(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&, float);

bool turnQuatFrontToDirDegreeH(sead::Quatf* a1, const sead::Vector3f& a2, float a3);
bool turnQuat(sead::Quatf*, const sead::Quatf&, const sead::Vector3f&, const sead::Vector3f&, float);
bool turnQuatYDirRate(sead::Quatf*, const sead::Quatf&, const sead::Vector3f&, float);

}  // namespace al
