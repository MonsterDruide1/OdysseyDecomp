#pragma once

#include <math/seadMatrix.h>
#include <prim/seadBitFlag.h>

#include "Library/Collision/IUseCollision.h"
#include "Library/HostIO/HioNode.h"

namespace al {
class CollisionPartsFilterBase;
class HitInfo;
class SpherePoseInterpolator;
}  // namespace al
class CollidedShapeResult;
class CollisionShapeKeeper;

class PlayerCollider : public al::HioNode, public al::IUseCollision {
public:
    PlayerCollider(al::CollisionDirector*, const sead::Matrix34f*, const sead::Vector3f*,
                   const sead::Vector3f*, bool);
    void onInvalidate();
    void setCollisionShapeKeeper(CollisionShapeKeeper*);
    void calcBoundingRadius(f32*) const;
    void setCollisionShapeScale(f32);
    void onCutCollideAffectDir(const sead::Vector3f&);
    void offCutCollideAffectDir();
    void clear();
    void calcCheckPos(sead::Vector3f*) const;
    void resetPose(const sead::Quatf&);
    sead::Vector3f collide(const sead::Vector3f&);
    bool calcMovePowerByContact(sead::Vector3f*, const sead::Vector3f&);
    void moveCollide(sead::Vector3f*, f32*, sead::Quatf*, const sead::Vector3f&, f32,
                     const sead::Quatf&, const sead::Vector3f&, f32, bool);
    bool findCollidePos(al::SpherePoseInterpolator*);
    void calcResultVec(sead::Vector3f*, sead::Vector3f*, const sead::Vector3f&);
    void calcGroundArrowAverage(bool*, sead::Vector3f*, bool*, sead::Vector3f*,
                                const CollisionShapeKeeper*);
    void calcResultVecArrow(sead::BitFlag32*, sead::Vector3f*, sead::Vector3f*, sead::Vector3f*,
                            sead::Vector3f*, const CollidedShapeResult*);
    void calcResultVecSphere(sead::BitFlag32*, sead::Vector3f*, sead::Vector3f*, sead::Vector3f*,
                             sead::Vector3f*, const CollidedShapeResult*);
    void calcResultVecDisk(sead::BitFlag32*, sead::Vector3f*, sead::Vector3f*, sead::Vector3f*,
                           sead::Vector3f*, const CollidedShapeResult*);
    void collectHitInfoArray(const al::HitInfo&, s32);
    bool isNeedWallBorderCheck(const al::HitInfo&) const;
    void setWallBorderCheckTypeNone();
    void setWallBorderCheckTypeNoFace();
    void setWallBorderCheckTypeAll();
    void setCollisionPartsFilter(const al::CollisionPartsFilterBase*);
    void calcBoundingCenter(sead::Vector3f*) const;
    void validateCorrectMovePartsCheck();
    al::CollisionDirector* getCollisionDirector() const override;

    f32 get_70() const { return _70; }

    const sead::Vector3f& getCollidedGroundNormal() const { return mCollidedGroundNormal; }

private:
    void* filler[13];
    f32 _70;
    void* filler2[37];
    s32 _1a0;
    sead::Vector3f mCollidedGroundNormal;
    void* filler3;
};

static_assert(sizeof(PlayerCollider) == 0x1B8);
