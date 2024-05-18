#pragma once

#include <math/seadVector.h>
#include <prim/seadDelegate.h>

namespace al {
class CollisionParts;
class HitInfo;
class CollisionCheckInfoBase;
class SphereCheckInfo;
class SphereHitResultBuffer;
class ArrowHitResultBuffer;
class ArrowCheckInfo;
class DiskCheckInfo;
class DiskHitResultBuffer;

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

}  // namespace al
