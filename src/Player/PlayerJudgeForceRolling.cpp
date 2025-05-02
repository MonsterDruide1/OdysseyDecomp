#include "Player/PlayerJudgeForceRolling.h"

#include "Util/PlayerCollisionUtil.h"

PlayerJudgeForceRolling::PlayerJudgeForceRolling(const al::LiveActor* player,
                                                 const IUsePlayerCollision* collider)
    : mPlayer(player), mCollider(collider) {}

bool PlayerJudgeForceRolling::judge() const {
    return rs::isOnGroundForceRollingCode(mPlayer, mCollider);
}
