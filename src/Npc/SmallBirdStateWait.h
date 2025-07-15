#pragma once

#include <math/seadVector.h>

#include "Npc/BirdStateWaitBase.h"

namespace al {
struct ActorInitInfo;
}

class SmallBirdStateWait : public BirdStateWaitBase {
public:
    SmallBirdStateWait(al::LiveActor* actor, const al::ActorInitInfo& info);

    void appear() override;

    void exeWait();
    void exeTurn();

    const char* getDefaultWaitActionName() override { return "GroundWaitA"; }

private:
    sead::Vector3f mInitialFront = {0, 0, 0};
    f32 mTurnAnglePerStep = 0;
    f32 mTurnRange = -1;
};

static_assert(sizeof(SmallBirdStateWait) == 0x38);
