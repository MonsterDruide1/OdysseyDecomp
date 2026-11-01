#pragma once

#include <container/seadObjArray.h>
#include <container/seadPtrArray.h>
#include <math/seadVector.h>
#include <prim/seadDelegate.h>

#include "Library/Collision/CollisionParts.h"
#include "Library/Execute/IUseExecutor.h"
#include "Library/HostIO/HioNode.h"

namespace al {
class ICollisionPartsKeeper;
class CollisionPartsKeeperOctree;
class CollisionPartsKeeperPtrArray;
class CollisionPartsFilterBase;
class TriangleFilterBase;
class Strike;
struct HitInfo;
struct ArrowHitInfo;
struct DiskHitInfo;
struct SphereHitInfo;
class CollisionParts;
class ExecuteDirector;

class CollisionDirector : public HioNode, public IUseExecutor {
public:
    CollisionDirector(ExecuteDirector* executeDirector);

    void setPartsKeeper(ICollisionPartsKeeper* partsKeeper);
    void endInit();

    void setPartsFilter(const CollisionPartsFilterBase* partsFilter);
    void setTriFilter(const TriangleFilterBase* triFilter);

    u32 checkStrikePoint(const sead::Vector3f& position, HitInfo* outHit);
    u32 checkStrikeSphere(const sead::Vector3f& position, f32 radius, bool checkMoving,
                          const sead::Vector3f& moveDirection);
    u32 checkStrikeArrow(const sead::Vector3f& position, const sead::Vector3f& direction);
    u32 checkStrikeSphereForPlayer(const sead::Vector3f& position, f32 radius);
    u32 checkStrikeDisk(const sead::Vector3f& position, f32 radius, f32 halfHeight,
                        const sead::Vector3f& normal);

    const ArrowHitInfo& getStrikeArrowInfo(u32 index);
    s32 getStrikeArrowInfoNum() const;
    const SphereHitInfo& getStrikeSphereInfo(u32 index);
    s32 getStrikeSphereInfoNum() const;
    const DiskHitInfo& getStrikeDiskInfo(u32 index);
    s32 getStrikeDiskInfoNum() const;

    void getSphereHitInfoArrayForCollider(SphereHitInfo** outArray, u32* outSize);
    void getDiskHitInfoArrayForCollider(DiskHitInfo** outArray, u32* outSize);

    void execute();

    void searchCollisionPartsWithSphere(const sead::Vector3f& position, f32 radius,
                                        CollisionPartsCallback& callback,
                                        const CollisionPartsFilterBase* partsFilter) const;

    void validateCollisionPartsPtrArray(sead::PtrArray<CollisionParts>* partsPtrArray);
    void invalidateCollisionPartsPtrArray();

    sead::PtrArray<CollisionParts>* getCollisionPartsPtrArray() const;

    ICollisionPartsKeeper* getActivePartsKeeper() const { return mActivePartsKeeper; }

private:
    ICollisionPartsKeeper* mActivePartsKeeper = nullptr;
    ICollisionPartsKeeper* mRootPartsKeeper = nullptr;
    CollisionPartsKeeperPtrArray* mPtrArrayPartsKeeper = nullptr;

    const CollisionPartsFilterBase* mPartsFilter = nullptr;
    const TriangleFilterBase* mTriFilter = nullptr;

    ArrowHitResultBuffer* mStrikeArrowHitInfoArray = nullptr;
    SphereHitResultBuffer* mStrikeSphereHitInfoArray = nullptr;
    DiskHitResultBuffer* mStrikeDiskHitInfoArray = nullptr;

    SphereHitInfo* mSphereHitInfoArrayForCollider = nullptr;
    DiskHitInfo* mDiskHitInfoArrayForCollider = nullptr;
};

static_assert(sizeof(CollisionDirector) == 0x58);

}  // namespace al
