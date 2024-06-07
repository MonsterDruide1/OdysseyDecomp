#include "Library/Base/StringUtil.h"
#include "Library/Collision/CollisionDirector.h"
#include "Library/Collision/CollisionParts.h"
#include "Library/Collision/CollisionPartsKeeper.h"
#include "Library/Collision/CollisionUtil.h"
#include "Library/Collision/IUseCollision.h"
#include "Library/Collision/KTriangle.h"
#include "Library/Controller/JoyPadAccelPoseAnalyzer.h"
#include "Library/Controller/SpinInputAnalyzer.h"
#include "Library/Execute/ExecuteTableHolderUpdate.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Math/MathAngleUtil.h"
#include "Library/Math/MathLengthUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Math/SegmentUtil.h"
#include "Library/Math/VectorUtil.h"
#include "Library/Matrix/MatrixUtil.h"
#include "Library/Nerve/NerveKeeper.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Yaml/ByamlIter.h"
#include "Library/Yaml/ByamlUtil.h"
#include "Library/stuff.h"
#include "basis/seadTypes.h"
#include "math/seadBoundBox.h"
#include "math/seadMatrix.h"
#include "math/seadQuatCalcCommon.h"

#include <cstdio>

namespace al {

void makeMtxRotateTrans(sead::Matrix34f* out, const sead::Vector3f& rotate, const sead::Vector3f& trans) {
    out->makeRT(sead::Vector3f(sead::Mathf::deg2rad(rotate.x), sead::Mathf::deg2rad(rotate.y), sead::Mathf::deg2rad(rotate.z)), trans);
}

void makeMtxFrontUpPos(sead::Matrix34f* out, const sead::Vector3f& front, const sead::Vector3f& up, const sead::Vector3f& pos) {
    sead::Vector3f frontNorm = front;
    al::normalize(&frontNorm);

    sead::Vector3f v18;
    v18.setCross(up, frontNorm);
    al::normalize(&v18);

    sead::Vector3f unk;
    unk.setCross(frontNorm, v18);

    out->m[0][0] = v18.x;
    out->m[0][1] = unk.x;
    out->m[1][0] = v18.y;
    out->m[1][1] = unk.y;
    out->m[0][2] = frontNorm.x;
    out->m[1][2] = frontNorm.y;
    out->m[2][0] = v18.z;
    out->m[2][1] = unk.z;
    out->m[2][2] = frontNorm.z;

    out->setTranslation(pos);
}

void makeMtxUpFrontPos(sead::Matrix34f* out, const sead::Vector3f& up, const sead::Vector3f& front, const sead::Vector3f& pos) {
    makeMtxFrontUpPos(out, front, up, pos);
}

void normalize(sead::Vector3f* vec) {
    sead::Vector3f tmp = *vec;
    al::normalize(vec, tmp);
}

void normalize(sead::Vector3f* out, const sead::Vector3f& vec) {
    *out = vec;
    out->normalize();
}

void calcQuatUp(sead::Vector3f* out, const sead::Quatf& quat) {
    out->x = 2.0f * (quat.x * quat.y - quat.w * quat.z);
    out->y = (1 - (quat.x * quat.x)) - 2.0f * (quat.z * quat.z);
    out->z = 2.0f * (quat.w * quat.x + quat.y * quat.z);
}

void calcQuatFront(sead::Vector3f* out, const sead::Quatf& quat) {
    out->x = 2.0f * (quat.x * quat.z + quat.w * quat.y);
    out->y = 2.0f * (quat.y * quat.z - quat.w * quat.x);
    out->z = (1 - (quat.x * quat.x)) - 2.0f * (quat.y * quat.y);
}

f32 calcRate01(f32 a2, f32 a3, f32 a4) {
    float v3; // s2
    float v4; // s3
    float result; // s0

    v3 = a4 - a3;
    v4 = -v3;
    if ( v3 > 0.0 )
        v4 = v3;

    if ( v4 < 0.001 )
        return 1.0;

    result = (float)(a2 - a3) / v3;
    if ( result < 0.0 )
        return 0.0;

    if ( result > 1.0 )
        return 1.0;

    return result;
}

void slerpQuat(sead::Quatf* out, const sead::Quatf& a, const sead::Quatf& b, float val) {
    sead::QuatCalcCommon<f32>::slerpTo(*out, a, b, val);
}

f32 calcCylinderRadiusDot(const sead::Vector3f& a1, const sead::Vector3f& a2, f32 a3) {
    float v6; // s0
    float v7; // s1
    float v8; // s0
    float v9; // s0

    v6 = a2.dot(a1);
    v7 = -v6;
    if ( v6 > 0.0 )
        v7 = a2.dot(a1);

    v8 = -1.0;
    if ( v7 >= -1.0 )
    {
        v8 = v7;
        if ( v7 > 1.0 )
        v8 = 1.0;
    }

    v9 = sead::Mathf::cos(v8);
    return sead::Mathf::sin(v9) * a3;
}

bool isParallelDirection(const sead::Vector3f& a1, const sead::Vector3f& a2, f32 a3) {
    float v7; // s3
    float v9; // s4
    float v10; // s1

    v7 = (float)(a1.y * a2.z) - (float)(a1.z * a2.y);
    if ( v7 <= 0.0 )
        v7 = -v7;

    if ( v7 > a3 )
        return 0LL;

    v9 = (float)(a1.z * a2.x) - (float)(a2.z * a1.x);
    if ( v9 <= 0.0 )
        v9 = -v9;

    if ( v9 > a3 )
        return 0LL;

    v10 = (float)(a2.y * a1.x) - (float)(a1.y * a2.x);
    if ( v10 <= 0.0 )
        v10 = -v10;

    return v10 <= a3;
}

bool isNearZero(f32 val, f32 epsilon) {
    return sead::Mathf::abs(val) < epsilon;
}

bool isNearZero(const sead::Vector3f& vec, f32 epsilon) {
    return vec.squaredLength() < epsilon*epsilon;
}

bool tryNormalizeOrZero(sead::Vector3f* vec) {
    f32 squaredLength = vec->squaredLength();
    if (squaredLength < 0.000001f) {
        *vec = sead::Vector3f::zero;
        return false;
    }

    vec->normalize();
    return true;
}

void preScaleMtx(sead::Matrix34f * mtx, const sead::Vector3f & vec) {
    mtx->scaleBases(vec.x, vec.y, vec.z);
}

void calcMtxScale(sead::Vector3f* scale, const sead::Matrix34f& mtx) {
    sead::Vector3f x,y,z;
    mtx.getBase(x, 0);
    mtx.getBase(y, 1);
    mtx.getBase(z, 2);

    scale->x = x.length();
    scale->y = y.length();
    scale->z = z.length();
}

void verticalizeVec(sead::Vector3f *out, const sead::Vector3f &vertical, const sead::Vector3f &vec) {
    *out = vec - (vertical * vec.dot(vertical));
}

bool tryNormalizeOrZero(sead::Vector3f* out, const sead::Vector3f& vec) {
    f32 squaredLength = vec.squaredLength();
    if (squaredLength < 0.000001f) {
        *out = sead::Vector3f::zero;
        return false;
    }

    *out = vec;
    out->normalize();
    return true;
}

void separateVectorParallelVertical(sead::Vector3f* p1, sead::Vector3f* p2, const sead::Vector3f& x1, const sead::Vector3f& x2) {
    *p1 = x1 * x2.dot(x1);
    *p2 = x2 - *p1;
}

const char* getCollisionCodeName(const al::Triangle& tri, const char* name) {
    ByamlIter attrs;
    tri.getAttributes(&attrs);
    if(!attrs.isValid())
        return nullptr;

    if(isTypeStringByKey(attrs, name)) {
        const char* result;
        if(attrs.tryGetStringByKey(&result, name))
            return result;
    }

    ByamlIter subIter;
    const char* result;
    if(attrs.tryGetIterByKey(&subIter, name) && subIter.tryGetStringByIndex(&result, 0)) {
        return result;
    }
    return nullptr;
}

bool isFloorCode(const al::HitInfo* a1, const char* a2) {
    return al::isEqualString(a2, getCollisionCodeName(a1->mTriangle, "FloorCode"));
}

void calcArrowAabb(sead::BoundBox3<float>* box, sead::Vector3<float> const& val1, sead::Vector3<float> const& val2) {
    box->setUndef();
    box->set(val1.x, val1.y, val1.z, val2.x, val2.y, val2.z);
}

bool isNearCollideSphereAabb(const sead::Vector3f & vec, f32 dist, const sead::BoundBox3f & box) {
    return vec.x >= box.getMin().x - dist && vec.x <= box.getMax().x + dist &&
           vec.y >= box.getMin().y - dist && vec.y <= box.getMax().y + dist &&
           vec.z >= box.getMin().z - dist && vec.z <= box.getMax().z + dist;
}

bool isNearZeroOrGreater(f32 value, f32 tolerance) {
    if(value >= 0.0f) return true;
    return sead::Mathf::abs(value) < tolerance;

}

void registerExecutorUser(IUseExecutor *, ExecuteDirector *, const char *) {
    WARN_UNIMPL;
}
CollisionPartsKeeperPtrArray::CollisionPartsKeeperPtrArray() {
    WARN_UNIMPL;
}
CollisionPartsKeeperOctree::CollisionPartsKeeperOctree(s32, s32, f32) {
    WARN_UNIMPL;
}

bool checkHitSegmentSphere(sead::Vector3f const& a1,sead::Vector3f const& a2,sead::Vector3f const& a3,float a4,sead::Vector3f* a5,sead::Vector3f* a6) {
  float z; // s21
  float x; // s0
  float y; // s2
  float v10; // s3
  float v11; // s19
  float v12; // s10
  float v13; // s4
  float v14; // s5
  float v15; // s16
  float v16; // s7
  float v17; // s11
  float v18; // s6
  float v19; // s9
  float v20; // s17
  float v21; // s18
  float v22; // s1
  float v23; // s0
  float v24; // w8
  float v25; // w9
  float v26; // w10
  float v27; // s19
  float v28; // s11
  float v29; // s10
  float v30; // s9
  float v31; // s0
  float v33; // s0
  float v34; // s0
  float v35; // s1
  float v36; // s11
  float v37; // s10
  float v38; // s9
  float v39; // s0
  float v40; // s1
  float v41; // s1
  float v42; // s0
  float v43; // s1
  float v44; // s1

  z = a2.z;
  x = a1.x;
  y = a1.y;
  v10 = a1.z;
  v11 = a2.y;
  v12 = y - v11;
  v13 = a3.x;
  v14 = a3.y;
  v15 = a3.x - a2.x;
  v16 = v14 - v11;
  v17 = a1.x - a2.x;
  v18 = a3.z;
  v19 = v10 - z;
  v20 = v18 - z;
  v21 = (float)((float)(v17 * v15) + (float)((float)(y - v11) * (float)(v14 - v11)))
      + (float)((float)(v10 - z) * (float)(v18 - z));
  v22 = a4 * a4;
  if ( v21 < 0.0 )
  {
    if ( (float)((float)((float)((float)(a2.x - x) * (float)(a2.x - x)) + (float)((float)(v11 - y) * (float)(v11 - y)))
               + (float)((float)(z - v10) * (float)(z - v10))) >= v22 ) {
      return 0LL;
    }

    v23 = (float)((float)(v17 * v17) + (float)(v12 * v12)) + (float)(v19 * v19);
    if ( v23 >= 0.000001 )
    {
      v35 = sqrtf(v23);
      if ( v35 <= 0.0 )
      {
        v24 = v10 - z;
        v25 = y - v11;
        v26 = a1.x - a2.x;
      }
      else
      {
        v26 = v17 * (float)(1.0 / v35);
        v25 = v12 * (float)(1.0 / v35);
        v24 = v19 * (float)(1.0 / v35);
      }
    }
    else
    {
      v24 = 0.0;
      v25 = 0.0;
      v26 = 0.0;
    }

    if ( a5 )
    {
      a5->x = v26;
      a5->y = v25;
      a5->z = v24;
    }

    if ( !a6 )
      return 1LL;

    goto LABEL_48;
  }

  v27 = (float)((float)(v15 * v15) + (float)(v16 * v16)) + (float)(v20 * v20);
  if ( v27 < v21 )
  {
    if ( (float)((float)((float)((float)(v13 - x) * (float)(v13 - x)) + (float)((float)(v14 - y) * (float)(v14 - y)))
               + (float)((float)(v18 - v10) * (float)(v18 - v10))) >= v22 ){
      return 0LL;
    }

    v28 = x - v13;
    v29 = y - v14;
    v30 = v10 - v18;
    v31 = (float)((float)(v28 * v28) + (float)(v29 * v29)) + (float)(v30 * v30);
    if ( v31 >= 0.000001 )
    {
      v40 = sqrtf(v31);
      if ( v40 <= 0.0 )
      {
        v24 = v10 - v18;
        v25 = y - v14;
        v26 = v28;
      }
      else
      {
        v26 = v28 * (float)(1.0 / v40);
        v25 = v29 * (float)(1.0 / v40);
        v24 = v30 * (float)(1.0 / v40);
      }
    }
    else
    {
      v24 = 0.0;
      v25 = 0.0;
      v26 = 0.0;
    }

    if ( a5 )
    {
      a5->x = v26;
      a5->y = v25;
      a5->z = v24;
    }

    if ( !a6 )
      return 1LL;

    goto LABEL_48;
  }

  v33 = -v27;
  if ( v27 > 0.0 )
    v33 = (float)((float)(v15 * v15) + (float)(v16 * v16)) + (float)(v20 * v20);

  if ( v33 < 0.001 )
  {
    v34 = (float)((float)(v17 * v17) + (float)(v12 * v12)) + (float)(v19 * v19);
    if ( v34 > v22 ){
      return 0LL;
    }

    if ( v34 >= 0.000001 )
    {
      v41 = sqrtf(v34);
      if ( v41 > 0.0 )
      {
        v17 = v17 * (float)(1.0 / v41);
        v12 = v12 * (float)(1.0 / v41);
        v19 = v19 * (float)(1.0 / v41);
      }
    }
    else
    {
      v19 = 0.0;
      v12 = 0.0;
      v17 = 0.0;
    }

    if ( a5 )
    {
      a5->x = v17;
      a5->y = v12;
      a5->z = v19;
    }

    if ( !a6 )
      return 1LL;

    a6->x = a1.x - (float)(v17 * a4);
    a6->y = a1.y - (float)(v12 * a4);
    v42 = v19 * a4;
    goto LABEL_49;
  }

  v36 = (float)(v15 * (float)(v21 / v27)) - v17;
  v37 = (float)(v16 * (float)(v21 / v27)) - v12;
  v38 = (float)(v20 * (float)(v21 / v27)) - v19;
  v39 = (float)(v38 * v38) + (float)((float)(v36 * v36) + (float)(v37 * v37));
  if ( v39 > v22 )
    return 0LL;

  if ( v39 >= 0.000001 )
  {
    v43 = sqrtf(v39);
    if ( v43 <= 0.0 )
    {
      v24 = -v38;
      v25 = -v37;
      v26 = -v36;
    }
    else
    {
      v44 = 1.0 / v43;
      v26 = v44 * (float)-v36;
      v25 = v44 * (float)-v37;
      v24 = v44 * (float)-v38;
    }
  }
  else
  {
    v24 = 0.0;
    v25 = 0.0;
    v26 = 0.0;
  }

  if ( a5 )
  {
    a5->x = v26;
    a5->y = v25;
    a5->z = v24;
  }

  if ( a6 )
  {
LABEL_48:
    a6->x = a1.x - (float)(v26 * a4);
    a6->y = a1.y - (float)(v25 * a4);
    v42 = v24 * a4;

LABEL_49:
    a6->z = a1.z - v42;
  }

  return 1LL;
}

void updatePoseMtx(LiveActor *actor, const sead::Matrix34f * mtx) {
    actor->mPoseKeeper->updatePoseMtx(mtx);
}
void updatePoseTrans(LiveActor *actor, const sead::Vector3f & trans) {
    actor->mPoseKeeper->updatePoseTrans(trans);
}
void updatePoseQuat(LiveActor *actor, const sead::Quatf &quat) {
    actor->mPoseKeeper->updatePoseQuat(quat);
}
void updatePoseRotate(LiveActor *actor, const sead::Vector3f &rot) {
    actor->mPoseKeeper->updatePoseRotate(rot);
}
const sead::Vector3f& getTrans(const LiveActor *actor) {
    return actor->mPoseKeeper->getTrans();
}
const sead::Vector3f& getGravity(const LiveActor *actor) {
    return actor->mPoseKeeper->getGravity();
}

sead::Vector3f* getTransPtr(LiveActor *actor) {
    return actor->mPoseKeeper->getTransPtr();
}

void setVelocity(LiveActor *actor, const sead::Vector3f &vel) {
    *actor->mPoseKeeper->getVelocityPtr() = vel;
}

void setVelocityZero(LiveActor *actor) {
    *actor->mPoseKeeper->getVelocityPtr() = {0.0f, 0.0f, 0.0f};
}

void calcFrontDir(sead::Vector3f * front, const LiveActor *actor) {
    sead::Matrix34f mtx;
    actor->mPoseKeeper->calcBaseMtx(&mtx);
    mtx.getBase(*front, 2);
}
void calcUpDir(sead::Vector3f * up, const LiveActor *actor) {
    sead::Matrix34f mtx;
    actor->mPoseKeeper->calcBaseMtx(&mtx);
    mtx.getBase(*up, 1);
}

bool isNearZero(const sead::Vector2f& vec, float epsilon) {
  return vec.squaredLength() < epsilon * epsilon;
}
void limitVelocityDir(LiveActor *actor, const sead::Vector3f & dir, f32 mag) {
  const sead::Vector3f *v6; // x0
  float x; // s1
  float y; // s2
  float z; // s3
  float v10; // s0
  float v11; // s1
  float v12; // s0
  float v13; // s8
  float v14; // s9
  float v15; // s10
  sead::Vector3f *result; // x0
  sead::Vector3f v17; // [xsp+0h] [xbp-50h] BYREF
  sead::Vector3f v18; // [xsp+10h] [xbp-40h] BYREF

  v18.x = 0.0;
  v18.y = 0.0;
  v18.z = 0.0;
  v17.x = 0.0;
  v17.y = 0.0;
  v17.z = 0.0;
  v6 = &actor->mPoseKeeper->getVelocity();
  al::separateVectorParallelVertical(&v18, &v17, dir, *v6);
  y = v18.y;
  x = v18.x;
  z = v18.z;
  v10 = (float)((float)(v18.x * v18.x) + (float)(v18.y * v18.y)) + (float)(z * z);
  if ( v10 > (float)(mag * mag) )
  {
    v11 = sqrtf(v10);
    if ( v11 <= 0.0 )
    {
      y = v18.y;
      x = v18.x;
      z = v18.z;
    }
    else
    {
      v12 = mag / v11;
      x = (float)(mag / v11) * v18.x;
      y = v12 * v18.y;
      z = v12 * v18.z;
      v18.x = x;
      v18.y = v12 * v18.y;
      v18.z = v12 * v18.z;
    }
  }

  v13 = x + v17.x;
  v14 = y + v17.y;
  v15 = z + v17.z;
  result = actor->mPoseKeeper->getVelocityPtr();
  result->x = v13;
  result->y = v14;
  result->z = v15;
}
void addVelocityToGravity(LiveActor *actor, f32 velocity) {
  *actor->getPoseKeeper()->getVelocityPtr() += actor->getPoseKeeper()->getGravity() * velocity;
}
void separateVelocityHV(sead::Vector3f *, sead::Vector3f *, const LiveActor *actor) {
  CRASH
}
void alongVectorNormalH(sead::Vector3f *a1, const sead::Vector3f& a2, const sead::Vector3f& a3, const sead::Vector3f& a4){
    float a2z; // s9
  float a3z; // s10
  float a4z; // s1
  float a4y; // s3
  float a3x; // s14
  float a3y; // s15
  float a4x; // s2
  float a2x; // s8
  float a2y; // s13
  float x1x; // s5
  float x1y; // s6
  float x1z; // s12
  float v17; // s0
  float v18; // s0
  float v19; // s0
  float v20; // s11
  float v21; // s1
  float x2x; // s0
  float x2y; // s2
  float x2z; // s3
  float v25; // s1
  float x1x_; // s10
  float x1y_; // s11
  float x3x; // s14
  float x3y; // s15
  float x3z; // s13
  float a3y_; // s1
  float a4z_; // s6
  float a4y_; // s3
  float a3z_; // s5
  float v35; // s8
  float v36; // s7
  float v37; // s5
  float v38; // s0
  float v39; // s8
  float cos; // s9
  float sin; // s0
  float x5x; // s1
  float x5y; // s2
  float x5z; // s0
  float x6x; // s3
  float x6y; // s5
  float x6z; // s4
  float v48; // s6
  float v49; // s7
  float v50; // s16

  a2z = a2.z;
  a3z = a3.z;
  a4y = a4.y;
  a4z = a4.z;
  a3x = a3.x;
  a3y = a3.y;
  a4x = a4.x;
  a2x = a2.x;
  a2y = a2.y;
  x1x = (float)(a3y * a4z) - (float)(a3z * a4y);
  x1y = (float)(a3z * a4.x) - (float)(a3.x * a4z);
  x1z = (float)(a3.x * a4y) - (float)(a3y * a4.x);
  v17 = (float)(x1z * x1z) + (float)((float)(x1x * x1x) + (float)(x1y * x1y));
  if ( v17 >= 0.000001 )
  {
    v20 = (float)((float)(a3x * a2x) + (float)(a3y * a2y)) + (float)(a3z * a2z);
    v21 = sqrtf(v17);
    x2x = a3x * v20;
    x2y = a3y * v20;
    x2z = a3z * v20;
    if ( v21 <= 0.0 )
    {
      x1y_ = x1y;
      x1x_ = x1x;
    }
    else
    {
      v25 = 1.0 / v21;
      x1z = x1z * v25;
      x1x_ = x1x * v25;
      x1y_ = x1y * v25;
    }

    x3x = a2x - x2x;
    x3y = a2y - x2y;
    x3z = a2z - x2z;
    a3y_ = a3.y;
    a4z_ = a4.z;
    a4y_ = a4.y;
    a3z_ = a3.z;
    v35 = (float)((float)(a3.x * a4.x) + (float)(a3y_ * a4y_)) + (float)(a3z_ * a4z_);
    v36 = a4y_ * a3z_;
    v37 = (float)(a4.x * a3z_) - (float)(a3.x * a4z_);
    v38 = (float)(a3.x * a4y_) - (float)(a4.x * a3y_);
    v39 = atan2f(
            sqrtf(
              (float)(v38 * v38)
            + (float)((float)((float)((float)(a3y_ * a4z_) - v36) * (float)((float)(a3y_ * a4z_) - v36))
                    + (float)(v37 * v37))),
            v35)
        * 0.5;
    cos = cosf(v39);
    sin = sinf(v39);
    x5x = x1x_ * sin;
    x5y = x1y_ * sin;
    x5z = x1z * sin;
    x6x = (float)(x3x * cos) + (float)((float)(x3z * x5y) - (float)(x3y * x5z));
    x6y = (float)(x3y * cos) + (float)((float)(x3x * x5z) - (float)(x3z * x5x));
    x6z = (float)(x3z * cos) + (float)((float)(x3y * x5x) - (float)(x3x * x5y));
    v48 = (float)((float)-(float)(x3x * x5x) - (float)(x3y * x5y)) - (float)(x3z * x5z);
    v49 = (float)(cos * x6x) - (float)(x5z * x6y);
    v50 = (float)(x5z * x6x) + (float)(cos * x6y);
    v19 = (float)((float)(cos * x6z) + (float)((float)(x5x * x6y) - (float)(x5y * x6x))) - (float)(x5z * v48);
    a1->x = (float)((float)(x5y * x6z) + v49) - (float)(x5x * v48);
    a1->y = (float)(v50 - (float)(x5x * x6z)) - (float)(x5y * v48);
  }
  else
  {
    v18 = (float)((float)(a4x * a2x) + (float)(a4y * a2y)) + (float)(a4z * a2z);
    a1->x = a2x - (float)(a4x * v18);
    a1->y = a2.y - (float)(a4.y * v18);
    v19 = a2.z - (float)(a4.z * v18);
  }

  a1->z = v19;
}

bool isEqualString(sead::SafeStringBase<char> const& a1, sead::SafeStringBase<char> const& a2) {
  return al::isEqualString(a1.cstr(), a2.cstr());
}

s32 converge(s32 val, s32 goal, s32 step) {
  if(val >= goal) {
    return sead::Mathi::clampMin(val - step, goal);
  } else {
    return sead::Mathi::clampMax(val + step, goal);
  }
}

f32 converge(f32 val, f32 goal, f32 step) {
  if(val >= goal) {
    return sead::Mathf::clampMin(val - step, goal);
  } else {
    return sead::Mathf::clampMax(val + step, goal);
  }
}

bool sub_7100926C0C(
        sead::Vector3f *a1,
        float *a2,
        const sead::Vector3f *a3,
        const sead::Vector3f *a4)
{
  float x; // s0
  float y; // s1
  float v8; // s2
  float v9; // s3
  float z; // s4
  float v11; // s5
  float v12; // s6
  float v13; // s8
  float v14; // s7
  float v15; // s7
  float v16; // s7
  bool result; // x0
  float v18; // s9
  float v19; // s10
  float v20; // s0
  float v21; // s1
  float v22; // s1
  float v23; // s0
  float v24; // s0
  float v25; // s1
  sead::Vector3f v26; // [xsp+0h] [xbp-50h] BYREF

  x = a3->x;
  y = a3->y;
  v8 = a4->x;
  v9 = a4->y;
  z = a3->z;
  v11 = a4->z;
  v12 = y * v11;
  if ( (float)((float)((float)(a3->x * a4->x) + (float)(y * v9)) + (float)(z * v11)) >= 0.0 )
  {
    v13 = v12 - (float)(z * v9);
  }
  else
  {
    v13 = v12 - (float)(v9 * z);
    if ( v13 <= 0.0 )
      v14 = -v13;
    else
      v14 = v12 - (float)(v9 * z);

    if ( v14 <= 0.01 )
    {
      v15 = (float)(v8 * z) - (float)(x * v11);
      if ( v15 <= 0.0 )
        v15 = -v15;

      if ( v15 <= 0.01 )
      {
        v16 = (float)(x * v9) - (float)(v8 * y);
        if ( v16 <= 0.0 )
          v16 = -v16;

        if ( v16 <= 0.01 )
        {
          al::calcDirVerticalAny(&v26, *a3);
          *a2 = 3.1416;
          *a1 = v26;
          return 1LL;
        }
      }
    }
  }

  v18 = (float)(z * v8) - (float)(v11 * x);
  v19 = (float)(v9 * x) - (float)(y * v8);
  v20 = (float)(v19 * v19) + (float)((float)(v13 * v13) + (float)(v18 * v18));
  if ( v20 < 0.000001 )
    goto LABEL_23;

  v21 = sqrtf(v20);
  if ( v21 > 0.0 )
  {
    v13 = v13 * (float)(1.0 / v21);
    v18 = v18 * (float)(1.0 / v21);
    v19 = v19 * (float)(1.0 / v21);
  }

  v22 = (float)((float)(a3->x * a4->x) + (float)(a3->y * a4->y)) + (float)(a3->z * a4->z);
  v23 = -1.0;
  if ( v22 >= -1.0 )
  {
    v23 = (float)((float)(a3->x * a4->x) + (float)(a3->y * a4->y)) + (float)(a3->z * a4->z);
    if ( v22 > 1.0 )
      v23 = 1.0;
  }

  v24 = acosf(v23);
  v25 = -v24;
  if ( v24 > 0.0 )
    v25 = v24;

  if ( v25 >= 0.001 )
  {
    result = 1LL;
    *a2 = v24;
    a1->x = v13;
    a1->y = v18;
    a1->z = v19;
  }
  else
  {
LABEL_23:
    *a2 = 0.0;
    a1->z = 0.0;
    result = 0LL;
    a1->x = 0.0;
    a1->y = 1.0;
  }

  return result;
}

bool makeQuatRotationLimit(sead::Quatf* a1, const sead::Vector3f& a2, const sead::Vector3f& a3, float a4) {
  float v6; // s1
  bool v7; // w20
  float v8; // s8
  float v9; // s9
  float v10; // s0
  float v11; // s1
  float v12; // s3
  float v13; // s0
  float v15; // [xsp+Ch] [xbp-34h] BYREF
  sead::Vector3f v16; // [xsp+10h] [xbp-30h] BYREF

  v15 = 0.0;
  if ( (sub_7100926C0C(&v16, &v15, &a2, &a3) & 1) != 0 )
  {
    v6 = a4 / v15;
    v7 = v15 > a4;
    if ( (float)(a4 / v15) >= 0.0 )
    {
      if ( v6 > 1.0 )
        v6 = 1.0;
    }
    else
    {
      v6 = 0.0;
    }

    v8 = (float)(v6 * v15) * 0.5;
    v9 = cosf(v8);
    v10 = sinf(v8);
    v11 = v10 * v16.x;
    v12 = v10 * v16.z;
    v13 = v10 * v16.y;
    a1->z = v12;
    a1->w = v9;
    a1->x = v11;
    a1->y = v13;
  }
  else
  {
    v7 = 0;
    a1->x = 0.0;
    a1->y = 0.0;
    a1->z = 0.0;
    a1->w = 1.0;
  }

  return v7;
}

void calcDirVerticalAny(sead::Vector3f* a1, const sead::Vector3f& a2) {
  float x; // s0
  float y; // s1
  float absX; // s3
  float absY; // s4
  float z; // s2
  float absZ; // s5
  int dominantDirection; // w8
  float *p_domZ; // x8
  float domY_; // s5
  float domZ_; // s4
  float domLen; // s1
  float resultX; // s0
  float resultY; // s2
  float resultZ; // s1
  float v17; // s1
  float result; // s0
  float v19; // s0
  float v20; // s2
  float domZ; // [xsp+Ch] [xbp-14h] BYREF
  float domY; // [xsp+18h] [xbp-8h] BYREF
  float domX; // [xsp+1Ch] [xbp-4h] BYREF

  domY = 0.0;
  domX = 0.0;
  domZ = 0.0;
  x = a2.x;
  y = a2.y;
  if ( a2.x <= 0.0 )
    absX = -a2.x;
  else
    absX = a2.x;

  if ( y <= 0.0 )
    absY = -y;
  else
    absY = a2.y;

  z = a2.z;
  absZ = -z;
  if ( z > 0.0 )
    absZ = a2.z;

  if ( absY >= absZ )
    dominantDirection = 2;
  else
    dominantDirection = 1;

  if ( absX < absZ && absX < absY )
    dominantDirection = 0;

  if ( dominantDirection )
  {
    if ( dominantDirection == 2 )
      p_domZ = &domZ;
    else
      p_domZ = &domY;
  }
  else
  {
    p_domZ = &domX;
  }

  *p_domZ = 1.0;
  domY_ = domY;
  domZ_ = domZ;
  domLen = (float)((float)(x * domX) + (float)(y * domY)) + (float)(z * domZ);
  resultX = domX - (float)(x * domLen);
  a1->x = resultX;
  resultY = domY_ - (float)(a2.y * domLen);
  a1->y = resultY;
  resultZ = domZ_ - (float)(a2.z * domLen);
  a1->z = resultZ;
  v17 = (float)(resultZ * resultZ) + (float)((float)(resultX * resultX) + (float)(resultY * resultY));
  result = 0.000001;
  if ( v17 >= 0.000001 )
  {
    result = sqrtf(v17);
    if ( result > 0.0 )
    {
      v19 = 1.0 / result;
      v20 = v19 * a1->y;
      a1->x = v19 * a1->x;
      a1->y = v20;
      result = v19 * a1->z;
      a1->z = result;
    }
  }
  else
  {
    a1->x = 0.0;
    a1->y = 0.0;
    a1->z = 0.0;
  }
}

f32 calcAngleDegree(const sead::Vector3f& a1, const sead::Vector3f& a2) {
  float y; // s1
  float z; // s6
  float v4; // s3
  float v5; // s5
  float v6; // s8
  float v7; // s7
  float v8; // s5
  float v9; // s0

  y = a1.y;
  z = a2.z;
  v4 = a2.y;
  v5 = a1.z;
  v6 = (float)((float)(a1.x * a2.x) + (float)(y * v4)) + (float)(v5 * z);
  v7 = v4 * v5;
  v8 = (float)(a2.x * v5) - (float)(a1.x * z);
  v9 = (float)(a1.x * v4) - (float)(a2.x * y);
  return atan2f(
           sqrtf(
             (float)(v9 * v9)
           + (float)((float)((float)((float)(y * z) - v7) * (float)((float)(y * z) - v7)) + (float)(v8 * v8))),
           v6)
       * 57.296;
}

void calcQuat(sead::Quatf* quat, const LiveActor* actor) {
  sead::Matrix34f mtx;
  actor->getPoseKeeper()->calcBaseMtx(&mtx);
  mtx.toQuat(*quat);
}
void makeQuatFrontUp(sead::Quatf* quat, const sead::Vector3f& front, const sead::Vector3f& up) {
  sead::Matrix34f mtx = sead::Matrix34f::ident;
  al::makeMtxFrontUp(&mtx, front, up);
  mtx.toQuat(*quat);
}
void makeQuatUpFront(sead::Quatf* quat, const sead::Vector3f& up, const sead::Vector3f& front) {
  sead::Matrix34f mtx = sead::Matrix34f::ident;
  al::makeMtxUpFront(&mtx, up, front);
  mtx.toQuat(*quat);
}

void makeMtxUpFront(sead::Matrix34f* mtx, const sead::Vector3f& up, const sead::Vector3f& front) {
  sead::Vector3f upn = up;
  al::normalize(&upn);

  sead::Vector3f v23;
  v23.setCross(upn, front);

  sead::Vector3f c;
  c.setCross(c, upn);

  mtx->setBase(0, v23);
  mtx->setBase(1, upn);
  mtx->setBase(2, c);
}
void makeMtxFrontUp(sead::Matrix34f* mtx, const sead::Vector3f& front, const sead::Vector3f& up) {
  makeMtxUpFront(mtx, up, front);
}

void separateVectorHV(sead::Vector3f* h, sead::Vector3f* v, const sead::Vector3f& vec, const sead::Vector3f& vertical) {
  f32 dot = vec.dot(vertical);
  *v = vertical * dot;
  *h = vec - *v;
}
void separateVelocityHV(sead::Vector3f* h, sead::Vector3f* v, al::LiveActor* actor) {
  separateVectorHV(h, v, actor->getPoseKeeper()->getVelocity(), al::getGravity(actor));
}

bool isFloorPolygonCos(const sead::Vector3f &a1, const sead::Vector3f& a2, float a3)
{
  float v7; // s0
  bool v8; // nf
  bool v9; // w8

  if ( al::isNearZero(a1, 0.001) )
    return 0LL;

  v7 = a1.dot(a2);
  if ( (float)-v7 < a3 )
    return 0LL;

  v8 = v7 < 0.0;
  if ( v7 <= 0.0 )
    v7 = -v7;

  v9 = v8;
  return v9 && (v7 >= 0.34202);
}

void parallelizeVec(sead::Vector3f* a1, const sead::Vector3f& a2, const sead::Vector3f& a3) {
  f32 dot = a2.dot(a3);
  *a1 = a2 * dot;
}

bool turnVecToVecDegree(sead::Vector3f* a1, const sead::Vector3f& a2, const sead::Vector3f& a3, f32 a4) {
  float v7; // s10
  float v8; // s11
  float v9; // s0
  float v10; // s0
  bool v11; // w20
  float v12; // s8
  float v13; // s2
  float v14; // s1
  float v15; // s9
  float v16; // s0
  float z; // s3
  float y; // s6
  float v19; // s4
  float v20; // s7
  float v21; // s16
  float v22; // s3
  float v23; // s5
  float v24; // s6
  float v25; // s0
  float v26; // s0
  float v27; // s0
  float v28; // s2
  sead::Vector3f v30; // [xsp+0h] [xbp-50h] BYREF
  float v31; // [xsp+3Ch] [xbp-14h] BYREF

  v31 = 0.0;
  if ( (sub_7100926C0C(&v30, &v31, &a2, &a3) & 1) != 0 )
  {
    v7 = a4 * 0.017453;
    v8 = v31;
    v9 = (float)(a4 * 0.017453) / v31;
    if ( v9 >= 0.0 )
    {
      if ( v9 > 1.0 )
        v9 = 1.0;
    }
    else
    {
      v9 = 0.0;
    }

    v15 = (float)(v31 * v9) * 0.5;
    v12 = cosf(v15);
    v16 = sinf(v15);
    v13 = v16 * v30.y;
    v14 = v16 * v30.x;
    v10 = v16 * v30.z;
    v11 = v8 <= v7;
  }
  else
  {
    v10 = 0.0;
    v11 = 1;
    v12 = 1.0;
    v13 = 0.0;
    v14 = 0.0;
  }

  y = a2.y;
  z = a2.z;
  v19 = (float)((float)(v13 * z) - (float)(v10 * y)) + (float)(v12 * a2.x);
  v20 = (float)(v12 * y) + (float)((float)(v10 * a2.x) - (float)(v14 * z));
  v21 = (float)(v12 * z) + (float)((float)(v14 * y) - (float)(v13 * a2.x));
  v22 = (float)((float)-(float)(v14 * a2.x) - (float)(v13 * y)) - (float)(v10 * z);
  v23 = (float)((float)(v13 * v21) + (float)((float)(v12 * v19) - (float)(v10 * v20))) - (float)(v14 * v22);
  v24 = (float)((float)((float)(v10 * v19) + (float)(v12 * v20)) - (float)(v14 * v21)) - (float)(v13 * v22);
  v25 = (float)((float)(v12 * v21) + (float)((float)(v14 * v20) - (float)(v13 * v19))) - (float)(v10 * v22);
  a1->z = v25;
  v26 = sqrtf((float)((float)(v23 * v23) + (float)(v24 * v24)) + (float)(v25 * v25));
  a1->x = v23;
  a1->y = v24;
  if ( v26 > 0.0 )
  {
    v27 = 1.0 / v26;
    v28 = v27 * a1->y;
    a1->x = v27 * a1->x;
    a1->y = v28;
    a1->z = v27 * a1->z;
  }

  return v11;
}

}

bool alCollisionUtil::isFarAway(al::CollisionParts const& a1, const sead::Vector3f& a2, float a3) {
    float v3; // s0
    float v4; // s1
    float v6; // s2
    float v7; // s3

    v3 = a1.mBoundingSphereRange + a3;
    v4 = a1.mBaseMtx.m[0][3] - a2.x;
    if ( v4 <= 0.0 )
        v4 = -v4;

    if ( v3 < v4 )
        return 1LL;

    v6 = a1.mBaseMtx.m[1][3] - a2.y;
    if ( v6 <= 0.0 )
        v6 = -v6;

    if ( v3 < v6 )
        return 1LL;

    v7 = a1.mBaseMtx.m[2][3] - a2.z;
    if ( v7 <= 0.0 )
        v7 = -v7;

    return v3 < v7 || (float)((float)((float)(v4 * v4) + (float)(v6 * v6)) + (float)(v7 * v7)) > (float)(v3 * v3);
}

bool alCollisionUtil::isCollisionMoving(const al::HitInfo* info) {
  return false;
  //return info->mTriangle.isHostMoved();
}
const sead::Vector3f& alCollisionUtil::getCollisionHitPos(const al::HitInfo* info) {
  return info->mCollisionHitPos;
}
const sead::Vector3f& alCollisionUtil::getCollisionHitNormal(const al::HitInfo* info) {
  return info->mTriangle.getNormal(0);
}

bool al::isNearDirection(const sead::Vector3f& a1, const sead::Vector3f& a2, float a3)
{
  float x; // s1
  float y; // s2
  float v5; // s3
  float v6; // s4
  float z; // s5
  float v8; // s6
  float v10; // s7
  float v11; // s5
  float v12; // s1

  x = a1.x;
  y = a1.y;
  v5 = a2.x;
  v6 = a2.y;
  z = a1.z;
  v8 = a2.z;
  if ( (float)((float)((float)(a1.x * a2.x) + (float)(y * v6)) + (float)(z * v8)) < 0.0 )
    return 0LL;

  v10 = (float)(y * v8) - (float)(v6 * z);
  if ( v10 <= 0.0 )
    v10 = -v10;

  if ( v10 > a3 )
    return 0LL;

  v11 = (float)(v5 * z) - (float)(x * v8);
  if ( v11 <= 0.0 )
    v11 = -v11;

  if ( v11 > a3 )
    return 0LL;

  v12 = (float)(x * v6) - (float)(v5 * y);
  if ( v12 <= 0.0 )
    v12 = -v12;

  return v12 <= a3;
}

void al::SphereHitInfo::calcFixVector(sead::Vector3f* a1, sead::Vector3f* a2) const {
  if ( mCollisionLocation == 1 )
  {
    *a1 = unk * mTriangle.mFaceNormal;
    if ( a2 )
      *a2 = mTriangle.mFaceNormal;
  }
  else
  {
    sead::Vector3f v20 = unk3 - mCollisionHitPos;
    al::tryNormalizeOrZero(&v20);
    *a1 = v20 * (unk * mTriangle.mFaceNormal).dot(v20);
    *a2 = v20 * mTriangle.mFaceNormal.dot(v20);
  }
}

bool al::separateScalarAndDirection(f32* a1, sead::Vector3f* a2, const sead::Vector3f& a3) {
  float x; // s1
  bool result; // w0
  float y; // w8
  float v6; // s0
  float v7; // s0
  float v8; // s2

  *a1 = sqrtf((float)((float)(a3.x * a3.x) + (float)(a3.y * a3.y)) + (float)(a3.z * a3.z));
  x = a3.x;
  if ( (float)((float)((float)(x * x) + (float)(a3.y * a3.y)) + (float)(a3.z * a3.z)) >= 0.000001 )
  {
    a2->x = x;
    y = a3.y;
    a2->y = y;
    v6 = sqrtf((float)((float)(x * x) + (float)(y * y)) + (float)(a3.z * a3.z));
    a2->z = a3.z;
    result = 0;
    if ( v6 > 0.0 )
    {
      v7 = 1.0 / v6;
      v8 = v7 * a2->y;
      a2->x = v7 * a2->x;
      a2->y = v8;
      a2->z = v7 * a2->z;
    }
  }
  else
  {
    a2->x = 0.0;
    a2->y = 0.0;
    result = 1;
    a2->z = 0.0;
  }

  return result;
}

void al::SphereHitInfo::calcFixVectorNormal(sead::Vector3f* a1, sead::Vector3f* a2) const {
  *a1 = unk * mTriangle.mFaceNormal;
  if ( a2 )
    *a2 = mTriangle.mFaceNormal;
}

bool al::isReverseDirection(const sead::Vector3f& a1, const sead::Vector3f& a2, f32 a3) {
  if(a1.dot(a2) >= 0.0f)
    return false;

  if(sead::Mathf::abs((a1.y * a2.z) - (a1.z * a2.y)) > a3)
    return false;
  if(sead::Mathf::abs((a1.z * a2.x) - (a1.x * a2.z)) > a3)
    return false;
  if(sead::Mathf::abs((a1.x * a2.y) - (a1.y * a2.x)) > a3)
    return false;
  return true;
}

void al::initNerve(LiveActor *actor, const Nerve *nerve, s32 maxNumNerves) {
  al::NerveKeeper* keeper = new al::NerveKeeper(actor, nerve, maxNumNerves);
  actor->initNerveKeeper(keeper);
}

const sead::Vector3f& alCollisionUtil::getCollisionMovingReaction(const al::HitInfo* info) {
  return info->mCollisionMovingReaction;
}

bool alCollisionUtil::getHitPosAndNormalOnArrow(const al::IUseCollision* a1, sead::Vector3f* a2, sead::Vector3<float> * a3, const sead::Vector3<float> & a4, const sead::Vector3<float> & a5, const al::CollisionPartsFilterBase *a6, const al::TriangleFilterBase *a7) {
  const al::ArrowHitInfo *v14; // x8
  al::Triangle mTriangle = {}; // [xsp+0h] [xbp-B0h] BYREF
  const al::ArrowHitInfo *v19; // [xsp+78h] [xbp-38h] BYREF

  v19 = 0LL;
  if ( (alCollisionUtil::getFirstPolyOnArrow(a1, &v19, a4, a5, a6, a7) & 1) == 0 )
    return 0;

  v14 = v19;
  if ( a2 )
    *a2 = v19->mCollisionHitPos;

  mTriangle = v14->mTriangle;
  *a3 = mTriangle.getNormal(0);
  return true;
}

bool alCollisionUtil::getFirstPolyOnArrow(al::IUseCollision const* a1, al::ArrowHitInfo const** a2, sead::Vector3<float> const& a3, sead::Vector3<float> const& a4, al::CollisionPartsFilterBase const*a5, al::TriangleFilterBase const*a6) {
  al::CollisionDirector *v12; // x25
  int v13; // w22
  float v14; // s8
  int v15; // w24
  int v16; // w21
  al::CollisionDirector *v17; // x0
  float *StrikeArrowInfo; // x25
  float v19; // s8
  al::CollisionDirector *v20; // x0
  float *v21; // x0
  float v22; // s0
  bool v23; // nf
  al::CollisionDirector *v24; // x0

  v12 = (al::CollisionDirector *)a1->getCollisionDirector();
  v12->setPartsFilter(a5);
  v12->setTriFilter(a6);
  v13 = v12->checkStrikeArrow(a3, a4);
  if ( !v13 )
    return 0LL;

  if ( a6 )
  {
    CRASH
    /*v14 = 1000000.0;
    v15 = 0;
    v16 = -1;
    do
    {
      v17 = (al::CollisionDirector *)a1->getCollisionDirector();
      StrikeArrowInfo = (float *)v17->getStrikeArrowInfo(v15);
      if ( ((**(__int64 (__fastcall ***)(const al::TriangleFilterBase *, void *))a6)(a6, StrikeArrowInfo) & 1) == 0
        && StrikeArrowInfo[28] < v14 )
      {
        v14 = StrikeArrowInfo[28];
        v16 = v15;
      }

      ++v15;
    }
    while ( v13 != v15 );*/
  }
  else
  {
    v19 = 1000000.0;
    v16 = -1;
    int i=0;
    do
    {
      v20 = (al::CollisionDirector *)a1->getCollisionDirector();
      v21 = (float *)v20->getStrikeArrowInfo(i);
      v22 = v21[28];
      v23 = v22 < v19;
      if ( v22 < v19 )
        v19 = v21[28];

      if ( v23 )
        v16 = i;

      i++;
    }
    while ( v13 != i );
  }

  if ( v16 == -1 )
    return 0LL;

  if ( a2 )
  {
    v24 = (al::CollisionDirector *)a1->getCollisionDirector();
    *a2 = (const al::ArrowHitInfo *)v24->getStrikeArrowInfo(v16);
  }

  return 1LL;
}

void alKCollisionFunc::calcSphereHitPos(sead::Vector3<float> * a1, const al::KCollisionServer * a2, const sead::Vector3<float> & a3, const al::KCPrismData & a4, const al::KCPrismHeader * a5, u8 a6) {
  float v14; // s0
  float x; // s9
  float y; // s10
  float z; // s8
  int v18; // w1
  const al::KCollisionServer *v19; // x0
  sead::Vector3f *v20; // x1
  const al::KCPrismData *v21; // x2
  int v22; // w3
  float v24; // s0
  float v26; // s2
  float v27; // s1
  sead::Vector3f v28; // [xsp+0h] [xbp-70h] BYREF
  sead::Vector3f v29; // [xsp+10h] [xbp-60h] BYREF
  sead::Vector3f v6_12;
  sead::Vector3f Normal;
  
  if ( a6 )
  {
    switch ( a6 )
    {
      case 1u:
      case 2u:
      case 3u:
      case 4u:
        a2->calcPosLocal(&v29, &a4, 0, a5);
        Normal = a2->getNormal(a4.mFaceNormalIndex, a5);
        v14 = (float)((float)((float)(a3.x - v29.x) * Normal.x) + (float)((float)(a3.y - v29.y) * Normal.y))
            + (float)((float)(a3.z - v29.z) * Normal.z);
        a1->x = a3.x - (float)(Normal.x * v14);
        a1->y = a3.y - (float)(Normal.y * v14);
        a1->z = a3.z - (float)(Normal.z * v14);
        if ( a6 == 1 )
          return;

        x = a3.x;
        y = a3.y;
        z = a3.z;
        if ( a6 == 4 )
        {
          v6_12 = a2->getNormal(a4.mEdgeNormalIndex[2], a5);
          a2->calcPosLocal(&v28, &a4, 1, a5);
          x = x - v28.x;
          y = y - v28.y;
          v24 = v28.z;
        }
        else
        {
          if ( a6 == 3 )
          {
            v18 = a4.mEdgeNormalIndex[1];
          }
          else
          {
            if ( a6 != 2 )
              goto LABEL_17;

            v18 = a4.mEdgeNormalIndex[0];
          }

          v6_12 = a2->getNormal(v18, a5);
          x = x - v29.x;
          y = y - v29.y;
          v24 = v29.z;
        }

        z = z - v24;

LABEL_17:
        v26 = (float)(v6_12.z * z) + (float)((float)(v6_12.y * y) + (float)(v6_12.x * x));
        v27 = a1->y - (float)(v6_12.y * v26);
        a1->x = a1->x - (float)(v6_12.x * v26);
        a1->y = v27;
        a1->z = a1->z - (float)(v6_12.z * v26);
        break;

      case 5u:
        v19 = a2;
        v20 = a1;
        v21 = &a4;
        v22 = 0;
        goto LABEL_12;

      case 6u:
        v22 = 1;
        goto LABEL_11;

      case 7u:
        v22 = 2;

LABEL_11:
        v19 = a2;
        v20 = a1;
        v21 = &a4;

LABEL_12:
        v19->calcPosLocal(v20, v21, v22, a5);
        return;

      default:
        return;
    }
  }
}
