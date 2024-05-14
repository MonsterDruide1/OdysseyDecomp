#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

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
    const KCollisionServer& getKCollisionServer() const { return *mKCollisionServer; }
    int getSomeCounter() const { return someCounter; }
    bool isMoving() const { return mIsMoving; }
    const HitSensor* getConnectedSensor() const { return mConnectedSensor; }

private:
    unsigned char gap1[24];
    void* mPartsList;
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
    al::HitSensor* mConnectedSensor;
    const char* mSpecialPurpose;
    const char* mOptionalPurpose;
    sead::Vector3f mMtxScaleVecAgain;
    int someCounter;
    float mBoundingSphereRange;
    float mBaseMtxScale;
    bool isValidatedByUser;
    bool isValidatedBySystem;
    bool bVar3;
    bool bVar4;
    bool bVar5;
    bool mIsMoving;
    bool bVar6;
    bool pad2;
};
static_assert(sizeof(CollisionParts) == 0x170);

}  // namespace al
