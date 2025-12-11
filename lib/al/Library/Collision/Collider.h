#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/Collision/CollisionPartsTriangle.h"
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
    Collider(CollisionDirector*, const sead::Matrix34f*, const sead::Vector3f*,
             const sead::Vector3f*, f32, f32, u32);
    void calcCheckPos(sead::Vector3f*) const;
    bool calcMovePowerByContact(sead::Vector3f*, const sead::Vector3f&);
    void clear();
    void clearContactPlane();
    void clearStoredPlaneNum();
    sead::Vector3f collide(const sead::Vector3f&);
    bool findCollidePos(s32*, SphereInterpolator*, SphereHitInfo*, u32);
    const Triangle& getPlane(s32) const;
    const sead::Vector3f& getRecentOnGroundNormal(u32) const;
    void obtainMomentFixReaction(SphereHitInfo*, sead::Vector3f*, sead::Vector3f*, bool, u32);
    void onInvalidate();
    bool preCollide(SphereInterpolator*, sead::Vector3f*, f32*, const sead::Vector3f&,
                    SphereHitInfo*, u32);
    void setCollisionPartsFilter(const CollisionPartsFilterBase*);
    void setTriangleFilter(const TriangleFilterBase*);
    void storeContactPlane(SphereHitInfo*);
    u32 storeCurrentHitInfo(SphereHitInfo*, u32);
    void updateRecentOnGroundInfo();

    CollisionDirector* getCollisionDirector() const override;

    const sead::Vector3f* get_30() const { return mActorGravity; }

    f32 getRadius() { return mRadius; }

    void setRadius(f32 radius) { mRadius = radius; }

    f32 getOffsetY() { return mOffsetY; }

    void setOffsetY(f32 offsetY) { mOffsetY = offsetY; }

    s32 get_48() const { return mPlaneNum; }

    u32 getPlaneCount() const { return mStoredPlaneNum; }

    const sead::Vector3f& getFixReaction() const { return mFixReaction; }

    const HitInfo& getFloorHit() const { return mFloorHit; }

    f32 get_110() const { return mFloor_70; }

    const HitInfo& getWallHit() const { return mWallHit; }

    f32 get_1b8() const { return mWall_70; }

    const HitInfo& getCeilingHit() const { return mCeilingHit; }

    f32 get_260() const { return mCeiling_70; }

    u32 get_264() const { return mNoGroundCounter; }

    void setReactMovePower(bool isEnabled) {
        flags2 &= ~1;
        flags2 |= isEnabled;
    }

    void validateRobustCheck() { flags2 |= 2; }

    void invalidateRobustCheck() { flags2 &= ~2; }

    bool isCollidedWallFace() { return flags2 >> 5 & 1; }

private:
    CollisionDirector* mCollisionDirector;
    const TriangleFilterBase* mTriangleFilter = nullptr;
    const CollisionPartsFilterBase* mCollisionPartsFilter = nullptr;
    const sead::Matrix34f* mActorBaseMtx;
    const sead::Vector3f* mActorTrans;
    const sead::Vector3f* mActorGravity;
    f32 mRadius;
    f32 mOffsetY;
    sead::Vector3f* unknown = nullptr;
    u32 mPlaneNum;
    u32 mStoredPlaneNum = 0;
    SphereHitInfo* mPlanes = nullptr;
    sead::Vector3f mFixReaction = {0.0f, 0.0f, 0.0f};
    sead::Vector3f mMovePower = {0.0f, 0.0f, 0.0f};
    HitInfo mFloorHit;
    f32 mFloor_70 = 0.0f;
    HitInfo mWallHit;
    f32 mWall_70 = 0.0f;
    HitInfo mCeilingHit;
    f32 mCeiling_70 = 0.0f;
    s32 mNoGroundCounter = 0;
    sead::Vector3f mRecentOnGroundNormal = {0.0f, 1.0f, 0.0f};
    u8 flags1 = 0;
    u8 flags2;
    sead::Vector3f mCurrentTrans;
    f32 mCurrentRadius;
};

static_assert(sizeof(Collider) == 0x288);
}  // namespace al
