#pragma once

#include "Library/Collision/CollisionPartsKeeper.h"
#include "container/seadPtrArray.h"

namespace game {

class CollisionPartsKeeper : public al::ICollisionPartsKeeper {
public:
    CollisionPartsKeeper();
    ~CollisionPartsKeeper();

    void endInit() override;
    void addCollisionParts(al::CollisionParts*) override;
    void connectToCollisionPartsList(al::CollisionParts*) override;
    void disconnectToCollisionPartsList(al::CollisionParts*) override;
    void resetToCollisionPartsList(al::CollisionParts*) override;
    bool checkStrikePoint(al::HitInfo*, const al::CollisionCheckInfoBase&) const override;
    bool checkStrikeSphere(al::SphereHitResultBuffer*, const al::SphereCheckInfo&, bool, const sead::Vector3f&) const override;
    bool checkStrikeArrow(al::ArrowHitResultBuffer*, const al::ArrowCheckInfo&) const override;
    bool checkStrikeSphereForPlayer(al::SphereHitResultBuffer*, const al::SphereCheckInfo&) const override;
    bool checkStrikeDisk(al::DiskHitResultBuffer*, const al::DiskCheckInfo&) const override;
    void searchWithSphere(const al::SphereCheckInfo&, sead::IDelegate1<al::CollisionParts*>&) const override;
    void movement() override;

private:
    sead::PtrArray<al::CollisionParts> mPartsList;
    
};

}
