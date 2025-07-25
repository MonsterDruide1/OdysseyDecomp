#include "Player/PlayerStateSquat.h"

#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Player/IPlayerModelChanger.h"
#include "Player/PlayerActionTurnControl.h"
#include "Player/PlayerActionVelocityControl.h"
#include "Player/PlayerAnimator.h"
#include "Player/PlayerConst.h"
#include "Player/PlayerInput.h"
#include "Player/PlayerTrigger.h"
#include "Util/JudgeUtil.h"
#include "Util/ObjUtil.h"
#include "Util/PlayerCollisionUtil.h"

namespace {
NERVE_IMPL(PlayerStateSquat, Brake);
NERVE_IMPL(PlayerStateSquat, Wait);
NERVE_IMPL(PlayerStateSquat, Walk);
NERVE_IMPL(PlayerStateSquat, StandUp);

NERVES_MAKE_NOSTRUCT(PlayerStateSquat, Brake);
NERVES_MAKE_STRUCT(PlayerStateSquat, Wait, Walk, StandUp);
}  // namespace

PlayerStateSquat::PlayerStateSquat(al::LiveActor* player, const PlayerConst* pConst,
                                   PlayerAnimator* animator, const PlayerInput* input,
                                   const IUsePlayerCollision* collider,
                                   const IPlayerModelChanger* modelChanger,
                                   const PlayerTrigger* trigger, IJudge* judgeStartSquat,
                                   IJudge* judgeStartRun, IJudge* judgeEnableStandUp)
    : al::ActorStateBase("しゃがみ", player), mConst(pConst), mAnimator(animator), mInput(input),
      mCollision(collider), mModelChanger(modelChanger), mTrigger(trigger),
      mActionTurnControl(new PlayerActionTurnControl(player)), mJudgeStartSquat(judgeStartSquat),
      mJudgeStartRun(judgeStartRun), mJudgeEnableStandUp(judgeEnableStandUp) {
    const PlayerConst* lConst = mConst;
    mActionTurnControl->setup(lConst->getRoundMinDegree(), 180.0f, lConst->getSquatWalkTurnSpeed(),
                              lConst->getSquatWalkTurnSpeed(), lConst->getSquatWalkTurnFrame(), 1,
                              lConst->getSquatWalkTurnFrame());
    mActionTurnControl->set_88(true);

    initNerve(&NrvPlayerStateSquat.Wait, 0);
}

void PlayerStateSquat::appear() {
    al::ActorStateBase::appear();
    mIsSkipSquatStartAnim =
        mAnimator->isAnim("RollingEnd") || mTrigger->isOn(PlayerTrigger::EActionTrigger_val12);
    if (mAnimator->isSubAnimPlaying())
        mAnimator->endSubAnim();

    al::setNerve(this, &Brake);
}

bool PlayerStateSquat::isEnableLongJump() const {
    if (isDead())
        return false;

    if (!al::isNerve(this, &Brake) || mModelChanger->is2DModel())
        return false;

    sead::Vector3f front = {0.0f, 0.0f, 0.0f};
    al::calcFrontDir(&front, mActor);
    return front.dot(al::getVelocity(mActor)) > 0.0f;
}

void PlayerStateSquat::setNerveStandUpOrKill() {
    if (mModelChanger->is2DModel())
        kill();
    else
        al::setNerve(this, &NrvPlayerStateSquat.StandUp);
}

void PlayerStateSquat::fixPressRumble() {
    if (!rs::isCollidedGround(mCollision) || !rs::isCollidedCeiling(mCollision))
        return;

    const sead::Vector3f& groundNormal = rs::getCollidedGroundNormal(mCollision);
    const sead::Vector3f& ceilingNormal = rs::getCollidedCeilingNormal(mCollision);
    if (!al::isReverseDirection(groundNormal, ceilingNormal))
        return;

    al::LiveActor* actor = mActor;
    f32 distanceClippedIntoGround = sead::Mathf::clampMin(
        (rs::getCollidedGroundPos(mCollision) - al::getTrans(actor)).dot(groundNormal), 0.0f);
    // instead of pulling Mario up by half the current "clip distance",
    // it clips him further into the ground by this amount. This is probably correct, because being
    // pushed up by the ground will move Mario up even further, so in total, he is
    // moved upwards (out of the ground) by about half the clip distance.
    al::setTrans(actor, al::getTrans(actor) - (groundNormal * (distanceClippedIntoGround * 0.5f)));
}

void PlayerStateSquat::exeBrake() {
    sead::Vector3f up = {0.0f, 0.0f, 0.0f};
    rs::calcGroundNormalOrUpDir(&up, mActor, mCollision);
    bool isSkate = rs::isOnGroundSkateCode(mActor, mCollision);
    if (al::isFirstStep(this)) {
        if (mModelChanger->is2DModel() || mIsSkipSquatStartAnim)
            mAnimator->startAnim("SquatWait");
        else
            mAnimator->startAnim("SquatStart");

        sead::Vector3f velPlane = {0.0f, 0.0f, 0.0f};
        sead::Vector3f velUp = {0.0f, 0.0f, 0.0f};
        al::separateVectorParallelVertical(&velUp, &velPlane, up, al::getVelocity(mActor));

        sead::Vector3f front = {0.0f, 0.0f, 0.0f};
        al::calcFrontDir(&front, mActor);
        al::verticalizeVec(&front, up, front);
        al::tryNormalizeOrZero(&front);

        f32 dot = front.dot(velPlane);
        if (dot < 0.0f && !isSkate) {
            velPlane -= front * dot;
            dot = 0.0f;
        }

        f32 hSpeed = velPlane.length();
        if (hSpeed <= mConst->getNormalMaxSpeed() && dot > 0.0f) {
            f32 squatAccelRate = mConst->getSquatAccelRate();
            f32 squatBrakeEndSpeed = mConst->getSquatBrakeEndSpeed();

            f32 accelRateTimesHSpeed = squatAccelRate * hSpeed;
            if (accelRateTimesHSpeed <= squatBrakeEndSpeed)
                accelRateTimesHSpeed = 0.0f;

            f32 hSpeed2 = velPlane.length();
            if (hSpeed2 > 0.0f)
                velPlane *= (accelRateTimesHSpeed / hSpeed2);
        }
        al::setVelocity(mActor, velPlane + velUp);
    }

    sead::Vector3f moveInput = {0.0f, 0.0f, 0.0f};
    mInput->calcMoveInput(&moveInput, up);

    sead::Vector3f inertiaSlide = {0.0f, 0.0f, 0.0f};
    if (isSkate) {
        rs::moveInertiaSlideOnSkate(
            &inertiaSlide, mActor, mCollision, moveInput, mConst->getSquatBrakeRateOnSkate(),
            mConst->getSquatBrakeSideAccelOnSkate(), mConst->getSquatBrakeSideRateOnSkate(),
            mConst->getSquatBrakeSideMaxSpeedRate(), mConst->getGravity(), mConst->getGravityAir(),
            mConst->getFallSpeedMax());
    } else {
        rs::moveInertiaSlide(&inertiaSlide, mActor, mCollision, moveInput,
                             mConst->getSquatBrakeRate(), mConst->getSquatBrakeSideAccel(),
                             mConst->getSquatBrakeSideRate(),
                             mConst->getSquatBrakeSideMaxSpeedRate(), mConst->getGravity(),
                             mConst->getGravityAir(), mConst->getFallSpeedMax());
    }

    rs::slerpUp(mActor, up, mConst->getSlerpQuatRate(), mConst->getHillPoseDegreeMax());
    if (inertiaSlide.length() <= mConst->getSquatBrakeEndSpeed()) {
        al::setVelocityZeroH(mActor);
        al::setNerve(this, &NrvPlayerStateSquat.Wait);
        return;
    }

    if (!rs::updateJudgeAndResult(mJudgeStartSquat) &&
        rs::updateJudgeAndResult(mJudgeEnableStandUp)) {
        setNerveStandUpOrKill();
    }
}

void PlayerStateSquat::exeWait() {
    if (al::isFirstStep(this) && !mModelChanger->is2DModel())
        mAnimator->startAnim("SquatWait");

    fixPressRumble();

    rs::waitGround(mActor, mCollision, mConst->getGravity(), mConst->getFallSpeedMax(),
                   mConst->getSlerpQuatRate(), mConst->getWaitPoseDegreeMax());

    bool startSquat = rs::updateJudgeAndResult(mJudgeStartSquat);
    bool enableStandUp = rs::updateJudgeAndResult(mJudgeEnableStandUp);
    if (!startSquat && enableStandUp) {
        setNerveStandUpOrKill();
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeStartRun) && !mModelChanger->is2DModel())
        al::setNerve(this, &NrvPlayerStateSquat.Walk);
}

void PlayerStateSquat::exeWalk() {
    al::LiveActor* actor = mActor;
    if (al::isFirstStep(this)) {
        mAnimator->startAnim("SquatWalk");
        mActionTurnControl->reset();
    }

    sead::Vector3f up = {0.0f, 0.0f, 0.0f};
    rs::calcGroundNormalOrUpDir(&up, actor, mCollision);
    if (rs::isCollidedGround(mCollision)) {
        PlayerActionVelocityControl velocityControl = {actor, mCollision};
        velocityControl.calcOnGround(up);
        velocityControl.apply();
    }

    if (!rs::updateJudgeAndResult(mJudgeStartSquat) &&
        rs::updateJudgeAndResult(mJudgeEnableStandUp)) {
        setNerveStandUpOrKill();
        // BUG: missing return here => Squat state can affect for one more frame
    }

    if (!mInput->isMove()) {
        al::setVelocity(actor, up * -mConst->getGravityMove());
        al::setNerve(this, &NrvPlayerStateSquat.Wait);
        return;
    }

    sead::Vector3f unkDir = {0.0f, 0.0f, 0.0f};
    sead::Vector3f moveDir = {0.0f, 0.0f, 0.0f};
    sead::Vector3f moveInput = {0.0f, 0.0f, 0.0f};
    mInput->calcMoveInput(&moveInput, up);
    mActionTurnControl->update(moveInput, up);

    // TODO: rename these variables (unk and unkDir)
    bool unk = mActionTurnControl->get_69();
    unkDir.set(mActionTurnControl->get_5c());
    if (!unk || !al::tryNormalizeOrZero(&moveDir, moveInput))
        moveDir.set(unkDir);

    sead::Vector3f velocity = al::getVelocity(mActor);
    sead::Vector3f velPlane = {0.0f, 0.0f, 0.0f};
    sead::Vector3f velUp = {0.0f, 0.0f, 0.0f};
    al::separateVectorParallelVertical(&velUp, &velPlane, up, velocity);
    if (!al::isNearZero(moveInput, 0.001f))
        velPlane = mConst->getSquatWalkSpeed() * moveDir;

    velUp = up * -mConst->getGravityMove();
    al::setVelocity(mActor, velUp + velPlane);
    rs::slerpUpFront(mActor, up, unkDir, mConst->getSlerpQuatRate(),
                     mConst->getHillPoseDegreeMax());
}

void PlayerStateSquat::exeStandUp() {
    if (al::isFirstStep(this)) {
        mAnimator->startAnim("SquatEnd");
        al::setVelocityZeroH(mActor);
    }

    rs::waitGround(mActor, mCollision, mConst->getGravity(), mConst->getFallSpeedMax(),
                   mConst->getSlerpQuatRate(), mConst->getWaitPoseDegreeMax());

    if (!rs::updateJudgeAndResult(mJudgeEnableStandUp)) {
        al::setNerve(this, &Brake);
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeStartRun) || mAnimator->isAnimEnd())
        kill();
}
