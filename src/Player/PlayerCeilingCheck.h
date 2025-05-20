#pragma once

#include <math/seadVector.h>

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
    PlayerCollisionCheckSphereMove* mCollisionCheckSphereMove = nullptr;
    float mSafetyCeilSpace = 0.0f;
    float mCeilCheckHeight = 0.0f;
    bool mIsCeilingCheckNormal = true;
    bool mIsCeilingCheckGrab = false;
    bool mIsEnableStandUp = false;
    bool mIsEnableHoldUp = false;
    bool mIsPressedCeil = false;
};

static_assert(sizeof(PlayerCeilingCheck) == 0x18);
