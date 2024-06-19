#include "Player/PlayerJudgeStartRolling.h"

#include "Player/IPlayerModelChanger.h"
#include "Player/PlayerCarryKeeper.h"
#include "Player/PlayerInput.h"
#include "Util/PlayerCollisionUtil.h"

PlayerJudgeStartRolling::PlayerJudgeStartRolling(const al::LiveActor* player,
                                                 const PlayerConst* pConst,
                                                 const PlayerInput* input,
                                                 const IUsePlayerCollision* collider,
                                                 const IPlayerModelChanger* modelChanger,
                                                 const PlayerCarryKeeper* carryKeeper)
    : mPlayer(player), mConst(pConst), mInput(input), mCollider(collider),
      mModelChanger(modelChanger), mCarryKeeper(carryKeeper) {}

bool PlayerJudgeStartRolling::isEnableTriggerRolling() const {
    return !mCarryKeeper->isCarry() && !mModelChanger->is2DModel() &&
           rs::isCollidedGround(mCollider) &&
           !rs::isOnGroundForceSlideCode(mPlayer, mCollider, mConst);
}

bool PlayerJudgeStartRolling::judgeCancelHipDrop() const {
    return isEnableTriggerRolling() && mInput->isTriggerRollingCancelHipDrop(
                                           rs::isOnGroundForceRollingCode(mPlayer, mCollider));
}

bool PlayerJudgeStartRolling::isTriggerRestartSwing() const {
    return isEnableTriggerRolling() && mInput->isTriggerRollingRestartSwing();
}

void PlayerJudgeStartRolling::reset() {}
void PlayerJudgeStartRolling::update() {}
bool PlayerJudgeStartRolling::judge() const {
    return isEnableTriggerRolling() &&
           mInput->isTriggerRolling(rs::isOnGroundForceRollingCode(mPlayer, mCollider));
}
