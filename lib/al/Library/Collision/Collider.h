#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/Collision/HitInfo.h"
#include "Library/Collision/IUseCollision.h"
#include "Library/HostIO/HioNode.h"

namespace al {
class CollisionDirector;
class CollisionPartsFilterBase;
struct HitInfo;
struct SphereHitInfo;
class SphereInterpolator;
class Triangle;
class TriangleFilterBase;

class Collider : public HioNode, public IUseCollision {
public:
    Collider(CollisionDirector* director, const sead::Matrix34f* mtxPtr,
             const sead::Vector3f* transPtr, const sead::Vector3f* gravityPtr, f32 radius,
             f32 offsetY, u32 planeNum);

    void clear();
    void setTriangleFilter(const TriangleFilterBase* triFilter);
    void setCollisionPartsFilter(const CollisionPartsFilterBase* partsFilter);

    void updateRecentOnGroundInfo();
    void clearContactPlane();
    void clearStoredPlaneNum();
    void onInvalidate();

    void calcCheckPos(sead::Vector3f* outPosition) const;
    const sead::Vector3f& getRecentOnGroundNormal(u32 coyoteTime) const;
    const Triangle& getPlane(s32 index) const;

    bool calcMovePowerByContact(sead::Vector3f* outMovePower, const sead::Vector3f& position);
    u32 storeCurrentHitInfo(SphereHitInfo* planes, u32 planeNum);
    void obtainMomentFixReaction(SphereHitInfo* planes, sead::Vector3f* outFixReaction,
                                 sead::Vector3f* outFixReactionNormal, bool isFirst,
                                 u32 startIndex);
    void storeContactPlane(SphereHitInfo* planes);

    sead::Vector3f collide(const sead::Vector3f& velocity);
    bool preCollide(SphereInterpolator* interp, sead::Vector3f* outPosition, f32* outRadius,
                    const sead::Vector3f& velocity, SphereHitInfo* planes, u32 planeNum);

    bool findCollidePos(s32* outStored, SphereInterpolator* interp, SphereHitInfo* planes,
                        u32 planeNum);

    CollisionDirector* getCollisionDirector() const override;

    const sead::Vector3f* getActorGravityPtr() const { return mActorGravityPtr; }

    f32 getRadius() { return mRadius; };

    void setRadius(f32 radius) { mRadius = radius; };

    f32 getOffsetY() { return mOffsetY; };

    void setOffsetY(f32 offsetY) { mOffsetY = offsetY; };

    s32 getPlaneNum() const { return mPlaneNum; }

    u32 getStoredPlaneNum() const { return mStoredPlaneNum; }

    const sead::Vector3f& getFixReaction() const { return mFixReaction; }

    const HitInfo& getFloorHit() const { return mGroundHit; }

    bool isCollidedGround() const { return mGroundDistance >= 0.0f; }

    const HitInfo& getWallHit() const { return mWallHit; }

    bool isCollidedWall() const { return mWallDistance >= 0.0f; }

    const HitInfo& getCeilingHit() const { return mCeilingHit; }

    bool isCollidedCeiling() const { return mCeilingDistance >= 0.0f; }

    u32 getNoGroundCounter() const { return mNoGroundCounter; }

    void setReactMovePower(bool isEnabled) { mIsReactMovePower = isEnabled; }

    void validateRobustCheck() { mIsCheckMovingReaction = true; }

    void invalidateRobustCheck() { mIsCheckMovingReaction = false; }

    bool isCollidedWallFace() { return mIsCollidedWallFace; }

private:
    sead::Vector3f getActorMtxSide() const { return mActorMtxPtr->getBase(0); }

    sead::Vector3f getActorMtxUp() const { return mActorMtxPtr->getBase(1); }

    sead::Vector3f getActorMtxFront() const { return mActorMtxPtr->getBase(2); }

    CollisionDirector* mDirector = nullptr;
    const TriangleFilterBase* mTriFilter = nullptr;
    const CollisionPartsFilterBase* mPartsFilter = nullptr;

    const sead::Matrix34f* mActorMtxPtr = nullptr;
    const sead::Vector3f* mActorTransPtr = nullptr;
    const sead::Vector3f* mActorGravityPtr = nullptr;

    f32 mRadius = 0.0f;
    f32 mOffsetY = 0.0f;
    sead::Vector3f* mCheckOffset = nullptr;

    u32 mPlaneNum = 0;
    u32 mStoredPlaneNum = 0;
    SphereHitInfo* mPlanes = nullptr;

    sead::Vector3f mFixReaction = {0.0f, 0.0f, 0.0f};
    sead::Vector3f mMovePower = {0.0f, 0.0f, 0.0f};

    HitInfo mGroundHit;
    f32 mGroundDistance = 0.0f;
    HitInfo mWallHit;
    f32 mWallDistance = 0.0f;
    HitInfo mCeilingHit;
    f32 mCeilingDistance = 0.0f;

    u32 mNoGroundCounter = 0;
    sead::Vector3f mRecentOnGroundNormal = {0.0f, 1.0f, 0.0f};

    bool mIsDisableGroundNormalCheck = false;  // Appears to always be false

    bool mIsReactMovePower : 1;
    bool mIsCheckMovingReaction : 1;
    bool mIsRotateCheckOffset : 1;
    bool mIsSkipStoredHitInfos : 1;
    bool mIsCollidedGround : 1;
    bool mIsCollidedWallFace : 1;
    bool mIsCollidedCeilingFace : 1;

    sead::Vector3f mCurrentTrans;
    f32 mCurrentRadius;
};

static_assert(sizeof(Collider) == 0x288);
}  // namespace al
