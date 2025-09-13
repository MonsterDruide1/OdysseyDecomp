#include "Player/PlayerStateLongJump.h"

#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Player/PlayerActionDiveInWater.h"
#include "Player/PlayerAnimator.h"
#include "Player/PlayerConst.h"
#include "Player/PlayerContinuousLongJump.h"
#include "Player/PlayerInput.h"
#include "Player/PlayerTrigger.h"
#include "Util/ObjUtil.h"
#include "Util/PlayerCollisionUtil.h"
#include "Util/SpecialBuildUtil.h"

namespace {
NERVE_IMPL(PlayerStateLongJump, Jump);

NERVES_MAKE_NOSTRUCT(PlayerStateLongJump, Jump);
}  // namespace

PlayerStateLongJump::PlayerStateLongJump(al::LiveActor* player, const PlayerConst* pConst,
                                         const PlayerInput* input,
                                         const IUsePlayerCollision* collider,
                                         PlayerTrigger* trigger, PlayerAnimator* animator,
                                         PlayerContinuousLongJump* continuousLongJump,
                                         PlayerActionDiveInWater* actionDiveInWater)
    : al::ActorStateBase("幅跳び", player), mConst(pConst), mInput(input), mCollision(collider),
      mTrigger(trigger), mAnimator(animator), mContinuousLongJump(continuousLongJump),
      mDiveInWater(actionDiveInWater) {
    initNerve(&Jump, 0);
}

void PlayerStateLongJump::appear() {
    al::ActorStateBase::appear();
    rs::noticePlayerJumpStart(mTrigger, mActor);
    al::setNerve(this, &Jump);
}

void PlayerStateLongJump::exeJump() {
    al::LiveActor* actor = mActor;
    const sead::Vector3f& gravity = al::getGravity(actor);

    if (al::isFirstStep(this)) {
        rs::setupLongJumpVelocity(mActor, mCollision, mConst->getJumpInertiaRate(),
                                  mConst->getLongJumpMovePow(), mConst->getLongJumpSpeedMin(),
                                  mConst->getLongJumpInitSpeed(), mConst->getLongJumpJumpPow());
        if (rs::isModeE3MovieRom())
            mAnimator->startAnim("JumpBroad3");
        else
            mAnimator->startAnim(mContinuousLongJump->getLongJumpAnimName());
        mContinuousLongJump->countUp();
    }

    if (mTrigger->isOnUpperPunchHit())
        rs::reflectCeilingUpperPunch(mActor, mCollision, mInput, mConst, mTrigger, false);

    if (rs::isCollidedCeiling(mCollision))
        rs::reflectCeiling(mActor, 0.0f);

    sead::Vector3f moveInput = {0.0f, 0.0f, 0.0f};
    mInput->calcMoveInput(&moveInput, -gravity);
    rs::moveDivingJump(mActor, moveInput, mConst->getLongJumpAccel(), mConst->getLongJumpBrake(),
                       mConst->getLongJumpSpeed(), mConst->getLongJumpSpeedMin(),
                       mConst->getLongJumpSideAccel(), mConst->getLongJumpGravity(),
                       mConst->getFallSpeedMax(), mConst->getSlerpQuatGrav());
    mDiveInWater->tryChangeDiveInWaterAnim();
    if (rs::isOnGround(actor, mCollision))
        kill();
}
