#pragma once

#include <container/seadPtrArray.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
}

class BossForest;
class BossForestSatellite;

class BossForestSatelliteHolder : public al::LiveActor {
public:
    BossForestSatelliteHolder(BossForest* boss);

    void init(const al::ActorInitInfo& initInfo) override;
    BossForestSatellite* findSatellite(s32 phase, s32 index) const;
    void makeActorDead() override;

private:
    BossForest* mBoss = nullptr;
    sead::PtrArray<BossForestSatellite> mSatellites;
    BossForestSatellite* mSatelliteBuffer[9];
};

static_assert(sizeof(BossForestSatelliteHolder) == 0x168);
