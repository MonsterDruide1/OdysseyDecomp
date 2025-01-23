#include "Player/PlayerCounterAfterCapCatch.h"

#include "Player/PlayerConst.h"
#include "Player/PlayerTrigger.h"

PlayerCounterAfterCapCatch::PlayerCounterAfterCapCatch(const PlayerConst* pConst,
                                                       const PlayerTrigger* trigger)
    : mConst(pConst), mTrigger(trigger) {}

void PlayerCounterAfterCapCatch::update(const PlayerTrigger* trigger) {
    if (mCounter <= (u32)sead::Mathi::maxNumber() - 1)
        mCounter++;

    if (trigger->isOn(PlayerTrigger::EAttackSensorTrigger_val0))
        mCounter = 0;
}

bool PlayerCounterAfterCapCatch::isCapCatch() const {
    if (mTrigger->isOn(PlayerTrigger::EAttackSensorTrigger_val0))
        return true;

    return mCounter <= (u32)mConst->getEnableActionFrameCapCatch();
}
