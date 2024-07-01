#pragma once

#include <math/seadVector.h>

#include "Library/HostIO/HioNode.h"
#include "Player/IUsePlayerCollision.h"
#include "Player/IUsePlayerCeilingCheck.h"

namespace al {
class LiveActor;
class CollisionPartsFilterBase;
class HitSensor;
}  // namespace al
class PlayerConst;
class PlayerCeilingCheck;
class CollisionShapeKeeper;

class IUsePlayerHeightCheck {
public:
    virtual bool isAboveGround() const = 0;
    virtual f32 getGroundHeight() const = 0;
    virtual f32 getShadowDropHeight() const = 0;
};
class IUsePlayerFallDistanceCheck {
    virtual f32 getFallDistance() const = 0;
};

class PlayerColliderHakoniwa : public al::HioNode,
                               public IUsePlayerCollision,
                               public IUsePlayerCeilingCheck,
                               public IUsePlayerHeightCheck,
                               public IUsePlayerFallDistanceCheck {
public:
    PlayerColliderHakoniwa(al::LiveActor*, const PlayerConst*);

    PlayerCollider* getPlayerCollider() const override;
    bool isEnableStandUp() const override;
    bool isEnableHoldUp() const override;
    bool isPressedCeil() const override;
    f32 getSafetyCeilSpace() const override;
    f32 getCeilCheckHeight() const override;
    bool isAboveGround() const override;
    f32 getGroundHeight() const override;
    f32 getShadowDropHeight() const override;
    f32 getFallDistance() const override;

    void init();
    sead::Vector3f updateCollider(const sead::Vector3f&);
    void updateHeightCheck(const sead::Vector3f&, const sead::Vector3f&, bool);
    void updateCeilingCheck(const sead::Vector3f&, const sead::Vector3f&, float, float);
    void updateFallDistanceCheck(const sead::Vector3f&, const sead::Vector3f&,
                                 const sead::Vector3f&, float);
    void changeCollisionNormal();
    void changeCollisionMini();
    void changeCollisionSwim();
    void changeCollisionWallGrab(const sead::Vector3f&);
    void changeCollisionGrabCeil(const sead::Vector3f&);
    void changeCollisionPoleClimb(const sead::Vector3f&);
    void changeCollision2DNormal();
    void changeCollision2DMini();
    void changeCollisionHack(const al::CollisionPartsFilterBase*);
    void changeCollisionRecovery(bool);
    void changeCollisionAbyss(const sead::Vector3f&);
    void prepareCollisionEndHack();
    void prepareCollisionEndRecovery();
    f32 getColliderRadius() const;
    f32 getColliderDiskHalfHeight() const;
    void calcBoundingCenter(sead::Vector3f*) const;
    void calcSeparateCapLocalOffset(sead::Vector3f*) const;
    void validateGroundSupport();
    void invalidateGroundSupport();

public:
    al::LiveActor* mPlayer;
    const PlayerConst* mPlayerConst;
    PlayerCollider* mPlayerCollider = nullptr;
    PlayerCeilingCheck* mPlayerCeilingCheck = nullptr;
    bool mIsAboveGround = false;
    f32 mGroundHeight = 0.0f;
    f32 mShadowDropHeight = 0.0f;
    sead::Vector3f mGroundNormal = {0.0f, 0.0f, 0.0f};
    sead::Vector3f mFallMaxSpeedStart = {0.0f, 0.0f, 0.0f};
    f32 mFallDistance = 0.0f;
    u32 mTimeSinceLastTouchGround = 0;
    al::HitSensor* mCollidedGroundSensor = nullptr;
    CollisionShapeKeeper* mShapeKeeperNormal = nullptr;
    CollisionShapeKeeper* mShapeKeeperMini = nullptr;
    CollisionShapeKeeper* mShapeKeeperSwim = nullptr;
    CollisionShapeKeeper* mShapeKeeperGrabCeil = nullptr;
    CollisionShapeKeeper* mShapeKeeperWallGrab = nullptr;
    CollisionShapeKeeper* mShapeKeeperPoleClimb = nullptr;
    CollisionShapeKeeper* mShapeKeeper2DNormal = nullptr;
    CollisionShapeKeeper* mShapeKeeper2DMini = nullptr;
    al::CollisionPartsFilterBase* mFilter2DOnly;
};
