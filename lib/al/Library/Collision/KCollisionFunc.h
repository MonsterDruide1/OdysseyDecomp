#pragma once

#include <math/seadVector.h>

#include "Library/Collision/KTriangle.h"

namespace al {

class HitInfo {
public:
    HitInfo();
    
private:
    al::Triangle mTriangle;
    f32 unk;
    sead::Vector3f mCollisionHitPos;
    sead::Vector3f unk3;
    sead::Vector3f mCollisionMovingReaction;
    u8 mCollisionLocation;
};
static_assert(sizeof(HitInfo) == 0xA0);

class ArrowHitInfo : public HitInfo {};

class DiskHitInfo : public HitInfo {};

class SphereHitInfo : public HitInfo {};

}
