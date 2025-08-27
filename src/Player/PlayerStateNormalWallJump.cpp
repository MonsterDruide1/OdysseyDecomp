#include "Player/PlayerStateNormalWallJump.h"

#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Player/PlayerActionAirMoveControl.h"
#include "Player/PlayerActionDiveInWater.h"
#include "Player/PlayerAnimator.h"
#include "Player/PlayerConst.h"
#include "Player/PlayerTrigger.h"
#include "Util/ObjUtil.h"
#include "Util/PlayerCollisionUtil.h"

namespace {
NERVE_IMPL(PlayerStateNormalWallJump, Jump)

NERVES_MAKE_NOSTRUCT(PlayerStateNormalWallJump, Jump)
}  // namespace

PlayerStateNormalWallJump::PlayerStateNormalWallJump(
    al::LiveActor* player, const PlayerConst* pConst, const PlayerInput* input,
    const IUsePlayerCollision* collider, const PlayerTrigger* trigger, PlayerAnimator* animator,
    PlayerActionDiveInWater* actionDiveInWater)
    : al::ActorStateBase("壁ジャンプ", player), mConst(pConst), mCollider(collider), mInput(input),
      mTrigger(trigger), mAnimator(animator), mActionDiveInWater(actionDiveInWater) {
    mAirMoveControl = new PlayerActionAirMoveControl(player, pConst, input, collider, false);
    initNerve(&Jump, 0);
}

void PlayerStateNormalWallJump::appear() {
    al::ActorStateBase::appear();
    al::calcFrontDir(&mFront, mActor);
    al::setNerve(this, &Jump);
}

void PlayerStateNormalWallJump::exeJump() {
    al::LiveActor* player = mActor;
    if (al::isFirstStep(this)) {
        mAnimator->startAnim(mAnimationName);

        sead::Vector3f front = {0.0f, 0.0f, 0.0f};
        al::calcFrontDir(&front, mActor);
        if (!mIsJumpTowardsWall)
            front.negate();
        al::setVelocity(mActor, mConst->getWallJumpHSpeed() * front);

        sead::Vector3f negGravity = -al::getGravity(mActor);
        sead::Quatf quat = sead::Quatf::unit;
        if (al::isParallelDirection(front, negGravity, 0.01f))
            al::calcUpDir(&front, mActor);
        al::makeQuatFrontUp(&quat, front, negGravity);
        al::updatePoseQuat(mActor, quat);

        mAirMoveControl->setup(100.0f, mConst->getWallJumpHSpeed(), 0, mConst->getWallJumpPower(),
                               mConst->getWallJumpGravity(),
                               mConst->getWallJumpInvalidateInputFrame(), 0.0f);
    }

    if (mTrigger->isOnUpperPunchHit())
        rs::reflectCeilingUpperPunch(mActor, mCollider, mInput, mConst, mTrigger, false);

    if (rs::isCollidedCeiling(mCollider))
        rs::reflectCeiling(mActor, 0.0f);

    mAirMoveControl->update();

    if (mAnimator->isAnim(mAnimationName) && mAnimator->isAnimEnd())
        mAnimator->startAnim("Fall");
    else if (mAnimator->isAnim("Fall"))
        mActionDiveInWater->tryChangeDiveInWaterAnim();

    if (rs::isOnGround(player, mCollider))
        kill();
}
