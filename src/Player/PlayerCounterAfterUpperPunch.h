#pragma once

#include <math/seadMathCalcCommon.h>

class PlayerTrigger;

class PlayerCounterAfterUpperPunch {
public:
    PlayerCounterAfterUpperPunch();
    void update(const PlayerTrigger* trigger);

    u32 getCounter() const { return mCounter; }

private:
    // yes, this is an u32, bounded by a signed s32
    u32 mCounter = sead::Mathi::maxNumber();
};
static_assert(sizeof(PlayerCounterAfterUpperPunch) == 0x4);
