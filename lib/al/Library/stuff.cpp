#include "Library/Base/String.h"
#include "Library/Collision/CollisionDirector.h"
#include "Library/Collision/CollisionParts.h"
#include "Library/Collision/CollisionPartsKeeper.h"
#include "Library/Collision/CollisionUtil.h"
#include "Library/Collision/IUseCollision.h"
#include "Library/Collision/KTriangle.h"
#include "Library/Execute/ExecuteTableHolderUpdate.h"
#include "Library/Math/MathAngleUtil.h"
#include "Library/Math/MathLengthUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Math/SegmentUtil.h"
#include "Library/Matrix/MatrixUtil.h"
#include "Library/Yaml/ByamlIter.h"
#include "Library/Yaml/ByamlUtil.h"
#include "math/seadBoundBox.h"
#include "math/seadMatrix.h"
#include "math/seadQuatCalcCommon.h"
#include "math/seadVectorFwd.h"

#include <cstdio>

#include "../../../../src/stubs/missing.h"

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
  float unk; // s1
  float v7; // s0
  float x; // s1
  float y; // s2
  float v10; // s6
  float z; // s3
  float v12; // s1
  float v13; // s0
  float v14; // s3
  float v15; // s6
  float v16; // s4
  float v17; // s5
  float v18; // s7
  float v19; // s0
  sead::Vector3f v20; // [xsp+0h] [xbp-30h] BYREF

  if ( this->mCollisionLocation == 1 )
  {
    unk = this->unk;
    a1->x = unk * this->mTriangle.mFaceNormal.x;
    a1->y = unk * this->mTriangle.mFaceNormal.y;
    a1->z = unk * this->mTriangle.mFaceNormal.z;
    if ( a2 )
      *a2 = this->mTriangle.mFaceNormal;
  }
  else
  {
    v20.x = this->unk3.x - this->mCollisionHitPos.x;
    v20.y = this->unk3.y - this->mCollisionHitPos.y;
    v20.z = this->unk3.z - this->mCollisionHitPos.z;
    al::tryNormalizeOrZero(&v20);
    v7 = this->unk;
    x = this->mTriangle.mFaceNormal.x;
    y = this->mTriangle.mFaceNormal.y;
    v10 = v7 * x;
    z = this->mTriangle.mFaceNormal.z;
    v12 = (float)((float)(x * v20.x) + (float)(y * v20.y)) + (float)(z * v20.z);
    v13 = (float)((float)(v10 * v20.x) + (float)((float)(v7 * y) * v20.y)) + (float)((float)(v7 * z) * v20.z);
    v14 = v20.x * v12;
    v15 = v20.y * v12;
    v16 = v20.x * v13;
    v17 = v20.y * v13;
    v18 = v13 * v20.z;
    v19 = v12 * v20.z;
    a1->x = v16;
    a1->y = v17;
    a1->z = v18;
    a2->x = v14;
    a2->y = v15;
    a2->z = v19;
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
  const sead::Vector3f *p_mFaceNormal; // x0
  float x; // t1
  float y; // s1

  x = this->mTriangle.mFaceNormal.x;
  p_mFaceNormal = &this->mTriangle.mFaceNormal;
  y = p_mFaceNormal[7].y;
  a1->x = y * x;
  a1->y = y * p_mFaceNormal->y;
  a1->z = y * p_mFaceNormal->z;
  if ( a2 )
    *a2 = *p_mFaceNormal;
}

bool al::isReverseDirection(const sead::Vector3f& a1, const sead::Vector3f& a2, f32 a3) {
  CRASH
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
