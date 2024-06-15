#include "Player/PlayerStateJump.h"
#include "Library/Base/StringUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Math/MathAngleUtil.h"
#include "Library/Math/MathLengthUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/stuff.h"
#include "Player/PlayerActionAirMoveControl.h"
#include "Player/PlayerActionFunction.h"
#include "Player/PlayerActionVelocityControl.h"
#include "Player/PlayerConst.h"
#include "Player/PlayerInput.h"
#include "Player/PlayerJumpMessageRequest.h"
#include "Player/PlayerTrigger.h"
#include "PlayerUtil.h"

#include "Library/Nerve/NerveSetupUtil.h"
#include "Stuff.h"
#include "Util/ActorDimensionKeeper.h"
#include "Util/StageSceneFunction.h"
#include "basis/seadTypes.h"

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

NERVE_MAKE(PlayerStateJump, Jump);
NERVE_MAKE(PlayerStateJump, HoveringJump2D);
NERVE_MAKE(PlayerStateJump, JumpSpinFlower);
NERVE_MAKE(PlayerStateJump, JumpBack);
NERVE_MAKE(PlayerStateJump, JumpTurn);
NERVE_MAKE(PlayerStateJump, JumpSpinGround);
NERVE_MAKE(PlayerStateJump, JumpSpinFlowerDownFall);
NERVE_MAKE(PlayerStateJump, JumpSpinGroundDownFall);
NERVE_MAKE(PlayerStateJump, Hovering2D);

}  // namespace

PlayerStateJump::PlayerStateJump(al::LiveActor* player, const PlayerConst* pConst,
                                 const IPlayerModelChanger* modelChanger,
                                 const IUseDimension* dimension, const PlayerInput* input,
                                 const PlayerJumpMessageRequest* jumpMessageRequest,
                                 const IJudge* judgeCapCatch,
                                 const PlayerCounterForceRun* counterForceRun,
                                 const PlayerJointControlKeeper* jointControlKeeper,
                                 const IUsePlayerCollision* collision, PlayerTrigger* trigger,
                                 PlayerContinuousJump* continuousJump, PlayerAnimator* animator,
                                 PlayerActionDiveInWater* diveInWater, HackCap* hackCap,
                                 IJudge* judgeCapHoldHoveringJump, bool isMoon)
    : al::ActorStateBase("ジャンプ", player), mConst(pConst), mModelChanger(modelChanger),
      mDimension(dimension), mJumpMessageRequest(jumpMessageRequest), mJudgeCapCatch(judgeCapCatch),
      mCounterForceRun(counterForceRun), mJointControlKeeper(jointControlKeeper),
      mCollision(collision), mAnimator(animator), mContinuousJump(continuousJump),
      mTrigger(trigger), mActionDiveInWater(diveInWater), mHackCap(hackCap),
      mJudgeHackCapHoldHoveringJump(judgeCapHoldHoveringJump), mIsMoon(isMoon), mInput(input) {
    mActionAirMoveControl = new PlayerActionAirMoveControl(player, pConst, input, collision, false);
    mActionAirMoveControl->unk = true;
    mActionAirMoveControl->unk3 = true;
    mActionAirMoveControl->val1 = 0.0f;
    mActionAirMoveControl->mJumpSpeedMax = mConst->getJumpBaseSpeedMax();
    initNerve(&Jump, 0);
}

PlayerStateJump::~PlayerStateJump() {}

void PlayerStateJump::appear() {
    rs::startHitReactionLandJumpIfLanding(mActor, mCollision, mModelChanger->is2DModel());
    // some stuff about rocket flowers
    mActionAirMoveControl->unk3 = 1;
    mActionAirMoveControl->val1 = 0.0f;
    mActionAirMoveControl->mJumpSpeedMax = mConst->getJumpBaseSpeedMax();
    mIsDead = false;

    if(rs::isJustLand(mCollision)) {
        sead::Vector3f alongVec;
        al::alongVectorNormalH(&alongVec, al::getVelocity(mActor), al::getGravity(mActor), rs::getCollidedGroundNormal(mCollision));
        sead::Vector3f newVel = alongVec - (mConst->getGravity() * rs::getCollidedGroundNormal(mCollision));
        al::setVelocity(mActor, newVel);
    }

    if(mJumpMessageRequest->mJumpType == PlayerJumpType::val_0F && !mJumpMessageRequest->mIsEnableStandUp) {
        CRASH
    }

    // BUG: seems to have swapped normal and 2d speed?
    rs::scaleVelocityInertiaWallHit(mActor, mCollision, 0.25f, 1.0f, mModelChanger->is2DModel() ? mConst->getNormalMaxSpeed() : mConst->getNormalMaxSpeed2D());

    bool v37 = ((unsigned int)((u32)this->mJumpMessageRequest->mJumpType - 1) < 0x10) & (0x9001u >> ((u32)this->mJumpMessageRequest->mJumpType - 1));
    mContinuousJumpCount = 0;
    if(!v37 && !mModelChanger->isMini()) {
        if(mContinuousJump->mCount < 2) {
            mContinuousJumpCount = mContinuousJump->mCount;
            _B4 = true;
        } else {
            PlayerActionVelocityControl velocityControl = PlayerActionVelocityControl(mActor, mCollision);
            f32 frontSpeed = velocityControl.mVelocityFront.length();
            sead::Vector3f offsetFront = {0.0f, 0.0f, 0.0f};
            al::verticalizeVec(&offsetFront, al::getGravity(mActor), velocityControl.mVelocityFront);
            al::tryNormalizeOrZero(&offsetFront);
            f32 normalMaxSpeed = mConst->getNormalMaxSpeed();

            if(mIsMoon) normalMaxSpeed *= 0.5f;
            if(frontSpeed > normalMaxSpeed) {
                f32 dot = offsetFront.dot(mContinuousJump->mLastJumpDir);
                if((mIsMoon && dot >= 0.70711f) || (!mIsMoon && dot >= 6.123e-17f)) {
                    mContinuousJumpCount = mContinuousJump->mCount;
                    _B4 = true;
                }
            }
        }
        _B4 = true || false;
    } else {
        mContinuousJump->clear();
        _B4 = false;
    }

    mActionAirMoveControl->mIs2D = mModelChanger->is2DModel() && rs::isIn2DArea(mDimension);
    _F0 = 0;
    vec = {0.0f, 0.0f, 0.0f};
    _B9 = false;
    mIsHoldCapSeparateJump = false;
    _B5 = false;
    _B6 = false;
    _B7 = false;
    _B8 = false;
    _BC = {0.0f, 0.0f, 0.0f};
    mJumpAnimName = nullptr;
    mMoveSpeedMax = sead::Mathf::min(mConst->getJumpMoveSpeedMax(), mActionAirMoveControl->mJumpSpeedMax);
    switch(mJumpMessageRequest->mJumpType) {
        case PlayerJumpType::val_0C:
            printf("PlayerStateJump::appear: unknown jump type %x\n", mJumpMessageRequest->mJumpType);
            CRASH
        case PlayerJumpType::val_0D:
            printf("PlayerStateJump::appear: unknown jump type %x\n", mJumpMessageRequest->mJumpType);
            CRASH
        case PlayerJumpType::val_0E:
            rs::noticePlayerJumpStart(mTrigger, mActor);
            mJumpPower = mConst->getSquatJumpPower();
            mJumpGravity = mConst->getSquatJumpGravity();
            _B8 = true;
            al::setNerve(this, &JumpBack);
            return;
        case PlayerJumpType::val_10:
            printf("PlayerStateJump::appear: unknown jump type %x\n", mJumpMessageRequest->mJumpType);
            CRASH
        case PlayerJumpType::val_12:
            printf("PlayerStateJump::appear: unknown jump type %x\n", mJumpMessageRequest->mJumpType);
            CRASH
        default:
            bool isMove = mInput->isMove();
            f32 dot = al::getVelocity(mActor).dot(al::getGravity(mActor));
            switch(mJumpMessageRequest->mJumpType) {
                case PlayerJumpType::val_01:
                    printf("PlayerStateJump::appear: unknown jump type %x\n", mJumpMessageRequest->mJumpType);
                    CRASH
                case PlayerJumpType::val_02:
                    printf("PlayerStateJump::appear: unknown jump type %x\n", mJumpMessageRequest->mJumpType);
                    CRASH
                case PlayerJumpType::val_04:
                case PlayerJumpType::val_0B:
                    printf("PlayerStateJump::appear: unknown jump type %x\n", mJumpMessageRequest->mJumpType);
                    CRASH
                case PlayerJumpType::val_05:
                    printf("PlayerStateJump::appear: unknown jump type %x\n", mJumpMessageRequest->mJumpType);
                    CRASH
                case PlayerJumpType::val_06:
                    printf("PlayerStateJump::appear: unknown jump type %x\n", mJumpMessageRequest->mJumpType);
                    CRASH
                case PlayerJumpType::val_07:
                    printf("PlayerStateJump::appear: unknown jump type %x\n", mJumpMessageRequest->mJumpType);
                    CRASH
                case PlayerJumpType::val_08:
                    printf("PlayerStateJump::appear: unknown jump type %x\n", mJumpMessageRequest->mJumpType);
                    CRASH
                case PlayerJumpType::val_09:
                    printf("PlayerStateJump::appear: unknown jump type %x\n", mJumpMessageRequest->mJumpType);
                    CRASH
                case PlayerJumpType::val_0A:
                    printf("PlayerStateJump::appear: unknown jump type %x\n", mJumpMessageRequest->mJumpType);
                    CRASH
                case PlayerJumpType::HipDropJump:
                    rs::noticePlayerJumpStart(mTrigger, mActor);
                    mJumpPower = mConst->getJumpHipDropPower();
                    mJumpGravity = mConst->getJumpGravity();
                    mExtendFrame = 0;
                    _C8 = "JumpHipDrop";
                    _B8 = true;
                    al::setNerve(this, &Jump);
                    return;
                default:
                    rs::noticePlayerJumpStart(mTrigger, mActor);
                    if(rs::isJudge(mJudgeCapCatch)) {
                        CRASH
                    }
                    PlayerActionVelocityControl velocityCtrl = PlayerActionVelocityControl(mActor, mCollision);
                    f32 frontVelocityLength = velocityCtrl.mVelocityFront.length();
                    f32 minSpeed = (mModelChanger->is2DModel() && rs::isIn2DArea(mDimension)) ? mConst->getJumpPowerMinBorder2D() : mConst->getNormalMinSpeed();
                    f32 maxSpeed = (mModelChanger->is2DModel() && rs::isIn2DArea(mDimension)) ? mConst->getJumpPowerMaxBorder2D() : mConst->getNormalMaxSpeed();
                    
                    f32 jumpPowerMin;
                    if(mModelChanger->is2DModel() && rs::isIn2DArea(mDimension)) {
                        jumpPowerMin = mConst->getJumpPowerMin2DArea();
                    } else if(mCounterForceRun->getCounter() < 1) {
                        if(mContinuousJumpCount == 2 || mContinuousJumpCount == 1)
                            jumpPowerMin = mConst->getContinuousJumpPowerMin();
                        else if(mContinuousJumpCount > 0)
                            jumpPowerMin = mConst->getJumpPowerMax();
                        else
                            jumpPowerMin = mConst->getJumpPowerMin();
                    } else {
                        jumpPowerMin = mConst->getJumpPowerForceRun();
                    }
                    
                    f32 jumpPowerMax;
                    if(mModelChanger->is2DModel() && rs::isIn2DArea(mDimension)) {
                        jumpPowerMax = mConst->getJumpPowerMax2DArea();
                    } else if(mCounterForceRun->getCounter() < 1) {
                        if(mContinuousJumpCount == 2)
                            jumpPowerMax = mConst->getJumpPowerMax3rd();
                        if(mContinuousJumpCount == 1)
                            jumpPowerMax = mConst->getJumpPowerMax2nd();
                        else if(mContinuousJumpCount > 0)
                            jumpPowerMax = mConst->getJumpPowerMax();
                        else
                            jumpPowerMax = mConst->getJumpPowerMax();
                    } else {
                        jumpPowerMax = mConst->getJumpPowerForceRun();
                    }

                    mJumpPower = PlayerActionFunction::calcJumpSpeed(frontVelocityLength, minSpeed, maxSpeed, jumpPowerMin, jumpPowerMax);

                    f32 jumpGravity;
                    if(mCounterForceRun->getCounter() < 1) {
                        if(mContinuousJumpCount == 2)
                            jumpGravity = mConst->getJumpGravity3rd();
                        else if(mContinuousJumpCount == 1)
                            jumpGravity = mConst->getJumpGravity2nd();
                        else if(mContinuousJumpCount > 0)
                            jumpGravity = mConst->getJumpGravity();
                        else
                            jumpGravity = mConst->getJumpGravity();
                    } else {
                        jumpGravity = mConst->getJumpGravityForceRun();
                    }
                    mJumpGravity = jumpGravity;

                    mExtendFrame = mConst->getExtendFrame();
                    _B7 = mTrigger->isOn(PlayerTrigger::EActionTrigger_val21);
                    if(_B7)
                        mJointControlKeeper->calcGroundPoseUp(&_BC);

                    if(mJumpMessageRequest->mJumpType == PlayerJumpType::val_0F)
                        mJumpAnimName = "JumpSquat";
                    else if(mJumpMessageRequest->mJumpType == PlayerJumpType::val_03 && !mContinuousJumpCount)
                        _C8 = "JumpDashFast";

                    al::setNerve(this, &Jump);
            }
    }
}

// f32 PlayerStateJump::calcJumpPowerBorderSpeedMin() const;
// f32 PlayerStateJump::calcJumpPowerBorderSpeedMax() const;
// f32 PlayerStateJump::calcJumpPowerMin() const;
// f32 PlayerStateJump::calcJumpPowerMax() const;
// f32 PlayerStateJump::calcJumpGravity() const;
void PlayerStateJump::tryCountUpContinuousJump(PlayerContinuousJump*) {WARN_UNIMPL;}
bool PlayerStateJump::isJumpCapCatch() const {
    if(mIsDead) return false;
    return al::isNerve(this, &Jump) && mAnimator->isAnim("JumpCapCatch");
}
bool PlayerStateJump::isJumpSpinFlower() const {
    if(mIsDead) return false;
    return al::isNerve(this, &JumpSpinFlower) || al::isNerve(this, &JumpSpinFlowerDownFall);
}
bool PlayerStateJump::isJumpSpinGround() const {
    if(mIsDead) return false;
    return al::isNerve(this, &JumpSpinGround) || al::isNerve(this, &JumpSpinGroundDownFall);
}
bool PlayerStateJump::isJumpSpinGroundClockwise() const {
    return al::isEqualString(_C8, "SpinJumpR");
}
bool PlayerStateJump::isJumpBack() const {
    if(mIsDead) return false;
    return al::isNerve(this, &JumpBack);
}
bool PlayerStateJump::isHovering() const {
    if(mIsDead) return false;
    return al::isNerve(this, &Hovering2D) || al::isNerve(this, &HoveringJump2D);
}
bool PlayerStateJump::isEndJumpDownFallLand() const {
    if(!al::isNerve(this, &JumpSpinFlowerDownFall) && !al::isNerve(this, &JumpSpinGroundDownFall)) {
        return false;
    }
    if(_F0) {
        if(rs::isCollidedGround(mCollision)) {
            return (rs::getCollidedGroundPos(mCollision) - vec).dot(al::getGravity(mActor)) > 10.0f;
        }
        return false;
    }
    return true;
}
// bool PlayerStateJump::isHoldDownFall() const;
bool PlayerStateJump::isEnableHipDropStart() const {
    return !isJumpSpinGround() && !isJumpSpinFlower();
}
// bool PlayerStateJump::isEnableTrampleByHipDropAttack() const;
// bool PlayerStateJump::isEnableReactionCapCatch() const;
bool PlayerStateJump::isEnableCancelCarryThrow() const {
    return isJumpSpinFlower() || isJumpSpinGround();
}
bool PlayerStateJump::isFormSquat2D() const {
    if(mIsDead) return false;
    if(mModelChanger->is2DModel()) {
        CRASH
    }
    return false;
}
void PlayerStateJump::exeJump() {
    // BUG: swapped 2D/non-2D speed?
    rs::scaleVelocityInertiaWallHit(mActor, mCollision, 0.25f, 1.0f, mModelChanger->is2DModel() ? mConst->getNormalMaxSpeed() : mConst->getNormalMaxSpeed2D());
    
    if(al::isFirstStep(this)) {
        // anim stuff
        const char* animName = calcJumpAnimName();
        // more anim stuff

        if(al::isEqualString(animName, "JumpCapCatch"))
            mHackCap->startCatch("JumpCapCatch", 0, sead::Vector3f::zero);

        if(_B7 && !mModelChanger->is2DModel()) {
            sead::Vector3f up = {0.0f, 0.0f, 0.0f};
            al::calcUpDir(&up, mActor);
            if(_BC.dot(up) <= sead::Mathf::cos(sead::Mathf::deg2rad(mConst->getCollisionResetLimit())))
                mTrigger->set(PlayerTrigger::EActionTrigger_val3);
            rs::slerpUp(mActor, _BC, 1.0f, 180.0f);
        }

        mActionAirMoveControl->setup(mMoveSpeedMax, 
            mModelChanger->is2DModel() ? mConst->getNormalMaxSpeed2D() : mConst->getNormalMaxSpeed(),
            mExtendFrame, mJumpPower, mJumpGravity, 0, mConst->getJumpInertiaRate());
    }

    if(mTrigger->isOnUpperPunchHit()) {
        // jumping into question-mark block
        CRASH
    }

    bool v43 = mCounterForceRun->getCounter() < 1 ? rs::isOnGround(mActor, mCollision) : rs::isOnGroundAndGravity(mActor, mCollision);
    printf("v43: %s\n", v43 ? "true" : "false");

    if(rs::isCollidedCeiling(mCollision) && !al::isFirstStep(this)) {
        rs::reflectCeiling(mActor, 0.0f);
        mActionAirMoveControl->setExtendFrame(0);
        _B4 = false;
    }

    if(mModelChanger->is2DModel() && rs::isIn2DArea(mDimension)) {
        CRASH
    }

    mActionDiveInWater->tryChangeDiveInWaterAnim();
    mActionAirMoveControl->update();

    if(v43) {
        sead::Vector3f h = {0.0f, 0.0f, 0.0f};
        sead::Vector3f v = {0.0f, 0.0f, 0.0f};
        al::separateVelocityHV(&h, &v, mActor);
        al::limitLength(&h, h, sead::Mathf::min(h.length(), mActionAirMoveControl->somethingHere));
        al::setVelocity(mActor, h + v);
        kill();
        return;
    }

    if(rs::updateJudgeAndResult(mJudgeHackCapHoldHoveringJump)) {
        if(!mIsDead && mModelChanger->is2DModel()) {
            CRASH
        }

        if(mIsHoldCapSeparateJump) {
            al::setNerve(this, &Hovering2D);
        } else {
            al::setNerve(this, &HoveringJump2D);
        }
        return;
    }

    if(mIsHoldCapSeparateJump) {
        mIsHoldCapSeparateJump = mInput->isHoldCapSeparateJump();
    }
}
// bool PlayerStateJump::trySubAnimJumpReaction();
const char* PlayerStateJump::calcJumpAnimName() const {
    if(mModelChanger->is2DModel())
        return mJumpAnimName ? mJumpAnimName : "Jump";
    if(_C8) return _C8;

    if(mContinuousJumpCount == 2)
        return "Jump3";
    if(mContinuousJumpCount == 1)
        return "Jump2";
    if(mContinuousJumpCount > 0)
        return "Jump";

    // extras based on animator and direction
    return "Jump";
}
void PlayerStateJump::exeJumpSpinFlower()  { WARN_UNIMPL; }
void PlayerStateJump::exeJumpSpinFlowerDownFall()  { WARN_UNIMPL; }
// void PlayerStateJump::updateNerveDownFall(const char*, f32, f32, f32, const al::Nerve*);
void PlayerStateJump::exeJumpSpinGround()  { WARN_UNIMPL; }
void PlayerStateJump::exeJumpSpinGroundDownFall()  { WARN_UNIMPL; }
void PlayerStateJump::exeJumpTurn()  { WARN_UNIMPL; }
void PlayerStateJump::exeJumpBack()  {
    if(al::isFirstStep(this)) {
        if(mAnimator->unk2) {
            if(_B8)
                mAnimator->endSubAnim();
            else
                al::startHitReaction(mActor, "アクションジャンプ");
        }

        mAnimator->startAnim("JumpBack");
        sead::Vector3f a3 = {0.0f, 0.0f, 0.0f};
        al::calcFrontDir(&a3, mActor);
        al::verticalizeVec(&a3, al::getGravity(mActor), a3);
        al::tryNormalizeOrZero(&a3);
        al::setVelocity(mActor, -((mConst->getSquatJumpBackPower() * a3) + (mJumpPower * al::getGravity(mActor))));
    }

    if(mTrigger->isOnUpperPunchHit()) {
        CRASH
    }

    if(rs::isCollidedCeiling(mCollision)) {
        rs::reflectCeiling(mActor, 0.0f);
        _B4 = false;
    }

    sead::Vector3f moveInput = {0.0f, 0.0f, 0.0f};
    mInput->calcMoveInput(&moveInput, -al::getGravity(mActor));
    rs::moveParallelJump(mActor, moveInput, mConst->getSquatJumpMovePowerFront(), mConst->getSquatJumpMovePowerFront(), mConst->getSquatJumpMovePowerSide(), mConst->getSquatJumpMoveSpeedMax(), mJumpGravity, mConst->getFallSpeedMax(), mConst->getSlerpQuatGrav());
    
    if(rs::isOnGround(mActor, mCollision))
        kill();
}
void PlayerStateJump::exeHoveringJump2D()  { WARN_UNIMPL; }
void PlayerStateJump::exeHovering2D()  { WARN_UNIMPL; }
