#include "Player/PlayerStateHeadSliding.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Math/MathAngleUtil.h"
#include "Library/Math/MathLengthUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Stuff.h"

namespace {
NERVE_IMPL(PlayerStateHeadSliding, Dive);
NERVE_MAKE(PlayerStateHeadSliding, Dive);
}

PlayerStateHeadSliding::PlayerStateHeadSliding(al::LiveActor* player, PlayerConst const* pConst,
                                               IUsePlayerCollision const* collider,
                                               PlayerInput const* input,
                                               PlayerActionDiveInWater const* actionDiveInWater,
                                               PlayerAnimator* animator)
    : al::ActorStateBase("ヘッドスライディング", player), mConst(pConst), mCollider(collider), mInput(input),
      mActionDiveInWater(actionDiveInWater), mAnimator(animator) {
    initNerve(&Dive, 0);
}
PlayerStateHeadSliding::~PlayerStateHeadSliding() = default;

void PlayerStateHeadSliding::appear() {
    mIsEnableDiveInWater = false;
    mIsDead = false;
    al::setNerve(this, &Dive);
}
void PlayerStateHeadSliding::kill() {
    mIsDead = true;
}

bool PlayerStateHeadSliding::isEnableDiveInWater() const {
    return mIsEnableDiveInWater;
}

void PlayerStateHeadSliding::exeDive() {
    sead::Vector3f gravity = al::getGravity(mActor);
    if(al::isFirstStep(this)) {
        mAnimator->startAnim("HeadSlidingStart");
        sead::Vector3f a1 = {0.0f, 0.0f, 0.0f};
        sead::Vector3f v27 = {0.0f, 0.0f, 0.0f};
        al::separateVectorParallelVertical(&v27, &a1, gravity, al::getVelocity(mActor));
        if(!al::tryNormalizeOrZero(&a1))
            al::calcFrontDir(&a1, mActor);

        if(v27.dot(gravity) > 0.0f)
            v27 = {0.0f, 0.0f, 0.0f};

        v27 -= gravity * mConst->getHeadSlidingJump();
        al::setVelocity(mActor, (mConst->getHeadSlidingSpeed() * a1) + v27);
    }

    sead::Vector3f moveInput = {0.0f, 0.0f, 0.0f};
    mInput->calcMoveInput(&moveInput, -gravity);
    rs::moveDivingJump(mActor, moveInput, 0.0f, mConst->getHeadSlidingBrake(), mConst->getHeadSlidingSpeed(), mConst->getHeadSlidingSpeedMin(),
        mConst->getHeadSlidingSideAccel(), mConst->getHeadSlidingGravityAir(), mConst->getFallSpeedMax(), mConst->getSlerpQuatGrav());

    if(mAnimator->isAnim("HeadSlidingStart") && mAnimator->isAnimEnd())
        mAnimator->startAnim("HeadSliding");

    mIsEnableDiveInWater = mActionDiveInWater->judgeEnableDiveInWater();
    if(rs::isOnGround(mActor, mCollider))
        kill();
}
