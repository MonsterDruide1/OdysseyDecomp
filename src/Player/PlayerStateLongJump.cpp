#include "Player/PlayerStateLongJump.h"

#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "PlayerUtil.h"
#include "Stuff.h"
#include "math/seadVectorFwd.h"

namespace {
NERVE_IMPL(PlayerStateLongJump, Jump);

NERVE_MAKE(PlayerStateLongJump, Jump);
}  // namespace

PlayerStateLongJump::PlayerStateLongJump(al::LiveActor* player, PlayerConst const* pConst,
                                         PlayerInput const* input,
                                         IUsePlayerCollision const* collider,
                                         PlayerTrigger* trigger, PlayerAnimator* animator,
                                         PlayerContinuousLongJump* continuousLongJump,
                                         PlayerActionDiveInWater* actionDiveInWater)
    : al::ActorStateBase("幅跳び", player), mConst(pConst), mInput(input), mCollision(collider),
      mTrigger(trigger), mAnimator(animator), mContinuousLongJump(continuousLongJump),
      mDiveInWater(actionDiveInWater) {
    initNerve(&Jump, 0);
}

PlayerStateLongJump::~PlayerStateLongJump() = default;

void PlayerStateLongJump::appear() {
    mIsDead = false;
    rs::noticePlayerJumpStart(mTrigger, mActor);
    al::setNerve(this, &Jump);
}

void PlayerStateLongJump::exeJump() {
    const sead::Vector3f& gravity = al::getGravity(mActor);
    if(al::isFirstStep(this)) {
        rs::setupLongJumpVelocity(mActor, mCollision, mConst->getJumpInertiaRate(), mConst->getLongJumpMovePow(), mConst->getLongJumpSpeedMin(), mConst->getLongJumpInitSpeed(), mConst->getLongJumpJumpPow());
        if(rs::isModeE3Rom()) {
            mAnimator->startAnim("JumpBroad3");
        } else {
            mAnimator->startAnim(mContinuousLongJump->getLongJumpAnimName());
        }
        mContinuousLongJump->countUp();
    }

    if(mTrigger->isOnUpperPunchHit()) {
        rs::reflectCeilingUpperPunch(mActor, mCollision, mInput, mConst, mTrigger, 0);
    }

    sead::Vector3f moveInput = {0.0f, 0.0f, 0.0f};
    mInput->calcMoveInput(&moveInput, -gravity);
    rs::moveDivingJump(mActor, moveInput, mConst->getLongJumpAccel(), mConst->getLongJumpBrake(), mConst->getLongJumpSpeed(), mConst->getLongJumpSpeedMin(), mConst->getLongJumpSideAccel(), mConst->getLongJumpGravity(), mConst->getFallSpeedMax(), mConst->getSlerpQuatGrav());
    mDiveInWater->tryChangeDiveInWaterAnim();
    if(rs::isOnGround(mActor, mCollision))
        kill();
}
