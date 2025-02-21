#include "Player/PlayerStateHeadSliding.h"

#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
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
NERVE_MAKE(PlayerStateHeadSliding, Dive);
}  // namespace

PlayerStateHeadSliding::PlayerStateHeadSliding(al::LiveActor* player, const PlayerConst* pConst,
                                               const IUsePlayerCollision* collision,
                                               const PlayerInput* input,
                                               const PlayerActionDiveInWater* actionDiveInWater,
                                               PlayerAnimator* animator)
    : al::ActorStateBase("ヘッドスライディング", player), mConst(pConst), mCollision(collision),
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

void test(sead::Vector3f* vec, const sead::Vector3f& a, f32 b) {
    //*vec -= a * b;
    f32 x = b * a.x;
    vec->x = vec->x - x;
    vec->y = vec->y - a.y * b;
    vec->z = vec->z - a.z * b;
}

void PlayerStateHeadSliding::exeDive() {
    al::LiveActor* actor = mActor;
    const sead::Vector3f& gravity = al::getGravity(actor);
    if (al::isFirstStep(this)) {
        mAnimator->startAnim("HeadSlidingStart");
        sead::Vector3f a1 = {0.0f, 0.0f, 0.0f};
        sead::Vector3f v27 = {0.0f, 0.0f, 0.0f};
        al::separateVectorParallelVertical(&v27, &a1, gravity, al::getVelocity(actor));
        if (!al::tryNormalizeOrZero(&a1))
            al::calcFrontDir(&a1, actor);

        if (v27.dot(gravity) > 0.0f)
            v27 = {0.0f, 0.0f, 0.0f};

        test(&v27, gravity, mConst->getHeadSlidingJump());
        al::setVelocity(actor, (mConst->getHeadSlidingSpeed() * a1) + v27);
    }

    sead::Vector3f moveInput = {0.0f, 0.0f, 0.0f};
    mInput->calcMoveInput(&moveInput, -gravity);
    rs::moveDivingJump(actor, moveInput, 0.0f, mConst->getHeadSlidingBrake(),
                       mConst->getHeadSlidingSpeed(), mConst->getHeadSlidingSpeedMin(),
                       mConst->getHeadSlidingSideAccel(), mConst->getHeadSlidingGravityAir(),
                       mConst->getFallSpeedMax(), mConst->getSlerpQuatGrav());

    if (mAnimator->isAnim("HeadSlidingStart") && mAnimator->isAnimEnd())
        mAnimator->startAnim("HeadSliding");

    mIsEnableDiveInWater = mActionDiveInWater->judgeEnableDiveInWater();
    if (rs::isOnGround(actor, mCollision))
        kill();
}
