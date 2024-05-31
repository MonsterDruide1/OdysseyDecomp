#include "Stuff.h"

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
#include "math/seadMatrix.h"
#include "math/seadVectorFwd.h"

namespace rs {

bool isCollisionCodePress(al::HitInfo* info) {
    return al::isFloorCode(info, "Press");
}

bool calcExistCollisionBorder(const al::IUseCollision* a1, const sead::Vector3f& a2, const sead::Vector3f& a3) {
  float y; // s1
  float z; // s2
  float v6; // s4
  float v7; // s5
  float v8; // s0
  sead::Vector3f v10; // [xsp+0h] [xbp-50h] BYREF
  sead::Vector3f v11; // [xsp+10h] [xbp-40h] BYREF
  sead::Vector3f v12; // [xsp+20h] [xbp-30h] BYREF
  sead::Vector3f v13; // [xsp+30h] [xbp-20h] BYREF

  v13.x = 0.0;
  v13.y = 0.0;
  v13.z = 0.0;
  v12.x = 0.0;
  v12.y = 0.0;
  v12.z = 0.0;
  y = a3.y;
  z = a3.z;
  v6 = a2.y - y;
  v7 = a2.z - z;
  v8 = a3.x + a3.x;
  v11.x = a2.x - a3.x;
  v11.y = v6;
  v11.z = v7;
  v10.x = v8;
  v10.y = y + y;
  v10.z = z + z;
  return (alCollisionUtil::getHitPosAndNormalOnArrow(a1, &v13, &v12, v11, v10, 0LL, 0LL) & 1) != 0
      && al::isReverseDirection(v12, a3, 0.01);
}

bool isPlayer2D(const al::LiveActor* actor) {
    return false;
}


bool sub_7100569734(const al::LiveActor *actor, const IUsePlayerCollision *a2, const PlayerConst *a3, bool a4)
{
  const sead::Vector3f *Gravity; // x22
  const sead::Vector3f *Velocity; // x0
  float v10; // q0
  float v11; // s8
  float v12; // q0
  float v13; // s0
  float v14; // s8
  float v15; // s9
  float v16; // s0
  float v17; // s8
  const sead::Vector3f *v18; // x20
  const sead::Vector3f *v19; // x19
  float v20; // s0
  sead::Vector3f v22; // [xsp+0h] [xbp-40h] BYREF

  v22.x = 0.0;
  v22.y = 0.0;
  v22.z = 0.0;
  if ( a4 )
    Gravity = &al::getGravity(actor);
  else
    Gravity = &a2->getPlayerCollider()->unk10;

  Velocity = &al::getVelocity(actor);
  al::verticalizeVec(&v22, *Gravity, *Velocity);
  v10 = a3->getHillAngleSpeedMin();
  v11 = *(float *)&v10;
  v12 = a3->getHillAngleSpeedMax();
  v13 = al::calcRate01(
          sqrtf((float)((float)(v22.x * v22.x) + (float)(v22.y * v22.y)) + (float)(v22.z * v22.z)),
          v11,
          *(float *)&v12);
  v14 = al::easeIn(v13);
  v15 = a3->getStandAngleMin();
  v16 = a3->getStandAngleMax();
  v17 = al::lerpValue(v15, v16, v14);
  v18 = &a2->getPlayerCollider()->unk10;
  v19 = &al::getGravity(actor);
  v20 = cosf(v17 * 0.017453);
  return al::isFloorPolygonCos(*v18, *v19, v20);
}

bool isLandGroundRunAngle(al::LiveActor const* actor,IUsePlayerCollision const* collision,PlayerConst const* pConst) {
  if(collision->getPlayerCollider()->val1 < 0.0f) return false;

  sead::Vector3f velocity = al::getVelocity(actor);
  al::tryNormalizeOrZero(&velocity);
  f32 dot = collision->getPlayerCollider()->unk10.dot(velocity);
  if(dot <= 0.0f || al::isNearZero(dot, 0.001f)) {
    return sub_7100569734(actor, collision, pConst, true);
  }
  return false;
}

bool isJustLand(const IUsePlayerCollision* collision) {
  printf("Collision: %p\n", collision);
  printf("PlayerCollider: %p\n", collision->getPlayerCollider());
  return collision->getPlayerCollider()->val1 >= 0.0f && collision->getPlayerCollider()->mTimeInAir == 1;
}

bool isOnGround(al::LiveActor const*,IUsePlayerCollision const*) {CRASH}
bool isOnGroundRunAngle(al::LiveActor const*actor,IUsePlayerCollision const*collision,PlayerConst const*pConst) {
  if(collision->getPlayerCollider()->val1 < 0.0f) return false;

  sead::Vector3f velocity = al::getVelocity(actor);
  al::tryNormalizeOrZero(&velocity);
  f32 dot = collision->getPlayerCollider()->unk10.dot(velocity);
  if(dot <= 0.0f || al::isNearZero(dot, 0.001f)) {
    bool check = collision->getPlayerCollider()->val1 >= 0.0f && collision->getPlayerCollider()->mTimeInAir == 1;
    return sub_7100569734(actor, collision, pConst, check);
  }
  return false;
}
void cutVerticalVelocityGroundNormal(al::LiveActor* actor, const IUsePlayerCollision* collision) {
  if(!rs::isCollidedGround(collision)) return;

  sead::Vector3f* velocity = al::getVelocityPtr(actor);
  al::parallelizeVec(velocity, rs::getCollidedGroundNormal(collision), *velocity);
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
      v13 = fmaxf((float)((float)(a1.x * v17.x) + (float)(a1.y * v17.y)) + (float)(a1.z * v17.z), 0.0) * val;
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

bool isCollidedGround(const IUsePlayerCollision* collision) {
    printf("%p\n", collision);
    printf("%p\n", collision->getPlayerCollider());
    return collision->getPlayerCollider()->val1 >= 0.0f;
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
    return collision->getPlayerCollider()->unk10;
}

const sead::Vector3f& getCollidedGroundPos(const IUsePlayerCollision* collision) {
    return collision->getPlayerCollider()->unk9;
}

void calcMovePowerGround(sead::Vector3f* result, const IUsePlayerCollision* collision, const sead::Vector3f& vec) {
    *result = {0.0f, 0.0f, 0.0f};
}
void calcMovePowerWall(sead::Vector3f* result, const IUsePlayerCollision* collision, const sead::Vector3f& vec) {
    *result = {0.0f, 0.0f, 0.0f};
}

bool isCollidedWall(const IUsePlayerCollision* collision) {
    return collision->getPlayerCollider()->val2 >= 0.0f;
}

void slerpUpFront(al::LiveActor* actor, const sead::Vector3f& up, const sead::Vector3f& front, float val1, float val2) {
  const sead::Vector3f *Gravity; // x0
  float y; // s1
  float z; // s2
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
  Gravity = &al::getGravity(actor);
  y = Gravity->y;
  z = Gravity->z;
  v19.x = -Gravity->x;
  v19.y = -y;
  v19.z = -z;
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
  al::alongVectorNormalH(&a1, &front, &a3, &a4);
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
  float v19; // s0
  float v20; // s8
  float v21; // s0
  float v22; // s0
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
          v19 = a4 / v18;
          a1.x = v19 * a1.x;
          a1.y = v19 * a1.y;
          a1.z = v19 * a1.z;
        }

        v20 = (float)((float)(x * v24.x) + (float)(y * v24.y)) + (float)(z * v24.z);
        if ( v24.dot(a3) > (float)-v20 )
        {
          v21 = a1.length();
          if ( v21 > 0.0 )
          {
            v22 = v20 / v21;
            a1.x = v22 * a1.x;
            a1.y = v22 * a1.y;
            a1.z = v22 * a1.z;
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

}  // namespace rs
