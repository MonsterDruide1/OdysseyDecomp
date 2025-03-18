#include "Player/PlayerJudgeDrawForward.h"

#include "Library/LiveActor/LiveActor.h"

#include "Player/PlayerJudgeDeadWipeStart.h"
#include "System/GameDataFunction.h"

PlayerJudgeDrawForward::PlayerJudgeDrawForward(
    const al::LiveActor* playerActor, const PlayerJudgeDeadWipeStart* playerJudgeDeadWipeStart)
    : mPlayerActor(playerActor), mPlayerJudgeDeadWipeStart(playerJudgeDeadWipeStart) {}

bool PlayerJudgeDrawForward::judge() const {
    if (!GameDataFunction::isPlayerLifeZero(mPlayerActor))
        return false;
    return mPlayerJudgeDeadWipeStart->isDeadTypeDrawForward();
}

void PlayerJudgeDrawForward::reset() {}

void PlayerJudgeDrawForward::update() {}
