#pragma once

#include <math/seadVector.h>
#include <prim/seadDelegate.h>

#include "Library/Collision/CollisionParts.h"
#include "Library/HostIO/HioNode.h"

namespace al {
class CollisionParts;
struct HitInfo;
struct CollisionCheckInfoBase;
struct SphereCheckInfo;
struct ArrowCheckInfo;
struct DiskCheckInfo;
class SphereHitResultBuffer;
class ArrowHitResultBuffer;
class DiskHitResultBuffer;

class ICollisionPartsKeeper : public IUseHioNode {
public:
    virtual void endInit() = 0;

    virtual void addCollisionParts(CollisionParts* parts) = 0;
    virtual void connectToCollisionPartsList(CollisionParts* parts) = 0;
    virtual void disconnectToCollisionPartsList(CollisionParts* parts) = 0;
    virtual void resetToCollisionPartsList(CollisionParts* parts) = 0;

    virtual u32 checkStrikePoint(HitInfo* hitInfo,
                                 const CollisionCheckInfoBase& checkInfo) const = 0;
    virtual u32 checkStrikeSphere(SphereHitResultBuffer* resultBuffer,
                                  const SphereCheckInfo& checkInfo, bool checkMoving,
                                  const sead::Vector3f& moveDirection) const = 0;
    virtual u32 checkStrikeArrow(ArrowHitResultBuffer* resultBuffer,
                                 const ArrowCheckInfo& checkInfo) const = 0;
    virtual u32 checkStrikeSphereForPlayer(SphereHitResultBuffer* resultBuffer,
                                           const SphereCheckInfo& checkInfo) const = 0;
    virtual u32 checkStrikeDisk(DiskHitResultBuffer* resultBuffer,
                                const DiskCheckInfo& checkInfo) const = 0;

    virtual void searchWithSphere(const SphereCheckInfo& checkInfo,
                                  CollisionPartsCallback& callback) const = 0;
    virtual void movement() = 0;
};

}  // namespace al
