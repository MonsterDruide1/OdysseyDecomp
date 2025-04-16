#pragma once

#include <math/seadMathCalcCommon.h>

class PlayerConst;
class PlayerTrigger;

class PlayerCounterAfterCapCatch {
public:
    PlayerCounterAfterCapCatch(const PlayerConst* pConst, const PlayerTrigger* trigger);
    void update(const PlayerTrigger* trigger);
    bool isCapCatch() const;

private:
    const PlayerConst* mConst;
    const PlayerTrigger* mTrigger;
    // yes, this is an u32, bounded by a signed s32
    u32 mCounter = sead::Mathi::maxNumber();
};
