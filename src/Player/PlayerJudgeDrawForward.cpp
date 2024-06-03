#include "Player/PlayerJudgeDrawForward.h"

PlayerJudgeDrawForward::PlayerJudgeDrawForward(
    const al::LiveActor* playerActor, const PlayerJudgeDeadWipeStart* playerJudgeDeadWipeStart)
    : mPlayerActor(playerActor), mPlayerJudgeDeadWipeStart(playerJudgeDeadWipeStart) {}
