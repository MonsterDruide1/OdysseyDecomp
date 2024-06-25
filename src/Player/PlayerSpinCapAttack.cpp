#include "Player/PlayerSpinCapAttack.h"
#include "CUSTOM/PlayerAnimator.h"
#include "Library/Math/MathAngleUtil.h"
#include "Library/Math/MathLengthUtil.h"
#include "Library/Math/MathUtil.h"
#include "Player/HackCap.h"
#include "Player/PlayerCounterAfterCapCatch.h"
#include "Player/PlayerInput.h"
#include "Player/PlayerInputFunction.h"
#include "Player/PlayerJudgePreInputCapThrow.h"
#include "Player/PlayerTrigger.h"

void sub_7100464C88(
        sead::FixedSafeString<64> *a1,
        const char *a2,
        const PlayerInput *a3,
        char a4,
        const sead::Vector3f& a5,
        const sead::Vector2f& a6,
        const sead::Vector2f& a7,
        int a8)
{
  const char *v16; // x1
  const char *v17; // x3
  sead::FixedSafeString<64> *v18; // x0
  const char *v19; // x2
  int v20; // w8
  unsigned int v21; // w20

  if ( a3->isThrowTypeRolling(a6) )
  {
    v16 = "%s%s";
    if ( a6.y <= 0.0 )
      v17 = "DoubleDown";
    else
      v17 = "DoubleUp";

    goto LABEL_18;
  }

  if ( a3->isThrowTypeSpiral(a6) )
  {
    v16 = "%s%s";
    if ( a6.x <= 0.0 )
      v17 = "DoubleLeft";
    else
      v17 = "DoubleRight";

    goto LABEL_18;
  }

  if ( !al::isNearZero(a5, 0.001) )
  {
    if ( a5.y >= 0.0 )
    {
LABEL_17:
      v16 = "%s%s";
      v17 = "Left";
      goto LABEL_18;
    }

LABEL_16:
    v16 = "%s%s";
    v17 = "Right";

LABEL_18:
    v18 = a1;
    v19 = a2;
    v18->format(v16, v19, v17);
    return;
  }

  if ( a8 != 1 || (a4 & 1) != 0 )
  {
    if ( a8 == 2 )
      v20 = 2;
    else
      v20 = 3;

    if ( (a4 & 1) != 0 )
      v21 = 3;
    else
      v21 = v20;

    if ( a3->isThrowTypeLeftRight(a7) )
    {
      v16 = "%s%d%s";
      v19 = "SpinCapStart";
      v18 = a1;
      v17 = (const char *)v21;
      v18->format(v16, v19, v17);
      return;
    }

    a1->format("%s%dRight", "SpinCapStart", v21);
    return;
  }
  else
  {
    if ( a3->isThrowTypeLeftRight(a7) )
    {
      if ( a7.x <= 0.0 )
        goto LABEL_17;

      goto LABEL_16;
    }

    a1->format("%s", a2);
    return;
  }
}

PlayerSpinCapAttack::PlayerSpinCapAttack(HackCap* hackCap, PlayerConst const* pConst,
                                         PlayerTrigger const* trigger, PlayerInput const* input,
                                         PlayerCounterAfterCapCatch const* counterAfterCapCatch,
                                         PlayerJudgePreInputCapThrow const* judgePreInputCapThrow)
    : mHackCap(hackCap), mConst(pConst), mTrigger(trigger), mInput(input),
      mCounterAfterCapCatch(counterAfterCapCatch), mJudgePreInputCapThrow(judgePreInputCapThrow) {}

void PlayerSpinCapAttack::clearAttackInfo() {
    _31 = 0;
    _40 = 0;
}

void PlayerSpinCapAttack::setupAttackInfo() {
    if (_40 && mCounterAfterCapCatch->isCapCatch())
        _40++;
    else
        _40 = 1;

    if (mCounterAfterCapCatch->isCapCatch() && _40 >= 3) {
        _31 = 1;
        _40 = 0;
    } else {
        _31 = 0;
    }

    _44 = 0;
    _45 = 0;
    _34 = {0.0f, 0.0f, 0.0f};
    _30 = mJudgePreInputCapThrow->_44_2;
    _50 = {0.0f, 0.0f};
    _48 = {0.0f, 0.0f};
    _58 = 0.0f;

    if (mJudgePreInputCapThrow->_30_2 != 4) {
        if (mTrigger->isOnSpinMoveCapThrow()) {
            _44 = 1;
            _45 = 1;
            if (mTrigger->isOn(PlayerTrigger::EActionTrigger_val27))
                _50 = -sead::Vector2f::ex;
            else
                _50 = sead::Vector2f::ey;
        } else {
            if (!mTrigger->isOnHipDropCancelThrow()) {
                if (mJudgePreInputCapThrow->_30_2 == 3) {
                    _45 = 1;
                    _48 = mJudgePreInputCapThrow->_34_2;
                } else if (mJudgePreInputCapThrow->_30_2 == 2) {
                    _44 = 1;
                    _48 = mJudgePreInputCapThrow->_34_2;
                }
            } else {
                _44 = 1;
                _45 = 1;
                _50 = -sead::Vector2f::ey;
            }
        }
    }

    if (mInput->isThrowTypeRolling(_50) || mInput->isThrowTypeSpiral(_50)) {
        _31 = 0;
        _34 = {0.0f, 0.0f, 0.0f};
    } else if (mInput->isThrowTypeLeftRight(_48)) {
        f32 degree = al::calcAngleDegree(
            al::isNearZeroOrGreater(_48.x, 0.001f) ? sead::Vector2f::ex : -sead::Vector2f::ex, _48);
        f32 rate = al::calcRate01(sead::Mathf::abs(degree) * 1.2f, 6.0f, 70.0f);
        if (rate > 0.0f)
            _58 = al::sign(degree) * al::lerpValue(6.0f, 70.0f, al::easeOut(rate));
    }
}

void PlayerSpinCapAttack::startCapSpinAttack(PlayerAnimator* animator, PlayerInput const* input) {
    if (animator->isUpperBodyAnimAttached())
        animator->clearUpperBodyAnim();

    sub_7100464C88(&_60, "SpinCapStart", mInput, _31, _34, _50, _48, _40);
    animator->startAnimSpinAttack(_60);
    animator->startSubAnim(_60);
    mHackCap->startSpinAttack(_60);
    _B8.format("");
}

void PlayerSpinCapAttack::startCapSpinAttackAir(PlayerAnimator* animator,
                                                PlayerInput const* input) {
    if (animator->isUpperBodyAnimAttached())
        animator->clearUpperBodyAnim();

    sub_7100464C88(&_60, "SpinCapAirStart", mInput, _31, _34, _50, _48, _40);
    animator->startAnimSpinAttack(_60);
    if (animator->unk2)
        animator->endSubAnim();
    mHackCap->startSpinAttack(_60);
    _B8.format("");
}

void PlayerSpinCapAttack::startCapSpinAttackSwim(PlayerAnimator* animator,
                                                 PlayerInput const* input) {
    _31 = false;
    if (animator->isUpperBodyAnimAttached())
        animator->clearUpperBodyAnim();

    sub_7100464C88(&_60, "SpinCapAirStart", mInput, _31, _34, _50, _48, _40);
    animator->startAnimSpinAttack(_60);
    mHackCap->startSpinAttack(_60);
    _B8.format("");
}

void PlayerSpinCapAttack::startSpinSeparate(PlayerAnimator* animator) {
    animator->startAnim("SpinSeparate");
    animator->startSubAnim("SpinSeparate");
}

void PlayerSpinCapAttack::startSpinSeparateSwim(PlayerAnimator* animator) {
    animator->startAnim("SpinSeparateSwim");
    animator->startSubAnim("SpinSeparateSwim");
}

void PlayerSpinCapAttack::startSpinSeparateSwimSurface(PlayerAnimator* animator) {
    animator->startAnim("SwimSpinCapStartRight");
    animator->startSubAnim("SwimSpinCapStartRight");
}

void PlayerSpinCapAttack::startCapThrow(sead::Vector3<float> const& a2,
                                        sead::Vector3<float> const& a3, float a4, bool a5,
                                        sead::Vector3<float> const& a6) {
    int v6 = 2;
    if (_44)
        v6 = 3;
    if (!_45)
        v6 = _44;
    mHackCap->startThrow(_31, a2, a3, a4, _48, _50, _34, a5, a6, (HackCap::SwingHandType)v6, _30, _58, _40);
}

void PlayerSpinCapAttack::attackSpinMsg(al::HitSensor* a2, al::HitSensor* a3) {
    mHackCap->attackSpin(a2, a3, mConst->getCollisionRadius());
}

bool PlayerSpinCapAttack::tryCancelCapState(PlayerAnimator* animator) {
    if(!mHackCap->cancelCapState())
        return false;
    CapFunction::putOnCapPlayer(mHackCap, animator);
    return true;
}

bool PlayerSpinCapAttack::tryStartCapSpinGroundMiss(PlayerAnimator* animator) {
    if(_60.isEmpty() || (animator->unk2 && !animator->isSubAnim(_60)))
        return false;
    _B8.format("SpinSeparate");
    animator->startSubAnim(_B8);
    return true;
}

bool PlayerSpinCapAttack::tryStartCapSpinAirMiss(PlayerAnimator* animator) {
    if(_60.isEmpty() || (animator->unk2 && !animator->isSubAnim(_60)))
        return false;
    if(animator->isAnim(_60))
        animator->startAnim("Fall");
    _B8.format("SpinSeparate");
    animator->startSubAnimOnlyAir(_B8);
    return true;
}

bool PlayerSpinCapAttack::isCapSpinAttack() const {
    return mHackCap->isSpinAttack();
}

bool PlayerSpinCapAttack::isValidAttackSensor(PlayerAnimator const* animator) const {
    if(animator->isSubAnim(_60))
        return animator->getSubAnimFrame() <= mConst->getSpinAttackFrame();
    if(animator->isAnim(_60))
        return animator->getAnimFrame() <= mConst->getSpinAttackFrame();
}

bool PlayerSpinCapAttack::isEnablePlaySpinCapMiss(PlayerAnimator const* animator) const {
    return !animator->unk2 || !animator->isSubAnim(_B8);
}

bool PlayerSpinCapAttack::isSeparateSingleSpin() const {
    return rs::isSeparatePlay(mHackCap) && !_30;
}

bool PlayerSpinCapAttack::isThrowSwingRightDir() const {
    return mInput->isThrowTypeLeftRight(_48) && al::isNearZeroOrGreater(_48.x, 0.001f);
}

s32 PlayerSpinCapAttack::getThrowFrameGround() const {
    if(_31)
        return mConst->getSpinCapThrowFrameContinuous();
    if(!al::isNearZero(_34, 0.001f) || _44 || _45) {}
    else {
        if(_40 != 2)
            return mConst->getSpinCapThrowFrameSwing();
    }
    return mConst->getSpinCapThrowFrame();
}

s32 PlayerSpinCapAttack::getThrowFrameAir() const {
    if(_31)
        return mConst->getSpinCapThrowFrameContinuous();
    if(!al::isNearZero(_34, 0.001f) || _44 || _45) {}
    else {
        if(_40 != 2)
            return mConst->getSpinCapThrowFrameSwing();
    }
    return mConst->getSpinCapThrowFrameAir();
}

s32 PlayerSpinCapAttack::getThrowFrameSwim() const {
    if(_44 || _45)
        return mConst->getSpinCapThrowFrameSwing();
    return mConst->getSpinCapThrowFrame();
}
