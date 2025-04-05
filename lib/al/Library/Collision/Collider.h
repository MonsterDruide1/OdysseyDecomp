#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/Collision/IUseCollision.h"
#include "Library/HostIO/HioNode.h"

namespace al {
struct SphereInterpolator;
struct SphereHitInfo;
class CollisionPartsFilterBase;
class TriangleFilterBase;
class CollisionDirector;
class Triangle;

bool isWallPolygon(const sead::Vector3f&, const sead::Vector3f&);
bool isFloorPolygon(const sead::Vector3f&, const sead::Vector3f&);
bool isCeilingPolygon(const sead::Vector3f&, const sead::Vector3f&);

class Collider : public HioNode, public IUseCollision {
public:
    Collider(CollisionDirector*, const sead::Matrix34f*, const sead::Vector3f*,
             const sead::Vector3f*, f32, f32, u32);
    void calcCheckPos(sead::Vector3f*);
    void calcMovePowerByContact(sead::Vector3f*, const sead::Vector3f&);
    void clear();
    void clearContactPlane();
    void clearStoredPlaneNum();
    sead::Vector3f collide(const sead::Vector3f&);
    void findCollidePos(s32*, SphereInterpolator*, SphereHitInfo*, u32);
    Triangle* getPlane(s32);
    void getRecentOnGroundNormal(u32);
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

    s32 get_48() const { return _48; }

    s32 get_4c() const { return _4c; }

private:
    void* unknown[0x8];
    s32 _48;
    s32 _4c;
    void* unknown2[0x47];
};

static_assert(sizeof(Collider) == 0x288);
}  // namespace al
