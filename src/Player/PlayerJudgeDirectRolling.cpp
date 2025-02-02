#include "Player/PlayerJudgeDirectRolling.h"

#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"

#include "Player/IPlayerModelChanger.h"
#include "Player/PlayerCarryKeeper.h"
#include "Player/PlayerConst.h"
#include "Player/PlayerCounterForceRun.h"
#include "Player/PlayerInput.h"
#include "Util/PlayerCollisionUtil.h"

PlayerJudgeDirectRolling::PlayerJudgeDirectRolling(
    const al::LiveActor* player, const PlayerConst* pConst, const PlayerInput* input,
    const IUsePlayerCollision* collider, const IPlayerModelChanger* modelChanger,
    const PlayerCarryKeeper* carryKeeper, const PlayerCounterForceRun* counterForceRun)
    : mPlayer(player), mConst(pConst), mInput(input), mCollider(collider),
      mModelChanger(modelChanger), mCarryKeeper(carryKeeper), mCounterForceRun(counterForceRun) {}

void PlayerJudgeDirectRolling::reset() {
    mIsJudge = false;
}

void PlayerJudgeDirectRolling::update() {
    mIsJudge = false;
    if (mCounterForceRun->isForceRun() || mCarryKeeper->isCarry() ||
        !rs::isOnGround(mPlayer, mCollider) || mModelChanger->is2DModel() || !mInput->isHoldSquat())
        return;
    sead::Vector3f horizontalSpeed = {0.0f, 0.0f, 0.0f};
    al::verticalizeVec(&horizontalSpeed, al::getGravity(mPlayer), al::getVelocity(mPlayer));
    mIsJudge = horizontalSpeed.length() >= mConst->getDashJudgeSpeed();
}

bool PlayerJudgeDirectRolling::judge() const {
    return mIsJudge;
}
