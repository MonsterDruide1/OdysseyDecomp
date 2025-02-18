#include "Player/PlayerCapActionHistory.h"

#include "Library/LiveActor/ActorPoseUtil.h"

#include "Player/PlayerConst.h"
#include "Player/PlayerCounterAfterCapCatch.h"
#include "Util/PlayerCollisionUtil.h"

PlayerCapActionHistory::PlayerCapActionHistory(const al::LiveActor* player,
                                               const PlayerConst* pConst,
                                               const PlayerTrigger* trigger,
                                               const IUsePlayerCollision* collider)
    : mPlayer(player), mConst(pConst), mTrigger(trigger), mCollision(collider),
      mCounterAfterCapCatch(new PlayerCounterAfterCapCatch(pConst, trigger)) {}

void PlayerCapActionHistory::update() {
    mCounterAfterCapCatch->update(mTrigger);
}

void PlayerCapActionHistory::clearLandLimit() {
    clearLimitHeight();
    clearCapJump();
    _39 = true;
}

void PlayerCapActionHistory::clearLimitHeight() {
    mIsLimitHeight = false;
}

void PlayerCapActionHistory::clearCapJump() {
    mIsCapJumpPossible = true;
}

void PlayerCapActionHistory::clearLandLimitStandAngle() {
    clearLimitHeight();
    _39 = true;
    if (rs::isOnGroundLessAngle(mPlayer, mCollision, mConst->getStandAngleMin()))
        clearCapJump();
}

void PlayerCapActionHistory::clearWallAirLimit() {
    clearLimitHeight();
    _39 = true;
}

void PlayerCapActionHistory::recordLimitHeight() {
    if (rs::isCollidedGround(mCollision))
        return;
    mIsLimitHeight = true;
    mHeightLimit = al::getTrans(mPlayer);
}

bool PlayerCapActionHistory::isOverLimitHeight() const {
    if (!mIsLimitHeight || rs::isCollidedGround(mCollision))
        return false;

    return (al::getTrans(mPlayer) - mHeightLimit).dot(al::getGravity(mPlayer)) < 0.0f;
}
