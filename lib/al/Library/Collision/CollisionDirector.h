#pragma once

#include <container/seadPtrArray.h>
#include <math/seadVector.h>
#include <prim/seadDelegate.h>

#include "Library/Execute/IUseExecutor.h"

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

class CollisionDirector : public IUseExecutor {
public:
    CollisionDirector(ExecuteDirector* executeDirector);
    void setPartsKeeper(ICollisionPartsKeeper* partsKeeper);
    void endInit();
    void setPartsFilter(const CollisionPartsFilterBase*);
    void setTriFilter(const TriangleFilterBase*);
    bool checkStrikePoint(const sead::Vector3f&, HitInfo*);
    bool checkStrikeSphere(const sead::Vector3f&, f32, bool, const sead::Vector3f&);
    bool checkStrikeArrow(const sead::Vector3f&, const sead::Vector3f&);
    bool checkStrikeSphereForPlayer(const sead::Vector3f&, f32);
    bool checkStrikeDisk(const sead::Vector3f&, f32, f32, const sead::Vector3f&);
    sead::PtrArray<ArrowHitInfo>* getStrikeArrowInfo(u32 index);
    u32 getStrikeArrowInfoNum();
    sead::PtrArray<DiskHitInfo>* getStrikeSphereInfo(u32 index);
    u32 getStrikeSphereInfoNum();
    sead::PtrArray<SphereHitInfo>* getStrikeDiskInfo(u32 index);
    u32 getStrikeDiskInfoNum();
    void getSphereHitInfoArrayForCollider(SphereHitInfo** infoArray, u32* count);
    void getDiskHitInfoArrayForCollider(DiskHitInfo** infoArray, u32* count);
    void execute();
    void searchCollisionPartsWithSphere(const sead::Vector3f&, f32,
                                        sead::IDelegate1<CollisionParts*>&,
                                        const CollisionPartsFilterBase*);
    void validateCollisionPartsPtrArray(sead::PtrArray<CollisionParts>*);
    void invalidateCollisionPartsPtrArray();
    sead::PtrArray<CollisionParts>* getCollisionPartsPtrArray();

public:
    ICollisionPartsKeeper* mActivePartsKeeper;
    CollisionPartsKeeperOctree* mRootOctree;
    CollisionPartsKeeperPtrArray* mCollisionPartsKeeperPtrArray;
    CollisionPartsFilterBase* mCollisionPartsFilterBase;
    TriangleFilterBase* mTriangleFilterBase;
    sead::PtrArray<ArrowHitInfo>* mStrikeArrowHitInfos;
    sead::PtrArray<DiskHitInfo>* mStrikeDiskHitInfos;
    sead::PtrArray<SphereHitInfo>* mStrikeSphereHitInfos;
    SphereHitInfo* mSphereHitArray;
    DiskHitInfo* mDiskHitArray;
};
}  // namespace al
