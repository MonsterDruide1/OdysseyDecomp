#include "Player/PlayerJudgeTalkGround.h"

#include <math/seadVector.h>

#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"

#include "Player/IPlayerModelChanger.h"
#include "Player/PlayerCarryKeeper.h"
#include "Player/PlayerConst.h"
#include "Player/PlayerHackKeeper.h"
#include "Player/PlayerInput.h"
#include "Player/PlayerStateWait.h"
#include "Util/PlayerCollisionUtil.h"
#include "Util/PlayerUtil.h"

PlayerJudgeTalkGround::PlayerJudgeTalkGround(
    const al::LiveActor* playerActor, const IPlayerModelChanger* playerModelChanger,
    const PlayerHackKeeper* playerHackKeeper, const PlayerCarryKeeper* playerCarryKeeper,
    const IUsePlayerCollision* playerCollider, const PlayerInput* playerInput,
    const PlayerConst* playerConst, const PlayerStateWait* playerStateWait)
    : mPlayerActor(playerActor), mPlayerModelChanger(playerModelChanger),
      mPlayerHackKeeper(playerHackKeeper), mPlayerCarryKeeper(playerCarryKeeper),
      mCollider(playerCollider), mPlayerInput(playerInput), mPlayerConst(playerConst),
      mPlayerStateWait(playerStateWait) {}

bool PlayerJudgeTalkGround::judge() const {
    auto* currentHackActor = mPlayerHackKeeper->getHack();
    if (mPlayerHackKeeper->getHackSensor()) {
        if (!rs::isPlayerOnGround(currentHackActor) || mPlayerInput->isMove())
            return false;

        return !(mPlayerConst->getNormalMaxSpeed() * 0.65f < al::calcSpeedH(currentHackActor));
    }

    if (mPlayerModelChanger->is2DModel() || !rs::isOnGround(mPlayerActor, mCollider) ||
        rs::isJustLand(mCollider) ||
        (!mPlayerStateWait->isDead() && !mPlayerStateWait->isEnableCancelAction()) ||
        mPlayerInput->isMove() || mPlayerCarryKeeper->isThrowHold())
        return false;

    const sead::Vector3f& gravity = rs::isJustLand(mCollider) ?
                                        al::getGravity(mPlayerActor) :
                                        rs::getCollidedGroundNormal(mCollider);
    sead::Vector3f velocity = al::getVelocity(mPlayerActor);
    al::verticalizeVec(&velocity, gravity, velocity);
    return !(mPlayerConst->getNormalMaxSpeed() * 0.65f < velocity.length());
}
