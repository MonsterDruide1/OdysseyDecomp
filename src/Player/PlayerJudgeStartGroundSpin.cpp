#include "Player/PlayerJudgeStartGroundSpin.h"

#include "Player/PlayerInput.h"
#include "Util/PlayerCollisionUtil.h"

PlayerJudgeStartGroundSpin::PlayerJudgeStartGroundSpin(const al::LiveActor* player,
                                                       const IUsePlayerCollision* collider,
                                                       const PlayerInput* input)
    : mPlayer(player), mCollider(collider), mInput(input) {}

void PlayerJudgeStartGroundSpin::reset() {}

void PlayerJudgeStartGroundSpin::update() {}

bool PlayerJudgeStartGroundSpin::judge() const {
    return mInput->isSpinInput() && rs::isOnGround(mPlayer, mCollider);
}
