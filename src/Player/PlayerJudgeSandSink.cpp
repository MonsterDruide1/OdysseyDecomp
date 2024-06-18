#include "Player/PlayerJudgeSandSink.h"

#include "Player/PlayerSandSinkAffect.h"
#include "Util/PlayerCollisionUtil.h"

PlayerJudgeSandSink::PlayerJudgeSandSink(const IUsePlayerCollision* collider,
                                         const PlayerSandSinkAffect* sandSinkAffect)
    : mCollider(collider), mSandSinkAffect(sandSinkAffect) {}

void PlayerJudgeSandSink::reset() {}

void PlayerJudgeSandSink::update() {}

bool PlayerJudgeSandSink::judge() const {
    return mSandSinkAffect->isSink() ||
           (rs::isCollidedGround(mCollider) && rs::isCollisionCodeSandSink(mCollider));
}
