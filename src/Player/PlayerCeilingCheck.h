#pragma once

#include <math/seadVector.h>
#include "Library/Collision/CollisionDirector.h"

namespace al {
class CollisionDirector;
class CollisionPartsFilterBase;
}  // namespace al
class PlayerCollisionCheckSphereMove;

class PlayerCeilingCheck {
public:
    PlayerCeilingCheck(al::CollisionDirector*);

    void setupCeilingCheckNormal();
    void update(const sead::Vector3f&, const sead::Vector3f&, float, float, float, float);
    void setupCeilingCheckGrab();
    void setCollisionPartsFilter(const al::CollisionPartsFilterBase*);

public:
    PlayerCollisionCheckSphereMove *mCollisionCheckSphereMove;
    float mSafetyCeilSpace;
    float mCeilCheckHeight;
    bool mIsCeilingCheckNormal;
    bool mIsCeilingCheckGrab;
    bool mIsEnableStandUp = true;
    bool mIsEnableHoldUp;
    bool mIsPressedCeil;
};
static_assert(sizeof(PlayerCeilingCheck) == 0x18);
