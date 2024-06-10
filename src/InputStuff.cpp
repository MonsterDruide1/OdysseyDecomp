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


bool PlayerInputFunction::isTriggerCancelWorldWarp(al::LiveActor const*, int) {CRASH}
bool PlayerInputFunction::isTriggerStartWorldWarp(al::LiveActor const*, int) {CRASH}
bool PlayerInputFunction::isTriggerTalk(al::LiveActor const*, int) {CRASH}
bool PlayerInputFunction::isTriggerJump(al::LiveActor const*, int port) {
    return al::isPadTriggerA(port) || al::isPadTriggerB(port);
}
bool PlayerInputFunction::isTriggerSubAction(al::LiveActor const*, int port) {
    return al::isPadTriggerZL(port) || al::isPadTriggerZR(port);
}
bool PlayerInputFunction::isHoldSubAction(al::LiveActor const*, int) {CRASH}
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
        const sead::Matrix34f *a4,
        float a5,
        float a6,
        float a7)
{
  float v7; // w12
  float v8; // w13
  float v9; // w8
  float v10; // w10
  float v11; // s6
  float v12; // s3
  float v13; // s4
  float v14; // s17
  float v15; // s5
  float v16; // s18
  float v17; // s0
  float x; // s1
  float v19; // s18
  float v20; // s19
  float v21; // s20
  float v22; // s21
  float y; // s1
  float z; // s3
  float v28; // s2
  float v29; // s5
  float v30; // s6
  float v31; // s2
  float v32; // s0
  sead::Vector3f v34; // [xsp+0h] [xbp-50h] BYREF
  sead::Vector3f a3; // [xsp+10h] [xbp-40h] BYREF
  sead::Vector3f a2; // [xsp+20h] [xbp-30h] BYREF

  v7 = a4->m[0][1];
  v8 = a4->m[0][2];
  a2.x = a4->m[0][0];
  a2.y = v7;
  a2.z = v8;
  v9 = a4->m[1][2];
  v10 = a4->m[1][1];
  v11 = a4->m[1][0];
  v13 = a4->m[2][0];
  v12 = a4->m[2][1];
  v14 = -v13;
  v15 = a4->m[2][2];
  v16 = (float)(v11 * a5) + (float)(v10 * a6);
  v17 = (float)(a5 * (float)-v13) - (float)(a6 * v12);
  x = a2.x;
  v19 = v16 + (float)(v9 * a7);
  v20 = v17 - (float)(a7 * v15);
  if ( v20 <= 0.0 )
    v21 = -v20;
  else
    v21 = v17 - (float)(a7 * v15);

  if ( v19 <= 0.0 )
    v22 = -v19;
  else
    v22 = v19;

  if ( v21 > v22 )
  {
    if ( v20 < 0.0 )
    {
      x1_0->x = a4->m[1][0];
      x1_0->y = v10;
      v13 = v11;
      v12 = v10;
      v15 = v9;
      x1_0->z = v9;
      goto LABEL_15;
    }

    v13 = -v11;
    v12 = -v10;
    v15 = -v9;
    goto LABEL_13;
  }

  if ( v19 < 0.0 )
  {
LABEL_13:
    x1_0->x = v13;
    x1_0->y = v12;
    goto LABEL_14;
  }

  v12 = -v12;
  v15 = -v15;
  v13 = -v13;
  x1_0->x = v14;
  x1_0->y = v12;

LABEL_14:
  x1_0->z = v15;

LABEL_15:
  a3.x = (float)(v7 * v15) - (float)(v8 * v12);
  a3.y = (float)(v8 * v13) - (float)(v15 * x);
  a3.z = (float)(v12 * x) - (float)(v7 * v13);
  al::normalize(&a3);
  v34.x = 0.0;
  v34.y = 0.0;
  v34.z = 0.0;
  al::verticalizeVec(&v34, a2, *x2_0);
  if ( !al::tryNormalizeOrZero(&v34) )
    v34 = *x2_0;

  al::alongVectorNormalH(x1_0, *x1_0, a3, v34);
  al::normalize(x1_0);
  y = x1_0->y;
  z = x1_0->z;
  v29 = x2_0->y;
  v28 = x2_0->z;
  v30 = y * v28;
  v31 = (float)(z * x2_0->x) - (float)(v28 * x1_0->x);
  v32 = (float)(v29 * x1_0->x) - (float)(y * x2_0->x);
  a1->x = v30 - (float)(z * v29);
  a1->y = v31;
  a1->z = v32;
  al::normalize(a1);
}

void PlayerInput::calcMoveInputImpl(sead::Vector3f* out, const sead::Vector3f& in, bool flag1, bool flag2, bool flag3) const {
  int PlayerInputPort; // w23
  const al::IUseSceneObjHolder *v13; // x0
  int PlayerControllerPort; // w0
  int somethingAboutP1; // w2
  float x; // s8
  float y; // s9
  sead::Vector2f MoveInputStick; // kr00_8
  sead::Matrix34f *PlayerViewMtx; // x0
  const IUseDimension *mDimension; // x25
  sead::Matrix34f *v21; // x23
  float v22; // s2
  float v23; // s0
  const sead::Vector3f *Gravity; // x0
  float v25; // s0
  float v26; // s1
  float z; // s2
  float v28; // s1
  float v29; // s2
  float v30; // s8
  float v31; // s0
  float v32; // s0
  float v33; // s2
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
  if ( this->mLiveActor )
    v13 = this->mLiveActor;
  else
    v13 = 0LL;

  if ( rs::isSeparatePlay(v13) )
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

  MoveInputStick = PlayerInputFunction::getMoveInputStick(this->mLiveActor, PlayerInputPort, somethingAboutP1);
  y = MoveInputStick.y;
  x = MoveInputStick.x;
  v51 = MoveInputStick;
  PlayerViewMtx = PlayerFunction::getPlayerViewMtx(this->mLiveActor);
  mDimension = this->mDimension;
  v21 = PlayerViewMtx;
  if ( mDimension && (rs::is2D(this->mDimension) & 1) != 0 && (rs::isIn2DArea(mDimension) & 1) != 0 )
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
    Gravity = &al::getGravity(this->mLiveActor);
    v25 = Gravity->x;
    v26 = Gravity->y;
    z = Gravity->z;
    v50.x = 0.0;
    v50.y = 0.0;
    v50.z = 0.0;
    v49.x = 0.0;
    v49.y = 0.0;
    v49.z = 0.0;
    sub_710044F864(&v50, &v49, &in, v21, -v25, -v26, -z);
    v28 = (float)(v51.x * v50.x) + (float)(v51.y * v49.x);
    v29 = (float)(v51.x * v50.y) + (float)(v51.y * v49.y);
    out->z = (float)(v51.x * v50.z) + (float)(v51.y * v49.z);
    out->x = v28;
    out->y = v29;
    al::isNearZero(v51, 0.001);
    y = v51.y;
    x = v51.x;
  }

  v30 = fminf(sqrtf((float)(x * x) + (float)(y * y)), 1.0);
  if ( v30 >= 0.1 )
  {
    v31 = sqrtf((float)((float)(out->x * out->x) + (float)(out->y * out->y)) + (float)(out->z * out->z));
    if ( v31 > 0.0 )
    {
      v32 = v30 / v31;
      v33 = v32 * out->y;
      out->x = v32 * out->x;
      out->y = v33;
      out->z = v32 * out->z;
    }
  }
  else
  {
    out->x = 0.0;
    out->y = 0.0;
    out->z = 0.0;
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

bool PlayerInput::isMoveDeepDown() const {
  if(mIsDisableInput || mIsMove) return false;

  // more utility stuff

  return PlayerInputFunction::getMoveInputStick(mLiveActor, 0, 0).squaredLength() > 0.64f;
}


namespace rs {

bool isSeparatePlay(al::IUseSceneObjHolder const*) {
  return false;
}
bool isHoldHackJump(IUsePlayerHack*) {CRASH}

}
