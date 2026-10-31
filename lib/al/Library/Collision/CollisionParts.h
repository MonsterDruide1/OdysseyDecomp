#pragma once

#include <container/seadObjArray.h>
#include <container/seadTList.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/Collision/CollisionUtil.h"
#include "Library/Collision/HitInfo.h"
#include "Library/Collision/ICollisionPartsKeeper.h"

namespace al {
struct HitInfo;
class HitSensor;
class KCollisionServer;
class LiveActor;
class TriangleFilterBase;

enum class ForceCollisionFlag : u8 {
    ScaleNone,
    ScaleAverage,
    ScaleOne,
};

class SphereHitResultBuffer : public sead::ObjArray<SphereHitInfo> {};

class ArrowHitResultBuffer : public sead::ObjArray<ArrowHitInfo> {};

class DiskHitResultBuffer : public sead::ObjArray<DiskHitInfo> {};

class CollisionParts : public sead::TListNode<CollisionParts*> {
public:
    CollisionParts(void* kcl, const void* byml);

    void calcInvMtxScale();
    const LiveActor* getConnectedHost() const;

    void initParts(const sead::Matrix34f& matrix);
    void resetAllMtx(const sead::Matrix34f& matrix);
    void updateBoundingSphereRange(sead::Vector3f scale);

    void validateByUser();
    void invalidateByUser();
    void validateBySystem();
    void invalidateBySystem();

    void onJoinList();

    f32 makeEqualScale(sead::Matrix34f* matrix);
    void resetAllMtx();

    void updateBoundingSphereRange();
    void forceResetAllMtxAndSetUpdateMtxOneTime(const sead::Matrix34f& matrix);
    void forceResetAllMtxAndSetUpdateMtxOneTime();

    void syncMtx(const sead::Matrix34f& matrix);
    void syncMtx();

    void updateMtx();
    void updateScale();

    bool checkBoundingSphereRange(const sead::Vector3f&, f32);
    bool checkStrikePoint(HitInfo*, const sead::Vector3f&, const TriangleFilterBase*) const;
    u32 checkStrikeSphere(SphereHitResultBuffer*, const sead::Vector3f&, f32, bool,
                          const sead::Vector3f&, const TriangleFilterBase*) const;
    s32 checkStrikeSphereCore(SphereHitResultBuffer*, const sead::Vector3f&, const sead::Vector3f&,
                              const sead::Vector3f&, f32, const TriangleFilterBase*) const;
    s32 checkStrikeArrow(ArrowHitResultBuffer*, const sead::Vector3f&, const sead::Vector3f&,
                         const TriangleFilterBase*) const;
    s32 checkStrikeSphereForPlayer(SphereHitResultBuffer*, const sead::Vector3f&, f32,
                                   const TriangleFilterBase*) const;
    s32 checkStrikeSphereForPlayerCore(SphereHitResultBuffer*, const sead::Vector3f&,
                                       const sead::Vector3f&, const sead::Vector3f&,
                                       const sead::Vector3f&, f32, const TriangleFilterBase*) const;
    s32 checkStrikeDisk(DiskHitResultBuffer*, const sead::Vector3f&, f32, f32,
                        const sead::Vector3f&, const TriangleFilterBase*) const;
    s32 checkStrikeDiskCore(DiskHitResultBuffer*, const sead::Vector3f&, const sead::Vector3f&,
                            const sead::Vector3f&, f32, f32, const sead::Vector3f&,
                            const TriangleFilterBase*) const;

    void calcForceMovePower(sead::Vector3f*, const sead::Vector3f&) const;
    void calcForceRotatePower(sead::Quatf*) const;

    bool hasSyncCollisionMtx() const { return mSyncCollisionMtxPtr != nullptr; }

    void setSyncCollisionMtxPtr(const sead::Matrix34f* mtxPtr) { mSyncCollisionMtxPtr = mtxPtr; }

    const sead::Matrix34f& getBaseMtx() const { return mBaseMtx; }

    const sead::Matrix34f& getBaseInvMtx() const { return mBaseInvMtx; }

    const sead::Matrix34f& getPrevBaseMtx() const { return mPrevBaseMtx; }

    const sead::Matrix34f& getPrevBaseInvMtx() const { return mPrevBaseInvMtx; }

    void setPriority(s32 priority) { mPriority = priority; }

    KCollisionServer* getServer() const { return mServer; }

    HitSensor* getConnectedSensor() const { return mConnectedSensor; }

    void setConnectedSensor(HitSensor* sensor) { mConnectedSensor = sensor; }

    void setSpecialPurpose(const char* specialPurpose) { mSpecialPurpose = specialPurpose; }

    void setOptionalPurpose(const char* optionalPurpose) { mOptionalPurpose = optionalPurpose; }

    bool isValidCollision() const { return mIsValidatedByUser && mIsValidatedBySystem; }

    // TODO: Rename to isMoved() ?
    bool isMoving() const { return mRestingTimer == 0 || mIsMoving; }

    // TODO: Remove
    void setIsMoving(bool isMoving) { mIsMoving = isMoving; }

    void setForceCollisionScaleNone() { mForceCollisionFlag = ForceCollisionFlag::ScaleNone; }

    void setForceCollisionScaleAverage() { mForceCollisionFlag = ForceCollisionFlag::ScaleAverage; }

    void setForceCollisionScaleOne() { mForceCollisionFlag = ForceCollisionFlag::ScaleOne; }

private:
    void resetAllMtxPrivate(const sead::Matrix34f&);
    void updateBoundingSphereRangePrivate(f32);

    static KCollisionServer::HitInfoBuffer sHitInfoBuffer;

    const sead::Matrix34f* mSyncCollisionMtxPtr = nullptr;
    sead::Matrix34f mSyncMtx;
    sead::Matrix34f mBaseMtx;
    sead::Matrix34f mBaseInvMtx;
    sead::Matrix34f mPrevBaseMtx;
    sead::Matrix34f mPrevBaseInvMtx;

    sead::Vector3f mMtxScaleVec = sead::Vector3f::ones;
    f32 mMtxScale = 1.0f;
    f32 mInvMtxScale = 1.0f;

    s32 mPriority = -1;

    KCollisionServer* mServer = nullptr;
    HitSensor* mConnectedSensor = nullptr;
    const char* mSpecialPurpose = nullptr;
    const char* mOptionalPurpose = nullptr;

    sead::Vector3f _150 = sead::Vector3f::ones;
    s32 mRestingTimer = 0;
    f32 mBoundingSphereRange = -1.0f;
    f32 mBaseMtxScale;  // TODO: Check name

    bool mIsValidatedByUser = true;
    bool mIsValidatedBySystem = true;
    bool mIsJustValidated = false;
    bool _16b = true;
    bool mIsUpdateMtxOneTime = false;
    bool mIsMoving = false;

    ForceCollisionFlag mForceCollisionFlag = ForceCollisionFlag::ScaleNone;
};

}  // namespace al
