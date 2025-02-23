#include "Player/PlayerActionDiveInWater.h"

#include "Player/IPlayerModelChanger.h"
#include "Player/PlayerAnimator.h"
#include "Player/PlayerCarryKeeper.h"
#include "Player/PlayerJudgeDiveInWater.h"
#include "Util/JudgeUtil.h"

PlayerActionDiveInWater::PlayerActionDiveInWater(PlayerAnimator* animator,
                                                 const IPlayerModelChanger* modelChanger,
                                                 const PlayerCarryKeeper* carryKeeper,
                                                 const PlayerJudgeDiveInWater* judgeDiveInWater)
    : mAnimator(animator), mModelChanger(modelChanger), mCarryKeeper(carryKeeper),
      mJudgeDiveInWater(judgeDiveInWater) {}

bool PlayerActionDiveInWater::tryChangeDiveInWaterAnim() {
    if (!judgeEnableDiveInWater())
        return false;

    mAnimator->startAnim("DiveInWater");
    return true;
}

bool PlayerActionDiveInWater::judgeEnableDiveInWater() const {
    return !mModelChanger->is2DModel() && !mCarryKeeper->isCarry() &&
           rs::isJudge(mJudgeDiveInWater) && !isDiveInWaterAnim();
}

bool PlayerActionDiveInWater::isDiveInWaterAnim() const {
    return mAnimator->isAnim("DiveInWater");
}
