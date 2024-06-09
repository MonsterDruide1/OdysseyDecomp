#include "Player/PlayerJudgeTalkGround.h"

#include <math/seadVector.h>

#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Math/VectorUtil.h"

#include "Player/IPlayerModelChanger.h"
#include "Player/PlayerCarryKeeper.h"
#include "Player/PlayerConst.h"
#include "Player/PlayerHackKeeper.h"
#include "Player/PlayerInput.h"
#include "Player/PlayerStateWait.h"
#include "Util/PlayerCollisionUtil.h"

PlayerJudgeTalkGround::PlayerJudgeTalkGround(
    const al::LiveActor* playerActor, const IPlayerModelChanger* playerModelChanger,
    const PlayerHackKeeper* playerHackKeeper, const PlayerCarryKeeper* playerCarryKeeper,
    const IUsePlayerCollision* playerCollider, const PlayerInput* playerInput,
    const PlayerConst* playerConst, const PlayerStateWait* playerStateWait)
    : mPlayerActor(playerActor), mPlayerModelChanger(playerModelChanger),
      mPlayerHackKeeper(playerHackKeeper), mPlayerCarryKeeper(playerCarryKeeper),
      mCollider(playerCollider), mPlayerInput(playerInput), mPlayerConst(playerConst),
      mPlayerStateWait(playerStateWait) {}

void PlayerJudgeTalkGround::reset() {}

void PlayerJudgeTalkGround::update() {}

bool PlayerJudgeTalkGround::judge() const {
    auto* currentHackActor = mPlayerHackKeeper->getCurrentHackActor();
    if (mPlayerHackKeeper->getUnkHitSensor()) {
        if (rs::isPlayerOnGround(currentHackActor) && !mPlayerInput->isMove()) {
            f32 maxSpeed = mPlayerConst->getNormalMaxSpeed() * 0.65f;
            f32 speedH = al::calcSpeedH(currentHackActor);
            return !(maxSpeed < speedH);
        }
    } else if (!mPlayerModelChanger->is2DModel() && rs::isOnGround(mPlayerActor, mCollider) &&
               !rs::isJustLand(mCollider)) {
        if ((mPlayerStateWait->isDead() || mPlayerStateWait->isEnableCancelAction()) &&
            !mPlayerInput->isMove() && !mPlayerCarryKeeper->isThrowHold()) {
            const sead::Vector3f& gravity = rs::isJustLand(mCollider) ?
                                                al::getGravity(mPlayerActor) :
                                                rs::getCollidedGroundNormal(mCollider);
            sead::Vector3f velocity = al::getVelocity(mPlayerActor);
            al::verticalizeVec(&velocity, gravity, velocity);
            return !(mPlayerConst->getNormalMaxSpeed() * 0.65f < velocity.length());
        }
    }
    return false;
}

/*
bool PlayerJudgeTalkGround::judge() const {
    auto* currentHackActor = mPlayerHackKeeper->getCurrentHackActor();
    if (!mPlayerHackKeeper->getUnkHitSensor()) {
        if (!mPlayerModelChanger->is2DModel() &&
               rs::isOnGround(mPlayerActor, mCollider) &&
               !rs::isJustLand(mCollider)) {
        if ((mPlayerStateWait->isDead() || mPlayerStateWait->isEnableCancelAction()) &&
            !mPlayerInput->isMove() && !mPlayerCarryKeeper->isThrowHold()) {
            const sead::Vector3f& gravity = rs::isJustLand(mCollider) ?
                                                al::getGravity(mPlayerActor) :
                                                rs::getCollidedGroundNormal(mCollider);
            sead::Vector3f velocity = al::getVelocity(mPlayerActor);
            al::verticalizeVec(&velocity, gravity, velocity);
            return !(mPlayerConst->getNormalMaxSpeed() * 0.65f < velocity.length());
        }

        }
    }
    if (rs::isPlayerOnGround(currentHackActor) && !mPlayerInput->isMove()) {
            f32 maxSpeed = mPlayerConst->getNormalMaxSpeed() * 0.65f;
            f32 speedH = al::calcSpeedH(currentHackActor);
            return !(maxSpeed < speedH);
        }
    return false;
}
*/
