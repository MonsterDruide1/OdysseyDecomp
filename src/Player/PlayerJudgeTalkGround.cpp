#include "Player/PlayerJudgeTalkGround.h"

PlayerJudgeTalkGround::PlayerJudgeTalkGround(
    const al::LiveActor* playerActor, const IPlayerModelChanger* playerModelChanger,
    const PlayerHackKeeper* playerHackKeeper, const PlayerCarryKeeper* playerCarryKeeper,
    const IUsePlayerCollision* playerCollision, const PlayerInput* playerInput,
    const PlayerConst* playerConst, const PlayerStateWait* playerStateWait)
    : mPlayerActor(playerActor), mPlayerModelChanger(playerModelChanger),
      mPlayerHackKeeper(playerHackKeeper), mPlayerCarryKeeper(playerCarryKeeper),
      mPlayerCollision(playerCollision), mPlayerInput(playerInput), mPlayerConst(playerConst),
      mPlayerStateWait(playerStateWait) {}

void PlayerJudgeTalkGround::reset() {}

void PlayerJudgeTalkGround::update() {}
