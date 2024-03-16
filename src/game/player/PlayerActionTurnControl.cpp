#include "game/player/PlayerActionTurnControl.h"

#include "game/player/PlayerActionFunction.h"
#include "al/util/MathUtil.h"
#include "al/util/VectorUtil.h"
#include "al/util/LiveActorUtil.h"
#include "math/seadQuat.h"
#include "math/seadVector.h"

/*
    PlayerActionTurnControl(al::LiveActor*);
    void PlayerActionTurnControl::setup(float, float, float, float, int, int, int);
    void PlayerActionTurnControl::reset();
*/

template<class T> char __SETS__(T x)
{
  if ( sizeof(T) == 1 )
    return char(x) < 0;
  if ( sizeof(T) == 2 )
    return short(x) < 0;
  if ( sizeof(T) == 4 )
    return int(x) < 0;
  return long(x) < 0;
}
template<class T, class U> char __OFSUB__(T x, U y)
{
  if ( sizeof(T) < sizeof(U) )
  {
    U x2 = x;
    char sx = __SETS__(x2);
    return (sx ^ __SETS__(y)) & (sx ^ __SETS__(x2-y));
  }
  else
  {
    T y2 = y;
    char sx = __SETS__(x);
    return (sx ^ __SETS__(y2)) & (sx ^ __SETS__(x-y2));
  }
}

void PlayerActionTurnControl::update(const sead::Vector3f& a2, const sead::Vector3f& a3) {
sead::Vector3f *v6; // x22
  sead::Vector3f v7; // x0
  float v8; // s0
  float v9; // s10
  float v10; // s11
  float v11; // s9
  bool v12; // w23
  bool v13; // w26
  bool v14; // w24
  int v15; // w27
  bool v16; // w0
  bool v17; // w25
  int v18; // w28
  float v19; // s12
  float v20; // s0
  int v21; // w9
  int v22[2]; // x8
  int v23; // w10
  int v24; // w9
  int v25; // w8
  int v26; // w11
  int v27; // w9
  int v28; // w8
  float *v29; // x9
  int v30; // w9
  int v31; // w8
  bool v32; // vf
  int v33; // w9
  int v34; // w11
  int v35; // w12
  float *v36; // x13
  float v37; // s0
  int v38; // w10
  int v39; // w14
  int v40; // w14
  bool v41; // w8
  bool v42; // w10
  int v43; // w9
  bool v44; // w0
  float v45; // s0
  float v46; // s1
  float v47; // s1
  float v48; // s0
  float v49; // s10
  const sead::Vector3f *v50; // x21
  float v51; // s1
  float v52; // s0
  float v53; // s9
  float v54; // s8
  const sead::Vector3f *v55; // x25
  bool v56; // w20
  bool v57; // w22
  float v58; // s8
  float v59; // s0
  float v60; // s2
  float v61; // s1
  float v62; // s0
  float v63; // s0
  float v64; // s1
  int v65; // s2
  float v66; // s10
  float v68; // s11
  float v70; // s12
  float v71; // s8
  float v72; // w22
  float v73; // w23
  float v74; // w24
  float v75; // s9
  float v76; // s0
  float v77; // s4
  float v78; // s5
  float v79; // s0
  float v80; // s6
  float v81; // s16
  float v82; // s3
  float v83; // s1
  float v84; // s7
  float v85; // s17
  float v86; // s5
  float v87; // s1
  float v88; // s1
  float v89; // s2
  float v90; // s0
  float v91; // s1
  float v92; // s0
  sead::Quatf v94; // [xsp+10h] [xbp-100h] BYREF
  sead::Vector3f v95; // [xsp+20h] [xbp-F0h] BYREF
  sead::Vector3f v96; // [xsp+30h] [xbp-E0h] BYREF
  sead::Vector3f v97; // [xsp+40h] [xbp-D0h] BYREF
  sead::Vector3f v99; // [xsp+60h] [xbp-B0h] BYREF
  const sead::Vector3f *v102; // [xsp+98h] [xbp-78h]

  sead::Vector3f a1a{0,0,0};
  al::calcFrontDir(&a1a, mLiveActor);

  sead::Vector3f v100{0,0,0};
  al::calcUpDir(&v100, mLiveActor);

  al::alongVectorNormalH(&a1a, a1a, v100, a3);
  al::tryNormalizeOrZero(&a1a);
  _50 = a1a;
  v6 = &_c;
  v99.x = 0.0;
  v99.y = 0.0;
  v99.z = 0.0;
  if ( !al::isNearZero(_c, 0.001f) || !al::isNearZero(_24, 0.001f) )
  {
    al::alongVectorNormalH(&v99, _c, _24, a3);
    al::tryNormalizeOrZero(&v99);
  }
  v7 = al::getGravity(mLiveActor);
  v8 = v7.x;
  v9 = v7.y;
  v10 = v7.z;
  v11 = -v8;
  sead::Vector3f a3a{0,0,0};
  v12 = al::tryNormalizeOrZero(&a3a, a2);
  v13 = false;
  if ( (float)((float)((float)(v100.x * v11) - (float)(v100.y * v9)) - (float)(v100.z * v10)) < 0.087156f )
    v13 = _88[1];
  v14 = al::isNearZero(a1a, 0.001f);
  v15 = v13 || v14;
  v16 = al::isNearZero(v99, 0.001f);
  v17 = v12 && !v16;
  v18 = v16 || !v12;
  v102 = &a2;
  if ( (v18 & 1) != 0
    || (float)((float)((float)(a3a.x * v99.x) + (float)(a3a.y * v99.y)) + (float)(a3a.z * v99.z)) < 0.34202f )
  {
    _40[1] = 0;
    _48[0] = 0;
    v19 = 0.0;
    _30 = 0.0;
  }
  else
  {
    if ( v12 && !v16 )
    {
      v20 = al::calcAngleDegree(v99, a3a);
      if ( v20 <= 0.0 )
        v20 = -v20;
    }
    else
    {
      v20 = 0.0;
    }
    v21 = _48[0];
    *(long *)v22 = *(long *)_40;
    if ( v21 < v22[0] )
      goto LABEL_21;
    v23 = v21 - 1;
    if ( v21 >= 1 )
    {
      v24 = _40[1];
      if ( v24 + 1 < v22[0] )
        v25 = v24 + 1;
      else
        v25 = 0;
      v21 = v23;
      _40[1] = v25;
      _48[0] = v23;
      *(long *)v22 = *(long *)_40;
    }
    if ( v21 < v22[0] )
    {
LABEL_21:
      v26 = v21 + 1;
      v27 = v22[1] + v21;
      if ( v27 < v22[0] )
        v22[0] = 0;
      v28 = v27 - v22[0];
      v29 = (float *)_38_array;
      _48[0] = v26;
      v29[v28] = v20;
    }
    v30 = _48[0];
    _30 = 0.0;
    if ( v30 <= 5 )
      v31 = v30;
    else
      v31 = 5;
    v32 = __OFSUB__(v30, 1);
    v33 = v30 - 1;
    if ( v33 < 0 != v32 )
    {
      v37 = 0.0;
    }
    else
    {
      v34 = _40[0];
      v35 = _40[1];
      v36 = (float *)_38_array;
      v37 = 0.0;
      v38 = 0;
      do
      {
        if ( v33 + v35 >= v34 )
          v39 = v34;
        else
          v39 = 0;
        v40 = v33 + v35 - v39;
        --v33;
        v37 = v36[v40] + v37;
        ++v38;
        _30 = v37;
      }
      while ( v38 < v31 );
    }
    v19 = v37 / 5.0;
    _30 = v37 / 5.0;
  }
  if ( v13 || v14 || !v12 )
  {
    v41 = 0;
    v42 = 0;
    v43 = v12;
  }
  else
  {
    v44 = PlayerActionFunction::isOppositeDir(a3a, a1a);
    v41 = 0;
    if ( ((v18 | (v17 && v19 <= 1.0)) & 1) != 0 )
    {
      v43 = v12;
      v42 = 0;
      if ( v44 )
      {
        *(int *)&v42 = _88[0];
        v41 = !_88[0];
        v43 = v12 & *(int *)&v42;
      }
    }
    else
    {
      v43 = v12;
      v42 = 0;
    }
  }
  _68[1] = v42;
  _68[0] = v41;
  _68[2] = v13;
  *(long *)&v97.x = 0LL;
  v97.z = 0.0;
  if ( (v43 & (v15 ^ 1) & 1) == 0 )
  {
    if ( v12 )
    {
      if ( v41 )
      {
        v97 = a1a;
        _8 = 0.0;
        goto LABEL_101;
      }
      v97 = a3a;
      if ( !v13 )
        goto LABEL_101;
    }
    else
    {
      if ( !v13 )
      {
        if ( (v15 & 1) != 0 )
          al::calcFrontDir(&v97, mLiveActor);
        else
          v97 = a1a;
        _24.z = 0.0;
        *(long *)&_18.y = 0LL;
        *(long *)&_24.x = 0LL;
        *(long *)&v6->x = 0LL;
        *(long *)&_c.z = 0LL;
        _8 = PlayerActionFunction::brake(_8, mTurnBrakeFrame, mTurnAngleLimit);
        goto LABEL_101;
      }
      al::calcFrontDir(&v97, mLiveActor);
      if ( (float)((float)((float)(v100.x * v11) + (float)(v100.y * (float)-v9)) + (float)(v100.z * (float)-v10)) < 0.0 )
      {
        v97.x = -v97.x;
        v97.y = -v97.y;
        v97.z = -v97.z;
      }
      _24.z = 0.0;
      *(long *)&_18.y = 0LL;
      *(long *)&_24.x = 0LL;
      *(long *)&v6->x = 0LL;
      *(long *)&_c.z = 0LL;
    }
    _8 = mTurnAngleFastLimit;
    goto LABEL_101;
  }
  if ( v17 )
  {
    v45 = (float)(v99.x * a3a.x) + (float)(v99.y * a3a.y);
    v46 = v99.z;
  }
  else
  {
    v45 = (float)(a1a.x * a3a.x) + (float)(a1a.y * a3a.y);
    v46 = a1a.z;
  }
  v47 = v45 + (float)(v46 * a3a.z);
  v48 = -1.0;
  if ( v47 >= -1.0 )
  {
    v48 = v47;
    if ( v47 > 1.0 )
      v48 = 1.0;
  }
  v49 = std::acos(v48);
  v50 = v102;
  v51 = (float)((float)(a1a.x * a3a.x) + (float)(a1a.y * a3a.y)) + (float)(a1a.z * a3a.z);
  v52 = -1.0;
  if ( v51 >= -1.0 )
  {
    v52 = (float)((float)(a1a.x * a3a.x) + (float)(a1a.y * a3a.y)) + (float)(a1a.z * a3a.z);
    if ( v51 > 1.0 )
      v52 = 1.0;
  }
  v53 = std::acos(v52);
  v96.x = (float)(v99.y * a3a.z) - (float)(v99.z * a3a.y);
  v96.y = (float)(v99.z * a3a.x) - (float)(a3a.z * v99.x);
  v96.z = (float)(a3a.y * v99.x) - (float)(v99.y * a3a.x);
  al::tryNormalizeOrZero(&v96);
  if ( !v17 || al::isNearZero(_18, 0.001) )
    goto LABEL_61;
  v54 = v49 * 57.296;
  if ( (float)((float)((float)(v96.x * _18.x) + (float)(v96.y * _18.y)) + (float)(v96.z * _18.z)) <= 0.0 )
  {
    v91 = mTurnAngleStart;
    v92 = _8;
    if ( v91 <= v92 )
      v91 = _8;
    if ( v54 >= v91 )
    {
      v56 = 0;
      v57 = 0;
      _8 = 0.0;
    }
    else
    {
      _8 = al::converge(v92, 0.0, v54);
      v56 = 0;
      v57 = v54 < mTurnAngleStart;
    }
    goto LABEL_62;
  }
  v94.x = (float)(a1a.y * a3a.z) - (float)(a1a.z * a3a.y);
  v94.y = (float)(a1a.z * a3a.x) - (float)(a3a.z * a1a.x);
  v94.z = (float)(a3a.y * a1a.x) - (float)(a1a.y * a3a.x);
  if ( (al::tryNormalizeOrZero((sead::Vector3f *)&v94) & 1) == 0 )
  {
LABEL_61:
    v56 = 0;
    v57 = 0;
LABEL_62:
    v55 = &a3;
    goto LABEL_63;
  }
  v55 = &a3;
  if ( (float)((float)((float)(v94.x * v96.x) + (float)(v94.y * v96.y)) + (float)(v94.z * v96.z)) >= 0.0 )
  {
    v56 = 0;
    v57 = 0;
  }
  else
  {
    v56 = v19 > 10.0 || v54 > 3.0;
    v57 = v19 <= 10.0 && v54 <= 3.0;
  }
LABEL_63:
  v58 = v53 * 57.296;
  v59 = al::calcRate01(
          sqrtf((float)((float)(v50->x * v50->x) + (float)(v50->y * v50->y)) + (float)(v50->z * v50->z)),
          0.0,
          0.8);
  v60 = 0.25;
  if ( v59 >= 0.25 )
  {
    v60 = v59;
    if ( v59 > 1.0 )
      v60 = 1.0;
  }
  v61 = mTurnAngleLimit;
  if ( v58 < mTurnAngleStart )
  {
    v62 = _8;
LABEL_68:
    v63 = PlayerActionFunction::brake(v62, mTurnBrakeFrame, v61);
    goto LABEL_82;
  }
  v62 = _8;
  if ( v58 >= mTurnAngleFast )
  {
    v64 = mTurnAngleFastLimit;
    v65 = mTurnAccelFrameFast;
  }
  else
  {
    v64 = v60 * v61;
    if ( v62 >= v64 )
    {
      v61 = mTurnAngleFastLimit;
      goto LABEL_68;
    }
    v65 = mTurnAccelFrame;
  }
  v63 = PlayerActionFunction::accel(v62, v64, v64 / (float)v65);
LABEL_82:
  _8 = v63;
  *(long *)&v95.x = 0LL;
  v95.z = 0.0;
  if ( v56 || al::isReverseDirection(a1a, a3a, 0.01) )
  {
    v66 = v55->x;
    v68 = v55->y;
    v70 = v55->z;
    if ( (float)((float)((float)(v96.x * v55->x) + (float)(v96.y * v68)) + (float)(v96.z * v70)) <= 0.0 )
    {
      v66 = -v66;
      v68 = -v68;
      v70 = -v70;
    }
    v71 = (float)(_8 * 0.017453) * 0.5;
    v72 = v66;
    v73 = v68;
    v74 = v70;
    v75 = std::cos(v71);
    v76 = std::sin(v71);
    v77 = v66 * v76;
    v78 = v68 * v76;
    v79 = v70 * v76;
    v80 = (float)((float)(v78 * a1a.z) - (float)(v79 * a1a.y)) + (float)(v75 * a1a.x);
    v81 = (float)(v75 * a1a.y) + (float)((float)(v79 * a1a.x) - (float)(a1a.z * v77));
    v82 = (float)(v75 * a1a.z) + (float)((float)(v77 * a1a.y) - (float)(v78 * a1a.x));
    v83 = (float)((float)-(float)(v77 * a1a.x) - (float)(v78 * a1a.y)) - (float)(a1a.z * v79);
    v97.x = (float)((float)(v78 * v82) + (float)((float)(v75 * v80) - (float)(v79 * v81))) - (float)(v77 * v83);
    v97.y = (float)((float)((float)(v79 * v80) + (float)(v75 * v81)) - (float)(v77 * v82)) - (float)(v78 * v83);
    v97.z = (float)((float)(v75 * v82) + (float)((float)(v77 * v81) - (float)(v78 * v80))) - (float)(v79 * v83);
    al::tryNormalizeOrZero(&v97);
    v95.x = v66;
    v95.y = v68;
    v95.z = v70;
  }
  else
  {
    v94.x = 0.0;
    v94.y = 0.0;
    v94.z = 0.0;
    v94.w = 1.0;
    al::makeQuatRotationLimit(&v94, a1a, a3a, _8 * 0.017453);
    v84 = (float)((float)(v94.y * a1a.z) - (float)(v94.z * a1a.y)) + (float)(v94.w * a1a.x);
    v85 = (float)(a1a.y * v94.w) + (float)((float)(v94.z * a1a.x) - (float)(a1a.z * v94.x));
    v86 = (float)(a1a.z * v94.w) + (float)((float)(a1a.y * v94.x) - (float)(v94.y * a1a.x));
    v87 = (float)((float)-(float)(a1a.x * v94.x) - (float)(v94.y * a1a.y)) - (float)(a1a.z * v94.z);
    v97.x = (float)((float)(v94.y * v86) + (float)((float)(v94.w * v84) - (float)(v94.z * v85))) - (float)(v94.x * v87);
    v97.y = (float)((float)((float)(v84 * v94.z) + (float)(v85 * v94.w)) - (float)(v86 * v94.x)) - (float)(v87 * v94.y);
    v97.z = (float)((float)((float)(v85 * v94.x) - (float)(v84 * v94.y)) + (float)(v86 * v94.w)) - (float)(v87 * v94.z);
    al::normalize(&v97);
    v95.x = (float)(a1a.y * a3a.z) - (float)(a1a.z * a3a.y);
    v95.y = (float)(a1a.z * a3a.x) - (float)(a3a.z * a1a.x);
    v95.z = (float)(a3a.y * a1a.x) - (float)(a1a.y * a3a.x);
    al::tryNormalizeOrZero(&v95);
    if ( v57 )
    {
      v89 = v95.y;
      v88 = v95.x;
      v90 = v95.z;
      if ( (float)((float)((float)(v95.x * _18.x) + (float)(v95.y * _18.y)) + (float)(v95.z * _18.z)) < 0.0 )
      {
        v88 = -v95.x;
        v89 = -v95.y;
        v90 = -v95.z;
        v95.x = -v95.x;
        v95.y = -v95.y;
        v95.z = -v95.z;
      }
      v72 = v88;
      v73 = v89;
      v74 = v90;
    }
    else
    {
      v72 = v95.x;
      v73 = v95.y;
      v74 = v95.z;
    }
  }
  _18.x = v72;
  _18.y = v73;
  _18.z = v74;
  _c = a3a;
  _24.x = v55->x;
  _24.y = v55->y;
  _24.z = v55->z;
LABEL_101:
  _5c = v97;
}
// FIXME
void PlayerActionTurnControl::calcTilt(sead::Vector3f* a1, const sead::Vector3f& a2, float a3) {
    sead::Vector3f v7;  // s10
    float v12;          // s0
    float v13;          // s8
    float cos;          // s9
    float sin;          // s0
    sead::Vector3f v18;
    float v21;  // s6
    float v23;  // s16
    float v24;  // s3
    float v25;  // s1

    v7 = _5c;
    v12 = calcTurnPowerRate(a2);
    v13 = ((v12 * a3) * sead::Mathf::deg2rad(1)) * 0.5f;
    cos = std::cos(v13);
    sin = std::sin(v13);
    v18 = v7 * sin;

    v21 = (v18.y * a2.z) - (v18.z * a2.y) + (cos * a2.x);
    v23 = (v18.z * a2.x) - (v18.x * a2.z) + (cos * a2.y);
    v24 = (v18.x * a2.y) - (v18.y * a2.x) + (cos * a2.z);

    v25 = -(v18.x * a2.x) - (v18.y * a2.y) - (a2.z * v18.z);

    a1->x = (v18.y * v24) + (cos * v21) - (v18.z * v23) - (v18.x * v25);
    a1->y = (v18.z * v21) + (cos * v23) - (v18.x * v24) - (v18.y * v25);
    a1->z = (v18.x * v23) + (cos * v24) - (v18.y * v21) - (v18.z * v25);
}
float PlayerActionTurnControl::calcTurnPowerRate(const sead::Vector3f& a2) const {
    float result = al::easeIn(al::calcRate01(_8, mTurnAngleStart, mTurnAngleLimit));
    if (a2.dot(_18) > 0) {
        result = -result;
    }
    return result;
}
