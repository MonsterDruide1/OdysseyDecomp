#pragma once

#include <math/seadVector.h>

namespace al {
class CollisionDirector;
class CollisionPartsFilterBase;
}  // namespace al
class PlayerCollisionCheckSphereMove;

class PlayerCeilingCheck {
public:
    PlayerCeilingCheck(al::CollisionDirector* director);

    void setupCeilingCheckNormal();
    void update(const sead::Vector3f& trans, const sead::Vector3f& up, f32 tall,
                f32 collisionRadiusSquat, f32 headClearance, f32 holdHeight);
    void setupCeilingCheckGrab();
    void setCollisionPartsFilter(const al::CollisionPartsFilterBase* filter);

private:
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
