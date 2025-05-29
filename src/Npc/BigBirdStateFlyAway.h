#pragma once

#include "Npc/BirdStateFlyAwayBase.h"

class BigBirdStateFlyAway : public BirdStateFlyAwayBase {
public:
    BigBirdStateFlyAway(al::LiveActor* actor);

    void appear() override;

    void exeGroundReaction();
    void exeFlyReaction();
    void exeFlyAwayPrepare();
    void exeFlyAwayStart();
    void exeFlyAway();
};

static_assert(sizeof(BigBirdStateFlyAway) == 0x38);
