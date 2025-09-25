#pragma once

#include "Npc/BirdStateWaitBase.h"

class BigBirdStateWait : public BirdStateWaitBase {
public:
    BigBirdStateWait(al::LiveActor* actor);

    void appear() override;

    void exeWaitLeft();
    void exeTurnLeft();
    void exeWaitRight();
    void exeTurnRight();

    const char* getDefaultWaitActionName() override { return "GroundWait"; }

private:
    f32 mTurnAnglePerStep = 0.0f;
};

static_assert(sizeof(BigBirdStateWait) == 0x28);
