#pragma once

#include <math/seadVector.h>
#include "Library/LiveActor/LiveActor.h"
#include "math/seadQuat.h"

namespace al {

void separateVelocityHV(sead::Vector3f*, sead::Vector3f*, al::LiveActor*);
void alongVectorNormalH(sead::Vector3f* a1, const sead::Vector3f* a2, const sead::Vector3f* a3,
                        const sead::Vector3f* a4);

bool isNearZero(const sead::Vector2f&, f32);

bool makeQuatRotationLimit(sead::Quatf*, sead::Vector3f const&, sead::Vector3f const&, float);

s32 converge(s32, s32, s32);
f32 converge(f32, f32, f32);

void calcDirVerticalAny(sead::Vector3f*, const sead::Vector3f&);

void makeQuatUpFront(sead::Quatf*, const sead::Vector3f&, const sead::Vector3f&);


}  // namespace al
