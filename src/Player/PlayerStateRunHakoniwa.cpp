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
    CRASH;
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
  if(!al::isNerve(this, &Turn))
    return false;

  al::calcFrontDir(a2, mActor);
  return true;
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
  int mCounter; // w9
  float _8C; // s8
  const PlayerConst *mConst; // x21
  int v7; // w0
  bool isLessEqualStep; // w0
  const PlayerConst *v9; // x8
  long double v12; // q0
  float v13; // s9
  int v14; // w22
  int v15; // w23
  int v16; // w24
  long double v17; // q0
  float v18; // s10
  long double v19; // q0
  float v20; // s11
  int v21; // w0
  float v22; // s8
  float v23; // s8
  const PlayerConst *v27; // x20
  int v28; // w21
  float v29; // s0
  int v30; // w1
  int v31; // w0
  float v32; // s11
  long double v33; // q0
  float v34; // s9
  float v35; // s0
  float v36; // s10
  float v37; // s9
  long double v38; // q0
  float v39; // s1
  float v40; // s0
  float v41; // s10
  float v42; // s2
  float v43; // s0
  PlayerActionGroundMoveControl *v44; // x0
  float v45; // s0
  const PlayerConst *v46; // x0
  int v47; // w20
  int v48; // w21
  int _90; // w0
  int v50; // w1
  int v51; // w0
  PlayerAnimator *v52; // x1
  long double v53; // q0
  PlayerActionGroundMoveControl *v54; // x8
  float v55; // s9
  PlayerAnimator *v56; // x1
  sead::Vector3f *VelocityPtr; // x0
  int v59; // w20
  int v60; // w0
  float v63; // s8
  float v64; // s0

  if ( al::isFirstStep((al::LiveActor *)this) )
  {
    mActionGroundMoveControl->reset(mActionGroundMoveControl->mGravityDir);
    this->_A0 = 0;
    this->_90 = 0;
  }

  mCounterForceRun = this->mCounterForceRun;
  mCounter = mCounterForceRun->getCounter();
  mActionGroundMoveControl = this->mActionGroundMoveControl;
  _8C = this->_8C;
  mConst = this->mConst;
  mActionGroundMoveControl->_9C = mCounter > 0;
  mActionGroundMoveControl->_A0 = mCounterForceRun->getSpeed();
  if ( al::isNerve(this, &RunAfterTurn) )
  {
    v7 = mConst->getRunAfterTurnFrame();
    isLessEqualStep = al::isLessEqualStep(this, v7);
    v9 = mConst;
    if ( !isLessEqualStep )
    {
      _8C = v9->getNormalMaxSpeed();
    }

    v13 = mConst->getNormalMinSpeed();
    v14 = isLessEqualStep ? mConst->getRunAfterTurnFrame() : mConst->getNormalAccelFrame();
    v15 = mConst->getStickOnBrakeFrame();
    v16 = mConst->getNormalBrakeFrame();
    v17 = mConst->getGravityMove();
    v18 = *(float *)&v17;
    v19 = mConst->getNormalMaxSpeed();
    v20 = *(float *)&v19 * mConst->getBrakeOnSpeedRate();
    v21 = mConst->getBrakeOnCounterBorder();
    mActionGroundMoveControl->setup(_8C, v13, v14, v15, v16, v18, v20, v21);
  }

  mActionGroundMoveControl->update();
  if ( rs::isOnGroundSkateCode(this->mActor, this->mCollision) )
  {
    v23 = this->mConst->getDashJudgeSpeed();
    v22 = v23 + this->mConst->getRunSkateAnimSpeedOffset();
  }

  if ( al::isFirstStep((al::LiveActor *)this) )
  {
    //this->mAnimControlRun->reset(v22, al::isNerve(this, &RunAfterTurn));
  }

  //PlayerAnimControlRun::update((__int64)this->mAnimControlRun, &this->mActionGroundMoveControl->_84.x, v24, v22);
  v27 = this->mConst;
  v28 = v27->getIKBlendFrameRun();
  v29 = v27->getNormalMaxSpeed();
  if ( al::isNearZeroOrGreater(v22 - v29, 0.001) )
    v30 = v28;
  else
    v30 = 0;

  v31 = al::converge(this->_A4, v30, 1);
  v32 = 1.0;
  this->_A4 = v31;
  if ( v28 >= 1 )
  {
    v32 = (float)v31 / (float)v28;
    if ( v32 >= 0.0 )
    {
      if ( v32 > 1.0 )
        v32 = 1.0;
    }
    else
    {
      v32 = 0.0;
    }
  }

  v33 = v27->getNormalMinSpeed();
  v34 = *(float *)&v33;
  v35 = v27->getNormalMaxSpeed();
  v36 = al::calcRate01(v22, v34, v35);
  v37 = v27->getIKBlendRateRunMin();
  v38 = v27->getIKBlendRateRunMax();
  v39 = *(float *)&v38;
  v40 = 1.0 - v36;
  v41 = 0.0;
  v42 = 0.0;
  if ( v40 >= 0.0 )
  {
    v42 = v40;
    if ( v40 > 1.0 )
      v42 = 1.0;
  }

  v43 = al::lerpValue(v37, v39, v42);
  if ( (float)(1.0 - v32) >= 0.0 )
  {
    v41 = 1.0 - v32;
    if ( (float)(1.0 - v32) > 1.0 )
      v41 = 1.0;
  }

  v44 = this->mActionGroundMoveControl;
  this->val1 = v43 * v41;
  v45 = v44->calcTurnTiltRate();
  v46 = this->mConst;
  this->mTurnTiltRate = v45;
  v47 = v46->getRunDeepDownFrame();
  v48 = this->mConst->getRunDeepDownMargine();
  if ( mInput->isMoveDeepDown() )
  {
    _90 = this->_90;
    v50 = v48 + v47;
  }
  else
  {
    _90 = this->_90;
    v50 = 0;
  }

  v51 = al::converge(_90, v50, 1);
  v54 = this->mActionGroundMoveControl;
  this->_90 = v51;
  if ( v54->_64 )
  {
    v55 = this->mConst->getRunBorderSpeed();
    if ( v22 > (float)(v55 - this->mConst->getRunBlendRange()) )
    {
      //rs::tryCancelHeadSlidingLandSubAnim((rs *)this->mAnimator, v56);
      al::setNerve(this, &Brake);
    }

    VelocityPtr = al::getVelocityPtr(this->mActor);
    al::parallelizeVec(VelocityPtr, this->mActionGroundMoveControl->mGravityDir, *VelocityPtr);
    if ( this->_90 < v47 )
      goto LABEL_42;

    goto LABEL_41;
  }

  if ( v54->_BC )
  {
    //rs::tryCancelHeadSlidingLandSubAnim((rs *)this->mAnimator, v52);
    if ( this->_90 >= v47 )
      mTrigger->set(PlayerTrigger::EActionTrigger_QuickTurn);

    al::calcFrontDir(&this->_80, this->mActor);
    al::setNerve(this, &Pivot);
    return;
  }

  if ( v54->_80 )
  {
    if ( v51 < v47 )
    {
LABEL_42:
      kill();
      return;
    }

LABEL_41:
    mTrigger->set(PlayerTrigger::EActionTrigger_QuickTurn);
    goto LABEL_42;
  }

  v59 = this->mConst->getWallPushFrame();
  if ( (rs::updateJudgeAndResult((IJudge *)this->mJudgeWallPush) & 1) != 0 )
  {
    v60 = al::converge(this->_A0, v59, 1);
    this->_A0 = v60;
  }
  else
  {
    v60 = 0;
    this->_A0 = 0;
  }

  if ( v60 == v59 )
  {
    mCollision = (IUsePlayerCollision *)this->mCollision;
    v63 = this->mConst->getTall();
    v64 = this->mConst->getCollisionRadius();
    if ( rs::findWallEnablePush(mActor, mCollision, v63, v64) )
    {
      al::setNerve(this, &Push);
      return;
    }

    this->_A0 = 0;
  }
}
void PlayerStateRunHakoniwa::exeBrake() {
    CRASH;
}
void PlayerStateRunHakoniwa::exeTurn() {
    CRASH;
}
void PlayerStateRunHakoniwa::exeWallPush() {
    CRASH;
}
