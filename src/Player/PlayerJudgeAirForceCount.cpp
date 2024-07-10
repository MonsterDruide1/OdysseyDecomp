#include "Player/PlayerJudgeAirForceCount.h"

#include "Library/Math/MathUtil.h"

#include "Player/PlayerExternalVelocity.h"
#include "Util/PlayerCollisionUtil.h"

PlayerJudgeAirForceCount::PlayerJudgeAirForceCount(const al::LiveActor* player,
                                                   const PlayerExternalVelocity* externalVelocity,
                                                   const IUsePlayerCollision* collider)
    : mPlayer(player), mExternalVelocity(externalVelocity), mCollider(collider) {}

void PlayerJudgeAirForceCount::reset() {
    mCounterAirForce = 0;
}

void PlayerJudgeAirForceCount::update() {
    if (rs::isOnGround(mPlayer, mCollider) || !mExternalVelocity->isExistForce()) {
        reset();
        return;
    }

    mCounterAirForce = al::converge(mCounterAirForce, 40, 1);
}

bool PlayerJudgeAirForceCount::judge() const {
    return mCounterAirForce >= 40;
}
