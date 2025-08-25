#include "Player/PlayerJudgeSleep.h"

#include "Player/PlayerStateWait.h"

PlayerJudgeSleep::PlayerJudgeSleep(const PlayerStateWait *pStateWait)
    : mPlayerStateWait(pStateWait) {}

bool PlayerJudgeSleep::judge() const { return mPlayerStateWait->isSleep(); }

void PlayerJudgeSleep::reset() {}

void PlayerJudgeSleep::update() {}