#pragma once

#include <math/seadVector.h>
#include <prim/seadDelegate.h>

namespace al {
class CollisionParts;
class HitInfo;
class SphereCheckInfo;
class ArrowCheckInfo;
class DiskCheckInfo;
class SphereHitResultBuffer;
class ArrowHitResultBuffer;
class DiskHitResultBuffer;
class CollisionCheckInfoBase;

class ICollisionPartsKeeper {
public:
    virtual void endInit() = 0;
    virtual void addCollisionParts(CollisionParts* parts) = 0;
    virtual void connectToCollisionPartsList(CollisionParts* parts) = 0;
    virtual void disconnectToCollisionPartsList(CollisionParts* parts) = 0;
    virtual void resetToCollisionPartsList(CollisionParts* parts) = 0;
    virtual s32 checkStrikePoint(HitInfo* hitInfo,
                                 const CollisionCheckInfoBase& checkInfo) const = 0;
    // TODO: rename parameters
    virtual s32 checkStrikeSphere(SphereHitResultBuffer* resultBuffer,
                                  const SphereCheckInfo& checkInfo, bool unk,
                                  const sead::Vector3f& unk2) const = 0;
    virtual s32 checkStrikeArrow(ArrowHitResultBuffer* resultBuffer,
                                 const ArrowCheckInfo& checkInfo) const = 0;
    virtual s32 checkStrikeSphereForPlayer(SphereHitResultBuffer* resultBuffer,
                                           const SphereCheckInfo& checkInfo) const = 0;
    virtual s32 checkStrikeDisk(DiskHitResultBuffer* resultBuffer,
                                const DiskCheckInfo& checkInfo) const = 0;
    virtual void searchWithSphere(const SphereCheckInfo& checkInfo,
                                  sead::IDelegate1<CollisionParts*>& callback) const = 0;
    virtual void movement() = 0;
};

}  // namespace al
