#pragma once

#include <math/seadVector.h>
#include <prim/seadDelegate.h>

#include <basis/seadTypes.h>
#include <math/seadVector.h>
#include <prim/seadDelegate.h>

#include "Library/Collision/CollisionResultBuffer.h"
#include "container/seadPtrArray.h"

namespace al {
class CollisionParts;
class HitInfo;
class CollisionCheckInfoBase;
class SphereCheckInfo;
class ArrowCheckInfo;
class DiskCheckInfo;

class ICollisionPartsKeeper {
public:
    virtual void endInit() = 0;
    virtual void addCollisionParts(CollisionParts*) = 0;
    virtual void connectToCollisionPartsList(CollisionParts*) = 0;
    virtual void disconnectToCollisionPartsList(CollisionParts*) = 0;
    virtual void resetToCollisionPartsList(CollisionParts*) = 0;
    virtual bool checkStrikePoint(HitInfo*, const CollisionCheckInfoBase*) const = 0;
    virtual bool checkStrikeSphere(SphereHitResultBuffer*, const SphereCheckInfo*, bool,
                                   const sead::Vector3f*) const = 0;
    virtual bool checkStrikeArrow(ArrowHitResultBuffer*, const ArrowCheckInfo*) const = 0;
    virtual bool checkStrikeSphereForPlayer(SphereHitResultBuffer*, const SphereCheckInfo*) const = 0;
    virtual bool checkStrikeDisk(DiskHitResultBuffer*, const DiskCheckInfo*) const = 0;
    virtual void searchWithSphere(const SphereCheckInfo*, sead::IDelegate1<CollisionParts*>&) const = 0;
};

class ArrowCheckInfo;
class DiskCheckInfo;

class CollisionPartsKeeperPtrArray : public ICollisionPartsKeeper {
public:
    CollisionPartsKeeperPtrArray();

    void setPtrArray(sead::PtrArray<CollisionParts>* ptrArray) { mPtrArray = ptrArray; }
    sead::PtrArray<CollisionParts>* getPtrArray() const { return mPtrArray; }

private:
    sead::PtrArray<CollisionParts>* mPtrArray;
};
static_assert(sizeof(CollisionPartsKeeperPtrArray) == 0x10);

class CollisionPartsKeeperOctree : public ICollisionPartsKeeper {
public:
    CollisionPartsKeeperOctree(s32, s32, f32);

private:
    void* size[0xFF/8];
};
static_assert(sizeof(CollisionPartsKeeperOctree) == 0x100);

}
