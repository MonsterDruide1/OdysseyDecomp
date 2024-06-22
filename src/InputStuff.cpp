#include <basis/seadTypes.h>
#include "Library/Controller/InputFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Math/MathAngleUtil.h"
#include "Library/Math/MathLengthUtil.h"
#include "Library/stuff.h"
#include "MapObj/AnagramAlphabetCharacter.h"
#include "Player/PlayerFunction.h"
#include "Player/PlayerInput.h"
#include "Player/PlayerInputFunction.h"
#include "Util/ActorDimensionKeeper.h"
#include "math/seadMathCalcCommon.h"


bool PlayerInputFunction::isTriggerCancelWorldWarp(al::LiveActor const*, int) {CRASH}
bool PlayerInputFunction::isTriggerStartWorldWarp(al::LiveActor const*, int) {CRASH}
bool PlayerInputFunction::isTriggerTalk(al::LiveActor const*, int) {CRASH}
bool PlayerInputFunction::isTriggerJump(al::LiveActor const*, int port) {
    return al::isPadTriggerA(port) || al::isPadTriggerB(port);
}
bool PlayerInputFunction::isTriggerSubAction(al::LiveActor const*, int port) {
    return al::isPadTriggerZL(port) || al::isPadTriggerZR(port);
}
bool PlayerInputFunction::isHoldSubAction(al::LiveActor const*, int port) {
    return al::isPadHoldZL(port) || al::isPadHoldZR(port);
}
bool PlayerInputFunction::isHoldAction(al::LiveActor const*, int) {CRASH}
bool PlayerInputFunction::isHoldJump(al::LiveActor const*, int port) {
    return al::isPadHoldA(port) || al::isPadHoldB(port);
}
bool PlayerInputFunction::isReleaseAction(al::LiveActor const*, int) {CRASH}
bool PlayerInputFunction::isReleaseJump(al::LiveActor const*, int) {CRASH}

void PlayerInput::calcMoveInput(sead::Vector3f* out, const sead::Vector3f & in) const {
    if(mIsDisableInput) {
        *out = sead::Vector3f::zero;
    }
    calcMoveInputImpl(out, in, false, false, false);
}

bool PlayerInput::isMove() const {
    return !mIsDisableInput && mIsMove;
}


void sub_710044F864(
        sead::Vector3f *a1,
        sead::Vector3f *x1_0,
        const sead::Vector3f *x2_0,
        const sead::Matrix34f *mtx,
        sead::Vector3f aVec)
{
  float v19; // s18
  float v20; // s19
  float v21; // s20
  float v22; // s21
  sead::Vector3f v34; // [xsp+0h] [xbp-50h] BYREF
  sead::Vector3f a3; // [xsp+10h] [xbp-40h] BYREF

  sead::Vector3f mtx0, mtx1, mtx2;

  mtx->getBase(mtx0, 0);
  mtx->getBase(mtx1, 1);
  mtx->getBase(mtx2, 2);

  v19 = aVec.dot(mtx1);
  v20 = aVec.dot(-mtx2);

  v21 = sead::Mathf::abs(v20);
  v22 = sead::Mathf::abs(v19);

  sead::Vector3f vec;

  if ( v21 > v22 ) {
    if ( v20 < 0.0 ) {
      vec = mtx1;
    } else {
      vec = -mtx1;
    }
  }
  else {
    if ( v19 >= 0.0 ) {
      vec = -mtx2;
    } else {
      vec = mtx2;
    }
  }
  *x1_0 = vec;

  a3.setCross(mtx0, vec);
  al::normalize(&a3);

  v34 = {0.0f, 0.0f, 0.0f};
  al::verticalizeVec(&v34, mtx0, *x2_0);
  if ( !al::tryNormalizeOrZero(&v34) )
    v34 = *x2_0;

  al::alongVectorNormalH(x1_0, *x1_0, a3, v34);
  al::normalize(x1_0);

  a1->setCross(*x1_0, *x2_0);
  al::normalize(a1);
}

void PlayerInput::calcMoveInputImpl(sead::Vector3f* out, const sead::Vector3f& in, bool flag1, bool flag2, bool flag3) const {
  int PlayerInputPort; // w23
  int PlayerControllerPort; // w0
  int somethingAboutP1; // w2
  const sead::Matrix34f *v21; // x23
  float v22; // s2
  float v23; // s0
  float v30; // s8
  float v31; // s0
  float v32; // s0
  float v34; // s8
  float v35; // s0
  float v36; // s0
  float v37; // s2
  const al::LiveActor *mLiveActor; // x22
  const IUseDimension *v39; // x21
  float v40; // s1
  float v41; // s0
  float v42; // s1
  float v43; // s0
  float v44; // s2
  float v45; // s0
  float v46; // s3
  sead::Vector3f v47; // [xsp+10h] [xbp-90h] BYREF
  sead::Vector3f v48; // [xsp+20h] [xbp-80h] BYREF
  sead::Vector3f v49; // [xsp+30h] [xbp-70h] BYREF
  sead::Vector3f v50; // [xsp+40h] [xbp-60h] BYREF
  sead::Vector2f v51; // [xsp+68h] [xbp-38h] BYREF

  if ( this->mIsDisableInput ) {
    *out = sead::Vector3f::zero;
    return;
  }

  PlayerInputPort = PlayerFunction::getPlayerInputPort(this->mLiveActor);
  if ( rs::isSeparatePlay(this->mLiveActor) )
  {
    if ( flag1 )
    {
      PlayerControllerPort = al::getPlayerControllerPort(1);
      somethingAboutP1 = this->somethingAboutP1;
    }
    else
    {
      PlayerControllerPort = al::getPlayerControllerPort(0);
      somethingAboutP1 = this->somethingAboutP0;
    }

    PlayerInputPort = PlayerControllerPort;
  }
  else
  {
    somethingAboutP1 = 0;
  }

  v51 = PlayerInputFunction::getMoveInputStick(this->mLiveActor, PlayerInputPort, somethingAboutP1);
  v21 = PlayerFunction::getPlayerViewMtx(this->mLiveActor);
  if ( this->mDimension && rs::is2D(this->mDimension) && rs::isIn2DArea(this->mDimension) )
  {
    CRASH
    // do not do 2d sections
  }

  if ( this->somethingFromUpdate )
  {
    PlayerInput::calcHoldMoveInput3D(out, in, v21);
  }
  else
  {
    v50 = {0.0f, 0.0f, 0.0f};
    v49 = {0.0f, 0.0f, 0.0f};
    sub_710044F864(&v50, &v49, &in, v21, -al::getGravity(this->mLiveActor));
    *out = (v51.x * v50) + (v51.y * v49);
    al::isNearZero(v51, 0.001);
  }

  v30 = sead::Mathf::clampMax(v51.length(), 1.0f);
  if ( v30 >= 0.1 )
  {
    v31 = out->length();
    if ( v31 > 0.0 )
    {
      v32 = v30 / v31;
      *out = v32 * *out;
    }
  }
  else
  {
    *out = {0.0f, 0.0f, 0.0f};
  }

  PlayerInput::snapWallAlongInput(out, in);
  if ( !flag3 )
    PlayerInput::snapAreaInput(out, in);
}

void PlayerInput::calcHoldMoveInput3D(sead::Vector3f *, const sead::Vector3f &, const sead::Matrix34f *) const {
  CRASH
}
void PlayerInput::snapWallAlongInput(sead::Vector3f * a2, const sead::Vector3f & a3) const {
  if(readInWalls > 0.0f && !al::isNearZero(a3, 0.001f))
    CRASH
}
void PlayerInput::snapAreaInput(sead::Vector3<float> *, const sead::Vector3<float> &) const {
  if(hasSomeSnapMoveDirArea)
    CRASH
}

void PlayerInput::calcMoveDirection(sead::Vector3f * dir, const sead::Vector3f & a3) const {
  if(mIsDisableInput || !mIsMove)
    *dir = {0.0f, 0.0f, 0.0f};
  else {
    calcMoveInputImpl(dir, a3, false, false, false);
    al::tryNormalizeOrZero(dir);
  }
}

bool PlayerInput::isMoveDeepDown() const {
  if(mIsDisableInput || !mIsMove) return false;

  // more utility stuff

  return PlayerInputFunction::getMoveInputStick(mLiveActor, 0, 0).squaredLength() > 0.64f;
}


namespace rs {

bool isSeparatePlay(al::IUseSceneObjHolder const*) {
  return false;
}
bool isHoldHackJump(IUsePlayerHack*) {CRASH}

}
