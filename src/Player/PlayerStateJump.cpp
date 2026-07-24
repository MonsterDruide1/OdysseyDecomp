#include "Player/PlayerStateJump.h"

#include "Library/Base/StringUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Player/HackCap.h"
#include "Player/IPlayerModelChanger.h"
#include "Player/PlayerActionAirMoveControl.h"
#include "Player/PlayerActionDiveInWater.h"
#include "Player/PlayerAnimator.h"
#include "Player/PlayerConst.h"
#include "Player/PlayerContinuousJump.h"
#include "Player/PlayerCounterForceRun.h"
#include "Player/PlayerInput.h"
#include "Player/PlayerTrigger.h"
#include "Util/ActorDimensionUtil.h"
#include "Util/JudgeUtil.h"
#include "Util/ObjUtil.h"
#include "Util/PlayerCollisionUtil.h"

namespace {
NERVE_IMPL(PlayerStateJump, Jump);
NERVE_IMPL(PlayerStateJump, HoveringJump2D);
NERVE_IMPL(PlayerStateJump, JumpSpinFlower);
NERVE_IMPL(PlayerStateJump, JumpBack);
NERVE_IMPL(PlayerStateJump, JumpTurn);
NERVE_IMPL(PlayerStateJump, JumpSpinGround);
NERVE_IMPL(PlayerStateJump, JumpSpinFlowerDownFall);
NERVE_IMPL(PlayerStateJump, JumpSpinGroundDownFall);
NERVE_IMPL(PlayerStateJump, Hovering2D);

NERVES_MAKE_STRUCT(PlayerStateJump, Jump, HoveringJump2D, JumpSpinFlower, JumpBack, JumpTurn,
                   JumpSpinGround, JumpSpinFlowerDownFall, JumpSpinGroundDownFall, Hovering2D);

static bool isIn2DAreaModel(const IPlayerModelChanger* modelChanger, const IUseDimension* dimension) {
    return modelChanger->is2DModel() && rs::isIn2DArea(dimension);
}

static f32 getNormalMaxSpeed(const PlayerConst* pConst, const IPlayerModelChanger* modelChanger) {
    return modelChanger->is2DModel() ? pConst->getNormalMaxSpeed() : pConst->getNormalMaxSpeed2D();
}
}  // namespace

PlayerStateJump::PlayerStateJump(al::LiveActor* player, const PlayerConst* pConst,
                                 const IPlayerModelChanger* modelChanger,
                                 const IUseDimension* dimension, const PlayerInput* input,
                                 const PlayerJumpMessageRequest* jumpMessageRequest,
                                 const IJudge* judgeCapCatchJump,
                                 const PlayerCounterForceRun* counterForceRun,
                                 const PlayerJointControlKeeper* jointControlKeeper,
                                 const IUsePlayerCollision* collider, PlayerTrigger* trigger,
                                 PlayerContinuousJump* continuousJump, PlayerAnimator* animator,
                                 PlayerActionDiveInWater* actionDiveInWater, HackCap* hackCap,
                                 IJudge* judgeWallCatch, bool isEnableCapJumpSlowly)
    : al::ActorStateBase("ジャンプ", player), mConst(pConst), mModelChanger(modelChanger),
      mDimension(dimension), mJumpMessageRequest(jumpMessageRequest),
      mJudgeCapCatchJump(judgeCapCatchJump), mCounterForceRun(counterForceRun),
      mJointControlKeeper(jointControlKeeper), mCollider(collider), mAnimator(animator),
      mContinuousJump(continuousJump), mTrigger(trigger), mDiveInWater(actionDiveInWater),
      mHackCap(hackCap), mJudgeWallCatch(judgeWallCatch),
      mIsEnableCapJumpSlowly(isEnableCapJumpSlowly), mInput(input) {
    mAirMoveControl = new PlayerActionAirMoveControl(player, pConst, input, collider, false);
    mAirMoveControl->set_40(true);
    mAirMoveControl->set_42(true);
    mAirMoveControl->set_44(0);
    mAirMoveControl->set_48(pConst->getJumpBaseSpeedMax());
    initNerve(&NrvPlayerStateJump.Jump, 0);
}

f32 PlayerStateJump::calcJumpPowerBorderSpeedMin() const {
    const IUseDimension* dimension = mDimension;
    bool is2D = mModelChanger->is2DModel() && rs::isIn2DArea(dimension);
    return is2D ? mConst->getJumpPowerMinBorder2D() : mConst->getNormalMinSpeed();
}

f32 PlayerStateJump::calcJumpPowerBorderSpeedMax() const {
    const IUseDimension* dimension = mDimension;
    bool is2D = mModelChanger->is2DModel() && rs::isIn2DArea(dimension);
    return is2D ? mConst->getJumpPowerMaxBorder2D() : mConst->getNormalMaxSpeed();
}

f32 PlayerStateJump::calcJumpPowerMin() const {
    const IUseDimension* dimension = mDimension;
    if (mModelChanger->is2DModel() && rs::isIn2DArea(dimension))
        return mConst->getJumpPowerMin2DArea();
    if (mCounterForceRun->isForceRun())
        return mConst->getJumpPowerForceRun();
    switch (mContinuousJumpCount) {
    case 0:
        return mConst->getJumpPowerMin();
    case 1:
        return mConst->getContinuousJumpPowerMin();
    case 2:
        return mConst->getContinuousJumpPowerMin();
    default:
        return mConst->getJumpPowerMax();
    }
}

f32 PlayerStateJump::calcJumpPowerMax() const {
    const IUseDimension* dimension = mDimension;
    if (mModelChanger->is2DModel() && rs::isIn2DArea(dimension))
        return mConst->getJumpPowerMax2DArea();
    if (mCounterForceRun->isForceRun())
        return mConst->getJumpPowerForceRun();
    switch (mContinuousJumpCount) {
    case 1:
        return mConst->getJumpPowerMax2nd();
    case 2:
        return mConst->getJumpPowerMax3rd();
    default:
        return mConst->getJumpPowerMax();
    }
}

f32 PlayerStateJump::calcJumpGravity() const {
    if (mCounterForceRun->isForceRun())
        return mConst->getJumpGravityForceRun();
    switch (mContinuousJumpCount) {
    case 1:
        return mConst->getJumpGravity2nd();
    case 2:
        return mConst->getJumpGravity3rd();
    default:
        return mConst->getJumpGravity();
    }
}

void PlayerStateJump::tryCountUpContinuousJump(PlayerContinuousJump* continuousJump) {
    if (!mIsContinuousJump || mModelChanger->is2DModel()) {
        continuousJump->clear();
        return;
    }

    sead::Vector3f jumpDir = {0.0f, 0.0f, 0.0f};
    al::verticalizeVec(&jumpDir, al::getGravity(mActor), al::getVelocity(mActor));
    al::tryNormalizeOrZero(&jumpDir);

    if (continuousJump->getCount() != 0) {
        f32 jumpPower = mJumpPower;
        f32 jumpPowerBorder = mConst->getJumpPowerMax() * 0.99f;
        f32 dot = jumpDir.dot(continuousJump->getLastJumpDir());
        // 6.123032e-17f is cos(90°), 0.70710678f is cos(45°)
        bool isInAngle = mIsEnableCapJumpSlowly ? dot >= 6.123032e-17f : dot >= 0.70710678f;
        if (jumpPower < jumpPowerBorder || !isInAngle) {
            continuousJump->clear();
            return;
        }
    }

    continuousJump->countUp(jumpDir);
}

bool PlayerStateJump::isJumpCapCatch() const {
    return !isDead() && al::isNerve(this, &NrvPlayerStateJump.Jump) &&
           mAnimator->isAnim("JumpCapCatch");
}

bool PlayerStateJump::isJumpSpinFlower() const {
    return !isDead() && (al::isNerve(this, &NrvPlayerStateJump.JumpSpinFlower) ||
                         al::isNerve(this, &NrvPlayerStateJump.JumpSpinFlowerDownFall));
}

bool PlayerStateJump::isJumpSpinGround() const {
    return !isDead() && (al::isNerve(this, &NrvPlayerStateJump.JumpSpinGround) ||
                         al::isNerve(this, &NrvPlayerStateJump.JumpSpinGroundDownFall));
}

bool PlayerStateJump::isJumpSpinGroundClockwise() const {
    return al::isEqualString(mAnimName, "SpinJumpR");
}

bool PlayerStateJump::isJumpBack() const {
    return !isDead() && al::isNerve(this, &NrvPlayerStateJump.JumpBack);
}

bool PlayerStateJump::isHovering() const {
    return !isDead() && (al::isNerve(this, &NrvPlayerStateJump.Hovering2D) ||
                         al::isNerve(this, &NrvPlayerStateJump.HoveringJump2D));
}

bool PlayerStateJump::isEndJumpDownFallLand() const {
    if (!al::isNerve(this, &NrvPlayerStateJump.JumpSpinFlowerDownFall) &&
        !al::isNerve(this, &NrvPlayerStateJump.JumpSpinGroundDownFall))
        return false;
    if (!mIsDownFall)
        return true;
    if (!rs::isCollidedGround(mCollider))
        return false;
    sead::Vector3f groundPos = rs::getCollidedGroundPos(mCollider);
    const sead::Vector3f& gravity = al::getGravity(mActor);
    return (groundPos - mDownFallStartPos).dot(gravity) > 10.0f;
}

bool PlayerStateJump::isHoldDownFall() const {
    if (!al::isNerve(this, &NrvPlayerStateJump.JumpSpinFlowerDownFall) &&
        !al::isNerve(this, &NrvPlayerStateJump.JumpSpinGroundDownFall))
        return false;
    return mInput->isHoldHipDrop();
}

bool PlayerStateJump::isEnableHipDropStart() const {
    if (isJumpSpinGround())
        return false;
    if (isJumpSpinFlower())
        return false;
    return true;
}

bool PlayerStateJump::isEnableTrampleByHipDropAttack() const {
    if (isDead())
        return false;
    if (!al::isNerve(this, &NrvPlayerStateJump.JumpSpinFlowerDownFall) &&
        !al::isNerve(this, &NrvPlayerStateJump.JumpSpinGroundDownFall))
        return false;
    return mHipDropAttackCount == 0;
}

bool PlayerStateJump::isEnableReactionCapCatch() const {
    return !isDead() && al::isNerve(this, &NrvPlayerStateJump.Jump);
}

bool PlayerStateJump::isEnableCancelCarryThrow() const {
    if (isJumpSpinFlower())
        return true;
    if (isJumpSpinGround())
        return true;
    return false;
}

bool PlayerStateJump::isFormSquat2D() const {
    if (isDead())
        return false;
    if (!mModelChanger->is2DModel())
        return false;
    if (mSubAnimName)
        return al::isEqualString(mSubAnimName, "JumpSquat");
    return mAnimator->isAnim("JumpSquat");
}

void PlayerStateJump::exeJump() {
    al::LiveActor* actor = mActor;

    rs::scaleVelocityInertiaWallHit(actor, mCollider, 0.25f, 1.0f,
                                    getNormalMaxSpeed(mConst, mModelChanger));

    if (al::isFirstStep(this)) {
        trySubAnimJumpReaction();

        if (_b9 && mAnimator->isUpperBodyAnimAttached())
            mAnimator->clearUpperBodyAnim();

        const char* animName = calcJumpAnimName();

        if (!_b5)
            mAnimator->startAnim(animName);

        if (al::isEqualString(animName, "JumpCapCatch"))
            mHackCap->startCatch("JumpCapCatch", false, sead::Vector3f::zero);

        if (_b7 && !mModelChanger->is2DModel()) {
            sead::Vector3f upDir = {0.0f, 0.0f, 0.0f};
            al::calcUpDir(&upDir, actor);
            if (mUpDir.dot(upDir) <=
                sead::Mathf::cos(sead::Mathf::deg2rad(mConst->getCollisionResetLimit())))
                mTrigger->set(PlayerTrigger::EActionTrigger_val3);
            rs::slerpUp(actor, mUpDir, 1.0f, 180.0f);
        }

        mAirMoveControl->setup(mMoveSpeedMax,
                               mModelChanger->is2DModel() ? mConst->getNormalMaxSpeed2D()
                                                          : mConst->getNormalMaxSpeed(),
                               mExtendFrame, mJumpPower, mJumpGravity, 0,
                               mConst->getJumpInertiaRate());
    }

    if (mTrigger->isOnUpperPunchHit()) {
        if (mModelChanger->is2DModel()) {
            rs::reflectCeilingUpperPunch(mActor, mCollider, mInput, mConst, mTrigger, true);
        } else {
            mAnimator->startAnim("Jump");
            mAnimator->setAnimFrame(mAnimator->getAnimFrameMax());
            rs::reflectCeilingUpperPunch(mActor, mCollider, mInput, mConst, mTrigger, false);
        }
        mAirMoveControl->setExtendFrame(0);
        mIsContinuousJump = false;
    }

    bool onGround;
    if (mCounterForceRun->isForceRun())
        onGround = rs::isOnGroundAndGravity(actor, mCollider);
    else
        onGround = rs::isOnGround(actor, mCollider);

    if (rs::isCollidedCeiling(mCollider) && !al::isFirstStep(this)) {
        rs::reflectCeiling(mActor, 0.0f);
        mAirMoveControl->setExtendFrame(0);
        mIsContinuousJump = false;
    }

    if (isIn2DAreaModel(mModelChanger, mDimension)) {
        sead::Vector3f lockDir = {0.0f, 0.0f, 0.0f};
        rs::calcLockDirection(&lockDir, mDimension);
        mAirMoveControl->verticalizeStartMoveDir(lockDir);
    }

    mDiveInWater->tryChangeDiveInWaterAnim();
    mAirMoveControl->update();

    if (onGround) {
        sead::Vector3f horizontalVel = {0.0f, 0.0f, 0.0f};
        sead::Vector3f verticalVel = {0.0f, 0.0f, 0.0f};
        al::separateVelocityHV(&horizontalVel, &verticalVel, mActor);
        f32 speed = sead::Mathf::min(horizontalVel.length(), mAirMoveControl->get_64());
        al::limitLength(&horizontalVel, horizontalVel, speed);
        al::setVelocity(mActor, verticalVel + horizontalVel);
        kill();
    } else if (rs::updateJudgeAndResult(mJudgeWallCatch)) {
        if (!isDead() && mModelChanger->is2DModel() &&
            (mSubAnimName ? al::isEqualString(mSubAnimName, "JumpSquat")
                          : mAnimator->isAnim("JumpSquat"))) {
            mSubAnimName = nullptr;
            mTrigger->set(PlayerTrigger::EActionTrigger_val3);
        }
        if (_ba)
            al::setNerve(this, &NrvPlayerStateJump.Hovering2D);
        else
            al::setNerve(this, &NrvPlayerStateJump.HoveringJump2D);
    } else {
        if (_ba)
            _ba = mInput->isHoldCapSeparateJump();
    }
}

bool PlayerStateJump::trySubAnimJumpReaction() {
    if (!mAnimator->isSubAnimPlaying())
        return false;

    if (mIsJumpMessageRequest) {
        mAnimator->endSubAnim();
        return false;
    }

    al::startHitReaction(mActor, "アクションジャンプ");
    return true;
}

const char* PlayerStateJump::calcJumpAnimName() const {
    if (mModelChanger->is2DModel())
        return mSubAnimName ? mSubAnimName : "Jump";

    if (mAnimName)
        return mAnimName;

    switch (mContinuousJumpCount) {
    case 0:
        if (mAnimator->isSubAnimPlaying())
            return rs::isPlayerSideFaceToCameraZ(mActor) ? "JumpInterp" : "JumpReverseInterp";
        return rs::isPlayerSideFaceToCameraZ(mActor) ? "Jump" : "JumpReverse";
    case 1:
        return "Jump2";
    case 2:
        return "Jump3";
    default:
        return "Jump";
    }
}

void PlayerStateJump::exeJumpSpinFlower() {
    al::LiveActor* actor = mActor;

    if (al::isFirstStep(this)) {
        trySubAnimJumpReaction();

        sead::Vector3f* velocity = al::getVelocityPtr(actor);
        al::verticalizeVec(velocity, al::getGravity(actor), *velocity);

        if (_b6) {
            mAnimator->startAnim("SpinJumpLoop");
            al::addVelocityToGravity(actor, mConst->getSpinFlowerJumpFallSpeedMax());
        } else {
            mAnimator->startAnim("SpinJumpStart");
            al::addVelocityToGravity(actor, -mJumpPower);
        }
    }

    if (mAnimator->isAnim("SpinJumpStart") && mAnimator->isAnimEnd())
        mAnimator->startAnim("SpinJumpLoop");

    if (mTrigger->isOnUpperPunchHit()) {
        rs::reflectCeilingUpperPunch(mActor, mCollider, mInput, mConst, mTrigger, false);
        mIsContinuousJump = false;
    }

    if (rs::isCollidedCeiling(mCollider)) {
        rs::reflectCeiling(mActor, 0.0f);
        mIsContinuousJump = false;
    }

    sead::Vector3f moveInput = {0.0f, 0.0f, 0.0f};
    mInput->calcMoveInput(&moveInput, -al::getGravity(actor));

    f32 gravity = mConst->getJumpGravity();
    f32 fallSpeedMax = mConst->getSpinFlowerJumpFallSpeedMax();
    if (al::calcSpeedV(actor) < 0.0f || al::isNearZero(al::calcSpeedV(actor))) {
        gravity = mConst->getSpinFlowerJumpGravity();
        if (_ac > 0) {
            _ac--;
            fallSpeedMax = mConst->getSpinFlowerJumpStaySpeedMax();
        }
    } else {
        _ac = mConst->getSpinFlowerJumpStayFrame();
    }

    rs::moveParallelJump(mActor, moveInput, mConst->getSpinFlowerJumpMovePower(),
                         mConst->getSpinFlowerJumpMovePower(), mConst->getSpinFlowerJumpMovePower(),
                         mConst->getSpinFlowerJumpVelMax(), gravity, fallSpeedMax,
                         mConst->getSlerpQuatGrav());

    if (!mInput->isMove())
        al::scaleVelocityHV(actor, mConst->getSpinFlowerJumpNoInputBrake(), 1.0f);

    if (mInput->isTriggerHipDrop()) {
        al::setNerve(this, &NrvPlayerStateJump.JumpSpinFlowerDownFall);
        return;
    }

    if (rs::isOnGround(actor, mCollider))
        kill();
}

void PlayerStateJump::exeJumpSpinFlowerDownFall() {
    updateNerveDownFall("SpinJumpDownFall", mConst->getSpinFlowerJumpDownFallInitSpeed(),
                        mConst->getSpinFlowerJumpDownFallPower(),
                        mConst->getSpinFlowerJumpDownFallSpeedMax(),
                        &NrvPlayerStateJump.JumpSpinFlower);
}

void PlayerStateJump::updateNerveDownFall(const char* animName, f32 initSpeed, f32 gravity,
                                          f32 fallSpeedMax, const al::Nerve* nerve) {
    al::LiveActor* actor = mActor;

    if (al::isFirstStep(this)) {
        mAnimator->startAnim(animName);
        al::setVelocityToGravity(actor, initSpeed);
        _b6 = true;
    }

    al::addVelocityToGravityLimit(mActor, gravity, fallSpeedMax);

    if (al::isGreaterEqualStep(this, mConst->getDownFallFrameMin()) && !mInput->isHoldHipDrop()) {
        if (rs::isOnGround(actor, mCollider))
            kill();
        else
            al::setNerve(this, nerve);
        return;
    }

    if (mTrigger->isOn(PlayerTrigger::ECollisionTrigger_val1)) {
        mHipDropAttackCount = mConst->getHipDropMsgInterval();
        mIsDownFall = false;
        if (rs::isCollidedGround(mCollider)) {
            mIsDownFall = true;
            mDownFallStartPos = rs::getCollidedGroundPos(mCollider);
        }
    }

    if (mHipDropAttackCount > 0) {
        if (rs::convergeOnGroundCount(&mHipDropAttackCount, mActor, mCollider, 0, 1))
            return;
        mHipDropAttackCount = 0;
    }

    if (!rs::isOnGround(actor, mCollider))
        return;

    if (mInput->isHoldHipDrop() && mInput->isMoveDeepDown()) {
        sead::Vector3f moveDir = {0.0f, 0.0f, 0.0f};
        mInput->calcMoveDirection(&moveDir, -al::getGravity(actor));
        sead::Vector3f groundNormal = rs::getCollidedGroundNormal(mCollider);
        rs::slerpUpFront(actor, groundNormal, moveDir, mConst->getSlerpQuatRate(),
                         mConst->getHillPoseDegreeMax());
    }

    kill();
}

void PlayerStateJump::exeJumpSpinGroundDownFall() {
    const char* animName = isJumpSpinGroundClockwise() ? "SpinJumpDownFallR" : "SpinJumpDownFallL";
    updateNerveDownFall(animName, mConst->getSpinJumpDownFallInitSpeed(),
                        mConst->getSpinJumpDownFallPower(), mConst->getSpinJumpDownFallSpeedMax(),
                        &NrvPlayerStateJump.JumpSpinGround);
}

void PlayerStateJump::exeHovering2D() {
    if (!mModelChanger->is2DModel()) {
        kill();
        return;
    }

    al::LiveActor* actor = mActor;
    rs::scaleVelocityInertiaWallHit(actor, mCollider, 0.25f, 1.0f,
                                    getNormalMaxSpeed(mConst, mModelChanger));

    if (al::isFirstStep(this)) {
        mAnimator->startAnim("Hovering");
        sead::Vector3f* velocity = al::getVelocityPtr(actor);
        al::verticalizeVec(velocity, al::getGravity(actor), *velocity);
        mAirMoveControl->setup(mMoveSpeedMax, mConst->getNormalMaxSpeed2D(), 0, 0.0f, 0.0f, 0,
                               0.0f);
    }

    mAirMoveControl->update();

    if (rs::isOnGround(actor, mCollider) || !mInput->isHoldCapSeparateJump() ||
        al::isGreaterEqualStep(this, 60))
        kill();
}

void PlayerStateJump::exeJumpBack() {
    al::LiveActor* actor = mActor;

    if (al::isFirstStep(this)) {
        trySubAnimJumpReaction();
        mAnimator->startAnim("JumpBack");

        sead::Vector3f frontDir = {0.0f, 0.0f, 0.0f};
        al::calcFrontDir(&frontDir, actor);
        al::verticalizeVec(&frontDir, al::getGravity(actor), frontDir);
        al::tryNormalizeOrZero(&frontDir);

        sead::Vector3f velocity =
            -(frontDir * mConst->getSquatJumpBackPower() + al::getGravity(actor) * mJumpPower);
        al::setVelocity(actor, velocity);
    }

    if (mTrigger->isOnUpperPunchHit()) {
        rs::reflectCeilingUpperPunch(mActor, mCollider, mInput, mConst, mTrigger, false);
        mIsContinuousJump = false;
    }

    if (rs::isCollidedCeiling(mCollider)) {
        rs::reflectCeiling(mActor, 0.0f);
        mIsContinuousJump = false;
    }

    sead::Vector3f moveInput = {0.0f, 0.0f, 0.0f};
    mInput->calcMoveInput(&moveInput, -al::getGravity(actor));

    rs::moveParallelJump(mActor, moveInput, mConst->getSquatJumpMovePowerFront(),
                         mConst->getSquatJumpMovePowerFront(), mConst->getSquatJumpMovePowerSide(),
                         mConst->getSquatJumpMoveSpeedMax(), mJumpGravity,
                         mConst->getFallSpeedMax(), mConst->getSlerpQuatGrav());

    if (rs::isOnGround(actor, mCollider))
        kill();
}

void PlayerStateJump::exeJumpSpinGround() {
    al::LiveActor* actor = mActor;

    rs::scaleVelocityInertiaWallHit(mActor, mCollider, 0.25f, 1.0f,
                                    getNormalMaxSpeed(mConst, mModelChanger));

    if (al::isFirstStep(this)) {
        trySubAnimJumpReaction();

        if (_b6)
            mAnimator->startAnim(al::StringTmp<64>("Restart%s", mAnimName));
        else
            mAnimator->startAnim(al::StringTmp<64>("Start%s", mAnimName));

        mAirMoveControl->setup(mMoveSpeedMax, mConst->getNormalMaxSpeed(), 0, mJumpPower,
                               mJumpGravity, 0, mConst->getJumpInertiaRate());

        if (_b6)
            al::setVelocityToGravity(actor, mConst->getFallSpeedMax());
    }

    if (mTrigger->isOnUpperPunchHit()) {
        rs::reflectCeilingUpperPunch(mActor, mCollider, mInput, mConst, mTrigger, false);
        mIsContinuousJump = false;
    }

    if (rs::isCollidedCeiling(mCollider)) {
        rs::reflectCeiling(mActor, 0.0f);
        mIsContinuousJump = false;
    }

    mAirMoveControl->update();

    if (mInput->isTriggerHipDrop()) {
        al::setNerve(this, &NrvPlayerStateJump.JumpSpinGroundDownFall);
        return;
    }

    if (rs::isOnGround(actor, mCollider)) {
        sead::Vector3f horizontalVel = {0.0f, 0.0f, 0.0f};
        sead::Vector3f verticalVel = {0.0f, 0.0f, 0.0f};
        al::separateVelocityHV(&horizontalVel, &verticalVel, mActor);
        al::limitLength(&horizontalVel, horizontalVel,
                        sead::Mathf::min(horizontalVel.length(), mAirMoveControl->get_64()));
        al::setVelocity(mActor, verticalVel + horizontalVel);
        kill();
    }
}

void PlayerStateJump::exeHoveringJump2D() {
    if (!mModelChanger->is2DModel()) {
        kill();
        return;
    }

    al::LiveActor* actor = mActor;

    rs::scaleVelocityInertiaWallHit(actor, mCollider, 0.25f, 1.0f,
                                    getNormalMaxSpeed(mConst, mModelChanger));

    if (al::isFirstStep(this)) {
        mAnimator->startAnim("Hovering");
        mAirMoveControl->setup(mMoveSpeedMax, mConst->getNormalMaxSpeed2D(), 0, 20.0f, 2.0f, 0,
                               0.0f);
    }

    if (mTrigger->isOnUpperPunchHit()) {
        rs::reflectCeilingUpperPunch(mActor, mCollider, mInput, mConst, mTrigger, true);
        mAirMoveControl->setExtendFrame(0);
        mIsContinuousJump = false;
    }

    bool onGround = rs::isOnGround(actor, mCollider);

    if (rs::isCollidedCeiling(mCollider) && !al::isFirstStep(this)) {
        rs::reflectCeiling(mActor, 0.0f);
        mAirMoveControl->setExtendFrame(0);
        mIsContinuousJump = false;
    }

    if (isIn2DAreaModel(mModelChanger, mDimension)) {
        sead::Vector3f lockDir = {0.0f, 0.0f, 0.0f};
        rs::calcLockDirection(&lockDir, mDimension);
        mAirMoveControl->verticalizeStartMoveDir(lockDir);
    }

    mAirMoveControl->update();

    if (onGround) {
        sead::Vector3f horizontalVel = {0.0f, 0.0f, 0.0f};
        sead::Vector3f verticalVel = {0.0f, 0.0f, 0.0f};
        al::separateVelocityHV(&horizontalVel, &verticalVel, mActor);
        f32 speed = sead::Mathf::min(horizontalVel.length(), mAirMoveControl->get_64());
        al::limitLength(&horizontalVel, horizontalVel, speed);
        al::setVelocity(mActor, verticalVel + horizontalVel);
        kill();
    } else if (rs::updateJudgeAndResult(mJudgeWallCatch) || al::calcSpeedV(actor) < 0.0f) {
        al::setNerve(this, &NrvPlayerStateJump.Hovering2D);
    }
}

void PlayerStateJump::exeJumpTurn() {
    al::LiveActor* actor = mActor;

    if (al::isFirstStep(this)) {
        trySubAnimJumpReaction();

        sead::Vector3f upDir = {0.0f, 0.0f, 0.0f};
        al::calcUpDir(&upDir, actor);
        sead::Quatf quat = sead::Quatf::unit;
        al::makeQuatFrontUp(&quat, mTurnDir, upDir);
        al::updatePoseQuat(actor, quat);

        sead::Vector3f dir = {0.0f, 0.0f, 0.0f};
        al::verticalizeVec(&dir, al::getGravity(actor), mTurnDir);
        if (!al::tryNormalizeOrZero(&dir))
            dir = upDir;

        al::setVelocity(actor, dir * mConst->getTurnJumpVelH());
        al::addVelocityToGravity(actor, -mJumpPower);
        mAnimator->startAnim("JumpTurn");
    }

    if (mTrigger->isOnUpperPunchHit()) {
        rs::reflectCeilingUpperPunch(mActor, mCollider, mInput, mConst, mTrigger, false);
        mIsContinuousJump = false;
    }

    if (rs::isCollidedCeiling(mCollider)) {
        rs::reflectCeiling(mActor, 0.0f);
        mIsContinuousJump = false;
    }

    mDiveInWater->tryChangeDiveInWaterAnim();

    sead::Vector3f moveInput = {0.0f, 0.0f, 0.0f};
    mInput->calcMoveInput(&moveInput, -al::getGravity(actor));

    rs::moveDivingJump(actor, moveInput, mConst->getTurnJumpAccel(), mConst->getTurnJumpBrake(),
                       mConst->getTurnJumpVelH(), 0.0f, mConst->getTurnJumpSideAccel(), mJumpGravity,
                       mConst->getFallSpeedMax(), 0.0f);

    rs::slerpUp(actor, -al::getGravity(actor), mConst->getSlerpQuatGrav(), 180.0f);

    if (rs::isOnGround(actor, mCollider))
        kill();
}
