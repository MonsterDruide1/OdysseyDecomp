#include "Player/PlayerColliderHakoniwa.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/LiveActor/LiveActorUtil.h"
#include "Library/Math/MathAngleUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/stuff.h"
#include "Player/CollisionShapeKeeper.h"
#include "Player/PlayerCeilingCheck.h"
#include "Player/PlayerCollider.h"
#include "Stuff.h"
#include "math/seadVectorFwd.h"

PlayerColliderHakoniwa::PlayerColliderHakoniwa(al::LiveActor* player, const PlayerConst* pConst) : mPlayer(player), mPlayerConst(pConst) {
    mFilter2DOnly = rs::createCollisionPartsFilter2DOnly();
}

PlayerCollider* PlayerColliderHakoniwa::getPlayerCollider() const {
    return mPlayerCollider;
}
bool PlayerColliderHakoniwa::isEnableStandUp() const {
    return mPlayerCeilingCheck->mIsEnableStandUp;
}
bool PlayerColliderHakoniwa::isEnableHoldUp() const {
    return mPlayerCeilingCheck->mIsEnableHoldUp;
}
bool PlayerColliderHakoniwa::isPressedCeil() const {
    return mPlayerCeilingCheck->mIsPressedCeil;
}
f32 PlayerColliderHakoniwa::getSafetyCeilSpace() const {
    return mPlayerCeilingCheck->mSafetyCeilSpace;
}
f32 PlayerColliderHakoniwa::getCeilCheckHeight() const {
    return mPlayerCeilingCheck->mCeilCheckHeight;
}
bool PlayerColliderHakoniwa::isAboveGround() const {
    return mIsAboveGround;
}
f32 PlayerColliderHakoniwa::getGroundHeight() const {
    return mGroundHeight;
}
f32 PlayerColliderHakoniwa::getShadowDropHeight() const {
    return mShadowDropHeight;
}
f32 PlayerColliderHakoniwa::getFallDistance() const {
    return mFallDistance;
}

void helperFunc(CollisionShapeKeeper* keeper, const sead::Vector3f& pos);
void helperFunc2(CollisionShapeKeeper* keeper);
void PlayerColliderHakoniwa::init() {
    mShapeKeeperNormal = new CollisionShapeKeeper(5, 64, 32);
    mShapeKeeperNormal->unk3 = 25.0f;
    mShapeKeeperNormal->unk4 = 25.0f;
    helperFunc(mShapeKeeperNormal, sead::Vector3f::zero);
    mShapeKeeperNormal->createShapeSphereSupportGround("Body", 55.0f, sead::Vector3f::ey*65.0f, sead::Vector3f::ey, 14.5f);
    mShapeKeeperNormal->createShapeSphere("Head", 55.0f, sead::Vector3f::ey*105.0f);
    mShapeKeeperNormal->updateShape();

    mShapeKeeperMini = new CollisionShapeKeeper(5, 64, 32);
    helperFunc(mShapeKeeperMini, sead::Vector3f::zero);
    mShapeKeeperMini->createShapeSphereSupportGround("Body", 55.0f, sead::Vector3f::ey*55.0f, sead::Vector3f::ey, 14.5f);
    mShapeKeeperMini->unk4 = 25.0f;
    mShapeKeeperMini->updateShape();

    mShapeKeeperSwim = new CollisionShapeKeeper(5, 64, 32);
    mShapeKeeperSwim->unk3 = 25.0f;
    helperFunc(mShapeKeeperSwim, sead::Vector3f::zero);
    mShapeKeeperSwim->createShapeSphereIgnoreGround("Body", 55.0f, sead::Vector3f::ey*55.0f);
    mShapeKeeperSwim->createShapeSphere("Head", 55.0f, sead::Vector3f::ey*105.0f);
    mShapeKeeperSwim->updateShape();

    mShapeKeeperGrabCeil = new CollisionShapeKeeper(5, 64, 32);
    helperFunc(mShapeKeeperGrabCeil, sead::Vector3f::ey*-140.0f);
    mShapeKeeperGrabCeil->createShapeSphereSupportGround("Body", 55.0f, sead::Vector3f::ey*-75.0f, sead::Vector3f::ey, 30.0f);
    mShapeKeeperGrabCeil->createShapeSphere("Head", 55.0f, sead::Vector3f::ey*-35.0f);
    mShapeKeeperGrabCeil->updateShape();

    mShapeKeeperWallGrab = new CollisionShapeKeeper(5, 64, 32);
    helperFunc(mShapeKeeperWallGrab, sead::Vector3f::ez*-50.0f - sead::Vector3f::ey*140.0f);
    mShapeKeeperWallGrab->createShapeSphereIgnoreGround("Body", 55.0f, sead::Vector3f::ez*-50.0f - sead::Vector3f::ey*75.0f);
    mShapeKeeperWallGrab->createShapeSphereIgnoreGround("Head", 55.0f, sead::Vector3f::ez*-50.0f - sead::Vector3f::ey*35.0f);
    mShapeKeeperWallGrab->updateShape();

    mShapeKeeperPoleClimb = new CollisionShapeKeeper(5, 64, 32);
    helperFunc(mShapeKeeperPoleClimb, sead::Vector3f::ez*-50.0f - sead::Vector3f::ey*35.0f);
    mShapeKeeperPoleClimb->createShapeSphereIgnoreGround("Body", 55.0f, sead::Vector3f::ey*30.0f - sead::Vector3f::ez*50.0f);
    mShapeKeeperPoleClimb->createShapeSphereIgnoreGround("Head", 55.0f, sead::Vector3f::ey*60.0f - sead::Vector3f::ez*50.0f);
    mShapeKeeperPoleClimb->updateShape();

    mShapeKeeper2DNormal = new CollisionShapeKeeper(5, 64, 32);
    helperFunc2(mShapeKeeper2DNormal);
    mShapeKeeper2DNormal->createShapeDiskSupportGround("Body", 55.0f, sead::Vector3f::ey*55.0f, sead::Vector3f::ex, 5.0f, sead::Vector3f::ey, 14.5f);
    mShapeKeeper2DNormal->createShapeDisk("Head", 55.0f, sead::Vector3f::ey*130.0f, sead::Vector3f::ex, 5.0f);
    mShapeKeeper2DNormal->updateShape();

    mShapeKeeper2DMini = new CollisionShapeKeeper(5, 64, 32);
    helperFunc2(mShapeKeeper2DMini);
    mShapeKeeper2DMini->createShapeDiskSupportGround("Body", 55.0f, sead::Vector3f::ey*55.0f, sead::Vector3f::ex, 5.0f, sead::Vector3f::ey, 14.5f);
    mShapeKeeper2DMini->createShapeDisk("Head", 55.0f, sead::Vector3f::ey*55.0f, sead::Vector3f::ex, 5.0f);
    mShapeKeeper2DMini->updateShape();

    mPlayerCollider = new PlayerCollider(mPlayer->getCollisionDirector(), mPlayer->getBaseMtx(), al::getTransPtr(mPlayer), al::getGravityPtr(mPlayer), false);
    mPlayerCollider->validateCorrectMovePartsCheck();

    mPlayerCeilingCheck = new PlayerCeilingCheck(mPlayer->getCollisionDirector());
}

sead::Vector3f PlayerColliderHakoniwa::updateCollider(const sead::Vector3f& velocity) {
    mPlayerCollider->mIsInFastMoveCollisionArea = al::isInAreaObj(mPlayer, "FastMoveCollisionArea", al::getTrans(mPlayer));
    sead::Vector3f collisionResult = mPlayerCollider->collide(velocity);
    if(rs::isCollidedGround(this)) {
        mCollidedGroundSensor = rs::tryGetCollidedGroundSensor(this);
        mTimeSinceLastTouchGround = 0;
    } else {
        mTimeSinceLastTouchGround = al::converge(mTimeSinceLastTouchGround, 10000, 1);
    }
    return collisionResult;
}

void PlayerColliderHakoniwa::updateHeightCheck(const sead::Vector3f& trans, const sead::Vector3f& up, bool someFlag) {
    if(rs::isCollidedGround(this) && someFlag) {
        mIsAboveGround = true;
        mGroundHeight = 0.0f;
        mShadowDropHeight = 1700.0f;
        rs::calcGroundHeight(&mShadowDropHeight, &mGroundNormal, mPlayerCollider, trans, up, 30.0f, 1700.0f);
        mGroundNormal = rs::getCollidedGroundNormal(this);
    } else {
        mGroundHeight = 1700.0f;
        mIsAboveGround = rs::calcGroundHeight(&mGroundHeight, &mGroundNormal, mPlayerCollider, trans, up, 30.0f, 1700.0f);
        mShadowDropHeight = mGroundHeight;
    }
}

void PlayerColliderHakoniwa::updateCeilingCheck(const sead::Vector3f& trans, const sead::Vector3f& up, float unk1, float unk2) {
    mPlayerCeilingCheck->update(trans, up, mPlayerConst->getTall(), mPlayerConst->getCollisionRadiusSquat(), unk1, unk2);
}

void PlayerColliderHakoniwa::updateFallDistanceCheck(const sead::Vector3f& trans, const sead::Vector3f& velocity, const sead::Vector3f& gravity, float fallSpeedMax) {
    if(rs::isOnGround(mPlayer, this) && !rs::isJustLand(this)) {
        mFallMaxSpeedStart = {0.0f, 0.0f, 0.0f};
        mFallDistance = 0.0f;
        return;
    }
    if(velocity.dot(gravity) < fallSpeedMax) {
        mFallMaxSpeedStart = {0.0f, 0.0f, 0.0f};
        mFallDistance = 0.0f;
        return;
    }

    if(al::isNearZero(mFallMaxSpeedStart, 0.001f)) {
        mFallMaxSpeedStart = trans;
    }
    mFallDistance = sead::Mathf::clampMin((trans - mFallMaxSpeedStart).dot(gravity), 0.0f);
}

void PlayerColliderHakoniwa::changeCollisionNormal() {
    mPlayerCollider->setCollisionShapeKeeper(mShapeKeeperNormal);
    mPlayerCollider->setCollisionPartsFilter(nullptr);
    mPlayerCollider->mIsDuringRecovery = false;
    mPlayerCollider->offCutCollideAffectDir();
    mPlayerCollider->setWallBorderCheckTypeNoFace();
    mPlayerCeilingCheck->setupCeilingCheckNormal();
    mPlayerCeilingCheck->setCollisionPartsFilter(nullptr);
}
//void PlayerColliderHakoniwa::changeCollisionMini();
//void PlayerColliderHakoniwa::changeCollisionSwim();
//void PlayerColliderHakoniwa::changeCollisionWallGrab(const sead::Vector3f&);
//void PlayerColliderHakoniwa::changeCollisionGrabCeil(const sead::Vector3f&);
//void PlayerColliderHakoniwa::changeCollisionPoleClimb(const sead::Vector3f&);
//void PlayerColliderHakoniwa::changeCollision2DNormal();
//void PlayerColliderHakoniwa::changeCollision2DMini();
//void PlayerColliderHakoniwa::changeCollisionHack(const al::CollisionPartsFilterBase*);
//void PlayerColliderHakoniwa::changeCollisionRecovery(bool);
//void PlayerColliderHakoniwa::changeCollisionAbyss(const sead::Vector3f&);
//void PlayerColliderHakoniwa::prepareCollisionEndHack();
//void PlayerColliderHakoniwa::prepareCollisionEndRecovery();
//f32 PlayerColliderHakoniwa::getColliderRadius() const;
//f32 PlayerColliderHakoniwa::getColliderDiskHalfHeight() const;
//void PlayerColliderHakoniwa::calcBoundingCenter(sead::Vector3f*) const;

void PlayerColliderHakoniwa::calcSeparateCapLocalOffset(sead::Vector3f* capOffset) const {
    if(mPlayerCollider->mCollisionShapeKeeper == mShapeKeeperPoleClimb)
        *capOffset = sead::Vector3f::ez * -50.0f;
    else if(mPlayerCollider->mCollisionShapeKeeper == mShapeKeeperWallGrab)
        *capOffset = sead::Vector3f::ez * -50.0f - sead::Vector3f::ey * 140.0f;
    else if(mPlayerCollider->mCollisionShapeKeeper == mShapeKeeperGrabCeil)
        *capOffset = sead::Vector3f::ey * -115.0f;
}

void PlayerColliderHakoniwa::validateGroundSupport() {
    mPlayerCollider->setIsValidGroundSupport(true);
}

void PlayerColliderHakoniwa::invalidateGroundSupport() {
    mPlayerCollider->setIsValidGroundSupport(false);
}



const char* legNames[3] = {"LegFront", "LegLeft", "LegRight"};

void helperFunc(CollisionShapeKeeper* keeper, const sead::Vector3f& pos) {
  float v2; // s14
  float v3; // s15
  float v4; // s8
  float v8; // s10
  float v9; // s0
  const char *v10; // x1
  float v11; // s2
  float v12; // s0
  float v13; // s5
  float v14; // s3
  float v15; // s6
  float v16; // s4
  float v17; // s7
  float v18; // s20
  float v19; // s3
  float y; // s16
  float v21; // s1
  float v22; // s2
  sead::Vector3f a4; // [xsp+10h] [xbp-90h] BYREF
  sead::Vector3f a3; // [xsp+20h] [xbp-80h] BYREF
  float v26; // [xsp+78h] [xbp-28h]
  float v27; // [xsp+7Ch] [xbp-24h]

  v2 = 0.0 * 30.0;
  v3 = 0.0 * 30.0;
  v4 = 1.0 * 30.0;
  v27 = 0.0 * -40.0;
  v26 = 1.0 * -40.0;
  for(int v7=0; v7!=3; v7++)
  {
    v8 = sead::Mathf::cos(sead::Mathf::deg2rad(v7 * 120.0) * 0.5);
    v9 = sead::Mathf::sin(sead::Mathf::deg2rad(v7 * 120.0) * 0.5);
    v10 = legNames[v7];
    v11 = v9 * 1.0;
    v12 = v9 * 0.0;
    v13 = (float)(v3 * v8) + (float)((float)(v2 * v12) - (float)(v4 * v12));
    v14 = (float)(v2 * v8) + (float)((float)(v4 * v11) - (float)(v3 * v12));
    v15 = (float)(v4 * v8) + (float)((float)(v3 * v12) - (float)(v2 * v11));
    v16 = (float)((float)-(float)(v2 * v12) - (float)(v3 * v11)) - (float)(v4 * v12);
    v17 = v8 * v14;
    v18 = v12 * v14;
    v19 = (float)(v12 * v13) - (float)(v11 * v14);
    y = pos.y;
    v21 = (float)((float)(v18 + (float)(v8 * v13)) - (float)(v12 * v15)) - (float)(v11 * v16);
    v22 = (float)((float)((float)((float)(v11 * v15) + (float)(v17 - (float)(v12 * v13))) - (float)(v12 * v16)) + pos.x)
        - v27;
    a3.z = (float)((float)((float)((float)(v8 * v15) + v19) - (float)(v12 * v16)) + pos.z) - (float)(0.0 * -40.0);
    a4.x = (float)(0.0 * -40.0) - (float)(0.0 * 20.0);
    a4.y = (float)(1.0 * -40.0) - (float)(1.0 * 20.0);
    a3.x = v22;
    a3.y = (float)(v21 + y) - v26;
    a4.z = a4.x;
    keeper->createShapeArrow(v10, a3, a4, 20.0, v7);
  }
}

void helperFunc2(CollisionShapeKeeper* keeper) {
  sead::Vector3f v3; // [xsp+0h] [xbp-60h] BYREF
  sead::Vector3f a3; // [xsp+10h] [xbp-50h] BYREF
  sead::Vector3f a4; // [xsp+20h] [xbp-40h] BYREF

  a4.y = (float)(1.0 * -40.0) - (float)(1.0 * 20.0);
  a4.x = (float)(0.0 * -40.0) - (float)(0.0 * 20.0);
  a4.z = a4.x;
  a3.x = 0.0 - (float)(0.0 * -40.0);
  a3.y = 0.0 - (float)(1.0 * -40.0);
  v3.x = (float)(0.0 * 40.0) + a3.x;
  v3.y = (float)(0.0 * 40.0) + a3.y;
  v3.z = (float)(1.0 * 40.0) + a3.x;
  a3.z = a3.x;
  keeper->createShapeArrow("LegLeft", v3, a4, 20.0, 0);
  keeper->createShapeArrow("LegCenter", a3, a4, 20.0, 1);
  v3.x = a3.x - (float)(0.0 * 40.0);
  v3.y = a3.y - (float)(0.0 * 40.0);
  v3.z = a3.z - (float)(1.0 * 40.0);
  keeper->createShapeArrow("LegRight", v3, a4, 20.0, 2);
}
