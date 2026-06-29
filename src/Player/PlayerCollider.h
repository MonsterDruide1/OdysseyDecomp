#pragma once

#include <container/seadPtrArray.h>
#include <math/seadMatrix.h>
#include <prim/seadBitFlag.h>

#include "Library/Collision/IUseCollision.h"
#include "Library/HostIO/HioNode.h"

namespace al {
class CollisionPartsFilterBase;
struct HitInfo;
class SpherePoseInterpolator;
}  // namespace al
class CollidedShapeResult;
class CollisionShapeKeeper;
class CollisionMultiShape;

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

    al::HitInfo* getGroundHitInfo() const { return _68; }

    f32 getGroundDistance() const { return _70; }

    al::HitInfo* getWallHitInfo() const { return _78; }

    f32 getWallDistance() const { return _7c; }

    al::HitInfo* getCeilingHitInfo() const { return _88; }

    f32 getCeilingDistance() const { return _8c; }

    const sead::Vector3f& getCollidedFixReaction() const { return mCollidedFixReaction; }

    bool isPressedGround() const { return _a0; }

    bool isPressedWall() const { return _a1; }

    const sead::Vector3f& getPressCollisionNormal() const { return mCollisionHitNormal; }

    const sead::Vector3f& getPressCollisionHitPos() const { return mCollisionHitPos; }

    s32 getTimeInAir() const { return mTimeInAir; }

    CollisionShapeKeeper* getCollisionShapeKeeper() const { return mCollisionShapeKeeper; }

    f32 getCollisionShapeScale() const { return mCollisionShapeScale; }

    const sead::PtrArray<al::HitInfo>& getGroundHitInfoArray() const { return _128[0]; }

    const sead::PtrArray<al::HitInfo>& getWallHitInfoArray() const { return _128[1]; }

    const sead::Vector3f& getCollidedGroundPos() const { return _198; }

    f32 get_70() const { return _70; }

    const sead::Vector3f& getCollidedGroundNormal() const { return mCollidedGroundNormal; }

    void set1b0(f32 value) { _1b0 = value; }

private:
    al::CollisionDirector* mCollisionDirector;
    const sead::Matrix34f* mMtxPtr;
    const sead::Vector3f* mTransPtr;
    const sead::Vector3f* mGravityPtr;
    sead::Vector3f mTrans;
    f32 mSize;
    sead::Matrix34f mMtx;
    al::HitInfo* _68;
    f32 _70;
    al::HitInfo* _78;
    f32 _7c;
    al::HitInfo* _88;
    f32 _8c;
    sead::Vector3f mCollidedFixReaction;
    bool _a0;
    bool _a1;
    sead::Vector3f mCollisionHitNormal;
    sead::Vector3f mCollisionHitPos;
    s32 mTimeInAir;
    sead::Matrix34f mCollidePosMtx;
    CollisionShapeKeeper* mCollisionShapeKeeper;
    f32 mCollisionShapeScale;
    CollisionMultiShape* mCollisionMultiShape;
    s32 _108;
    bool mIsInFastMoveCollisionArea;
    bool mIsValidGroundSupport;
    bool mIsDuringRecovery;
    sead::Vector3f mCutCollideAffectDir;
    s32 mWallBorderCheckType;
    const al::CollisionPartsFilterBase* mCollisionPartsFilter;
    sead::PtrArray<al::HitInfo> _128[3];
    al::HitInfo* _158;
    u32 _160;
    s32 _164;
    sead::PtrArray<al::HitInfo> _168;
    s32 _178;
    f32* _180;
    s32 _188;
    f32* _190;
    sead::Vector3f _198;
    sead::Vector3f mCollidedGroundNormal;
    f32 _1b0;
};

static_assert(sizeof(PlayerCollider) == 0x1b8);
