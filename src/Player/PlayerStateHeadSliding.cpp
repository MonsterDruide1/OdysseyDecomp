#include "Player/PlayerStateHeadSliding.h"

#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Player/PlayerActionDiveInWater.h"
#include "Player/PlayerAnimator.h"
#include "Player/PlayerConst.h"
#include "Player/PlayerInput.h"
#include "Util/ObjUtil.h"
#include "Util/PlayerCollisionUtil.h"

namespace {
NERVE_IMPL(PlayerStateHeadSliding, Dive);
NERVES_MAKE_NOSTRUCT(PlayerStateHeadSliding, Dive);
}  // namespace

PlayerStateHeadSliding::PlayerStateHeadSliding(al::LiveActor* player, const PlayerConst* pConst,
                                               const IUsePlayerCollision* collider,
                                               const PlayerInput* input,
                                               const PlayerActionDiveInWater* actionDiveInWater,
                                               PlayerAnimator* animator)
    : al::ActorStateBase("ヘッドスライディング", player), mConst(pConst), mCollider(collider),
      mInput(input), mActionDiveInWater(actionDiveInWater), mAnimator(animator) {
    initNerve(&Dive, 0);
}

void PlayerStateHeadSliding::appear() {
    mIsEnableDiveInWater = false;
    setDead(false);
    al::setNerve(this, &Dive);
}

void PlayerStateHeadSliding::kill() {
    setDead(true);
}

bool PlayerStateHeadSliding::isEnableDiveInWater() const {
    return mIsEnableDiveInWater;
}

void PlayerStateHeadSliding::exeDive() {
    al::LiveActor* actor = mActor;
    const sead::Vector3f& gravity = al::getGravity(actor);
    if (al::isFirstStep(this)) {
        mAnimator->startAnim("HeadSlidingStart");
        sead::Vector3f velH = {0.0f, 0.0f, 0.0f};
        sead::Vector3f velV = {0.0f, 0.0f, 0.0f};
        al::separateVectorParallelVertical(&velV, &velH, gravity, al::getVelocity(actor));
        if (!al::tryNormalizeOrZero(&velH))
            al::calcFrontDir(&velH, actor);

        if (velV.dot(gravity) > 0.0f)
            velV = {0.0f, 0.0f, 0.0f};

        velV.setScaleAdd(-mConst->getHeadSlidingJump(), gravity, velV);
        al::setVelocity(actor, (mConst->getHeadSlidingSpeed() * velH) + velV);
    }

    sead::Vector3f moveInput = {0.0f, 0.0f, 0.0f};
    mInput->calcMoveInput(&moveInput, -gravity);
    rs::moveDivingJump(actor, moveInput, 0.0f, mConst->getHeadSlidingBrake(),
                       mConst->getHeadSlidingSpeed(), mConst->getHeadSlidingSpeedMin(),
                       mConst->getHeadSlidingSideAccel(), mConst->getHeadSlidingGravityAir(),
                       mConst->getFallSpeedMax(), mConst->getSlerpQuatGrav());

    if (mAnimator->isAnim("HeadSlidingStart") && mAnimator->isAnimEnd())
        mAnimator->startAnim("HeadSliding");

    mIsEnableDiveInWater |= mActionDiveInWater->judgeEnableDiveInWater();
    if (rs::isOnGround(actor, mCollider))
        kill();
}
