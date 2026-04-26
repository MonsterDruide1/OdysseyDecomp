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

bool isWallPolygon(const sead::Vector3f&, const sead::Vector3f&);
bool isFloorPolygon(const sead::Vector3f&, const sead::Vector3f&);
bool isCeilingPolygon(const sead::Vector3f&, const sead::Vector3f&);

class Collider : public HioNode, public IUseCollision {
public:
    Collider(CollisionDirector*, const sead::Matrix34f*, const sead::Vector3f*,
             const sead::Vector3f*, f32, f32, u32);
    void calcCheckPos(sead::Vector3f*) const;
    void calcMovePowerByContact(sead::Vector3f*, const sead::Vector3f&);
    void clear();
    void clearContactPlane();
    void clearStoredPlaneNum();
    sead::Vector3f collide(const sead::Vector3f&);
    void findCollidePos(s32*, SphereInterpolator*, SphereHitInfo*, u32);
    Triangle* getPlane(s32) const;
    const sead::Vector3f& getRecentOnGroundNormal(u32) const;
    void obtainMomentFixReaction(SphereHitInfo*, sead::Vector3f*, sead::Vector3f*, bool, u32);
    void onInvalidate();
    void preCollide(SphereInterpolator*, sead::Vector3f*, f32*, const sead::Vector3f&,
                    SphereHitInfo*, u32);
    void setCollisionPartsFilter(const CollisionPartsFilterBase*);
    void setTriangleFilter(const TriangleFilterBase*);
    void storeContactPlane(SphereHitInfo*);
    void storeCurrentHitInfo(SphereHitInfo*, u32);
    void updateRecentOnGroundInfo();

    CollisionDirector* getCollisionDirector() const override;

    const sead::Vector3f* getActorGravityPtr() const { return mActorGravityPtr; }

    f32 getRadius() { return mRadius; };

    void setRadius(f32 radius) { mRadius = radius; };

    f32 getOffsetY() { return mOffsetY; };

    void setOffsetY(f32 offsetY) { mOffsetY = offsetY; };

    s32 getPlaneNum() const { return mPlaneNum; }

    u32 getStoredPlaneNum() const { return mStoredPlaneNum; }

    const sead::Vector3f& getFixReaction() const { return mFixReaction; }

    const HitInfo& getFloorHit() const { return mFloorHit; }

    f32 get_110() const { return _110; }

    const HitInfo& getWallHit() const { return mWallHit; }

    f32 get_1b8() const { return _1b8; }

    const HitInfo& getCeilingHit() const { return mCeilingHit; }

    f32 get_260() const { return _260; }

    u32 getNoGroundCounter() const { return mNoGroundCounter; }

    void setReactMovePower(bool isEnabled) {
        mFlag &= ~1;
        mFlag |= isEnabled;
    }

    void validateRobustCheck() { mFlag |= 2; }

    void invalidateRobustCheck() { mFlag &= ~2; }

    bool isCollidedWallFace() { return mFlag >> 5 & 1; }

private:
    CollisionDirector* mCollisionDirector;
    const TriangleFilterBase* mTriangleFilter;
    const CollisionPartsFilterBase* mCollisionPartsFilter;
    const sead::Matrix34f* mActorBaseMtxPtr;
    const sead::Vector3f* mActorTransPtr;
    const sead::Vector3f* mActorGravityPtr;
    f32 mRadius;
    f32 mOffsetY;
    sead::Vector3f* _40;
    u32 mPlaneNum;
    u32 mStoredPlaneNum;
    HitInfo* mPlanes;
    sead::Vector3f mFixReaction;
    sead::Vector3f _64;
    HitInfo mFloorHit;
    f32 _110;
    char filler3[0x4];
    HitInfo mWallHit;
    f32 _1b8;
    char filler4[0x4];
    HitInfo mCeilingHit;
    f32 _260;
    u32 mNoGroundCounter;
    sead::Vector3f mRecentOnGroundNormal;
    char _274;
    char mFlag;
    sead::Vector3f mCurrentTrans;
    f32 mCurrentRadius;
};

static_assert(sizeof(Collider) == 0x288);
}  // namespace al
