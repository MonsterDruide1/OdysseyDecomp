#pragma once

#include <container/seadPtrArray.h>
#include <math/seadMatrix.h>

#include "Library/Collision/IUseCollision.h"
#include "Library/Collision/KCollisionServer.h"
#include "Player/CollidedShapeResult.h"
#include "Library/HostIO/HioNode.h"
#include "prim/seadBitFlag.h"

namespace al {
class HitInfo;
class CollisionPartsFilterBase;
}
class CollisionShapeKeeper;
class CollisionMultiShape;

class PlayerCollider : public al::HioNode, public al::IUseCollision
{
public:
    PlayerCollider(al::CollisionDirector *,sead::Matrix34f const*,sead::Vector3f const*,sead::Vector3f const*,bool);

    void onInvalidate();
    void setCollisionShapeKeeper(CollisionShapeKeeper *);
    void calcBoundingRadius(float *) const;
    void setCollisionShapeScale(float);
    void onCutCollideAffectDir(sead::Vector3f const&);
    void offCutCollideAffectDir();
    void clear();
    void calcCheckPos(sead::Vector3f *) const;
    void resetPose(sead::Quatf const&);
    sead::Vector3f collide(sead::Vector3f const&);
    bool calcMovePowerByContact(sead::Vector3f *,sead::Vector3f const&);
    void moveCollide(sead::Vector3f *,float *,sead::Quatf *,sead::Vector3f const&,float,sead::Quatf const&,sead::Vector3f const&,float,bool);
    bool findCollidePos(al::SpherePoseInterpolator *);
    void calcResultVec(sead::Vector3f *,sead::Vector3f *,sead::Vector3f const&);
    void calcGroundArrowAverage(bool *,sead::Vector3f *,bool *,sead::Vector3f *,CollisionShapeKeeper const*);
    void calcResultVecArrow(sead::BitFlag<u32> *,sead::Vector3f *,sead::Vector3f *,sead::Vector3f *,sead::Vector3f *,CollidedShapeResult const*);
    void calcResultVecSphere(sead::BitFlag<u32> *,sead::Vector3f *,sead::Vector3f *,sead::Vector3f *,sead::Vector3f *,CollidedShapeResult const*);
    void calcResultVecDisk(sead::BitFlag<u32> *,sead::Vector3f *,sead::Vector3f *,sead::Vector3f *,sead::Vector3f *,CollidedShapeResult const*);
    void collectHitInfoArray(al::HitInfo const&,int);
    bool isNeedWallBorderCheck(al::HitInfo const&) const;
    void setWallBorderCheckTypeNone();
    void setWallBorderCheckTypeNoFace();
    void setWallBorderCheckTypeAll();
    void setCollisionPartsFilter(al::CollisionPartsFilterBase const*);
    void calcBoundingCenter(sead::Vector3f *) const;
    void validateCorrectMovePartsCheck() const;

    al::CollisionDirector* getCollisionDirector() const override;

    void setIsValidGroundSupport(bool isValidGroundSupport) { mIsValidGroundSupport = isValidGroundSupport; }
    

    
    ~PlayerCollider();
public:
    al::CollisionDirector *mCollisionDirector;
    const sead::Matrix34f *mMtxPtr;
    const sead::Vector3f *mTransPtr;
    const sead::Vector3f *mGravityPtr;
    sead::Vector3f mTrans;
    float mSize;
    sead::Matrix34f mMtx;
    al::HitInfo *info1;
    float val1;
    int pad1;
    al::HitInfo *info2;
    float val2;
    int pad2;
    al::HitInfo *info3;
    float val3;
    sead::Vector3f unk3;
    bool flag1;
    bool flag2;
    bool pads[2];
    sead::Vector3f mCollisionHitNormal;
    sead::Vector3f mCollisionHitPos;
    int mTimeInAir;
    sead::Matrix34f mCollidePosMtx;
    CollisionShapeKeeper *mCollisionShapeKeeper;
    float mCollisionShapeScale;
    int pad;
    CollisionMultiShape *mCollisionMultiShape;
    sead::BitFlag<u32> someBitField;
    bool mIsInFastMoveCollisionArea;
    bool mIsValidGroundSupport;
    bool mIsDuringRecovery;
    bool pad3[1];
    sead::Vector3f mCutCollideAffectDir;
    int mWallBorderCheckType;
    const al::CollisionPartsFilterBase *mCollisionPartsFilter;
    sead::PtrArray<al::HitInfo const> ptrArraysChangeMe[3];
    al::HitInfo *someHitInfos;
    unsigned int numHitInfosAbove;
    int pad5;
    sead::PtrArray<al::HitInfo> anotherPtrArray;
    int sizeOfArrayBelowIs3;
    int pad4;
    float *someThreeFloats;
    int sizeOfArrayBelowIs3_2;
    int pad4_2;
    float *anotherThreeFloats;
    sead::Vector3f mCollidedGroundPos;
    sead::Vector3f mCollidedGroundNormal;
    float unk11;
};
