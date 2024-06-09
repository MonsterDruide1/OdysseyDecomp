#pragma once

#include <math/seadVector.h>

#include "Library/HostIO/HioNode.h"

#include "Player/IUsePlayerCollision.h"

namespace al {
class LiveActor;
class CollisionDirector;
class CollisionPartsFilterBase;
}
class PlayerCollider;
class PlayerConst;
class CollisionShapeKeeper;


class IUsePlayerCeilingCheck {
public:
    virtual bool isEnableStandUp() const {WARN_UNIMPL;return true;}
    virtual bool isEnableHoldUp() const {WARN_UNIMPL;return true;}
    virtual bool isPressedCeil() const {WARN_UNIMPL;return false;}
    virtual f32 getSafetyCeilSpace() const {CRASH}
    virtual f32 getCeilCheckHeight() const {CRASH}
};
class IUsePlayerHeightCheck {
public:
    virtual bool isAboveGround() const {WARN_UNIMPL;return true;}
    virtual f32 getGroundHeight() const {CRASH}
    virtual f32 getShadowDropHeight() const {WARN_UNIMPL;return getGroundHeight();}
};
class IUsePlayerFallDistanceCheck {
    virtual void something() {CRASH}
};


namespace game {

class PlayerColliderHakoniwa : public al::HioNode, public IUsePlayerCollision, public IUsePlayerCeilingCheck, public IUsePlayerHeightCheck, public IUsePlayerFallDistanceCheck {
public:
    PlayerColliderHakoniwa(al::LiveActor* actor, const PlayerConst* c);
    ~PlayerColliderHakoniwa();

    sead::Vector3f updateCollider(const sead::Vector3f&);
    void init();

    void calcSeparateCapLocalOffset(sead::Vector3f*);
    
    virtual ::PlayerCollider* getPlayerCollider() const { return (::PlayerCollider*)mPlayerCollider; }

    void updateHeightCheck(sead::Vector3<float> const&,sead::Vector3<float> const&,bool) { WARN_UNIMPL; }
    void updateCeilingCheck(sead::Vector3<float> const&,sead::Vector3<float> const&,float,float) { WARN_UNIMPL; }
    void updateFallDistanceCheck(sead::Vector3<float> const&,sead::Vector3<float> const&,sead::Vector3<float> const&,float);

public:
    al::LiveActor* mPlayer;
    const PlayerConst* mPlayerConst;
    PlayerCollider* mPlayerCollider;
    bool mIsAboveGround;
    float mGroundHeight;
    float mShadowDropHeight;
    sead::Vector3f mGroundNormal;
    sead::Vector3f unkFallDistance2;
    float mFallDistance;
    int mTimeSinceLastTouchGround;
    void *mCollidedGroundSensorUnknownType;
    CollisionShapeKeeper *mShapeKeeperNormal;
    CollisionShapeKeeper *mShapeKeeperMini;
    CollisionShapeKeeper *mShapeKeeperSwim;
    CollisionShapeKeeper *mShapeKeeperGrabCeil;
    CollisionShapeKeeper *mShapeKeeperWallGrab;
    CollisionShapeKeeper *mShapeKeeperPoleClimb;
    CollisionShapeKeeper *mShapeKeeper2DNormal;
    CollisionShapeKeeper *mShapeKeeper2DMini;
    al::CollisionPartsFilterBase *mFilter2DOnly;
};

}

class PlayerColliderHakoniwa : public game::PlayerColliderHakoniwa {
public:
    PlayerColliderHakoniwa(al::LiveActor* actor, const PlayerConst* c) : game::PlayerColliderHakoniwa(actor, c) {}
};
