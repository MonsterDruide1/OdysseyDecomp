#include "Player/PlayerStateSquat.h"

#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Math/MathAngleUtil.h"
#include "Library/Math/MathLengthUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Player/PlayerActionTurnControl.h"
#include "Player/PlayerActionVelocityControl.h"
#include "PlayerUtil.h"
#include "Player/PlayerTrigger.h"
#include "Player/PlayerInput.h"
#include "CUSTOM/PlayerAnimator.h"
#include "Stuff.h"
#include "Util/StageSceneFunction.h"
#include "math/seadVectorFwd.h"

namespace {
NERVE_IMPL(PlayerStateSquat, Brake);
NERVE_IMPL(PlayerStateSquat, Wait);
NERVE_IMPL(PlayerStateSquat, Walk);
NERVE_IMPL(PlayerStateSquat, StandUp);

NERVE_MAKE(PlayerStateSquat, Brake);
NERVE_MAKE(PlayerStateSquat, Wait);
NERVE_MAKE(PlayerStateSquat, Walk);
NERVE_MAKE(PlayerStateSquat, StandUp);
} // namespace

PlayerStateSquat::PlayerStateSquat(al::LiveActor* player, PlayerConst const* pConst,
                                   PlayerAnimator* animator, PlayerInput const* input,
                                   IUsePlayerCollision const* collider,
                                   IPlayerModelChanger const* modelChanger,
                                   PlayerTrigger const* trigger, IJudge* judgeStartSquat,
                                   IJudge* judgeStartRun, IJudge* judgeEnableStandUp)
    : al::ActorStateBase("しゃがみ", player), mConst(pConst), mAnimator(animator), mInput(input),
      mCollision(collider), mModelChanger(modelChanger), mTrigger(trigger),
      mJudgeStartSquat(judgeStartSquat), mJudgeStartRun(judgeStartRun),
      mJudgeEnableStandUp(judgeEnableStandUp) {
        mActionTurnControl = new PlayerActionTurnControl(player);
        mActionTurnControl->setup(mConst->getRoundMinDegree(), 180.0f, mConst->getSquatWalkTurnSpeed(), mConst->getSquatWalkTurnSpeed(), mConst->getSquatWalkTurnFrame(), 1, mConst->getSquatWalkTurnFrame());
        mActionTurnControl->_88 = true;
        initNerve(&Wait, 0);
      }

PlayerStateSquat::~PlayerStateSquat() = default;

void PlayerStateSquat::appear() {
    mIsDead = false;
    someFlag = mAnimator->isAnim("RollingEnd") || mTrigger->isOn(PlayerTrigger::EActionTrigger_val12);
    if(mAnimator->unk2)
        mAnimator->endSubAnim();
    
    al::setNerve(this, &Brake);
}

bool PlayerStateSquat::isEnableLongJump() const {
    if(mIsDead) return false;

    if(!al::isNerve(this, &Brake) || mModelChanger->is2DModel()) return false;

    sead::Vector3f front = {0.0f, 0.0f, 0.0f};
    al::calcFrontDir(&front, mActor);
    return front.dot(al::getVelocity(mActor)) > 0.0f;
}

void PlayerStateSquat::setNerveStandUpOrKill() {
    if(mModelChanger->is2DModel())
        kill();
    else
        al::setNerve(this, &StandUp);
}

void PlayerStateSquat::fixPressRumble() {
    if(rs::isCollidedGround(mCollision) && rs::isCollidedCeiling(mCollision)) {
        sead::Vector3f groundNormal = rs::getCollidedGroundNormal(mCollision);
        sead::Vector3f ceilingNormal = rs::getCollidedCeilingNormal(mCollision);
        if(al::isReverseDirection(groundNormal, ceilingNormal, 0.01f)) {
            f32 v7 = sead::Mathf::clampMin((rs::getCollidedGroundPos(mCollision) - al::getTrans(mActor)).dot(groundNormal), 0.0f);
            al::setTrans(mActor, al::getTrans(mActor) - (groundNormal * (v7 * 0.5f)));
        }
    }
}

void PlayerStateSquat::exeBrake() {
    sead::Vector3f up = {0.0f, 0.0f, 0.0f};
    rs::calcGroundNormalOrUpDir(&up, mActor, mCollision);
    bool isSkate = rs::isOnGroundSkateCode(mActor, mCollision);
    if(al::isFirstStep(this)) {
        if(mModelChanger->is2DModel() || someFlag)
            mAnimator->startAnim("SquatWait");
        else
            mAnimator->startAnim("SquatStart");
        
        sead::Vector3f v52 = {0.0f, 0.0f, 0.0f};
        sead::Vector3f v51 = {0.0f, 0.0f, 0.0f};
        al::separateVectorParallelVertical(&v51, &v52, up, al::getVelocity(mActor));

        sead::Vector3f a3 = {0.0f, 0.0f, 0.0f};
        al::calcFrontDir(&a3, mActor);
        al::verticalizeVec(&a3, up, a3);
        al::tryNormalizeOrZero(&a3);

        f32 dot = a3.dot(v52);
        if(!isSkate && dot < 0.0f) {
            v52 -= a3 * dot;
        }

        if(dot > 0.0f && v52.length() <= mConst->getNormalMaxSpeed()) {
            f32 squatAccelRate = mConst->getSquatAccelRate();
            f32 squatBrakeEndSpeed = mConst->getSquatBrakeEndSpeed();
            f32 v52_len = v52.length();
            if(v52_len > 0.0f) {
                f32 v21 = squatAccelRate * v52_len;
                if(v21 <= squatBrakeEndSpeed)
                    v21 = 0.0f;
                v52 = (v21 / v52_len) * v52;
            }
        }
        al::setVelocity(mActor, v52 + v51);
    }

    sead::Vector3f moveInput = {0.0f, 0.0f, 0.0f};
    mInput->calcMoveInput(&moveInput, up);

    sead::Vector3f inertiaSlide = {0.0f, 0.0f, 0.0f};
    if(isSkate) {
        CRASH
    } else {
        rs::moveInertiaSlide(&inertiaSlide, mActor, mCollision, moveInput, mConst->getSquatBrakeRate(), mConst->getSquatBrakeSideAccel(),
                             mConst->getSquatBrakeSideRate(), mConst->getSquatBrakeSideMaxSpeedRate(), mConst->getGravity(), mConst->getGravityAir(),
                             mConst->getFallSpeedMax());
    }

    rs::slerpUp(mActor, up, mConst->getSlerpQuatRate(), mConst->getHillPoseDegreeMax());
    if(inertiaSlide.length() <= mConst->getSquatBrakeEndSpeed()) {
        al::setVelocityZeroH(mActor);
        al::setNerve(this, &Wait);
        return;
    }

    if(!rs::updateJudgeAndResult(mJudgeStartSquat) && rs::updateJudgeAndResult(mJudgeEnableStandUp)) {
        if(mModelChanger->is2DModel()) {
            kill();
            return;
        }
        al::setNerve(this, &StandUp);
        return;
    }
}

void PlayerStateSquat::exeWait() {
    if(al::isFirstStep(this)) {
        mAnimator->startAnim("SquatWait");
    }

    fixPressRumble();

    rs::waitGround(mActor, mCollision, mConst->getGravity(), mConst->getFallSpeedMax(), mConst->getSlerpQuatRate(), mConst->getWaitPoseDegreeMax());

    if(!rs::updateJudgeAndResult(mJudgeStartSquat) && rs::updateJudgeAndResult(mJudgeEnableStandUp)) {
        if(mModelChanger->is2DModel()) {
            kill();
            return;
        }
        al::setNerve(this, &StandUp);
        return;
    }

    if(rs::updateJudgeAndResult(mJudgeStartRun) && !mModelChanger->is2DModel()) {
        al::setNerve(this, &Walk);
        return;
    }
}

void PlayerStateSquat::exeWalk() {
    if(al::isFirstStep(this)) {
        mAnimator->startAnim("SquatWalk");
        mActionTurnControl->reset();
    }

    sead::Vector3f up = {0.0f, 0.0f, 0.0f};
    rs::calcGroundNormalOrUpDir(&up, mActor, mCollision);
    if(rs::isCollidedGround(mCollision)) {
        PlayerActionVelocityControl velocityControl = PlayerActionVelocityControl(mActor, mCollision);
        velocityControl.calcOnGround(up);
        velocityControl.apply();
    }

    if(!rs::updateJudgeAndResult(mJudgeStartSquat) && rs::updateJudgeAndResult(mJudgeEnableStandUp)) {
        if(mModelChanger->is2DModel())
            kill();
        else
            al::setNerve(this, &StandUp);
    }

    if(mInput->isMove()) {
        sead::Vector3f v22 = {0.0f, 0.0f, 0.0f};
        sead::Vector3f a1 = {0.0f, 0.0f, 0.0f};
        sead::Vector3f a2 = {0.0f, 0.0f, 0.0f};
        mInput->calcMoveInput(&a2, up);
        mActionTurnControl->update(a2, up);
        
        v22 = mActionTurnControl->_5c;
        if(!mActionTurnControl->_69 || !al::tryNormalizeOrZero(&a1, a2)) {
            a1 = v22;
        }

        sead::Vector3f v19 = al::getVelocity(mActor);
        sead::Vector3f v18 = {0.0f, 0.0f, 0.0f};
        sead::Vector3f v17 = {0.0f, 0.0f, 0.0f};
        al::separateVectorParallelVertical(&v17, &v18, up, v19);
        if(!al::isNearZero(a2, 0.001f)) {
            v18 = mConst->getSquatWalkSpeed() * a1;
        }

        v17 = up * -mConst->getGravityMove();
        al::setVelocity(mActor, v17 + v18);
        rs::slerpUpFront(mActor, up, v22, mConst->getSlerpQuatRate(), mConst->getHillPoseDegreeMax());
    } else {
        al::setVelocity(mActor, up * -mConst->getGravityMove());
        al::setNerve(this, &Wait);
    }
}
void PlayerStateSquat::exeStandUp() {
    if(al::isFirstStep(this)) {
        mAnimator->startAnim("SquatEnd");
        al::setVelocityZeroH(mActor);
    }

    rs::waitGround(mActor, mCollision, mConst->getGravity(), mConst->getFallSpeedMax(), mConst->getSlerpQuatRate(), mConst->getWaitPoseDegreeMax());

    if(rs::updateJudgeAndResult(mJudgeEnableStandUp)) {
        if(rs::updateJudgeAndResult(mJudgeStartRun) || mAnimator->isAnimEnd()) {
            kill();
            return;
        }
    } else {
        al::setNerve(this, &Brake);
    }
}
