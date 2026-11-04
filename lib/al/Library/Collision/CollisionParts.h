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

enum class ForceCollisionScale : u8 {
    None = 0,
    Average = 1,
    One = 2,
};

class SphereHitResultBuffer : public sead::ObjArray<SphereHitInfo> {};

class ArrowHitResultBuffer : public sead::ObjArray<ArrowHitInfo> {};

class DiskHitResultBuffer : public sead::ObjArray<DiskHitInfo> {};

using CollisionPartsList = sead::TList<CollisionParts*>;
using CollisionPartsListNode = sead::TListNode<CollisionParts*>;

class CollisionParts : public CollisionPartsListNode {
public:
    CollisionParts(void* kclData, const void* bymlData);

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

    bool checkBoundingSphereRange(const sead::Vector3f& position, f32 radius);

    u32 checkStrikePoint(HitInfo* outHit, const sead::Vector3f& position,
                         const TriangleFilterBase* triFilter) const;
    u32 checkStrikeSphere(SphereHitResultBuffer* resultBuffer, const sead::Vector3f& position,
                          f32 radius, bool checkMoving, const sead::Vector3f& moveDirection,
                          const TriangleFilterBase* triFilter) const;
    u32 checkStrikeArrow(ArrowHitResultBuffer* resultBuffer, const sead::Vector3f& position,
                         const sead::Vector3f& direction,
                         const TriangleFilterBase* triFilter) const;
    u32 checkStrikeSphereForPlayer(SphereHitResultBuffer* resultBuffer,
                                   const sead::Vector3f& position, f32 radius,
                                   const TriangleFilterBase* triFilter) const;
    u32 checkStrikeDisk(DiskHitResultBuffer* resultBuffer, const sead::Vector3f& position,
                        f32 radius, f32 halfHeight, const sead::Vector3f& normal,
                        const TriangleFilterBase* triFilter) const;

    void calcForceMovePower(sead::Vector3f* outMovePower, const sead::Vector3f& position) const;
    void calcForceRotatePower(sead::Quatf* outRotatePower) const;

    bool hasSyncCollisionMtx() const { return mSyncCollisionMtxPtr != nullptr; }

    void setSyncCollisionMtxPtr(const sead::Matrix34f* mtxPtr) { mSyncCollisionMtxPtr = mtxPtr; }

    sead::Vector3f getTrans() const { return mBaseMtx.getTranslation(); }

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

    f32 getBoundingSphereRange() const { return mBoundingSphereRange; }

    bool isValidCollision() const { return mIsValidatedByUser && mIsValidatedBySystem; }

    bool isJustValidated() const { return mIsJustValidated; }

    void resetJustValidated() { mIsJustValidated = false; }

    bool needsUpdate() const { return mIsUpdateAlways || mIsUpdateMtxOneTime; }

    bool isMoving() const { return mNoMoveTime == 0 || mIsMoving; }

    void setForceCollisionScaleNone() { mForceCollisionScale = ForceCollisionScale::None; }

    void setForceCollisionScaleAverage() { mForceCollisionScale = ForceCollisionScale::Average; }

    void setForceCollisionScaleOne() { mForceCollisionScale = ForceCollisionScale::One; }

private:
    void resetAllMtxPrivate(const sead::Matrix34f& matrix);
    void updateBoundingSphereRangePrivate(f32 scale);

    u32 checkStrikeSphereCore(SphereHitResultBuffer* resultBuffer, const sead::Vector3f& worldPos,
                              const sead::Vector3f& modelPos, const sead::Vector3f& moveReaction,
                              f32 modelRadius, const TriangleFilterBase* triFilter) const;
    u32 checkStrikeSphereForPlayerCore(SphereHitResultBuffer* resultBuffer,
                                       const sead::Vector3f& worldPos,
                                       const sead::Vector3f& modelPos, const sead::Vector3f& unused,
                                       const sead::Vector3f& moveReaction, f32 modelRadius,
                                       const TriangleFilterBase* triFilter) const;
    u32 checkStrikeDiskCore(DiskHitResultBuffer* resultBuffer, const sead::Vector3f& worldPos,
                            const sead::Vector3f& modelPos, const sead::Vector3f& moveReaction,
                            f32 modelRadius, f32 modelHalfHeight, const sead::Vector3f& modelNormal,
                            const TriangleFilterBase* triFilter) const;

    const sead::Matrix34f* mSyncCollisionMtxPtr = nullptr;
    sead::Matrix34f mSyncMtx;
    sead::Matrix34f mBaseMtx;
    sead::Matrix34f mBaseInvMtx;
    sead::Matrix34f mPrevBaseMtx;
    sead::Matrix34f mPrevBaseInvMtx;

    sead::Vector3f mMtxScaleVec = {1.0f, 1.0f, 1.0f};
    f32 mMtxScale = 1.0f;
    f32 mInvMtxScale = 1.0f;

    s32 mPriority = -1;

    KCollisionServer* mServer = nullptr;
    HitSensor* mConnectedSensor = nullptr;
    const char* mSpecialPurpose = nullptr;
    const char* mOptionalPurpose = nullptr;

    sead::Vector3f mSyncMtxScaleVec = {1.0f, 1.0f, 1.0f};
    s32 mNoMoveTime = 0;
    f32 mBoundingSphereRange = -1.0f;
    f32 mBaseMtxScale = 1.0f;

    bool mIsValidatedByUser = true;
    bool mIsValidatedBySystem = true;
    bool mIsJustValidated = false;
    bool mIsUpdateAlways = true;  // Appears to be always true
    bool mIsUpdateMtxOneTime = false;
    bool mIsMoving = false;

    ForceCollisionScale mForceCollisionScale = ForceCollisionScale::None;
};

}  // namespace al
