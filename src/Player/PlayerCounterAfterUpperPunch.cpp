#include "Player/PlayerCounterAfterUpperPunch.h"

#include "Player/PlayerTrigger.h"

PlayerCounterAfterUpperPunch::PlayerCounterAfterUpperPunch() {}

void PlayerCounterAfterUpperPunch::update(const PlayerTrigger* trigger) {
    if (mCounter <= sead::Mathi::maxNumber() - 1)
        mCounter++;

    if (trigger->isOnUpperPunchHit())
        mCounter = 0;
}
