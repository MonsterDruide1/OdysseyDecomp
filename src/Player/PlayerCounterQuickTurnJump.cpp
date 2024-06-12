#include "Player/PlayerCounterQuickTurnJump.h"

#include "Library/Math/MathUtil.h"

#include "Player/PlayerConst.h"
#include "Player/PlayerTrigger.h"

PlayerCounterQuickTurnJump::PlayerCounterQuickTurnJump(const PlayerConst* pConst,
                                                       const PlayerTrigger* trigger)
    : mConst(pConst), mTrigger(trigger) {}

void PlayerCounterQuickTurnJump::update() {
    if (mTrigger->isOn(PlayerTrigger::EActionTrigger_QuickTurn))
        mCounter = mConst->getQuickTurnJumpFrame();
    else
        mCounter = al::converge(mCounter, 0, 1);
}

bool PlayerCounterQuickTurnJump::isEnableTurnJump() const {
    return mTrigger->isOn(PlayerTrigger::EActionTrigger_QuickTurn) || mCounter > 0;
}
