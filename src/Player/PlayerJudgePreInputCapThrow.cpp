
#include "Player/PlayerJudgePreInputCapThrow.h"
#include "Player/PlayerCarryKeeper.h"
#include "Player/PlayerConst.h"
#include "Player/PlayerInput.h"

PlayerJudgePreInputCapThrow::PlayerJudgePreInputCapThrow(const PlayerConst* pConst,
                                                         const PlayerInput* input,
                                                         const PlayerCarryKeeper* carryKeeper,
                                                         const HackCap* hackCap)
    : mConst(pConst), mInput(input), mCarryKeeper(carryKeeper), mHackCap(hackCap) {}

void PlayerJudgePreInputCapThrow::recordJudgeAndReset() {
    _30_2 = mThrowType;
    _34_2 = mCapThrowDir;
    _3C_2 = _3C;
    _44_2 = _44;
    reset();
}

void PlayerJudgePreInputCapThrow::recordSeparateJudge() {
    _44_2 = 0;
    _30_2 = 1;
    _34_2 = {0.0f, 0.0f};
    _3C_2 = {0.0f, 0.0f};
}

void PlayerJudgePreInputCapThrow::recordCooperateAndReset() {
    _30_2 = mThrowType;
    _34_2 = mCapThrowDir;
    _3C_2 = _3C;
    _44_2 = true;
    reset();
}

void PlayerJudgePreInputCapThrow::reset() {
    mCounter1 = 0;
    mCounter2 = 0;
}

void PlayerJudgePreInputCapThrow::update() {
  int mCounter2; // w9
  int v3; // w8
  PlayerInput *mInput; // x0
  int v5; // w8
  PlayerInput *v6; // x0
  const sead::Vector2f *CapThrowDir; // x0
  float y; // w8
  PlayerInput *v10; // x0
  bool v11; // zf
  PlayerInput *v12; // x0
  int v13; // w8
  const sead::Vector2f *v14; // x0
  float v15; // w8
  const PlayerConst *v16; // x0
  float v17; // w8
  const PlayerConst *v18; // x0
  float x; // w9

  mCounter2 = this->mCounter2;
  v3 = this->mCounter1 - 1;
  if ( v3 < 0 )
    v3 = 0;

  mInput = (PlayerInput *)this->mInput;
  this->mCounter1 = v3;
  v5 = mCounter2 - 1;
  if ( mCounter2 - 1 < 0 )
    v5 = 0;

  this->mCounter2 = v5;
  if ( mInput->isTriggerSpinCap() )
  {
    if ( (mInput->isTriggerCapDoubleHandThrow() & 1) != 0 )
    {
      v6 = (PlayerInput *)this->mInput;
      this->_44 = 0;
      this->_3C.x = 0.0;
      this->_3C.y = 0.0;
      this->mCapThrowDir.x = 0.0;
      this->mCapThrowDir.y = 0.0;
      this->mThrowType = 4;
      CapThrowDir = &mInput->getCapThrowDir();
      this->_3C.x = CapThrowDir->x;
      y = CapThrowDir->y;
      this->_3C.y = y;
      this->mCounter1 = mConst->getPreInputFrameCapThrow();

LABEL_8:
      this->mCounter2 = this->mConst->getPreInputFrameCapThrow();
      goto LABEL_18;
    }

    if ( mInput->isTriggerCapSingleHandThrow() )
    {
      if ( this->mCounter2 <= 0 )
      {
        v10 = (PlayerInput *)this->mInput;
        *(float *)((char *)&this->_3C.x + 1) = 0.0;
        *(float *)((char *)&this->_3C.y + 1) = 0.0;
        this->mThrowType = 0;
        this->mCapThrowDir.x = 0.0;
        this->mCapThrowDir.y = 0.0;
        this->_3C.x = 0.0;
        v11 = !mInput->isTriggerSwingRightHand();
        v12 = (PlayerInput *)this->mInput;
        v13 = v11 ? 2 : 3;
        this->mThrowType = v13;
        v14 = &mInput->getCapThrowDir();
        this->mCapThrowDir.x = v14->x;
        v15 = v14->y;
        v16 = this->mConst;
        this->mCapThrowDir.y = v15;
        this->mCounter1 = v16->getPreInputFrameCapThrow();
        if ( mInput->isEnableConsiderCapThrowDoubleSwing() )
        {
          if ( mInput->isThrowTypeSpiral(this->mCapThrowDir) )
          {
            v17 = this->mCapThrowDir.y;

LABEL_27:
            this->mThrowType = 4;
            x = this->mCapThrowDir.x;
            this->mCapThrowDir = {0.0f, 0.0f};
            this->_3C.x = x;
            this->_3C.y = v17;
            goto LABEL_8;
          }

          if ( this->mCapThrowDir.y > 0.0 )
          {
            v17 = this->mCapThrowDir.y;
            goto LABEL_27;
          }
        }
      }
    }
    else
    {
      v18 = this->mConst;
      this->_44 = 0;
      this->_3C.x = 0.0;
      this->_3C.y = 0.0;
      this->mCapThrowDir.x = 0.0;
      this->mCapThrowDir.y = 0.0;
      this->mThrowType = 1;
      this->mCounter1 = v18->getPreInputFrameCapThrow();
    }
  }

LABEL_18:
  if ( mCarryKeeper->isThrow() )
    this->mCounter1 = 0;

  if ( (this->mHackCap->isRequestableReturn() & 1) != 0 )
    this->mCounter1 = 0;

  if ( (this->mHackCap->isEnablePreInput() & 1) == 0 )
    this->mCounter1 = 0;
}

bool PlayerJudgePreInputCapThrow::judge() const {
    if(mCarryKeeper->isThrow())
        return false;
    if(mInput->isTriggerSpinCap())
        return true;
    return mCounter1 > 0;
}
