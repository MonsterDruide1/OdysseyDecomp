#include "Player/PlayerActionPivotTurnControl.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Math/MathAngleUtil.h"
#include "Library/Math/MathLengthUtil.h"
#include "Library/stuff.h"
#include "Stuff.h"
#include "Util/Hack.h"

PlayerActionPivotTurnControl::PlayerActionPivotTurnControl(al::LiveActor* player,
                                                           PlayerConst const* pConst,
                                                           PlayerInput const* input,
                                                           IUsePlayerCollision const* collider,
                                                           float gravity)
    : mPlayer(player), mConst(pConst), mInput(input), mCollider(collider), mGravity(gravity) {}
void PlayerActionPivotTurnControl::reset() {
    _44 = 0;
    _45 = 0;
    _48 = 0;
    al::calcFrontDir(&_38, mPlayer);
    rs::calcGroundNormalOrGravityDir(&_28, mPlayer, mCollider);
}
void PlayerActionPivotTurnControl::update() {
  const sead::Vector3f *p__28; // x19
  float z; // w9
  const IUsePlayerHack **_20; // x8
  const sead::Vector3f *Velocity; // x0
  float mGravity; // s1
  float v8; // s5
  float v9; // s0
  al::LiveActor *mPlayer; // x21
  float v13; // s0
  bool v14; // w0
  int _48; // w8
  int v16; // w8
  float v17; // s7
  float v18; // s17
  float v19; // s5
  float v20; // s1
  al::LiveActor *v21; // x21
  const sead::Vector3f *v24; // x0
  float v25; // s0
  al::LiveActor *v26; // x22
  float v27; // s1
  float v28; // s2
  float v29; // s0
  al::LiveActor *v30; // x22
  sead::Vector3f v33; // [xsp+0h] [xbp-B0h] BYREF
  sead::Vector3f v34; // [xsp+10h] [xbp-A0h] BYREF
  sead::Vector3f v35; // [xsp+20h] [xbp-90h] BYREF
  sead::Quatf a1; // [xsp+30h] [xbp-80h] BYREF
  sead::Vector3f a2; // [xsp+40h] [xbp-70h] BYREF
  sead::Vector3f v38; // [xsp+50h] [xbp-60h] BYREF
  sead::Vector3f v39; // [xsp+60h] [xbp-50h] BYREF
  sead::Vector3f a3; // [xsp+70h] [xbp-40h] BYREF

  p__28 = &this->_28;
  a3 = this->_28;
  rs::calcGroundNormalOrGravityDir(&this->_28, this->mPlayer, this->mCollider);
  v39.x = 0.0;
  v39.y = 0.0;
  v39.z = 0.0;
  _20 = this->_20;
  if ( _20 )
    rs::calcHackerMoveDir(&v39, *_20, *p__28);
  else
    mInput->calcMoveDirection(&v39, *p__28);

  v38 = al::getVelocity(this->mPlayer);
  if ( rs::isOnGround(this->mPlayer, this->mCollider) )
  {
    Velocity = &al::getVelocity(this->mPlayer);
    al::alongVectorNormalH(&v38, *Velocity, a3, *p__28);
  }

  mGravity = this->mGravity;
  v8 = mGravity * this->_28.y;
  v9 = v38.z - (float)(mGravity * this->_28.z);
  v38.x = v38.x - (float)(mGravity * this->_28.x);
  v38.y = v38.y - v8;
  v38.z = v9;
  al::setVelocity(this->mPlayer, v38);
  mPlayer = this->mPlayer;
  v13 = this->mConst->getFallSpeedMax();
  al::limitVelocityDir(mPlayer, -this->_28, v13);
  v14 = al::isNearZero(v39, 0.001f);
  this->_44 = !v14;
  if ( v14 )
    al::alongVectorNormalH(&this->_38, this->_38, a3, *p__28);
  else
    this->_38 = v39;

  if ( al::tryNormalizeOrZero(&this->_38) )
  {
    a2.x = 0.0;
    a2.y = 0.0;
    a2.z = 0.0;
    al::calcFrontDir(&a2, this->mPlayer);
    al::verticalizeVec(&a2, *p__28, a2);
    if ( al::tryNormalizeOrZero(&a2) )
    {
      _48 = this->_48;
      if ( _48 + 1 <= 10 )
        v16 = _48 + 1;
      else
        v16 = 10;

      this->_48 = v16;
      a1.x = 0.0;
      a1.y = 0.0;
      a1.z = 0.0;
      a1.w = 1.0;
      al::makeQuatAxisRotation(&a1, a2, this->_38, *p__28, (float)v16 / 10.0);
      v17 = (float)((float)(a1.y * a2.z) - (float)(a1.z * a2.y)) + (float)(a1.w * a2.x);
      v18 = (float)(a2.y * a1.w) + (float)((float)(a1.z * a2.x) - (float)(a2.z * a1.x));
      v19 = (float)(a2.z * a1.w) + (float)((float)(a2.y * a1.x) - (float)(a1.y * a2.x));
      v20 = (float)((float)-(float)(a2.x * a1.x) - (float)(a1.y * a2.y)) - (float)(a2.z * a1.z);
      a2.x = (float)((float)(a1.y * v19) + (float)((float)(a1.w * v17) - (float)(a1.z * v18))) - (float)(a1.x * v20);
      a2.y = (float)((float)((float)(v17 * a1.z) + (float)(v18 * a1.w)) - (float)(v19 * a1.x)) - (float)(v20 * a1.y);
      a2.z = (float)((float)((float)(v18 * a1.x) - (float)(v17 * a1.y)) + (float)(v19 * a1.w)) - (float)(v20 * a1.z);
      al::tryNormalizeOrZero(&a2);
      if ( al::makeQuatRotationLimit(&a1, a2, this->_38, 0.087266f) )
      {
        v21 = this->mPlayer;
        rs::slerpUpFront(v21, *p__28, a2, this->mConst->getSlerpQuatRate(), this->mConst->getHillPoseDegreeMax());
        return;
      }

      v35.x = 0.0;
      v35.y = 0.0;
      v35.z = 0.0;
      v34.x = 0.0;
      v34.y = 0.0;
      v34.z = 0.0;
      v24 = &al::getVelocity(this->mPlayer);
      al::separateVectorParallelVertical(&v34, &v35, *p__28, *v24);
      v25 = v35.length();
      v26 = this->mPlayer;
      v27 = (float)(v25 * this->_38.x) + v34.x;
      v28 = (float)(v25 * this->_38.y) + v34.y;
      v29 = (float)(v25 * this->_38.z) + v34.z;
      v33.x = v27;
      v33.y = v28;
      v33.z = v29;
      al::setVelocity(v26, v33);
      v30 = this->mPlayer;
      rs::slerpUpFront(v30, *p__28, this->_38, this->mConst->getSlerpQuatRate(), this->mConst->getHillPoseDegreeMax());
    }
  }

  this->_45 = 1;
}
void PlayerActionPivotTurnControl::calcMoveDirection(sead::Vector3<float>*,
                                                     sead::Vector3<float> const&) {
    CRASH
}
