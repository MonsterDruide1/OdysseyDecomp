#pragma once

#include "Library/LiveActor/LiveActor.h"

#include "Util/IUseDimension.h"

namespace al {
class CollisionPartsFilterBase;
}  // namespace al

class KoopaFire2D : public al::LiveActor, public IUseDimension {
public:
    KoopaFire2D(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void appear() override;

    void exeFlyWait();

    ActorDimensionKeeper* getActorDimensionKeeper() const override { return mDimensionKeeper; }

private:
    inline bool tryUpdate2D();
    inline void attackAndKill(al::HitSensor* sensor);

    ActorDimensionKeeper* mDimensionKeeper = nullptr;
    al::CollisionPartsFilterBase* mCollisionPartsFilter = nullptr;
};

static_assert(sizeof(KoopaFire2D) == 0x120);
