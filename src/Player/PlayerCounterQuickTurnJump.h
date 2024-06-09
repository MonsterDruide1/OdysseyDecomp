#pragma once

#include <basis/seadTypes.h>

class PlayerConst;
class PlayerTrigger;

class PlayerCounterQuickTurnJump {
public:
    PlayerCounterQuickTurnJump(const PlayerConst* pConst, const PlayerTrigger* trigger);

    void update();
    bool isEnableTurnJump() const;

private:
    const PlayerConst* mConst;
    const PlayerTrigger* mTrigger;
    s32 mCounter = 0;
};
