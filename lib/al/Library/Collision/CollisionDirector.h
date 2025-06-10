#pragma once

#include <container/seadObjArray.h>
#include <math/seadVector.h>
#include <prim/seadDelegate.h>

#include "Library/Collision/CollisionResultBuffer.h"
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
class CollisionParts;
class ExecuteDirector;

class CollisionDirector : public HioNode, public IUseExecutor {
public:
    CollisionDirector(ExecuteDirector* executeDirector);

    void execute() override;

    void setPartsKeeper(ICollisionPartsKeeper* partsKeeper);
    void endInit();
    void setPartsFilter(const CollisionPartsFilterBase*);
    void setTriFilter(const TriangleFilterBase*);
    s32 checkStrikePoint(const sead::Vector3f&, HitInfo*);
    s32 checkStrikeSphere(const sead::Vector3f&, f32, bool, const sead::Vector3f&);
    s32 checkStrikeArrow(const sead::Vector3f&, const sead::Vector3f&);
    s32 checkStrikeSphereForPlayer(const sead::Vector3f&, f32);
    s32 checkStrikeDisk(const sead::Vector3f&, f32, f32, const sead::Vector3f&);
    ArrowHitInfo* getStrikeArrowInfo(u32 index);
    u32 getStrikeArrowInfoNum() const;
    SphereHitInfo* getStrikeSphereInfo(u32 index);
    u32 getStrikeSphereInfoNum() const;
    DiskHitInfo* getStrikeDiskInfo(u32 index);
    u32 getStrikeDiskInfoNum() const;
    void getSphereHitInfoArrayForCollider(SphereHitInfo** infoArray, u32* count);
    void getDiskHitInfoArrayForCollider(DiskHitInfo** infoArray, u32* count);
    void searchCollisionPartsWithSphere(const sead::Vector3f&, f32,
                                        sead::IDelegate1<CollisionParts*>&,
                                        const CollisionPartsFilterBase*) const;
    void validateCollisionPartsPtrArray(sead::PtrArray<CollisionParts>*);
    void invalidateCollisionPartsPtrArray();
    sead::PtrArray<CollisionParts>* getCollisionPartsPtrArray() const;

    ~CollisionDirector();

public:
    ICollisionPartsKeeper* mActivePartsKeeper = nullptr;
    ICollisionPartsKeeper* mPartsKeeper = nullptr;
    CollisionPartsKeeperPtrArray* mCollisionPartsKeeperPtrArray;
    const CollisionPartsFilterBase* mCollisionPartsFilterBase = nullptr;
    const TriangleFilterBase* mTriangleFilterBase = nullptr;
    ArrowHitResultBuffer* mStrikeArrowHitInfos = nullptr;
    SphereHitResultBuffer* mStrikeSphereHitInfos = nullptr;
    DiskHitResultBuffer* mStrikeDiskHitInfos = nullptr;
    SphereHitInfo* mSphereHitArray = nullptr;
    DiskHitInfo* mDiskHitArray = nullptr;
};

static_assert(sizeof(CollisionDirector) == 0x58);

}  // namespace al
