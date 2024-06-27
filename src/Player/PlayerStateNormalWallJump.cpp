#include "Player/PlayerStateNormalWallJump.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Math/MathAngleUtil.h"
#include "Library/Math/VectorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Player/PlayerActionAirMoveControl.h"
#include "PlayerUtil.h"
#include "Stuff.h"
#include "math/seadQuat.h"

namespace {
NERVE_IMPL(PlayerStateNormalWallJump, Jump)

NERVE_MAKE(PlayerStateNormalWallJump, Jump)
}  // namespace

PlayerStateNormalWallJump::PlayerStateNormalWallJump(
    al::LiveActor* player, PlayerConst const* pConst, PlayerInput const* input,
    IUsePlayerCollision const* collider, PlayerTrigger const* trigger, PlayerAnimator* animator,
    PlayerActionDiveInWater* actionDiveInWater)
    : al::ActorStateBase("壁ジャンプ", player), mConst(pConst), mInput(input), mCollider(collider),
      mTrigger(trigger), mAnimator(animator), mActionDiveInWater(actionDiveInWater) {
    mAirMoveControl = new PlayerActionAirMoveControl(player, pConst, input, collider, false);
    initNerve(&Jump, 0);
}

PlayerStateNormalWallJump::~PlayerStateNormalWallJump() = default;

void PlayerStateNormalWallJump::appear() {
    mIsDead = false;
    al::calcFrontDir(&mFront, mActor);
    al::setNerve(this, &Jump);
}

void PlayerStateNormalWallJump::exeJump() {
    if(al::isFirstStep(this)) {
        mAnimator->startAnim(mAnimationName);
        sead::Vector3f front = {0.0f, 0.0f, 0.0f};
        al::calcFrontDir(&front, mActor);
        if(!_6C) {
            front = -front;
        }
        al::setVelocity(mActor, mConst->getWallJumpHSpeed() * front);

        sead::Vector3f negGravity = -al::getGravity(mActor);
        sead::Quatf quat = sead::Quatf::unit;
        if(al::isParallelDirection(front, negGravity, 0.01f))
            al::calcUpDir(&front, mActor);
        al::makeQuatFrontUp(&quat, front, negGravity);
        al::updatePoseQuat(mActor, quat);

        mAirMoveControl->setup(100.0f, mConst->getWallJumpHSpeed(), 0, mConst->getWallJumpPower(), mConst->getWallJumpGravity(), mConst->getWallJumpInvalidateInputFrame(), 0.0f);
    }

    if(mTrigger->isOnUpperPunchHit())
        rs::reflectCeilingUpperPunch(mActor, mCollider, mInput, mConst, mTrigger, false);

    if(rs::isCollidedCeiling(mCollider))
        rs::reflectCeiling(mActor, 0.0f);

    mAirMoveControl->update();

    if(mAnimator->isAnim(mAnimationName)) {
        if(mAnimator->isAnimEnd()) {
            mAnimator->startAnim("Fall");
        }
    } else {
        if(mAnimator->isAnim("Fall"))
            mActionDiveInWater->tryChangeDiveInWaterAnim();
    }

    if(rs::isOnGround(mActor, mCollider))
        kill();
}
