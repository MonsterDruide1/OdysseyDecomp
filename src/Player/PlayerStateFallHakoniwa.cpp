#include "Player/PlayerStateFallHakoniwa.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Player/PlayerActionAirMoveControl.h"
#include "Player/PlayerConst.h"
#include "Player/PlayerJudgeInvalidateInputFall.h"
#include "Player/PlayerTrigger.h"
#include "Stuff.h"
#include "Util/ActorDimensionKeeper.h"
#include "Util/StageSceneFunction.h"

namespace {

NERVE_IMPL(PlayerStateFallHakoniwa, Fall);

NERVE_MAKE(PlayerStateFallHakoniwa, Fall);

}  // namespace

PlayerStateFallHakoniwa::PlayerStateFallHakoniwa(
    al::LiveActor* actor, const PlayerConst* pConst, const IUsePlayerCollision* collision,
    const PlayerInput* input, const PlayerTrigger* trigger, const PlayerAreaChecker* areaChecker,
    const IUseDimension* dimension, PlayerAnimator* animator)
    : ActorStateBase("落下", actor), mConst(pConst), mCollision(collision), mTrigger(trigger),
      mDimension(dimension), mAnimator(animator) {
    mJudgeInvalidateInputFall = new PlayerJudgeInvalidateInputFall(actor, areaChecker, mCollision);
    mAirMoveControl = new PlayerActionAirMoveControl(actor, pConst, input, collision, false);
    mAirMoveControl->setupCollideWallScaleVelocity(mConst->getFallWallScaleVelocity(), 0.0f,
                                                   mConst->getNormalMaxSpeed());
    initNerve(&Fall, 0);
}

PlayerStateFallHakoniwa::~PlayerStateFallHakoniwa() {
    CRASH
}
void PlayerStateFallHakoniwa::appear() {
    mIsDead = false;
    unk = {0.0f, 0.0f, 0.0f};
    flag1 = false;
    flag2 = false;
    gap1 = 0;
    gap2 = 0;
    if(mTrigger) {
        flag1 = mTrigger->isOn(PlayerTrigger::ECollisionTrigger_val7);
        flag2 = mTrigger->isOn(PlayerTrigger::EActionTrigger_val14);
        if(mTrigger->isOn(PlayerTrigger::EActionTrigger_val32)) {
            gap1 = 8;
            gap2 = 15;
            al::calcFrontDir(&unk, mActor);
        }
    }

    mMaxMoveSpeed = sead::Mathf::max3(al::calcSpeedH(mActor), mConst->getNormalMaxSpeed(), mConst->getJumpMoveSpeedMax());
    mInvalidateInputFallArea = nullptr;
    updateFallTargetArea();
    mAirMoveControl->mIs2D = rs::isPlayer2D(mActor);
    al::setNerve(this, &Fall);
}
void PlayerStateFallHakoniwa::updateFallTargetArea() {
    if(!mInvalidateInputFallArea) {
        if(!rs::updateJudgeAndResult(mJudgeInvalidateInputFall))
            return;
        mInvalidateInputFallArea = mJudgeInvalidateInputFall->mInvalidateInputFallArea;
    }
    CRASH
}
bool PlayerStateFallHakoniwa::isValidWallCatch() {
    if(mIsDead) return true;
    if(!gap2 || al::isGreaterStep(this, gap2)) return true;

    sead::Vector3f front = {0.0f, 0.0f, 0.0f};
    al::calcFrontDir(&front, mActor);
    return front.dot(unk) < 0.70711f;
}
void PlayerStateFallHakoniwa::exeFall() {
    updateFallTargetArea();
    if(al::isFirstStep(this)) {
        // animation stuff
        mAirMoveControl->setup(mConst->getJumpMoveSpeedMax(), mConst->getJumpMoveSpeedMin(), 0, al::calcSpeedV(mActor), mConst->getJumpGravity(), gap1, 0.0f);
    }
    if(rs::isPlayer2D(mActor) && rs::isIn2DArea(mDimension)) {
        CRASH
    }
    mAirMoveControl->update();
}
