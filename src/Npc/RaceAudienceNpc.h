#pragma once

#include "Library/LiveActor/LiveActor.h"

class RaceAudienceNpc : public al::LiveActor {
public:
    RaceAudienceNpc(const char* name);

    void init(const al::ActorInitInfo& info) override;

    void exeWait();
    void exeDance();
    void exeDanceRandom();
    void exeJump();

private:
    s32 mAudienceActionType = 0;
    s32 mDanceTimer = 0;
};
