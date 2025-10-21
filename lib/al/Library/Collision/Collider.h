#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/Collision/CollisionPartsTriangle.h"
#include "Library/Collision/IUseCollision.h"
#include "Library/HostIO/HioNode.h"

namespace al {
class SphereInterpolator;
struct SphereHitInfo;
class CollisionPartsFilterBase;
class TriangleFilterBase;
class CollisionDirector;
class Triangle;

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

    void setRadius(f32 radius) { mRadius = radius; };

    s32 get_48() const { return mPlaneNum; }

    s32 get_4c() const { return mStoredPlaneNum; }

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
