#pragma once

#include <basis/seadTypes.h>

#include "Library/Nerve/NerveStateBase.h"

class GrowFlowerPot;

class GrowFlowerPotStateReactionWater : public al::HostStateBase<GrowFlowerPot> {
public:
    GrowFlowerPotStateReactionWater(GrowFlowerPot* pot);

    void appear() override;
    void control() override;

    void exeStart();
    void exeReaction();
    void exeReactionGrow();
    void exeEnd();
    void requestReaction();

private:
    s32 mReactionTimer = 0;
};

static_assert(sizeof(GrowFlowerPotStateReactionWater) == 0x28);
