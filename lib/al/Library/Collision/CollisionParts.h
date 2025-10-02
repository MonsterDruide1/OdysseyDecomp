#pragma once

#include <container/seadTList.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {
class KCollisionServer;
class LiveActor;
class HitSensor;

class CollisionParts {
public:
    CollisionParts(void* kcl, const void* byml);

    const LiveActor* getConnectedHost() const;
    void calcForceMovePower(sead::Vector3f*, const sead::Vector3f&) const;
    void calcForceRotatePower(sead::Quatf*) const;
    void initParts(const sead::Matrix34f&);
    void invalidateBySystem();

    const sead::Matrix34f& getBaseMtx() const { return mBaseMtx; }

    const sead::Matrix34f& getBaseInvMtx() const { return mBaseInvMtx; }

    const sead::Matrix34f& getPrevBaseMtx() const { return mPrevBaseMtx; }

    const sead::Matrix34f& getPrevBaseInvMtx() const { return mPrevBaseInvMtx; }

    KCollisionServer* getKCollisionServer() const { return mKCollisionServer; }

    s32 get_15c() const { return _15c; }

    bool isValidCollision() const { return mIsValidatedByUser && mIsValidatedBySystem; }

    bool isMoving() const { return mIsMoving; }

    const HitSensor* getConnectedSensor() const { return mConnectedSensor; }

    void set_16e(bool val) { _16e = val; }

    void setSpecialPurpose(const char* specialPurpose) { mSpecialPurpose = specialPurpose; }

    void setOptionalPurpose(const char* optionalPurpose) { mOptionalPurpose = optionalPurpose; }

    void setPriority(s32 priority) { mPriority = priority; }

    void setConnectedSensor(HitSensor* sensor) { mConnectedSensor = sensor; }

    void setJointMtx(const sead::Matrix34f* jointMtx) { mJointMtx = jointMtx; }

private:
    void* unk[2];
    CollisionParts* _10;  // self-reference
    sead::TList<CollisionParts*>* mPartsList;
    const sead::Matrix34f* mJointMtx;
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
