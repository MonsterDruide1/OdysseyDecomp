#include "Player/PlayerJudgeDead.h"

#include "Library/LiveActor/ActorFlagFunction.h"

#include "System/GameDataFunction.h"

PlayerJudgeDead::PlayerJudgeDead(const al::LiveActor* player) : mPlayer(player) {}

bool PlayerJudgeDead::judge() const {
    return GameDataFunction::isPlayerLifeZero(mPlayer) || al::isDead(mPlayer);
}
