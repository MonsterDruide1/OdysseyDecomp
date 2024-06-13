#include "Stuff.h"

#include "Library/Collision/CollisionParts.h"
#include "Library/Collision/CollisionUtil.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Math/MathAngleUtil.h"
#include "Library/Math/MathLengthUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Math/VectorUtil.h"
#include "Library/stuff.h"
#include "Player/PlayerCollider.h"
#include "Player/PlayerFunction.h"
#include "Player/PlayerInput.h"
#include "Player/PlayerTrigger.h"
#include "PlayerUtil.h"
#include "math/seadMatrix.h"
#include "math/seadVectorFwd.h"

namespace rs {

bool isCollisionCodePress(al::HitInfo* info) {
    return al::isFloorCode(info, "Press");
}

bool calcExistCollisionBorder(const al::IUseCollision* a1, const sead::Vector3f& a2, const sead::Vector3f& a3) {
  sead::Vector3f v13 = {0.0f, 0.0f, 0.0f};
  sead::Vector3f v12 = {0.0f, 0.0f, 0.0f};
  sead::Vector3f v11 = a2 - a3;
  sead::Vector3f v10 = a3 + a3;
  return alCollisionUtil::getHitPosAndNormalOnArrow(a1, &v13, &v12, v11, v10, nullptr, nullptr) && al::isReverseDirection(v12, a3, 0.01f);
}

bool sub_7100569734(const al::LiveActor *actor, const IUsePlayerCollision *a2, const PlayerConst *a3, bool a4)
{
  sead::Vector3f vec = {0.0f, 0.0f, 0.0f};
  al::verticalizeVec(&vec, a4 ? al::getGravity(actor) : a2->getPlayerCollider()->mCollidedGroundNormal, al::getVelocity(actor));
  f32 rate = al::calcRate01(
          vec.length(),
          a3->getHillAngleSpeedMin(),
          a3->getHillAngleSpeedMax());
  f32 lerp = al::lerpValue(a3->getStandAngleMin(), a3->getStandAngleMax(), al::easeIn(rate));
  return al::isFloorPolygonCos(a2->getPlayerCollider()->mCollidedGroundNormal, al::getGravity(actor), cosf(sead::Mathf::deg2rad(lerp)));
}

bool isOnGround(al::LiveActor const*actor,IUsePlayerCollision const*collision) {
  if(collision->getPlayerCollider()->val1 < 0.0f) return false;

  sead::Vector3f velocity = al::getVelocity(actor);
  al::tryNormalizeOrZero(&velocity);
  f32 dot = collision->getPlayerCollider()->mCollidedGroundNormal.dot(velocity);
  return dot <= 0.0f || al::isNearZero(dot, 0.001f);
}

bool isLandGroundRunAngle(al::LiveActor const* actor,IUsePlayerCollision const* collision,PlayerConst const* pConst) {
  return isOnGround(actor, collision) && sub_7100569734(actor, collision, pConst, true);
}
bool isCollidedGround(const IUsePlayerCollision* collision) {
    return collision->getPlayerCollider()->val1 >= 0.0f;
}
bool isJustLand(const IUsePlayerCollision* collision) {
  return isCollidedGround(collision) && collision->getPlayerCollider()->mTimeInAir == 1;
}
bool isOnGroundRunAngle(al::LiveActor const*actor,IUsePlayerCollision const*collision,PlayerConst const*pConst) {
  return isOnGround(actor, collision) && sub_7100569734(actor, collision, pConst, isJustLand(collision));
}
bool isCollidedGroundRunAngle(al::LiveActor const*actor,IUsePlayerCollision const*collision,PlayerConst const*pConst) {
  return isCollidedGround(collision) && sub_7100569734(actor, collision, pConst, isJustLand(collision));
}

void cutVerticalVelocityGroundNormal(al::LiveActor* actor, const IUsePlayerCollision* collision) {
  if(!rs::isCollidedGround(collision)) return;

  sead::Vector3f* velocity = al::getVelocityPtr(actor);
  al::parallelizeVec(velocity, rs::getCollidedGroundNormal(collision), *velocity);
}

void calcGroundNormalOrGravityDir(sead::Vector3f* result, const al::LiveActor* actor, const IUsePlayerCollision* collision) {
    if(isCollidedGround(collision)) {
        *result = rs::getCollidedGroundNormal(collision);
    } else {
        *result = al::getGravity(actor);
    }
}

void calcGroundNormalOrUpDir(sead::Vector3f* result, const al::LiveActor* actor, const IUsePlayerCollision* collision) {
    if(isCollidedGround(collision)) {
        *result = rs::getCollidedGroundNormal(collision);
    } else {
        al::calcUpDir(result, actor);
    }
}

const sead::Vector3f& getCollidedGroundNormal(const IUsePlayerCollision* collision) {
    return collision->getPlayerCollider()->mCollidedGroundNormal;
}

const sead::Vector3f& getCollidedGroundPos(const IUsePlayerCollision* collision) {
    return collision->getPlayerCollider()->mCollidedGroundPos;
}

bool isCollidedWall(const IUsePlayerCollision* collision) {
    return collision->getPlayerCollider()->val2 >= 0.0f;
}

void resetCollision(IUsePlayerCollision*collision) {
    return collision->getPlayerCollider()->onInvalidate();
}

void calcJumpInertia(sead::Vector3f* out, al::LiveActor* actor, const IUsePlayerCollision* collision, const sead::Vector3f& vec, f32 val) {
  float v13; // s2
  float v14; // s0
  float v15; // s1
  float v16; // s2
  sead::Vector3f v17; // [xsp+0h] [xbp-60h] BYREF
  sead::Vector3f a1; // [xsp+10h] [xbp-50h] BYREF
  sead::Vector3f a3; // [xsp+20h] [xbp-40h] BYREF

  if ( rs::isCollidedGround(collision) )
  {
    a3.x = 0.0;
    a3.y = 0.0;
    a3.z = 0.0;
    rs::calcMovePowerGround(&a3, collision, al::getTrans(actor));
    al::verticalizeVec(&a3, al::getGravity(actor), a3);
    a1 = a3;
    al::tryNormalizeOrZero(&a1);
    v17.x = 0.0;
    v17.y = 0.0;
    v17.z = 0.0;
    al::verticalizeVec(&v17, al::getGravity(actor), vec);
    if ( al::tryNormalizeOrZero(&v17) )
    {
      v13 = fmaxf(a1.dot(v17), 0.0) * val;
      v14 = a3.x * v13;
      v15 = a3.y * v13;
      v16 = a3.z * v13;
    }
    else
    {
      v15 = a3.y * val;
      v14 = a3.x * val;
      v16 = a3.z * val;
    }

    out->x = v14;
    out->y = v15;
    out->z = v16;
  }
  else
  {
    out->x = 0.0;
    out->y = 0.0;
    out->z = 0.0;
  }
}
void calcJumpInertiaWall(sead::Vector3f* a1, al::LiveActor* a2, const IUsePlayerCollision* a3, f32 a4) {
  float v10; // s1
  float v11; // s0
  float v12; // s2
  sead::Vector3f v13; // [xsp+0h] [xbp-30h] BYREF

  if ( rs::isCollidedWall(a3) )
  {
    v13.x = 0.0;
    v13.y = 0.0;
    v13.z = 0.0;
    rs::calcMovePowerWall(&v13, a3, al::getTrans(a2));
    al::verticalizeVec(&v13, al::getGravity(a2), v13);
    v10 = v13.y * a4;
    v11 = v13.x * a4;
    v12 = v13.z * a4;
  }
  else
  {
    v11 = 0.0;
    v10 = 0.0;
    v12 = 0.0;
  }

  a1->x = v11;
  a1->y = v10;
  a1->z = v12;
}

bool findWallEnablePush(const al::LiveActor *actor, const IUsePlayerCollision *collider, float a3, float a4) {
  float v8; // s10
  float v9; // s9
  bool result; // x0
  const sead::Vector3f *CollidedWallNormal; // x0
  float y; // s12
  float x; // s13
  float z; // s14
  float v15; // s15
  const sead::Vector3f *Trans; // x0
  float v17; // s0
  float v18; // s1
  float v19; // s10
  float v20; // s8
  float v21; // s11
  float v22; // s12
  float v23; // s13
  float v24; // s14
  al::IUseCollision *v25; // x19
  int v26; // w8
  sead::Vector3f v27; // [xsp+0h] [xbp-90h] BYREF
  sead::Vector3f v28; // [xsp+10h] [xbp-80h] BYREF
  sead::Vector3f a1; // [xsp+20h] [xbp-70h] BYREF
  sead::Vector3f v30; // [xsp+30h] [xbp-60h] BYREF

  if ( !rs::isCollidedWall(collider) )
    return 0LL;

  v8 = (float)(a3 + a3) / 3.0;
  v9 = a4 * 0.5;
  v30.x = 0.0;
  v30.y = 0.0;
  v30.z = 0.0;
  if ( collider && rs::isCollidedGround(collider) )
    v30 = rs::getCollidedGroundNormal(collider);
  else
    al::calcUpDir(&v30, actor);

  a1.x = 0.0;
  a1.y = 0.0;
  a1.z = 0.0;
  al::calcSideDir(&a1, actor);
  al::getGravity(actor);
  CollidedWallNormal = &rs::getCollidedWallNormal(collider);
  x = CollidedWallNormal->x;
  y = CollidedWallNormal->y;
  z = CollidedWallNormal->z;
  v15 = a4 + 10.0;
  Trans = &al::getTrans(actor);
  v17 = v8 * v30.x;
  v18 = v8 * v30.y;
  v19 = (float)(v8 * v30.z) + Trans->z;
  v20 = v17 + Trans->x;
  v21 = v18 + Trans->y;
  v22 = -(float)(v15 * y);
  v23 = -(float)(v15 * x);
  v27.x = v23;
  v27.y = v22;
  v24 = -(float)(v15 * z);

  v28.x = v20 + (float)(v9 * a1.x);
  v28.y = v21 + (float)(v9 * a1.y);
  v28.z = v19 + (float)(v9 * a1.z);
  v27.z = v24;
  v26 = alCollisionUtil::checkStrikeArrow(actor, v28, v27, 0LL, 0LL);
  result = 0LL;
  if ( v26 )
  {
    v27.x = v23;
    v27.y = v22;
    v27.z = v24;
    v28.x = v20 - (float)(v9 * a1.x);
    v28.y = v21 - (float)(v9 * a1.y);
    v28.z = v19 - (float)(v9 * a1.z);
    return (unsigned int)alCollisionUtil::checkStrikeArrow(v25, v28, v27, 0LL, 0LL) != 0;
  }

  return result;
}

bool isCollidedWallFace(const IUsePlayerCollision* collision) {
  if(collision->getPlayerCollider()->val2 < 0.0f) return false;
  return collision->getPlayerCollider()->info2->isCollisionAtFace();
}

void calcHackerMoveVec(sead::Vector3<float>* vec, IUsePlayerHack const* hack, sead::Vector3<float> const& vec2) {
  hack->getPlayerHackKeeper()->mInput->calcMoveInput(vec, vec2);
}

void calcMovePowerGround(sead::Vector3f* result, const IUsePlayerCollision* collision, const sead::Vector3f& vec) {
    *result = {0.0f, 0.0f, 0.0f};
}
void calcMovePowerWall(sead::Vector3f* result, const IUsePlayerCollision* collision, const sead::Vector3f& vec) {
    *result = {0.0f, 0.0f, 0.0f};
}

void slerpUpFront(al::LiveActor* actor, const sead::Vector3f& up, const sead::Vector3f& front, float val1, float val2) {
  sead::Quatf v13; // [xsp+0h] [xbp-B0h] BYREF
  sead::Quatf v14; // [xsp+10h] [xbp-A0h] BYREF
  sead::Vector3f v15; // [xsp+20h] [xbp-90h] BYREF
  sead::Vector3f a1; // [xsp+30h] [xbp-80h] BYREF
  sead::Vector3f v17; // [xsp+40h] [xbp-70h] BYREF
  sead::Vector3f a3; // [xsp+50h] [xbp-60h] BYREF
  sead::Vector3f v19; // [xsp+60h] [xbp-50h] BYREF
  sead::Quatf v20; // [xsp+60h] [xbp-50h] FORCED BYREF
  sead::Vector3f a4; // [xsp+70h] [xbp-40h] BYREF

  a4 = up;
  v19 = -al::getGravity(actor);
  al::turnVecToVecDegree(&a4, v19, up, val2);
  v20.x = 0.0;
  v20.y = 0.0;
  v20.z = 0.0;
  v20.w = 1.0;
  al::makeQuatFrontUp(&v20, front, up);
  a3.x = 0.0;
  a3.y = 0.0;
  a3.z = 0.0;
  v17.x = 0.0;
  v17.y = 0.0;
  v17.z = 0.0;
  al::calcQuatUp(&a3, v20);
  al::calcQuatFront(&v17, v20);
  a1 = front;
  al::alongVectorNormalH(&a1, front, a3, a4);
  if ( al::tryNormalizeOrZero(&a1) )
  {
    al::makeQuatUpFront(&v14, a4, a1);
    al::calcQuat(&v13, actor);
    al::slerpQuat(&v13, v13, v14, val1);
    al::calcQuatUp(&v15, v13);
    al::makeQuatUpFront(&v13, v15, a1);
  }
  else
  {
    v15.x = 0.0;
    v15.y = 0.0;
    v15.z = 0.0;
    al::calcFrontDir(&v15, actor);
    if ( al::isParallelDirection(a4, v15, 0.01) )
      al::calcUpDir(&v15, actor);

    al::makeQuatUpFront(&v14, a4, v15);
    al::calcQuat(&v13, actor);
    al::slerpQuat(&v13, v13, v14, val1);
  }

  al::updatePoseQuat(actor, v13);
}
void slerpSkyPoseAir(al::LiveActor *, const sead::Vector3f &, const sead::Vector3f &, float) {CRASH}
void slerpGravity(al::LiveActor *, float) {CRASH}
void slerpSkyPoseAirSnapSide(al::LiveActor *, const sead::Vector3f &, const sead::Vector3f &, float, float) {CRASH}
void scaleVelocityInertiaWallHit(al::LiveActor *actor, const IUsePlayerCollision *collision, float s0_0, float a4, float a5) {
  const sead::Vector3f *Gravity; // x0
  const sead::Vector3f *Trans; // x0
  const sead::Vector3f *v12; // x0
  float x; // s11
  float y; // s13
  float z; // s12
  float v16; // s0
  float v17; // s1
  float v18; // s0
  float v20; // s8
  float v21; // s0
  sead::Vector3f v23; // [xsp+0h] [xbp-90h] BYREF
  sead::Vector3f v24; // [xsp+10h] [xbp-80h] BYREF
  sead::Vector3f v25; // [xsp+20h] [xbp-70h] BYREF
  sead::Vector3f a3; // [xsp+30h] [xbp-60h] BYREF
  sead::Vector3f a1; // [xsp+40h] [xbp-50h] BYREF

  if ( rs::isCollidedWall(collision) && (rs::isCollidedNoScaleVelocityWall(collision) & 1) == 0 )
  {
    a3 = rs::getCollidedWallNormal(collision);
    Gravity = &al::getGravity(actor);
    al::verticalizeVec(&a3, *Gravity, a3);
    if ( al::tryNormalizeOrZero(&a3) )
    {
      if ( rs::isCollidedWall(collision) )
      {
        a1.x = 0.0;
        a1.y = 0.0;
        a1.z = 0.0;
        Trans = &al::getTrans(actor);
        rs::calcMovePowerWall(&a1, collision, *Trans);
        v12 = &al::getGravity(actor);
        al::verticalizeVec(&a1, *v12, a1);
        x = a1.x;
        y = a1.y;
        z = a1.z;
      }
      else
      {
        x = 0.0;
        y = 0.0;
        z = 0.0;
      }

      a1.x = 0.0;
      a1.y = 0.0;
      a1.z = 0.0;
      v25.x = 0.0;
      v25.y = 0.0;
      v25.z = 0.0;
      al::separateVelocityHV(&a1, &v25, actor);
      v24.x = 0.0;
      v24.y = 0.0;
      v24.z = 0.0;
      al::tryNormalizeOrZero(&v24, a1);
      if ( v24.dot(a3) < -0.5 )
      {
        v16 = a1.length();
        v17 = v16 * s0_0;
        if ( v16 < a4 )
          a4 = v16;

        if ( v17 >= a4 )
        {
          a4 = v16 * s0_0;
          if ( v17 > a5 )
            a4 = a5;
        }

        v18 = a1.length();
        if ( v18 > 0.0 )
        {
          a1 *= a4 / v18;
        }

        v20 = (float)((float)(x * v24.x) + (float)(y * v24.y)) + (float)(z * v24.z);
        if ( v24.dot(a3) > (float)-v20 )
        {
          v21 = a1.length();
          if ( v21 > 0.0 )
          {
            a1 *= v20 / v21;
          }
        }
      }

      v23.x = a1.x + v25.x;
      v23.y = a1.y + v25.y;
      v23.z = a1.z + v25.z;
      al::setVelocity(actor, v23);
    }
  }
}

bool isCollidedNoScaleVelocityWall(const IUsePlayerCollision* collision) {
  if(collision->getPlayerCollider()->val2 < 0.0f) return false;
  return al::isFloorCode(collision->getPlayerCollider()->info2, "GrabCeil");
}
const sead::Vector3f& getCollidedWallNormal(const IUsePlayerCollision* collision) {
  return collision->getPlayerCollider()->info2->mTriangle.getFaceNormal();
}

void noticePlayerJumpStart(PlayerTrigger*trig, al::LiveActor const*actor) {
  rs::addPlayerJumpCount(actor);
  trig->set(PlayerTrigger::EActionTrigger::EActionTrigger_val22);
}

bool calcSlideDir(sead::Vector3<float>* dir, sead::Vector3<float> const& a, sead::Vector3<float> const& b) {
  al::verticalizeVec(dir, b, a);
  return al::tryNormalizeOrZero(dir);
}

void landGroundPoseAndSnap(
        al::LiveActor *x0_0,
        PlayerTrigger *a2,
        float *a3,
        const IUsePlayerCollision *a4,
        const PlayerConst *a5)
{
  const sead::Vector3f *Gravity; // x0
  float z; // s2
  float v12; // s1
  float v13; // s0
  float v14; // s8
  float v15; // s0
  const sead::Vector3f *Trans; // x22
  const sead::Vector3f *CollidedGroundPos; // x0
  float v18; // s8
  float v19; // s0
  sead::Vector3f v20; // [xsp+0h] [xbp-80h] BYREF
  sead::Vector3f v21; // [xsp+10h] [xbp-70h] BYREF
  sead::Vector3f v22; // [xsp+20h] [xbp-60h] BYREF
  sead::Vector3f v23; // [xsp+30h] [xbp-50h] BYREF
  sead::Vector3f a1; // [xsp+40h] [xbp-40h] BYREF

  a1.x = 0.0;
  a1.y = 0.0;
  a1.z = 0.0;
  al::calcUpDir(&a1, x0_0);
  v23.x = 0.0;
  v23.y = 0.0;
  v23.z = 0.0;
  if ( rs::isCollidedGround(a4) )
  {
    v23 = rs::getCollidedGroundNormal(a4);
  }
  else
  {
    Gravity = &al::getGravity(x0_0);
    z = Gravity->z;
    v12 = -Gravity->y;
    v23.x = -Gravity->x;
    v23.y = v12;
    v23.z = -z;
  }

  v13 = a5->getWaitPoseDegreeMax();
  rs::slerpUp(x0_0, v23, 1.0, v13);
  v22.x = 0.0;
  v22.y = 0.0;
  v22.z = 0.0;
  al::calcUpDir(&v22, x0_0);
  v14 = (float)((float)(a1.x * v22.x) + (float)(a1.y * v22.y)) + (float)(a1.z * v22.z);
  v15 = a5->getCollisionResetLimit();
  if ( v14 <= sead::Mathf::cos(v15 * 0.017453) )
    a2->set(PlayerTrigger::EActionTrigger_val3);

  *a3 = 0.0;
  if ( rs::isCollidedGround(a4) )
  {
    Trans = &al::getTrans(x0_0);
    CollidedGroundPos = &rs::getCollidedGroundPos(a4);
    v20.x = -v23.x;
    v20.y = -v23.y;
    v21.x = 0.0;
    v21.y = 0.0;
    v21.z = 0.0;
    v20.z = -v23.z;
    if ( (al::checkHitLinePlane(&v21, *Trans, v20, *CollidedGroundPos, v23) & 1) != 0 )
    {
      v18 = (float)((float)((float)(Trans->x - v21.x) * v23.x) + (float)((float)(Trans->y - v21.y) * v23.y))
          + (float)((float)(Trans->z - v21.z) * v23.z);
      v19 = a5->getFallSpeedMax();
      if ( v18 >= 0.0 )
      {
        if ( v18 > v19 )
          v18 = v19;
      }
      else
      {
        v18 = 0.0;
      }

      *a3 = v18;
    }
  }
}

void waitGround(al::LiveActor *x0_0, const IUsePlayerCollision *x1_0, float a3, float a4, float a5, float a6)
{  
  const al::CollisionParts *CollidedGroundCollisionParts; // x0
  float x; // s12
  float y; // s13
  float z; // s14
  const sead::Vector3f *Gravity; // x0
  const sead::Vector3f *CollidedGroundNormal; // x21
  const sead::Vector3f *CollidedCeilingNormal; // x0
  sead::Vector3f *VelocityPtr; // x0
  const sead::Vector3f *v20; // x0
  sead::Quatf v23; // [xsp+0h] [xbp-A0h] BYREF
  sead::Quatf a2; // [xsp+10h] [xbp-90h] BYREF
  sead::Vector3f a1; // [xsp+20h] [xbp-80h] BYREF
  sead::Vector3f v26; // [xsp+30h] [xbp-70h] BYREF
  sead::Vector3f v27; // [xsp+40h] [xbp-60h] BYREF

  if ( rs::isCollidedGround(x1_0) )
  {
    CollidedGroundCollisionParts = (const al::CollisionParts *)rs::getCollidedGroundCollisionParts(x1_0);
    al::followRotateFrontAxisUp(x0_0, CollidedGroundCollisionParts);
  }

  v27.x = 0.0;
  v27.y = 0.0;
  v27.z = 0.0;
  if ( rs::isCollidedGround(x1_0) )
  {
    v27 = rs::getCollidedGroundNormal(x1_0);
    y = v27.y;
    x = v27.x;
    z = v27.z;
  }
  else
  {
    Gravity = &al::getGravity(x0_0);
    y = -Gravity->y;
    x = -Gravity->x;
    z = -Gravity->z;
    v27.x = x;
    v27.y = y;
    v27.z = z;
  }

  rs::reboundVelocityFromCollision(x0_0, x1_0, 0.0, 0.0, 0.0);
  if ( rs::isCollidedGround(x1_0) && rs::isCollidedCeiling(x1_0) )
  {
    CollidedGroundNormal = &rs::getCollidedGroundNormal(x1_0);
    CollidedCeilingNormal = &rs::getCollidedCeilingNormal(x1_0);
    if ( al::isReverseDirection(*CollidedGroundNormal, *CollidedCeilingNormal, 0.01) )
    {
      VelocityPtr = al::getVelocityPtr(x0_0);
      al::verticalizeVec(VelocityPtr, *CollidedGroundNormal, *VelocityPtr);
    }
  }

  al::tryAddVelocityLimit(x0_0, (-v27 * a3), a4);
  v26 = v27;
  v20 = &al::getGravity(x0_0);
  al::turnVecToVecDegree(&v26, -*v20, v27, a6);
  a1 = {0.0f, 0.0f, 0.0f};
  al::calcFrontDir(&a1, x0_0);
  if ( al::isParallelDirection(v26, a1, 0.01) )
    al::calcUpDir(&a1, x0_0);

  a2.x = 0.0;
  a2.y = 0.0;
  a2.z = 0.0;
  a2.w = 1.0;
  al::makeQuatUpFront(&a2, v26, a1);
  v23.x = 0.0;
  v23.y = 0.0;
  v23.z = 0.0;
  v23.w = 1.0;
  al::calcQuat(&v23, x0_0);
  al::slerpQuat(&v23, v23, a2, a5);
  al::updatePoseQuat(x0_0, v23);
}

bool sub_7100568E3C(al::LiveActor *a1, const sead::Vector3f& a2, float a3, float a4, float a5)
{
  float v11; // s11
  sead::Vector3f *v12; // x0
  float v13; // s1
  float v14; // s2
  sead::Vector3f *v15; // x0
  float v16; // s1
  sead::Vector3f *v17; // x0
  float v18; // s5
  float v19; // s1
  float z; // s3
  bool result; // w0
  sead::Vector3f *VelocityPtr; // x8
  float v23; // s2
  float v24; // s1

  v11 = a2.dot(al::getVelocity(a1));
  if ( v11 >= (float)-a4 )
  {
    if ( v11 >= 0.0 )
    {
      return 0;
    }
    else
    {
      VelocityPtr = al::getVelocityPtr(a1);
      result = 0;
      v23 = v11 * a2.z;
      v24 = VelocityPtr->y - (float)(v11 * a2.y);
      VelocityPtr->x = VelocityPtr->x - (float)(v11 * a2.x);
      VelocityPtr->y = v24;
      VelocityPtr->z = VelocityPtr->z - v23;
    }
  }
  else
  {
    v12 = al::getVelocityPtr(a1);
    v13 = v12->y - (float)(v11 * a2.y);
    v14 = v11 * a2.z;
    v12->x = v12->x - (float)(v11 * a2.x);
    v12->y = v13;
    v12->z = v12->z - v14;
    v15 = al::getVelocityPtr(a1);
    v16 = v15->y * a5;
    v15->x = v15->x * a5;
    v15->y = v16;
    v15->z = v15->z * a5;
    v17 = al::getVelocityPtr(a1);
    v18 = (float)(v11 * a2.z) * a3;
    v19 = v17->y - (float)((float)(v11 * a2.y) * a3);
    z = v17->z;
    v17->x = v17->x - (float)((float)(v11 * a2.x) * a3);
    v17->y = v19;
    v17->z = z - v18;
    return 1;
  }

  return result;
}

bool reboundVelocityFromCollision(al::LiveActor * a1,IUsePlayerCollision const*a2,float a3,float a4,float a5) {
  int v10; // w21
  sead::Vector3f v17; // [xsp+0h] [xbp-40h] BYREF

  if ( *(float *)(a2->getPlayerCollider() + 112) >= 0.0
    || *(float *)(a2->getPlayerCollider() + 128) >= 0.0
    || *(float *)(a2->getPlayerCollider() + 144) >= 0.0 )
  {
    v17 = {0.0f, 0.0f, 0.0f};
    rs::calcCollidedNormalSum(&v17, a2);
    if ( al::isNearZero(v17, 0.001) )
    {
      if ( *(float *)(a2->getPlayerCollider() + 112) >= 0.0 )
      {
        v10 = sub_7100568E3C(a1, a2->getPlayerCollider()->mCollidedGroundNormal, a3, a4, a5);
      }
      else
      {
        v10 = 0;
      }

      if ( *(float *)(a2->getPlayerCollider() + 128) >= 0.0 )
      {
        v10 |= sub_7100568E3C(a1, a2->getPlayerCollider()->info2->mTriangle.getFaceNormal(), a3, a4, a5);
      }

      if ( *(float *)(a2->getPlayerCollider() + 144) >= 0.0 )
      {
        v10 |= sub_7100568E3C(a1, a2->getPlayerCollider()->info3->mTriangle.getFaceNormal(), a3, a4, a5);
      }
    }
    else
    {
      al::normalize(&v17);
      v10 = sub_7100568E3C(a1, v17, a3, a4, a5);
    }
  }
  else
  {
    v10 = 0;
  }

  return v10 & 1;
}

void calcCollidedNormalSum(sead::Vector3<float> *a1,IUsePlayerCollision const*a2) {
  float *v4; // x0
  float y; // s2
  const sead::Vector3f *FaceNormal; // x0
  float v8; // s2
  float x; // s0
  float v10; // s1
  float v11; // s2
  const sead::Vector3f *v13; // x0
  float v14; // s2
  float z; // s2

  a1->x = 0.0;
  a1->y = 0.0;
  a1->z = 0.0;
  if ( *(float *)(a2->getPlayerCollider() + 112) >= 0.0 )
  {
    v4 = (float *)a2->getPlayerCollider();
    y = a1->y;
    a1->x = a1->x + v4[105];
    a1->y = y + v4[106];
    a1->z = a1->z + v4[107];
  }

  if ( *(float *)(a2->getPlayerCollider() + 128) >= 0.0 )
  {
    FaceNormal = &a2->getPlayerCollider()->info2->mTriangle.getFaceNormal();
    v8 = a1->y;
    a1->x = a1->x + FaceNormal->x;
    a1->y = v8 + FaceNormal->y;
    a1->z = a1->z + FaceNormal->z;
  }

  if ( *(float *)(a2->getPlayerCollider() + 144) >= 0.0 )
  {
    v13 = &a2->getPlayerCollider()->info3->mTriangle.getFaceNormal();
    v14 = a1->y;
    x = a1->x + v13->x;
    a1->x = x;
    v10 = v14 + v13->y;
    z = a1->z;
    a1->y = v10;
    v11 = z + v13->z;
    a1->z = v11;
  }
  else
  {
    x = a1->x;
    v10 = a1->y;
    v11 = a1->z;
  }

  a1->x = x * 0.33333;
  a1->y = v10 * 0.33333;
  a1->z = v11 * 0.33333;
}

al::HitSensor* tryGetCollidedGroundSensor(IUsePlayerCollision const* collider) {
  if(collider->getPlayerCollider()->val1 < 0.0f)
    return nullptr;
  return collider->getPlayerCollider()->info1->mTriangle.mCollisionParts->mConnectedSensor;
}

bool calcGroundHeight(float *a1,sead::Vector3<float> *a2,al::IUseCollision const*a3,sead::Vector3<float> const&a4,sead::Vector3<float> const&a5,float a6,float a7) {
  float v8; // s8
  float v10; // s1
  float y; // s3
  float z; // s4
  float v16; // s6
  float v17; // s5
  float v18; // s7
  int v19; // w24
  char v20; // w28
  int v21; // w25
  al::ArrowHitInfo *StrikeArrowInfo; // x27
  const sead::Vector3f *FaceNormal; // x26
  float v26; // s0
  bool result; // w0
  sead::Vector3f v28; // [xsp+0h] [xbp-80h] BYREF
  sead::Vector3f v29; // [xsp+10h] [xbp-70h] BYREF

  v8 = a7;
  *a2 = a5;
  v10 = (float)(a6 + a7) + 5.0;
  y = a5.y;
  z = a5.z;
  v16 = (float)(y * a6) + a4.y;
  v17 = (float)(a5.x * a6) + a4.x;
  v18 = a4.z;
  v28.x = -(float)(v10 * a5.x);
  v28.y = -(float)(v10 * y);
  v29.x = v17;
  v29.y = v16;
  v29.z = (float)(z * a6) + v18;
  v28.z = -(float)(v10 * z);
  v19 = alCollisionUtil::checkStrikeArrow(a3, v29, v28, 0LL, 0LL);
  if ( v19 < 1 )
    return 0;

  v20 = 0;
  v21 = 0;
  do
  {
    StrikeArrowInfo = (al::ArrowHitInfo *)alCollisionUtil::getStrikeArrowInfo(a3, v21);
    FaceNormal = &StrikeArrowInfo->mTriangle.getFaceNormal();
    if ( FaceNormal->dot(a5) >= 0.5 )
    {
      v26 = (StrikeArrowInfo->mCollisionHitPos - a4).length();
      if ( v26 < v8 )
      {
        *a2 = *FaceNormal;
        v8 = v26;
        v20 = 1;
      }
    }

    ++v21;
  }
  while ( v19 != v21 );

  if ( (v20 & 1) == 0 )
    return 0;

  result = 1;
  *a1 = fmaxf(v8, 0.0);
  return result;
}

}  // namespace rs
