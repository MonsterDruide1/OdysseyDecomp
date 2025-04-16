#pragma once

#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace sead {
template <typename T>
class TList;
}  // namespace sead

namespace al {
class HitSensor;
class KCollisionServer;
class LiveActor;

class CollisionParts {
public:
    const LiveActor* getConnectedHost() const;
    void calcForceMovePower(sead::Vector3f*, const sead::Vector3f&) const;
    void calcForceRotatePower(sead::Quatf*) const;

    const sead::Matrix34f& getBaseMtx() const { return mBaseMtx; }

    const sead::Matrix34f& getBaseInvMtx() const { return mBaseInvMtx; }

    const sead::Matrix34f& getPrevBaseMtx() const { return mPrevBaseMtx; }

    KCollisionServer* getKCollisionServer() const { return mKCollisionServer; }

    s32 get_15c() const { return _15c; }

    bool isMoving() const { return mIsMoving; }

    const HitSensor* getConnectedSensor() const { return mConnectedSensor; }

private:
    void* unk[2];
    CollisionParts* _10;  // self-reference
    sead::TList<CollisionParts*>* mPartsList;
    sead::Matrix34f* mJointMtx;
    sead::Matrix34f mSyncMtx;
    sead::Matrix34f mBaseMtx;
    sead::Matrix34f mBaseInvMtx;
    sead::Matrix34f mPrevBaseMtx;
    sead::Matrix34f mPrevBaseInvMtx;
    sead::Vector3f mMtxScaleVec;
    f32 mMtxScale;
    f32 mInvMtxScale;
    s32 mPriority;
    KCollisionServer* mKCollisionServer;
    HitSensor* mConnectedSensor;
    const char* mSpecialPurpose;
    const char* mOptionalPurpose;
    sead::Vector3f _150;  // same as mMtxScaleVec?
    s32 _15c;
    f32 mBoundingSphereRange;
    f32 mBaseMtxScale;
    bool mIsValidatedByUser;
    bool mIsValidatedBySystem;
    bool _16a;
    bool _16b;
    bool _16c;
    bool mIsMoving;
    bool _16e;
};

}  // namespace al
