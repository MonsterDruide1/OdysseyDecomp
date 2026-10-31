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

    void setPartsFilter(const CollisionPartsFilterBase*);
    void setTriFilter(const TriangleFilterBase*);

    bool checkStrikePoint(const sead::Vector3f&, HitInfo*);
    bool checkStrikeSphere(const sead::Vector3f&, f32, bool, const sead::Vector3f&);
    bool checkStrikeArrow(const sead::Vector3f&, const sead::Vector3f&);
    bool checkStrikeSphereForPlayer(const sead::Vector3f&, f32);
    bool checkStrikeDisk(const sead::Vector3f&, f32, f32, const sead::Vector3f&);

    const ArrowHitInfo* getStrikeArrowInfo(u32 index);
    u32 getStrikeArrowInfoNum();
    const DiskHitInfo* getStrikeSphereInfo(u32 index);
    u32 getStrikeSphereInfoNum();
    const SphereHitInfo* getStrikeDiskInfo(u32 index);
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

    ICollisionPartsKeeper* getActivePartsKeeper() const { return mActivePartsKeeper; }

private:
    ICollisionPartsKeeper* mActivePartsKeeper;
    CollisionPartsKeeperOctree* mOctreePartsKeeper;
    CollisionPartsKeeperPtrArray* mPtrArrayPartsKeeper;

    CollisionPartsFilterBase* mPartsFilter;
    TriangleFilterBase* mTriFilter;

    ArrowHitResultBuffer* mStrikeArrowHitInfos;
    SphereHitResultBuffer* mStrikeSphereHitInfos;
    DiskHitResultBuffer* mStrikeDiskHitInfos;

    // TODO: Probably sead::(Un)safeArray
    SphereHitInfo* mSphereHitArray;
    DiskHitInfo* mDiskHitArray;
};
}  // namespace al
