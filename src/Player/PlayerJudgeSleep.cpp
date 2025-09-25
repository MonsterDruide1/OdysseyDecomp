#include "Player/PlayerJudgeSleep.h"

#include "Player/PlayerStateWait.h"

PlayerJudgeSleep::PlayerJudgeSleep(const PlayerStateWait* stateWait) : mStateWait(stateWait) {}

bool PlayerJudgeSleep::judge() const {
    return mStateWait->isSleep();
}
