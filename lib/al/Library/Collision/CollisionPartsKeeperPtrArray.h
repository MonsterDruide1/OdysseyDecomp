#pragma once

#include <container/seadPtrArray.h>

#include "Library/Collision/ICollisionPartsKeeper.h"

namespace al {
class CollisionPartsKeeperPtrArray : public ICollisionPartsKeeper {
public:
    CollisionPartsKeeperPtrArray();

    void endInit() override;

    void addCollisionParts(CollisionParts* parts) override;
    void connectToCollisionPartsList(CollisionParts* parts) override;
    void disconnectToCollisionPartsList(CollisionParts* parts) override;
    void resetToCollisionPartsList(CollisionParts* parts) override;

    u32 checkStrikePoint(HitInfo* outInfo, const CollisionCheckInfoBase& checkInfo) const override;
    u32 checkStrikeSphere(SphereHitResultBuffer* resultBuffer, const SphereCheckInfo& checkInfo,
                          bool checkMoving, const sead::Vector3f& moveDirecrion) const override;
    u32 checkStrikeArrow(ArrowHitResultBuffer* resultBuffer,
                         const ArrowCheckInfo& checkInfo) const override;
    u32 checkStrikeSphereForPlayer(SphereHitResultBuffer* resultBuffer,
                                   const SphereCheckInfo& checkInfo) const override;
    u32 checkStrikeDisk(DiskHitResultBuffer* resultBuffer,
                        const DiskCheckInfo& checkInfo) const override;

    void searchWithSphere(const SphereCheckInfo& checkInfo,
                          CollisionPartsCallback& callback) const override;

    void movement() override;

    sead::PtrArray<CollisionParts>* getPartsPtrArray() const { return mPartsPtrArray; }

    void setPartsPtrArray(sead::PtrArray<CollisionParts>* ptrArray) { mPartsPtrArray = ptrArray; }

private:
    sead::PtrArray<CollisionParts>* mPartsPtrArray = nullptr;
};
}  // namespace al
