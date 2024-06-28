#include "Player/PlayerStateRunHakoniwa.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Math/MathAngleUtil.h"
#include "Library/Math/MathLengthUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Player/PlayerActionGroundMoveControl.h"
#include "Player/PlayerActionPivotTurnControl.h"
#include "Player/PlayerTrigger.h"
#include "PlayerUtil.h"
#include "Stuff.h"
#include "Util/StageSceneFunction.h"

namespace {
NERVE_IMPL(PlayerStateRunHakoniwa, Run);
NERVE_IMPL_(PlayerStateRunHakoniwa, Push, WallPush);
NERVE_IMPL(PlayerStateRunHakoniwa, Brake);
NERVE_IMPL_(PlayerStateRunHakoniwa, RunAfterTurn, Run);
NERVE_IMPL(PlayerStateRunHakoniwa, Turn);
NERVE_IMPL(PlayerStateRunHakoniwa, Pivot);

NERVE_MAKE(PlayerStateRunHakoniwa, Run);
NERVE_MAKE(PlayerStateRunHakoniwa, Push);
NERVE_MAKE(PlayerStateRunHakoniwa, Brake);
NERVE_MAKE(PlayerStateRunHakoniwa, RunAfterTurn);
NERVE_MAKE(PlayerStateRunHakoniwa, Turn);
NERVE_MAKE(PlayerStateRunHakoniwa, Pivot);
}  // namespace

PlayerStateRunHakoniwa::PlayerStateRunHakoniwa(
    al::LiveActor* player, PlayerConst const* pConst, PlayerInput const* input,
    IUsePlayerCollision const* collider, PlayerCounterForceRun const* counterForceRun,
    PlayerCounterQuickTurnJump const* counterQuickTurnJump, PlayerTrigger* trigger,
    PlayerAnimator* animator, PlayerEffect* effect, PlayerJointParamCenterDynamics* jointDynamics,
    IJudge const* judgeWaterSurfaceRun, bool isMoon)
    : al::ActorStateBase("箱庭走り", player), mConst(pConst), mInput(input),
      mCollision(collider), mCounterForceRun(counterForceRun), mCounterQuickTurnJump(counterQuickTurnJump), mTrigger(trigger),
      mAnimator(animator), mEffect(effect), mJointDynamics(jointDynamics) {
    mAnimControlRun = new PlayerAnimControlRun(animator, pConst, judgeWaterSurfaceRun, effect, isMoon);
    mActionPivotTurnControl = new PlayerActionPivotTurnControl(player, mConst, mInput, mCollision, mConst->getGravity());
    mActionGroundMoveControl = new PlayerActionGroundMoveControl(player, mConst, mInput, mCollision);
    mActionGroundMoveControl->_78 = true;
    mActionGroundMoveControl->someFlags[0] = true;
    mActionGroundMoveControl->_BB = false;
    mActionGroundMoveControl->_BD = true;
    mActionGroundMoveControl->_C4 = true;
    mActionGroundMoveControl->_D4 = true;
    mJudgeWallPush = new PlayerJudgeWallPush(player, mCollision, mInput);
    initNerve(&Run, 0);
}
PlayerStateRunHakoniwa::~PlayerStateRunHakoniwa() {
    CRASH;
}

void PlayerStateRunHakoniwa::appear() {
    rs::startHitReactionLandJumpIfLanding(mActor, mCollision, rs::isPlayer2D(mActor));
    _BC = 0;
    if(rs::isJustLand(mCollision)) {
        _BC = 3;
        // joint control
    }

    mActionGroundMoveControl->setup(mConst->getNormalMaxSpeed(), mConst->getNormalMinSpeed(), mConst->getNormalAccelFrame(), mConst->getStickOnBrakeFrame()
    , mConst->getNormalBrakeFrame(), mConst->getGravityMove(), mConst->getNormalMaxSpeed()*mConst->getBrakeOnSpeedRate(), mConst->getBrakeOnCounterBorder());
    mActionGroundMoveControl->appear();

    _A0 = 0;
    _90 = 0;
    _80 = {0.0f, 0.0f, 0.0f};
    _74 = {0.0f, 0.0f, 0.0f};
    _A4 = 0;
    val1 = 1.0;

    // animator end

    // should be call to setup animation + return value
    _C8 = mTrigger->isOn(PlayerTrigger::EActionTrigger_val16);
    _70 = 0;
    al::setNerve(this, &Run);
    mIsDead = false;
}
void PlayerStateRunHakoniwa::kill() {
    mIsDead = true;
    //rs::tryCancelHeadSlidingLandSubAnim(mAnimator);
    // stop rumble
    _B8 = false;
    _C8 = false;
}
void PlayerStateRunHakoniwa::control() {
    // collided wall vibration
    if(_BC >= 1) {
      if(al::isNerve(this, &Run)) {
        _BC = al::converge(_BC, 0, 1);
        // joint controlling
      }
    }

    if(_C8 && true) {
      _C8 = false;
    }
}
bool PlayerStateRunHakoniwa::isEnableLookAt() const {
    CRASH;
}
bool PlayerStateRunHakoniwa::isRunDashFast() const {
    return al::isNerve(this, &Run) && mAnimControlRun->isAnimDashFast();
}
bool PlayerStateRunHakoniwa::tryTurnJump(sead::Vector3f* a2) {
  if(al::isNerve(this, &Brake) || (al::isNerve(this, &Turn) && al::getNerveStep(this) <= 0)) {
    if(!_70) return false;
    al::calcFrontDir(a2, mActor);
    *a2 = -*a2;
    return true;
  }
  if(al::isNerve(this, &Pivot)) {
    if(mCounterQuickTurnJump->isEnableTurnJump()) {
      sead::Vector3f vec = mActionPivotTurnControl->_38;
      if(al::tryNormalizeOrZero(&vec)) {
        if(vec.dot(_80) >= 0.0f) {
          return false;
        }
        *a2 = vec;
        return true;
      }
    }
    return false;
  }

  if(al::isNerve(this, &Turn)) {
    al::calcFrontDir(a2, mActor);
    return true;
  }
  return false;
}
f32 PlayerStateRunHakoniwa::getCenterTiltRateMax() const {
    CRASH;
}
f32 PlayerStateRunHakoniwa::getCapDynamicsRate() const {
    CRASH;
}
void PlayerStateRunHakoniwa::exePivot() {
    if(al::isFirstStep(this)) {
      // animator
      mActionPivotTurnControl->reset();
      mTurnTiltRate = 0.0f;
    }
    mActionPivotTurnControl->update();
    _A4 = 0;
    val1 = 0.0f;
    if(mActionPivotTurnControl->_45) {
      if(mActionPivotTurnControl->_44)
        al::setNerve(this, &Run);
      else
        kill();
    }
}
void PlayerStateRunHakoniwa::exeRun() {
  if ( al::isFirstStep(this) )
  {
    mActionGroundMoveControl->reset(mActionGroundMoveControl->mGravityDir);
    this->_A0 = 0;
    this->_90 = 0;
  }

  mActionGroundMoveControl->_9C = mCounterForceRun->getCounter() > 0;
  mActionGroundMoveControl->_A0 = mCounterForceRun->getSpeed();
  if ( al::isNerve(this, &RunAfterTurn) )
  {
    bool isLessEqualStep = al::isLessEqualStep(this, mConst->getRunAfterTurnFrame());
    mActionGroundMoveControl->setup(
      isLessEqualStep ? _8C : mConst->getNormalMaxSpeed(),
      mConst->getNormalMinSpeed(),
      isLessEqualStep ? mConst->getRunAfterTurnFrame() : mConst->getNormalAccelFrame(),
      mConst->getStickOnBrakeFrame(),
      mConst->getNormalBrakeFrame(),
      mConst->getGravityMove(),
      mConst->getNormalMaxSpeed() * mConst->getBrakeOnSpeedRate(),
      mConst->getBrakeOnCounterBorder()
    );
  }

  f32 v22 = mActionGroundMoveControl->update();
  if ( rs::isOnGroundSkateCode(this->mActor, this->mCollision) )
    v22 = this->mConst->getDashJudgeSpeed() + this->mConst->getRunSkateAnimSpeedOffset();

  if ( al::isFirstStep(this) )
  {
    //this->mAnimControlRun->reset(v22, al::isNerve(this, &RunAfterTurn));
  }

  //PlayerAnimControlRun::update((__int64)this->mAnimControlRun, &this->mActionGroundMoveControl->_84.x, v24, v22);
  f32 ikBlendFrameRun = this->mConst->getIKBlendFrameRun();
  s32 v30 = al::isNearZeroOrGreater(v22 - this->mConst->getNormalMinSpeed(), 0.001f) ? ikBlendFrameRun : 0;

  this->_A4 = al::converge(this->_A4, v30, 1);
  f32 v32 = 1.0;
  if ( ikBlendFrameRun >= 1 )
    v32 = sead::Mathf::clamp((float)this->_A4 / (float)ikBlendFrameRun, 0.0f, 1.0f);

  f32 v40 = 1.0f - al::calcRate01(v22, this->mConst->getNormalMinSpeed(), this->mConst->getNormalMaxSpeed());
  f32 v43 = al::lerpValue(this->mConst->getIKBlendRateRunMin(), this->mConst->getIKBlendRateRunMax(), sead::Mathf::clamp(v40, 0.0f, 1.0f));
  f32 v41 = sead::Mathf::clamp(1.0f - v32, 0.0f, 1.0f);

  this->val1 = v43 * v41;
  this->mTurnTiltRate = this->mActionGroundMoveControl->calcTurnTiltRate();
  s32 runDeepDownFrame = this->mConst->getRunDeepDownFrame();
  s32 runDeepDownMargine = this->mConst->getRunDeepDownMargine();
  s32 v50 = mInput->isMoveDeepDown() ? runDeepDownMargine + runDeepDownFrame : 0;

  this->_90 = al::converge(this->_90, v50, 1);
  if ( this->mActionGroundMoveControl->_64 )
  {
    if ( v22 > this->mConst->getRunBorderSpeed() - this->mConst->getRunBlendRange() )
    {
      //rs::tryCancelHeadSlidingLandSubAnim((rs *)this->mAnimator, v56);
      al::setNerve(this, &Brake);
      return;
    }

    sead::Vector3f* velocityPtr = al::getVelocityPtr(this->mActor);
    al::parallelizeVec(velocityPtr, this->mActionGroundMoveControl->mGravityDir, *velocityPtr);
    if ( this->_90 >= runDeepDownFrame )
      mTrigger->set(PlayerTrigger::EActionTrigger_QuickTurn);

    kill();
    return;
  }

  if ( this->mActionGroundMoveControl->_BC )
  {
    //rs::tryCancelHeadSlidingLandSubAnim((rs *)this->mAnimator, v52);
    if ( this->_90 >= runDeepDownFrame )
      mTrigger->set(PlayerTrigger::EActionTrigger_QuickTurn);

    al::calcFrontDir(&this->_80, this->mActor);
    al::setNerve(this, &Pivot);
    return;
  }

  if ( this->mActionGroundMoveControl->_80 )
  {
    if ( this->_90 < runDeepDownFrame )
      mTrigger->set(PlayerTrigger::EActionTrigger_QuickTurn);

    kill();
    return;
  }

  s32 wallPushFrame = this->mConst->getWallPushFrame();
  if ( rs::updateJudgeAndResult(this->mJudgeWallPush) )
    this->_A0 = al::converge(this->_A0, wallPushFrame, 1);
  else
    this->_A0 = 0;

  if ( this->_A0 == wallPushFrame )
  {
    if ( rs::findWallEnablePush(mActor, mCollision, this->mConst->getTall(), this->mConst->getCollisionRadius()) )
    {
      al::setNerve(this, &Push);
      return;
    }

    this->_A0 = 0;
  }
}
void PlayerStateRunHakoniwa::exeBrake() {
    if(al::isFirstStep(this)) {
      mAnimator->startAnim("Brake");
      mTurnTiltRate = 0.0f;
      _8C = 0.0f;
      al::calcFrontDir(&_74, mActor);
      _A4 = 0;
      val1 = 0.0f;
    }

    f32 v20 = 0.0f;
    rs::moveBrakeRun(&v20, &_74, mActor, mActionGroundMoveControl, mConst->getNormalMaxSpeed(), mConst->getNormalBrakeFrame(), mConst->getGravityMove(), mConst->getSlerpQuatRate(), mConst->getHillPoseDegreeMax());

    if(al::isFirstStep(this)) {
      _8C = sead::Mathf::clamp(v20, mConst->getNormalMaxSpeed(), mConst->getRunAfterTurnSpeedMax());
    }

    sead::Vector3f moveDirection = {0.0f, 0.0f, 0.0f};
    mInput->calcMoveDirection(&moveDirection, mActionGroundMoveControl->mGravityDir);

    bool isMoveDeepDown = mInput->isMoveDeepDown();
    f32 dot = _74.dot(moveDirection);
    _70 = isMoveDeepDown && dot <= 0.0f;

    if(isMoveDeepDown) {
      if(dot > 0.0f) {
        al::setNerve(this, &Run);
        return;
      }
      if(al::isGreaterEqualStep(this, mConst->getBrakeTurnStartFrame())) {
        al::setNerve(this, &Turn);
        return;
      }
    }

    if(!al::isLessStep(this, mConst->getNormalBrakeFrame())) {
      kill();
      return;
    }
}
void PlayerStateRunHakoniwa::exeTurn() {
    if(al::isFirstStep(this)) {
      mAnimator->startAnim("Turn");
      al::faceToDirectionSupportUp(mActor, -_74);
      mTurnTiltRate = 0.0f;
      _A4 = 0;
      val1 = 0.0f;
    }

    rs::moveBrakeRun(nullptr, &_74, mActor, mActionGroundMoveControl, mConst->getNormalMaxSpeed(), mConst->getNormalBrakeFrame(), mConst->getGravityMove(), mConst->getSlerpQuatRate(), mConst->getHillPoseDegreeMax());
    if(mAnimator->isAnimEnd()) {
      sead::Vector3f front = {0.0f, 0.0f, 0.0f};
      al::calcFrontDir(&front, mActor);
      al::setVelocity(mActor, ((_8C * front) * mConst->getRunAfterTurnScale()) - (mActionGroundMoveControl->mGravityDir * mConst->getGravityMove()));
      al::setNerve(this, &RunAfterTurn);
    }
}
void PlayerStateRunHakoniwa::exeWallPush() {
    CRASH;
}
