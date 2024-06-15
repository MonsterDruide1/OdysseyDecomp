#include "Player/PlayerActionSlopeSlideControl.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Math/MathAngleUtil.h"
#include "Library/Math/MathLengthUtil.h"
#include "Library/stuff.h"
#include "Player/PlayerInput.h"
#include "PlayerUtil.h"
#include "Stuff.h"

PlayerActionSlopeSlideControl::PlayerActionSlopeSlideControl(al::LiveActor* player, PlayerConst const* pConst,
                                                             PlayerInput const* input,
                                                             IUsePlayerCollision const* collider) : mPlayer(player),
                                                                                                    mConst(pConst),
                                                                                                    mInput(input),
                                                                                                    mCollision(collider) {}

void PlayerActionSlopeSlideControl::setup() {
    _2c = al::getGravity(mPlayer);
    _20 = {0.0f, 0.0f, 0.0f};

    if(rs::isCollidedGround(mCollision)) {
        al::calcDirSlide(&_20, al::getGravity(mPlayer), rs::getCollidedGroundNormal(mCollision));
        if(!rs::isJustLand(mCollision)) {
            _2c = rs::getCollidedGroundNormal(mCollision);
        }
    }

    _38 = {0.0f, 0.0f, 0.0f};
}

void PlayerActionSlopeSlideControl::setupCutSlideOppositeDir() {
    setup();
    sead::Vector3f a4 = {0.0f, 0.0f, 0.0f};
    rs::calcGroundNormalOrGravityDir(&a4, mPlayer, mCollision);
    sead::Vector3f v10 = {0.0f, 0.0f, 0.0f};
    sead::Vector3f* velocityPtr = al::getVelocityPtr(mPlayer);
    al::alongVectorNormalH(velocityPtr, *velocityPtr, _2c, a4);
    if(rs::calcSlideDir(&v10, al::getGravity(mPlayer), a4)) {
        al::limitVectorOppositeDir(velocityPtr, v10, *velocityPtr, velocityPtr->length());
    }

    *velocityPtr -= mConst->getGravityMove() * a4;
    _2c = a4;
}

f32 PlayerActionSlopeSlideControl::update(float a1, float a2, float a3, float a5, float a6, float a7, float a8, float a9,
                                          float a11, float a12, bool a13) {
  float y; // x8^4
  const sead::Vector3f *p__2c; // x20
  float z; // w9
  const sead::Vector3f *Gravity; // x0
  float v23; // s9
  float v24; // s13
  float v25; // s0
  float v26; // s10
  char v27; // w22
  const sead::Vector3f *CollidedGroundNormal; // x0
  float x; // w8
  float v30; // w9
  float v31; // w10
  const sead::Vector3f *v32; // x0
  float v33; // s12
  float v34; // s1
  sead::Vector3f *p_vec1; // x0
  const sead::Vector3f *v36; // x1
  float v37; // s0
  al::LiveActor *mPlayer; // x0
  const sead::Vector3f *Velocity; // x0
  float v40; // s9
  sead::Vector3f *VelocityPtr; // x21
  const sead::Vector3f *v42; // x0
  sead::Vector3f *v43; // x0
  float v44; // s2
  float v45; // s1
  sead::Vector3f *v46; // x0
  sead::Vector3f *v47; // x20
  float v48; // s0
  float v49; // s1
  float v50; // s2
  float v51; // s3
  float v52; // s0
  float v53; // s15
  float v54; // s1
  float v55; // s2
  float v56; // s2
  float v57; // s0
  const sead::Vector3f *v58; // x0
  float v59; // s0
  float v60; // s0
  float v61; // s1
  float v62; // s2
  float v63; // s3
  float v64; // s1
  float v65; // s5
  float v66; // s4
  float v67; // s2
  float v68; // s1
  float v69; // s2
  float v70; // s0
  float v71; // s3
  float v72; // s10
  float v73; // s0
  float v74; // s1
  al::LiveActor *v75; // x20
  float v76; // s0
  const sead::Vector3f *v77; // x0
  float v78; // s0
  float v79; // s8
  float v80; // s8
  float v81; // s0
  al::LiveActor *v82; // x20
  float v83; // s9
  float v84; // s0
  sead::Vector3f v87; // [xsp+10h] [xbp-100h] BYREF
  sead::Vector3f v88; // [xsp+20h] [xbp-F0h] BYREF
  sead::Vector3f v89; // [xsp+30h] [xbp-E0h] BYREF
  sead::Vector3f v90; // [xsp+40h] [xbp-D0h] BYREF
  sead::Vector3f v91; // [xsp+50h] [xbp-C0h] BYREF
  sead::Vector3f vec1; // [xsp+60h] [xbp-B0h] BYREF
  sead::Vector3f vec2; // [xsp+70h] [xbp-A0h] BYREF
  sead::Vector3f vec3; // [xsp+80h] [xbp-90h] BYREF
  sead::Vector3f vec4; // [xsp+90h] [xbp-80h] BYREF
  float v96; // [xsp+E8h] [xbp-28h]
  float v97; // [xsp+ECh] [xbp-24h]

  v96 = a7;
  v97 = a6;
  p__2c = &this->_2c;
  y = this->_2c.y;
  z = this->_2c.z;
  vec4.x = this->_2c.x;
  vec4.y = y;
  vec4.z = z;
  Gravity = &al::getGravity(this->mPlayer);
  v23 = -Gravity->y;
  v24 = -Gravity->x;
  v25 = Gravity->z;
  vec3.x = v24;
  vec3.y = v23;
  v26 = -v25;
  vec3.z = -v25;
  vec2 = al::getVelocity(this->mPlayer);
  v27 = rs::isOnGroundSlopeSlideEnd(this->mPlayer, this->mCollision, this->mConst);
  if ( !rs::isOnGround(this->mPlayer, this->mCollision) )
  {
    mPlayer = this->mPlayer;
    this->_2c.x = v24;
    this->_2c.y = v23;
    this->_2c.z = v26;
    Velocity = &al::getVelocity(mPlayer);
    v40 = (float)((float)(vec4.x * Velocity->x) + (float)(vec4.y * Velocity->y)) + (float)(vec4.z * Velocity->z);
    VelocityPtr = al::getVelocityPtr(this->mPlayer);
    v42 = &al::getVelocity(this->mPlayer);
    al::alongVectorNormalH(VelocityPtr, *v42, vec4, *p__2c);
    v43 = al::getVelocityPtr(this->mPlayer);
    v44 = v43->y;
    v45 = v43->z;
    v43->x = (float)(v40 * this->_2c.x) + v43->x;
    v43->y = (float)(v40 * this->_2c.y) + v44;
    v43->z = (float)(v40 * this->_2c.z) + v45;
    v46 = al::getVelocityPtr(this->mPlayer);
    vec1.x = vec3.x * (float)-a12;
    vec1.y = vec3.y * (float)-a12;
    vec1.z = vec3.z * (float)-a12;
    v47 = v46;
    v48 = this->mConst->getFallSpeedMax();
    al::addVectorLimit(v47, vec1, v48);
    goto LABEL_29;
  }

  CollidedGroundNormal = &rs::getCollidedGroundNormal(this->mCollision);
  x = CollidedGroundNormal->x;
  this->_2c.x = CollidedGroundNormal->x;
  v30 = CollidedGroundNormal->y;
  this->_2c.y = v30;
  v31 = CollidedGroundNormal->z;
  this->_2c.z = v31;
  vec3.x = x;
  vec3.y = v30;
  vec3.z = v31;
  v32 = &al::getGravity(this->mPlayer);
  al::calcDirSlide(&_20, *v32, vec3);
  al::alongVectorNormalH(&vec2, vec2, vec4, vec3);
  vec1.x = 0.0;
  vec1.y = 0.0;
  vec1.z = 0.0;
  v91.x = 0.0;
  v91.y = 0.0;
  v91.z = 0.0;
  if ( (v27 & 1) == 0 )
  {
    al::separateVectorParallelVertical(&vec1, &v91, _20, vec2);
    v90.x = 0.0;
    v90.y = 0.0;
    v90.z = 0.0;
    mInput->calcMoveInput(&v90, vec3);
    v49 = this->_20.x;
    v50 = this->_20.y;
    v51 = this->_20.z;
    v52 = (float)((float)(vec1.x * v49) + (float)(vec1.y * v50)) + (float)(vec1.z * v51);
    if ( v52 < 0.0 )
    {
      vec1.x = vec1.x * a2;
      vec1.y = vec1.y * a2;
      vec1.z = vec1.z * a2;
    }

    v53 = 0.0;
    v54 = (float)((float)(v49 * v90.x) + (float)(v50 * v90.y)) + (float)(v51 * v90.z);
    v55 = 0.0;
    if ( v54 <= -0.0 )
    {
      if ( v54 >= -1.0 )
        v55 = -v54;
      else
        v55 = 1.0;
    }

    v56 = 1.0 - (float)(v55 * a3);
    if ( v56 >= 0.0 )
    {
      v53 = v56;
      if ( v56 > 1.0 )
        v53 = 1.0;
    }

    v57 = v52 + 1.0;
    if ( v57 >= v53 )
    {
      v53 = v57;
      if ( v57 > 1.0 )
        v53 = 1.0;
    }

    v58 = &al::getGravity(this->mPlayer);
    v59 = 90.0 - al::calcAngleDegree(this->_20, *v58);
    if ( v59 >= 0.0 )
    {
      if ( v59 > 90.0 )
        v59 = 90.0;
    }
    else
    {
      v59 = 0.0;
    }

    if ( a5 <= 0.0 )
      v60 = 1.0;
    else
      v60 = fminf(v59 / a5, 1.0);

    v61 = this->_20.x * a1;
    v62 = this->_20.y * a1;
    v63 = this->_20.z * a1;
    v33 = v97;
    v89.z = v53 * (float)(v60 * v63);
    v89.x = v53 * (float)(v60 * v61);
    v89.y = v53 * (float)(v60 * v62);
    al::addVectorLimit(&vec1, v89, v97);
    v91.x = v91.x * a8;
    v91.y = v91.y * a8;
    v89.x = 0.0;
    v89.y = 0.0;
    v89.z = 0.0;
    v91.z = v91.z * a8;
    if ( !al::tryNormalizeOrZero(&v89, v90) )
      goto LABEL_28;

    v65 = this->_20.y;
    v64 = this->_20.z;
    v66 = this->_20.x;
    v67 = vec3.y * v64;
    v68 = (float)(vec3.z * v66) - (float)(v64 * vec3.x);
    v69 = v67 - (float)(vec3.z * v65);
    v70 = (float)(v65 * vec3.x) - (float)(vec3.y * v66);
    v71 = (float)((float)(v69 * v89.x) + (float)(v68 * v89.y)) + (float)(v70 * v89.z);
    v72 = v70 * v71;
    v73 = sqrtf((float)((float)(v90.x * v90.x) + (float)(v90.y * v90.y)) + (float)(v90.z * v90.z));
    v88.x = (float)((float)(v69 * v71) * v73) * v96;
    v88.y = (float)((float)(v68 * v71) * v73) * v96;
    v74 = (float)(v72 * v73) * v96;
    v37 = a9;
    v88.z = v74;
    p_vec1 = &v91;
    v36 = &v88;
    goto LABEL_27;
  }

  al::separateVectorParallelVertical(&v91, &vec1, vec3, vec2);
  vec1.x = vec1.x * a2;
  vec1.y = vec1.y * a2;
  v91.x = v91.x * a8;
  vec1.z = vec1.z * a2;
  v91.y = v91.y * a8;
  v33 = v97;
  v91.z = v91.z * a8;
  if ( a13 )
  {
    v90.x = 0.0;
    v90.y = 0.0;
    v90.z = 0.0;
    mInput->calcMoveInput(&v90, vec3);
    v89.x = 0.0;
    v89.y = 0.0;
    v89.z = 0.0;
    al::tryNormalizeOrZero(&v89, vec1);
    v88.x = (float)(vec3.y * v89.z) - (float)(vec3.z * v89.y);
    v88.y = (float)(vec3.z * v89.x) - (float)(v89.z * vec3.x);
    v88.z = (float)(v89.y * vec3.x) - (float)(vec3.y * v89.x);
    al::tryNormalizeOrZero(&v88);
    v34 = (float)((float)(v88.x * v90.x) + (float)(v88.y * v90.y)) + (float)(v88.z * v90.z);
    v87.x = (float)(v88.x * v34) * v96;
    v87.y = (float)(v34 * v88.y) * v96;
    v87.z = (float)(v34 * v88.z) * v96;
    p_vec1 = &vec1;
    v36 = &v87;
    v37 = v33;

LABEL_27:
    al::addVectorLimit(p_vec1, *v36, v37);
  }

LABEL_28:
  v90.x = vec1.x + v91.x;
  v90.y = vec1.y + v91.y;
  v90.z = vec1.z + v91.z;
  al::limitLength(&v90, v90, v33);
  v75 = this->mPlayer;
  v76 = this->mConst->getGravityMove();
  v89.x = v90.x - (float)(v76 * vec3.x);
  v89.y = v90.y - (float)(v76 * vec3.y);
  v89.z = v90.z - (float)(v76 * vec3.z);
  al::setVelocity(v75, v89);

LABEL_29:
  vec1.x = 0.0;
  vec1.y = 0.0;
  vec1.z = 0.0;
  v77 = &al::getVelocity(this->mPlayer);
  al::verticalizeVec(&vec1, vec3, *v77);
  v91.x = 0.0;
  v91.y = 0.0;
  v91.z = 0.0;
  if ( al::tryNormalizeOrZero(&v91, vec1) )
  {
    v90.x = 0.0;
    v90.y = 0.0;
    v90.z = 0.0;
    al::calcFrontDir(&v90, this->mPlayer);
    al::verticalizeVec(&v90, vec3, v90);
    al::tryNormalizeOrZero(&v90);
    if ( a11 <= 0.0 )
    {
      v80 = 0.0;
    }
    else
    {
      v78 = al::calcAngleDegree(v90, v91);
      v79 = (float)(v78 + v78) / a11;
      if ( v79 >= 0.0 )
      {
        if ( v79 > 1.0 )
          v79 = 1.0;
      }
      else
      {
        v79 = 0.0;
      }

      v80 = -(float)(v79
                   * al::sign(
                       (float)((float)((float)((float)(v90.y * v91.z) - (float)(v90.z * v91.y)) * vec3.x)
                             + (float)(vec3.y * (float)((float)(v90.z * v91.x) - (float)(v91.z * v90.x))))
                     + (float)((float)((float)(v91.y * v90.x) - (float)(v90.y * v91.x)) * vec3.z)));
    }

    v81 = sead::Mathf::cos(a11 * 0.017453);
    al::turnVecToVecCosOnPlane(&v90, v91, vec3, v81);
    v82 = this->mPlayer;
    rs::slerpUpFront(v82, vec3, v90, this->mConst->getSlerpQuatRate(), this->mConst->getHillPoseDegreeMax());
  }
  else
  {
    v80 = 0.0;
  }

  this->_38 = vec1;
  return v80;
}
