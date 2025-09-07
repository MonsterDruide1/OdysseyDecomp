#include "Player/PlayerCounterAfterUpperPunch.h"

#include "Player/PlayerTrigger.h"

PlayerCounterAfterUpperPunch::PlayerCounterAfterUpperPunch() = default;

void PlayerCounterAfterUpperPunch::update(const PlayerTrigger* trigger) {
    if (mCounter <= (u32)sead::Mathi::maxNumber() - 1)
        mCounter++;

    if (trigger->isOnUpperPunchHit())
        mCounter = 0;
}
