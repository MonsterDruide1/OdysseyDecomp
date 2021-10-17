#pragma once

#include <math/seadVector.h>
#include <math/seadQuat.h>

namespace al {

bool isNearZero(const sead::Vector2f&, float);
void verticalizeVec(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&);
bool tryNormalizeOrZero(sead::Vector3f*);
void normalize(sead::Vector3f*);
void turnVecToVecDegree(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&, float);
void turnVecToVecRate(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&, float);


void calcQuatFront(sead::Vector3f*, const sead::Quatf&);
void makeQuatFrontUp(sead::Quatf*, const sead::Vector3f&, const sead::Vector3f&);

}
