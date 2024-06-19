#include "Player/PlayerJudgeStartSquat.h"

#include "Player/PlayerCarryKeeper.h"
#include "Player/PlayerCounterForceRun.h"
#include "Player/PlayerInput.h"

PlayerJudgeStartSquat::PlayerJudgeStartSquat(const PlayerInput* input,
                                             const PlayerCounterForceRun* counterForceRun,
                                             const PlayerCarryKeeper* carryKeeper)
    : mInput(input), mCounterForceRun(counterForceRun), mCarryKeeper(carryKeeper) {}

bool PlayerJudgeStartSquat::judge() const {
    return !mCarryKeeper->isCarry() && mInput->isHoldSquat() && !mCounterForceRun->isForceRun();
}

void PlayerJudgeStartSquat::reset() {}

void PlayerJudgeStartSquat::update() {}
