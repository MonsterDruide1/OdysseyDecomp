#include "Stuff.h"

#include "Library/Collision/CollisionParts.h"
#include "Library/Collision/CollisionUtil.h"
#include "Library/Collision/TriangleFilter.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Math/MathAngleUtil.h"
#include "Library/Math/MathLengthUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Math/VectorUtil.h"
#include "Library/stuff.h"
#include "Player/PlayerActionFunction.h"
#include "Player/PlayerActionVelocityControl.h"
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
bool isOnGroundLessAngle(al::LiveActor const*actor,IUsePlayerCollision const*collision,float val) {
  return isOnGround(actor, collision) && al::isFloorPolygonCos(collision->getPlayerCollider()->mCollidedGroundNormal, al::getGravity(actor), sead::Mathf::cos(sead::Mathf::deg2rad(val)));
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

const sead::Vector3f& getCollidedWallPos(const IUsePlayerCollision* collision) {
    return collision->getPlayerCollider()->info2->mCollisionHitPos;
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
al::HitSensor* tryGetCollidedWallSensor(IUsePlayerCollision const* collider) {
  if(collider->getPlayerCollider()->val2 < 0.0f)
    return nullptr;
  return collider->getPlayerCollider()->info2->mTriangle.mCollisionParts->mConnectedSensor;
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

bool convergeOnGroundCount(
        int *a1,
        const al::LiveActor *a2,
        const IUsePlayerCollision *a3,
        int a4,
        int a5)
{
  if ( !rs::isOnGround(a2, a3) )
    return false;

  *a1 = al::converge(*a1, a4, a5);
  return true;
}


void moveInertiaSlide(sead::Vector3<float> *a1,al::LiveActor *a2,IUsePlayerCollision const*a3,sead::Vector3<float> const&a4,float a5,float a6,float a7,float a8,float a9,float a10,float a11) {
  PlayerActionVelocityControl velocityControl = PlayerActionVelocityControl(a2, a3);

  bool isOnGround = rs::isOnGround(a2, a3);
  sead::Vector3f up = {0.0f, 0.0f, 0.0f};
  if(isOnGround) {
    up = rs::getCollidedGroundNormal(a3);
    velocityControl.calcOnGround(up);
    velocityControl.calcFrontBrake(a5);
  }
  else {
    up = -al::getGravity(a2);
  }

  velocityControl.calcSideVelocityLimit(a4, a6, a7, velocityControl.mVelocityFront.length() * a8);

  sead::Vector3f down = -up;

  if(isOnGround) {
    velocityControl.calcSnap(down, a9);
    velocityControl.apply();
  }
  else {
    velocityControl.calcSnap(down, a10);
    velocityControl.apply();
    al::limitVelocityDir(a2, al::getGravity(a2), a11);
  }

  al::verticalizeVec(a1, up, al::getVelocity(a2));
}

void moveParallelJump(al::LiveActor *actor,sead::Vector3<float> const&a2,float a3,float a4,float a5,float a6,float a7,float a8,float a9) {
  const sead::Vector3f *Gravity; // x20
  const sead::Vector3f *Velocity; // x0
  float z; // s5
  float y; // s4
  float v22; // s3
  float v23; // s0
  float v24; // s2
  float v25; // s6
  float v26; // s3
  float v27; // s7
  float v28; // s3
  float v29; // s1
  float v30; // s0
  const sead::Vector3f *v31; // x0
  float v32; // s2
  float v33; // s1
  float v34; // s2
  float v35; // s1
  sead::Vector3f v36; // [xsp+0h] [xbp-90h] BYREF
  sead::Vector3f v37; // [xsp+10h] [xbp-80h] BYREF
  sead::Vector3f v38; // [xsp+20h] [xbp-70h] BYREF
  sead::Vector3f a1; // [xsp+30h] [xbp-60h] BYREF

  Gravity = &al::getGravity(actor);
  a1.x = 0.0;
  a1.y = 0.0;
  a1.z = 0.0;
  Velocity = &al::getVelocity(actor);
  al::verticalizeVec(&a1, *Gravity, *Velocity);
  v38.x = 0.0;
  v38.y = 0.0;
  v38.z = 0.0;
  if ( al::tryNormalizeOrZero(&v38, a1) )
  {
    v37.x = 0.0;
    v37.y = 0.0;
    v37.z = 0.0;
    z = Gravity->z;
    y = Gravity->y;
    v22 = v38.y * Gravity->x;
    v23 = (float)(v38.z * Gravity->x) - (float)(v38.x * z);
    v37.x = (float)(v38.y * z) - (float)(v38.z * y);
    v37.y = v23;
    v37.z = (float)(v38.x * y) - v22;
    al::normalize(&v37);
    v24 = a2.y;
    v25 = a2.z;
    v26 = (float)((float)(v38.x * a2.x) + (float)(v38.y * v24)) + (float)(v38.z * v25);
    if ( v26 >= 0.0 )
      v27 = a3;
    else
      v27 = a4;

    v28 = v26 * v27;
    v29 = (float)((float)((float)(a2.x * v37.x) + (float)(v24 * v37.y)) + (float)(v25 * v37.z)) * a5;
    a1.x = a1.x + (float)((float)(v38.x * v28) + (float)(v37.x * v29));
    a1.y = a1.y + (float)((float)(v38.y * v28) + (float)(v37.y * v29));
    v30 = a1.z + (float)((float)(v38.z * v28) + (float)(v37.z * v29));
  }
  else
  {
    al::verticalizeVec(&v38, *Gravity, a2);
    a1.x = (float)(v38.x * a3) + a1.x;
    a1.y = (float)(v38.y * a3) + a1.y;
    v30 = (float)(v38.z * a3) + a1.z;
  }

  a1.z = v30;
  al::limitLength(&a1, a1, a6);
  v37.x = 0.0;
  v37.y = 0.0;
  v37.z = 0.0;
  v31 = &al::getVelocity(actor);
  al::parallelizeVec(&v37, *Gravity, *v31);
  v32 = Gravity->z;
  v33 = Gravity->y * a7;
  v36.x = Gravity->x * a7;
  v36.y = v33;
  v36.z = v32 * a7;
  al::addVectorLimit(&v37, v36, a8);
  v36.x = a1.x + v37.x;
  v36.y = a1.y + v37.y;
  v36.z = a1.z + v37.z;
  al::setVelocity(actor, v36);
  v34 = Gravity->z;
  v35 = -Gravity->y;
  v36.x = -Gravity->x;
  v36.y = v35;
  v36.z = -v34;
  rs::slerpUp(actor, v36, a9, 180.0);
}

bool isOnGroundSlopeSlideEnd(al::LiveActor const* a1,IUsePlayerCollision const*a2,PlayerConst const*a3) {
  PlayerCollider *v6; // x0
  float v7; // s0
  const sead::Vector3f *v8; // x21
  const sead::Vector3f *Gravity; // x20
  float v10; // s0
  float v11; // s0
  char isFloorPolygonCos; // w0
  sead::Vector3f v14; // [xsp+0h] [xbp-30h] BYREF

  if ( *(float *)(a2->getPlayerCollider() + 112) >= 0.0
    && ((v14 = al::getVelocity(a1),
         al::tryNormalizeOrZero(&v14),
         v6 = (PlayerCollider *)a2->getPlayerCollider(),
         v7 = (float)((float)(v14.x * v6->mCollidedGroundNormal.x) + (float)(v14.y * v6->mCollidedGroundNormal.y))
            + (float)(v14.z * v6->mCollidedGroundNormal.z),
         v7 <= 0.0)
     || al::isNearZero(v7, 0.001)) )
  {
    v8 = &a2->getPlayerCollider()->mCollidedGroundNormal;
    Gravity = &al::getGravity(a1);
    v10 = a3->getSlopeSlideAngleEnd();
    v11 = sead::Mathf::cos(v10 * 0.017453);
    isFloorPolygonCos = al::isFloorPolygonCos(*v8, *Gravity, v11);
  }
  else
  {
    isFloorPolygonCos = 0;
  }

  return isFloorPolygonCos & 1;
}

f32 moveBrakeRun(float * a1,sead::Vector3<float> *a2,al::LiveActor *x2_0,PlayerActionGroundMoveControl *x3_0,float a6,int a5,float a7,float a8,float a9) {
  float x; // w8
  float y; // x8^4
  const sead::Vector3f *p_mGravityDir; // x23
  float v20; // s0
  float v21; // s0
  float v22; // s11
  float v23; // s4
  float v24; // s5
  sead::Vector3f v26; // [xsp+0h] [xbp-90h] BYREF
  sead::Vector3f a4; // [xsp+10h] [xbp-80h] BYREF
  sead::Vector3f v28; // [xsp+20h] [xbp-70h] BYREF
  sead::Vector3f a3; // [xsp+30h] [xbp-60h] BYREF

  p_mGravityDir = &x3_0->mGravityDir;
  x = x3_0->mGravityDir.x;
  y = x3_0->mGravityDir.y;
  a3.z = x3_0->mGravityDir.z;
  a3.x = x;
  a3.y = y;
  v28.x = 0.0;
  v28.y = 0.0;
  v28.z = 0.0;
  x3_0->updateNormalAndSnap(&v28);
  v20 = sqrtf((float)((float)(v28.x * v28.x) + (float)(v28.y * v28.y)) + (float)(v28.z * v28.z));
  if ( a1 )
    *a1 = v20;

  v21 = PlayerActionFunction::brake(v20, a5, a6);
  a4 = *p_mGravityDir;
  v22 = fmaxf(v21, 0.0);
  al::alongVectorNormalH(a2, *a2, a3, a4);
  al::normalize(a2);
  rs::slerpUp(x2_0, a4, a8, a9);
  v23 = v22 * a2->y;
  v24 = v22 * a2->z;
  v26.x = (float)(v22 * a2->x) - (float)(a4.x * a7);
  v26.y = v23 - (float)(a4.y * a7);
  v26.z = v24 - (float)(a4.z * a7);
  al::setVelocity(x2_0, v26);
  return v22;
}

void moveDivingJump(
        al::LiveActor *x0_0,
        const sead::Vector3f& a2,
        float a3,
        float a4,
        float a5,
        float a6,
        float a7,
        float a8,
        float a9,
        float a10)
{
  const sead::Vector3f *Gravity; // x20
  const sead::Vector3f *Velocity; // x0
  const sead::Vector3f *v22; // x0
  float z; // s2
  float v24; // s1
  float v25; // s2
  float v26; // s1
  sead::Vector3f v27; // [xsp+0h] [xbp-A0h] BYREF
  sead::Vector3f v28; // [xsp+10h] [xbp-90h] BYREF
  sead::Vector3f v29; // [xsp+20h] [xbp-80h] BYREF
  sead::Vector3f a1; // [xsp+30h] [xbp-70h] BYREF

  Gravity = &al::getGravity(x0_0);
  a1.x = 0.0;
  a1.y = 0.0;
  a1.z = 0.0;
  Velocity = &al::getVelocity(x0_0);
  al::verticalizeVec(&a1, *Gravity, *Velocity);
  rs::controlDirectionalVelocity(&a1, x0_0, a2, a3, a4, a5, a6, a7);
  v29.x = 0.0;
  v29.y = 0.0;
  v29.z = 0.0;
  v22 = &al::getVelocity(x0_0);
  al::parallelizeVec(&v29, *Gravity, *v22);
  z = Gravity->z;
  v24 = Gravity->y * a8;
  v28.x = Gravity->x * a8;
  v28.y = v24;
  v28.z = z * a8;
  al::addVectorLimit(&v29, v28, a9);
  v28.x = a1.x + v29.x;
  v28.y = a1.y + v29.y;
  v28.z = a1.z + v29.z;
  al::setVelocity(x0_0, v28);
  if ( a10 > 0.0 )
  {
    v28.x = 0.0;
    v28.y = 0.0;
    v28.z = 0.0;
    if ( !al::tryNormalizeOrZero(&v28, a1) )
      al::calcFrontDir(&v28, x0_0);

    v25 = Gravity->z;
    v26 = -Gravity->y;
    v27.x = -Gravity->x;
    v27.y = v26;
    v27.z = -v25;
    rs::slerpUpFront(x0_0, v27, v28, a10, 180.0);
  }
}

void controlDirectionalVelocity(
        sead::Vector3f *x0_0,
        const al::LiveActor *a2,
        const sead::Vector3f& a3,
        float a4,
        float a5,
        float a6,
        float a7,
        float a8)
{
  float x; // s0
  float y; // s1
  float z; // s2
  float v19; // s3
  float v20; // s4
  float v21; // s6
  float v22; // s4
  float v23; // s7
  float v24; // s16
  float v25; // s3
  float v26; // s4
  float v27; // s5
  float v28; // s6
  float v29; // s1
  float v30; // s0
  float v31; // s2
  float v32; // s1
  float v33; // s2
  float v34; // s0
  sead::Vector3f v35; // [xsp+0h] [xbp-80h] BYREF
  sead::Vector3f v36; // [xsp+10h] [xbp-70h] BYREF
  sead::Vector3f a1; // [xsp+20h] [xbp-60h] BYREF
  sead::Vector3f v38; // [xsp+30h] [xbp-50h] BYREF

  v38.x = 0.0;
  v38.y = 0.0;
  v38.z = 0.0;
  a1.x = 0.0;
  a1.y = 0.0;
  a1.z = 0.0;
  al::calcFrontDir(&v38, a2);
  al::calcUpDir(&a1, a2);
  v36.x = (float)(a1.y * v38.z) - (float)(a1.z * v38.y);
  v36.y = (float)(a1.z * v38.x) - (float)(v38.z * a1.x);
  v36.z = (float)(v38.y * a1.x) - (float)(a1.y * v38.x);
  al::tryNormalizeOrZero(&v36);
  x = v38.x;
  y = v38.y;
  z = v38.z;
  v19 = (float)((float)(v38.x * a3.x) + (float)(v38.y * a3.y)) + (float)(v38.z * a3.z);
  if ( v19 >= 0.0 )
    v20 = a4;
  else
    v20 = a5;

  v21 = v19 * v20;
  v22 = x0_0->y;
  v23 = x0_0->z;
  v24 = (float)((float)(v38.x * x0_0->x) + (float)(v38.y * v22)) + (float)(v38.z * v23);
  v25 = x0_0->x - (float)(v38.x * v24);
  v26 = v22 - (float)(v38.y * v24);
  v27 = v23 - (float)(v38.z * v24);
  v28 = v24 + v21;
  x0_0->x = v25;
  x0_0->y = v26;
  x0_0->z = v27;
  if ( v28 >= a7 )
  {
    a7 = v28;
    if ( v28 > a6 )
      a7 = a6;
  }

  x0_0->x = (float)(a7 * x) + v25;
  x0_0->y = (float)(a7 * y) + v26;
  x0_0->z = (float)(a7 * z) + v27;
  v35 = a3;
  if ( al::tryNormalizeOrZero(&v35) )
  {
    v29 = (float)((float)((float)(v36.x * v35.x) + (float)(v36.y * v35.y)) + (float)(v36.z * v35.z)) * a8;
    v30 = x0_0->x + (float)(v36.x * v29);
    v31 = v36.y * v29;
    v32 = v36.z * v29;
    v33 = x0_0->y + v31;
    x0_0->x = v30;
    v34 = v32 + x0_0->z;
    x0_0->y = v33;
    x0_0->z = v34;
  }
}

void reflectCeiling(al::LiveActor* a2, float a3) {
  const sead::Vector3f *Gravity; // x20
  const sead::Vector3f *Velocity; // x0
  float v6; // s9
  sead::Vector3f *VelocityPtr; // x0
  float z; // s3
  float v9; // s2
  sead::Vector3f v10; // [xsp+0h] [xbp-30h] BYREF

  Gravity = &al::getGravity(a2);
  Velocity = &al::getVelocity(a2);
  v6 = fmaxf(
         (float)((float)(Velocity->x * Gravity->x) + (float)(Velocity->y * Gravity->y))
       + (float)(Velocity->z * Gravity->z),
         0.0);
  VelocityPtr = al::getVelocityPtr(a2);
  al::verticalizeVec(VelocityPtr, *Gravity, *VelocityPtr);
  z = Gravity->z;
  v9 = Gravity->y * (float)(v6 + a3);
  v10.x = Gravity->x * (float)(v6 + a3);
  v10.y = v9;
  v10.z = (float)(v6 + a3) * z;
  al::addVelocity(a2, v10);
}

bool isCollidedCeiling(IUsePlayerCollision const* collider) {
  return collider->getPlayerCollider()->val3 >= 0.0f;
}

bool isOnGroundAndGravity(al::LiveActor const*a1, IUsePlayerCollision const*a2) {
  float *v4; // x0
  float v5; // s0
  const sead::Vector3f *Gravity; // x20
  const sead::Vector3f *Velocity; // x0
  sead::Vector3f v9; // [xsp+0h] [xbp-20h] BYREF

  if ( *(float *)(a2->getPlayerCollider() + 112) < 0.0 )
    return 0LL;

  if ( *(float *)(a2->getPlayerCollider() + 112) < 0.0 )
    return 0LL;

  v9 = al::getVelocity(a1);
  al::tryNormalizeOrZero(&v9);
  v4 = (float *)a2->getPlayerCollider();
  v5 = (float)((float)(v9.x * v4[105]) + (float)(v9.y * v4[106])) + (float)(v9.z * v4[107]);
  if ( v5 > 0.0 && !al::isNearZero(v5, 0.001) )
    return 0LL;

  Gravity = &al::getGravity(a1);
  Velocity = &al::getVelocity(a1);
  return (float)((float)((float)(Gravity->x * Velocity->x) + (float)(Gravity->y * Velocity->y))
               + (float)(Gravity->z * Velocity->z)) > 0.0;
}

void slerpUp(al::LiveActor*a1, sead::Vector3<float> const&a2, float a3, float a4) {
  float z; // s1
  float y; // s4
  float v10; // s6
  float v11; // s7
  float v12; // s1
  sead::Vector3f v13; // [xsp+0h] [xbp-40h] BYREF
  sead::Vector3f v14; // [xsp+10h] [xbp-30h] BYREF

  v14.x = 0.0;
  v14.y = 0.0;
  v14.z = 0.0;
  al::calcFrontDir(&v14, a1);
  if ( al::isParallelDirection(a2, v14, 0.01) )
  {
    v13.x = 0.0;
    v13.y = 0.0;
    v13.z = 0.0;
    al::calcSideDir(&v13, a1);
    y = a2.y;
    z = a2.z;
    v10 = v13.y * z;
    v11 = v13.y * a2.x;
    v12 = (float)(v13.z * a2.x) - (float)(z * v13.x);
    v14.x = v10 - (float)(v13.z * y);
    v14.y = v12;
    v14.z = (float)(y * v13.x) - v11;
    al::normalize(&v14);
  }

  rs::slerpUpFront(a1, a2, v14, a3, a4);
}

void setupLongJumpVelocity(al::LiveActor *self,
        const IUsePlayerCollision *a2,
        float a3,
        float a4,
        float a5,
        float a6,
        float a7)
{
  float v14; // s0
  const sead::Vector3f *Gravity; // x0
  float x; // s0
  float y; // s1
  float z; // s2
  const sead::Vector3f *Velocity; // x0
  float v20; // s0
  float v21; // s0
  float v22; // s0
  sead::Vector3f v23; // [xsp+0h] [xbp-B0h] BYREF
  sead::Vector3f v24; // [xsp+10h] [xbp-A0h] BYREF
  sead::Vector3f v25; // [xsp+20h] [xbp-90h] BYREF
  sead::Vector3f vec; // [xsp+30h] [xbp-80h] BYREF
  sead::Vector3f out; // [xsp+40h] [xbp-70h] BYREF
  sead::Vector3f v28; // [xsp+50h] [xbp-60h] BYREF
  sead::Vector3f v29; // [xsp+60h] [xbp-50h] BYREF

  v29.x = 0.0;
  v29.y = 0.0;
  v29.z = 0.0;
  v28.x = 0.0;
  v28.y = 0.0;
  v28.z = 0.0;
  rs::calcFrontVelocityAndDirH(&v29, &v28, self, a2);
  v14 = v29.length();
  out.x = 0.0;
  out.y = 0.0;
  out.z = 0.0;
  vec.z = v14 * v28.z;
  vec.x = v14 * v28.x;
  vec.y = v14 * v28.y;
  rs::calcJumpInertia(&out, self, a2, vec, a3);
  Gravity = &al::getGravity(self);
  x = Gravity->x;
  y = Gravity->y;
  z = Gravity->z;
  v25.x = 0.0;
  v25.y = 0.0;
  v25.z = 0.0;
  vec.x = -x;
  vec.y = -y;
  vec.z = -z;
  Velocity = &al::getVelocity(self);
  al::parallelizeVec(&v25, vec, *Velocity);
  if ( (float)((float)((float)(v25.x * vec.x) + (float)(v25.y * vec.y)) + (float)(v25.z * vec.z)) < 0.0 )
  {
    v25.x = 0.0;
    v25.y = 0.0;
    v25.z = 0.0;
  }

  v20 = sqrtf((float)((float)(v29.x * v29.x) + (float)(v29.y * v29.y)) + (float)(v29.z * v29.z)) + a4;
  if ( v20 >= a5 )
  {
    a5 = v20;
    if ( v20 > a6 )
      a5 = a6;
  }

  v21 = sqrtf((float)((float)(v29.x * v29.x) + (float)(v29.y * v29.y)) + (float)(v29.z * v29.z));
  if ( v21 > 0.0 )
  {
    v22 = a5 / v21;
    v29.x = v22 * v29.x;
    v29.y = v22 * v29.y;
    v29.z = v22 * v29.z;
  }

  v24.x = (float)(v29.x + v25.x) + (float)(vec.x * a7);
  v24.y = (float)(v29.y + v25.y) + (float)(vec.y * a7);
  v24.z = (float)(v29.z + v25.z) + (float)(vec.z * a7);
  al::limitLength(&v24, v24, sqrtf((float)(a6 * a6) + (float)(a7 * a7)));
  v23.x = v24.x + out.x;
  v23.y = v24.y + out.y;
  v23.z = v24.z + out.z;
  al::setVelocity(self, v23);
}

void calcFrontVelocityAndDirH(
        sead::Vector3f *a1,
        sead::Vector3f *a2,
        const al::LiveActor *self,
        const IUsePlayerCollision *a4)
{
  const sead::Vector3f *Velocity; // x23
  const sead::Vector3f *Gravity; // x21
  sead::Vector3f v10; // [xsp+0h] [xbp-40h] BYREF

  Velocity = &al::getVelocity(self);
  Gravity = &al::getGravity(self);
  if ( a4 && rs::isCollidedGround(a4) )
    v10 = rs::getCollidedGroundNormal(a4);
  else
    al::calcUpDir(&v10, self);

  al::verticalizeVec(a1, v10, *Velocity);
  al::verticalizeVec(a1, *Gravity, *a1);
  if ( a2 && !al::tryNormalizeOrZero(a2, *a1) )
  {
    al::calcFrontDir(a2, self);
    al::verticalizeVec(a2, *Gravity, *a2);
    if ( !al::tryNormalizeOrZero(a2) )
      *a2 = v10;
  }
}

bool isCollidedGroundOverAngle(const al::LiveActor * actor, const IUsePlayerCollision * collider, float angle) {
  if(collider->getPlayerCollider()->val1 < 0.0f) return false;
  sead::Vector3f normal = rs::getCollidedGroundNormal(collider);
  return !al::isFloorPolygonCos(normal, al::getGravity(actor), sead::Mathf::cos(sead::Mathf::deg2rad(angle)));
}

bool isCollidedGroundLessAngle(const al::LiveActor * actor, const IUsePlayerCollision * collider, float angle) {
  if(collider->getPlayerCollider()->val1 < 0.0f) return false;
  sead::Vector3f normal = rs::getCollidedGroundNormal(collider);
  return al::isFloorPolygonCos(normal, al::getGravity(actor), sead::Mathf::cos(sead::Mathf::deg2rad(angle)));
}

bool calcAlongDirFront(sead::Vector3<float> *a1,al::LiveActor const*a2,sead::Vector3<float> const&a3) {
  al::calcFrontDir(a1, a2);
  sead::Vector3f v7 = {0.0f, 0.0f, 0.0f};
  al::calcUpDir(&v7, a2);
  al::alongVectorNormalH(a1, *a1, v7, a3);
  return al::tryNormalizeOrZero(a1);
}


void moveInertiaTurn(sead::Vector3<float> *x0_0,sead::Quat<float> *x1_0,al::LiveActor *actor,IUsePlayerCollision const*a4,sead::Vector3<float> const&a5,float a6,float a7,float a8,float a9,float a10) {
  PlayerActionVelocityControl velocityControl = PlayerActionVelocityControl(actor, a4);

  sead::Vector3f up = {0.0f, 0.0f, 0.0f};
  if(rs::isOnGround(actor, a4)) {
    up = rs::getCollidedGroundNormal(a4);
    velocityControl.calcOnGround(up);
    velocityControl.calcFrontBrake(a6);
    velocityControl.calcSnap(-up, a8);
    velocityControl.apply();
  }
  else {
    sead::Vector3f gravity = al::getGravity(actor);
    up = -gravity;
    velocityControl.calcSnap(gravity, a9);
    velocityControl.apply();
    al::limitVelocityDir(actor, al::getGravity(actor), a10);
  }

  sead::Vector3f v42 = {0.0f, 0.0f, 0.0f};
  sead::Vector3f v41 = {0.0f, 0.0f, 0.0f};
  al::separateVectorParallelVertical(&v41, &v42, up, al::getVelocity(actor));
  f32 v26 = v42.length();
  sead::Vector3f a2 = {0.0f, 0.0f, 0.0f};
  if(al::tryNormalizeOrZero(&a2, v42)) {
    sead::Vector3f a3;
    al::verticalizeVec(&a3, up, a5);
    if(al::tryNormalizeOrZero(&a3) && !PlayerActionFunction::isOppositeDir(a3, a2)) {
      sead::Quatf a1 = sead::Quatf::unit;
      al::makeQuatRotationLimit(&a1, a2, a3, sead::Mathf::deg2rad(a7));
      f32 v30 = (float)((float)(a1.y * a2.z) - (float)(a1.z * a2.y)) + (float)(a1.w * a2.x);
      f32 v31 = (float)(a2.y * a1.w) + (float)((float)(a1.z * a2.x) - (float)(a2.z * a1.x));
      f32 v32 = (float)(a2.z * a1.w) + (float)((float)(a2.y * a1.x) - (float)(a1.y * a2.x));
      f32 v33 = (float)((float)-(float)(a2.x * a1.x) - (float)(a1.y * a2.y)) - (float)(a2.z * a1.z);
      a2.x = (float)((float)(a1.y * v32) + (float)((float)(a1.w * v30) - (float)(a1.z * v31))) - (float)(a1.x * v33);
      a2.y = (float)((float)((float)(v30 * a1.z) + (float)(v31 * a1.w)) - (float)(v32 * a1.x)) - (float)(v33 * a1.y);
      a2.z = (float)((float)((float)(v31 * a1.x) - (float)(v30 * a1.y)) + (float)(v32 * a1.w)) - (float)(v33 * a1.z);
      *x1_0 = a1;
    }
  }

  al::setVelocity(actor, (v26 * a2) + v41);
  *x0_0 = (v26 * a2);
}

bool isActionCodeNoWallGrab(IUsePlayerCollision const* a1) {
  if(a1->getPlayerCollider()->val1 < 0.0f) return false;
  return al::isWallCode(a1->getPlayerCollider()->info2, "NoAction") ||
    al::isWallCode(a1->getPlayerCollider()->info2, "NoWallGrab") ||
    al::isWallCode(a1->getPlayerCollider()->info2, "ReflectStickNoWallGrab") ||
    al::isWallCode(a1->getPlayerCollider()->info2, "OnlyWallHitDown") ||
    al::isFloorCode(a1->getPlayerCollider()->info2, "GrabCeil") ||
    al::isFloorCode(a1->getPlayerCollider()->info2, "Pole") ||
    al::isFloorCode(a1->getPlayerCollider()->info2, "Needle") ||
    al::isFloorCode(a1->getPlayerCollider()->info2, "DamageFire");
}
bool isActionCodeNoWallGrab(al::HitInfo const* a1) {
  return al::isWallCode(a1, "NoAction") ||
    al::isWallCode(a1, "NoWallGrab") ||
    al::isWallCode(a1, "ReflectStickNoWallGrab") ||
    al::isWallCode(a1, "OnlyWallHitDown") ||
    al::isFloorCode(a1, "GrabCeil") ||
    al::isFloorCode(a1->mTriangle, "Pole") ||
    al::isFloorCode(a1->mTriangle, "Needle") ||
    al::isFloorCode(a1->mTriangle, "DamageFire");
}
bool isActionCodeNoWallPopUp(al::HitInfo const* a1) {
  return al::isWallCode(a1, "NoAction") ||
    al::isWallCode(a1, "NoWallGrab") ||
    al::isWallCode(a1, "ReflectStickNoWallGrab") ||
    al::isWallCode(a1, "OnlyWallHitDown") ||
    al::isFloorCode(a1, "GrabCeil") ||
    al::isFloorCode(a1->mTriangle, "Needle") ||
    al::isFloorCode(a1->mTriangle, "DamageFire") ||
    al::isFloorCode(a1->mTriangle, "Poison");
}
bool isActionCodeNoWallKeepWall(IUsePlayerCollision const* a1) {
  if(a1->getPlayerCollider()->val2 < 0.0f) return false;
  return al::isWallCode(a1->getPlayerCollider()->info2, "NoAction") ||
    al::isWallCode(a1->getPlayerCollider()->info2, "OnlyWallHitDown") ||
    al::isFloorCode(a1->getPlayerCollider()->info2, "GrabCeil") ||
    al::isFloorCode(a1->getPlayerCollider()->info2, "Pole");
}

bool calcExistCollisionBorderWallCatch(al::IUseCollision const* a1,sead::Vector3<float> const& x1_0,sead::Vector3<float> const& x2_0,sead::Vector3<float> const& x3_0) {
  float y; // s1
  float z; // s2
  float v9; // s4
  float v10; // s3
  float v11; // s5
  float v14; // s1
  float v15; // s2
  float v16; // s2
  float v17; // s1
  sead::Vector3f v18; // [xsp+0h] [xbp-60h] BYREF
  sead::Vector3f a4; // [xsp+10h] [xbp-50h] BYREF
  sead::Vector3f a3; // [xsp+20h] [xbp-40h] BYREF
  sead::Vector3f a2; // [xsp+30h] [xbp-30h] BYREF

  a2.x = 0.0;
  a2.y = 0.0;
  a2.z = 0.0;
  a3.x = 0.0;
  a3.y = 0.0;
  a3.z = 0.0;
  y = x2_0.y;
  z = x2_0.z;
  v9 = x1_0.y - y;
  v10 = x1_0.x - x2_0.x;
  v11 = x1_0.z - z;
  v18.x = x2_0.x + x2_0.x;
  v18.y = y + y;
  a4.x = v10;
  a4.y = v9;
  a4.z = v11;
  v18.z = z + z;
  if ( alCollisionUtil::getHitPosAndNormalOnArrow(a1, &a2, &a3, a4, v18, 0LL, 0LL)
    && al::isReverseDirection(a3, x2_0, 0.01) )
  {
    return 1LL;
  }

  v14 = x1_0.y - (float)(x2_0.y * 5.0);
  v15 = x1_0.z - (float)(x2_0.z * 5.0);
  a4.x = x1_0.x - (float)(x2_0.x * 5.0);
  a4.y = v14;
  a4.z = v15;
  v16 = x3_0.z;
  v17 = x3_0.y * 65.0;
  v18.x = x3_0.x * 65.0;
  v18.y = v17;
  v18.z = v16 * 65.0;
  return alCollisionUtil::getHitPosAndNormalOnArrow(a1, &a2, &a3, a4, v18, 0LL, 0LL)
      && (float)((float)((float)(a3.x * x3_0.x) + (float)(a3.y * x3_0.y)) + (float)(a3.z * x3_0.z)) < -0.70711;
}

bool sub_710055F974(
        const al::CollisionParts **x0_0,
        sead::Vector3f *a2,
        sead::Vector3f *x2_0,
        const al::LiveActor *a4,
        const sead::Vector3f& a5,
        const sead::Vector3f& a6,
        const sead::Vector3f& a7,
        bool a8,
        float a9,
        float a10,
        float a11,
        float a12,
        float a13,
        float a14,
        float a15,
        bool a16,
        bool a17)
{
  const sead::Vector3f *Gravity; // x0
  float v32; // s8
  const sead::Vector3f *v33; // x22
  float v34; // s0
  float v35; // s14
  float v36; // s1
  float v37; // s2
  float v38; // s0
  float v39; // s4
  float v40; // s3
  float v41; // s5
  const al::IUseCollision *v42; // x24
  float v43; // s13
  float v44; // s8
  float v45; // s9
  float x; // s0
  float y; // s1
  float v48; // s3
  float v49; // s4
  float z; // s2
  float v51; // s5
  float v52; // s6
  float v53; // s17
  float v54; // s5
  float v55; // s3
  float v56; // s4
  float v57; // s8
  float v58; // s0
  const sead::Vector3f *Trans; // x0
  float v60; // s1
  float v61; // s4
  float v62; // s6
  float v63; // s7
  float v64; // s1
  float v65; // s10
  float v66; // s9
  float v67; // s14
  float v68; // s15
  float v69; // s13
  int v70; // w0
  int v71; // w25
  int v72; // w0
  float v73; // s1
  float v74; // s2
  float v75; // s3
  float v76; // s2
  float v77; // s0
  float v78; // s11
  float v79; // s9
  float v80; // s10
  float v83; // [xsp+8h] [xbp-138h]
  sead::Vector3f v85; // [xsp+10h] [xbp-130h] BYREF
  const al::ArrowHitInfo *v86; // [xsp+20h] [xbp-120h] BYREF
  sead::Vector3f a1; // [xsp+28h] [xbp-118h] BYREF
  sead::Vector3f v88; // [xsp+38h] [xbp-108h] BYREF
  sead::Vector3f v89; // [xsp+48h] [xbp-F8h] OVERLAPPED BYREF
  sead::Vector3f v91; // [xsp+58h] [xbp-E8h] BYREF
  sead::Vector3f v92; // [xsp+68h] [xbp-D8h] BYREF
  sead::Vector3f a3; // [xsp+78h] [xbp-C8h] BYREF
  al::HitInfo *v95; // [xsp+98h] [xbp-A8h] BYREF
  sead::Vector3f v96; // [xsp+A0h] [xbp-A0h] BYREF

  Gravity = &al::getGravity(a4);
  v32 = -a5.dot(a7);
  v33 = Gravity;
  if ( v32 < sead::Mathf::cos(a9 * 0.017453) || !al::isWallPolygon(a7, *v33) )
    return 0LL;

  v83 = a13;
  v34 = fmaxf(a11, 3.0);
  v35 = v34 + fmaxf(a12, 3.0);
  v36 = a6.x - (float)(v34 * v33->x);
  v37 = a6.y - (float)(v34 * v33->y);
  v38 = a6.z - (float)(v34 * v33->z);
  v39 = a7.y * 5.0;
  v40 = a7.x * 5.0;
  v41 = a7.z * 5.0;
  v95 = 0LL;
  v42 = a4;
  al::TriangleFilterGroundOnly v94 = {};
  v94._8 = v33;
  v96.x = v36 - v40;
  v96.y = v37 - v39;
  v96.z = v38 - v41;
  a3 = *v33;
  v43 = v35 * a3.y;
  v44 = v35 * a3.x;
  v45 = v35 * a3.z;
  v89.z = v35 * a3.z;
  v89.x = v35 * a3.x;
  v89.y = v35 * a3.y;
  if ( (alCollisionUtil::getFirstPolyOnArrow(v42, (const al::ArrowHitInfo **)&v95, v96, v89, 0LL, &v94) & 1) == 0 )
  {
    x = a6.x;
    y = a6.y;
    v48 = a7.x;
    v49 = a7.y;
    z = a6.z;
    v51 = a7.z;
    v89.x = v44;
    v89.y = v43;
    v89.z = v45;
    v96.x = (float)(x - v44) - (float)(v48 * 2.5);
    v96.y = (float)(y - v43) - (float)(v49 * 2.5);
    v96.z = (float)(z - v45) - (float)(v51 * 2.5);
    if ( (alCollisionUtil::getFirstPolyOnArrow(v42, (const al::ArrowHitInfo **)&v95, v96, v89, 0LL, &v94) & 1) == 0 )
    {
      if ( al::isNearZeroOrGreater(a7.dot(*v33), 0.001f) )
      {
        return 0LL;
      }

      al::verticalizeVec(&a3, a7, a3);
      if ( !al::tryNormalizeOrZero(&a3) )
      {
        a3 = *v33;
        return 0LL;
      }

      v52 = a7.x * 5.0;
      v53 = a7.z * 5.0;
      v54 = a6.z - (float)(v35 * a3.z);
      v55 = a6.x - (float)(v35 * a3.x);
      v56 = (float)(a6.y - (float)(v35 * a3.y)) - (float)(a7.y * 5.0);
      v89.z = v35 * a3.z;
      v96.x = v55 - v52;
      v96.y = v56;
      v96.z = v54 - v53;
      v89.x = v35 * a3.x;
      v89.y = v35 * a3.y;
      if ( (alCollisionUtil::getFirstPolyOnArrow(v42, (const al::ArrowHitInfo **)&v95, v96, v89, 0LL, &v94) & 1) == 0 )
        return 0LL;
    }
  }

  if ( a17 )
  {
    if ( !rs::isActionCodeNoWallPopUp(v95) )
      goto LABEL_15;

    return 0LL;
  }

  if ( rs::isActionCodeNoWallGrab(v95) )
    return 0LL;

LABEL_15:
  *x0_0 = alCollisionUtil::getCollisionHitParts(v95);
  v92 = alCollisionUtil::getCollisionHitNormal(v95);
  *a2 = alCollisionUtil::getCollisionHitPos(v95);
  if ( rs::calcExistCollisionBorderWallCatch(v42, *a2, v92, a7) )
    return 0LL;

  v57 = (float)((float)-(float)(v33->y * v92.y) - (float)(v33->x * v92.x)) - (float)(v33->z * v92.z);
  v58 = sead::Mathf::cos(a10 * 0.017453);
  if ( !al::isNearZeroOrGreater(v57 - v58, 0.001) )
    return 0LL;

  if ( a16 )
  {
    Trans = &al::getTrans(a4);
    if ( (float)((float)((float)-(float)((float)(a2->y - Trans->y) * v33->y)
                       - (float)((float)(a2->x - Trans->x) * v33->x))
               - (float)((float)(a2->z - Trans->z) * v33->z)) > a11 )
      return 0LL;
  }

  v61 = a7.y;
  v60 = a7.z;
  v62 = v92.y * v60;
  v63 = v92.y * a7.x;
  v64 = (float)(v92.z * a7.x) - (float)(v60 * v92.x);
  v91.x = v62 - (float)(v92.z * v61);
  v91.y = v64;
  v91.z = (float)(v61 * v92.x) - v63;
  if ( !al::tryNormalizeOrZero(&v91) )
    return 0LL;

  al::TriangleFilterIgnoreGround v90 = {};
  v90._8 = v33;
  v65 = (float)(a14 + 25.0) + -7.0;
  v66 = a8 ? a15 + 10.0 : 15.0;
  v67 = (float)(v92.y * 5.0) + a2->y;
  v68 = (float)(v92.z * 5.0) + a2->z;
  v69 = (float)(v92.x * 5.0) + a2->x;
  a1.x = v65 * v91.x;
  a1.y = v65 * v91.y;
  v88.x = v69 - (float)(v91.x * 25.0);
  v88.y = v67 - (float)(v91.y * 25.0);
  v88.z = v68 - (float)(v91.z * 25.0);
  a1.z = v65 * v91.z;
  v70 = alCollisionUtil::checkStrikeArrow(v42, v88, a1, 0LL, &v90);
  a1.x = -(float)(v65 * v91.x);
  a1.y = -(float)(v65 * v91.y);
  v71 = v70;
  v88.x = v69 + (float)(v91.x * 25.0);
  v88.y = v67 + (float)(v91.y * 25.0);
  v88.z = v68 + (float)(v91.z * 25.0);
  a1.z = -(float)(v65 * v91.z);
  v72 = alCollisionUtil::checkStrikeArrow(v42, v88, a1, 0LL, &v90);
  v73 = a7.y;
  v74 = a7.z;
  v75 = v69 + (float)(a7.x * 10.0);
  a1.x = -(float)(v66 * a7.x);
  a1.y = -(float)(v66 * v73);
  v88.x = v75;
  v88.y = v67 + (float)(v73 * 10.0);
  v88.z = v68 + (float)(v74 * 10.0);
  a1.z = -(float)(v66 * v74);
  if ( v72 | v71 | (unsigned int)alCollisionUtil::checkStrikeArrow(v42, v88, a1, 0LL, &v90) )
    return 0LL;

  v86 = 0LL;
  v76 = (float)(a7.y * 20.0) + a2->y;
  v77 = (float)(a7.z * 20.0) + a2->z;
  v88.x = (float)(a7.x * 20.0) + a2->x;
  v88.y = v76;
  v88.z = v77;
  v78 = v33->z;
  v79 = v33->x;
  v80 = v33->y;
  a1 = v92;
  al::verticalizeVec(&a1, *v33, a1);
  if ( al::tryNormalizeOrZero(&a1) )
  {
    if ( al::isNearZeroOrGreater(a1.dot(a7), 0.001) )
    {
      v79 = v33->x;
      v80 = v33->y;
      v78 = v33->z;
    }
    else
    {
      v79 = -v92.x;
      v80 = -v92.y;
      v78 = -v92.z;
    }
  }

  v85.x = v79 * v83;
  v85.y = v80 * v83;
  v85.z = v78 * v83;
  if ( (alCollisionUtil::getFirstPolyOnArrow(v42, &v86, v88, v85, 0LL, 0LL) & 1) != 0 )
    return 0LL;

  if ( al::isInAreaObj(a4, "WallCatchNoEntryArea", *a2) )
    return 0LL;

  *x2_0 = v92;
  return 1LL;
}

bool findWallCatchPos(al::CollisionParts const** parts, sead::Vector3<float>* a2, sead::Vector3<float>*a3, al::LiveActor const* a4, sead::Vector3<float> const& a5, sead::Vector3<float> const& a6, sead::Vector3<float> const& a7, float a8, float a9, float a10, float a11, float a12, float a13, float a14) {
  const sead::Vector3f& gravity = al::getGravity(a4);
  sead::Vector3f velocity = al::getVelocity(a4);
  if(al::tryNormalizeOrZero(&velocity, velocity) && velocity.dot(gravity) > 0.0f) {
    return sub_710055F974(parts, a2, a3, a4, a5, a6, a7, 1, a8, a9, a10, a11, a12, a13, a14, 1, 0);
  }
  return false;
}

bool isAboveGround(const IUsePlayerHeightCheck* a1) {
  return a1->isAboveGround();
}

void resetCollisionExpandCheck(IUsePlayerCollision * collision) {
  PlayerCollider* collider = collision->getPlayerCollider();
  f32 boundingRadius = 0.0f;
  collider->calcBoundingRadius(&boundingRadius);

  if(al::isNearZero(boundingRadius, 0.001f)) {
    collision->getPlayerCollider()->onInvalidate();
    return;
  }
  f32 collisionShapeScale = collider->mCollisionShapeScale;
  if(al::isNearZero(collisionShapeScale, 0.001f)) {
    collision->getPlayerCollider()->onInvalidate();
    return;
  }

  f32 v5 = sead::Mathf::clampMax(boundingRadius / collisionShapeScale, 20.0f) / (boundingRadius / collisionShapeScale);
  collider->setCollisionShapeScale(sead::Mathf::clamp(v5, 0.0f, 1.0f));
  collision->getPlayerCollider()->onInvalidate();
  collider->setCollisionShapeScale(collisionShapeScale);
}


}  // namespace rs
