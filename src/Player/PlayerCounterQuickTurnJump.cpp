#include "Player/PlayerCounterQuickTurnJump.h"

#include "Library/Math/MathUtil.h"

#include "Player/PlayerConst.h"
#include "Player/PlayerTrigger.h"

PlayerCounterQuickTurnJump::PlayerCounterQuickTurnJump(const PlayerConst* pConst,
                                                       const PlayerTrigger* trigger)
    : mConst(pConst), mTrigger(trigger) {}

void PlayerCounterQuickTurnJump::update() {
    if (mTrigger->isOn(PlayerTrigger::EActionTrigger_QuickTurn)) {
        mFrame = mConst->getQuickTurnJumpFrame();
    } else {
        mFrame = al::converge(mFrame, 0, 1);
    }
}

bool PlayerCounterQuickTurnJump::isEnableTurnJump() const {
    return mTrigger->isOn(PlayerTrigger::EActionTrigger_QuickTurn) || mFrame > 0;
}
