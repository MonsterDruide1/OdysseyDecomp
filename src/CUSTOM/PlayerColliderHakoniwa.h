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
    virtual bool isEnableStandUp() {WARN_UNIMPL;return true;}
};
class IUsePlayerHeightCheck {
    virtual void something() {CRASH}
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
    
    virtual ::PlayerCollider* getPlayerCollider() const { return (::PlayerCollider*)mPlayerCollider; }

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
