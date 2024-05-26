#pragma once

#include <container/seadPtrArray.h>
#include <math/seadMatrix.h>

#include "Library/Collision/IUseCollision.h"
#include "Library/HostIO/HioNode.h"

namespace al {
class HitInfo;
class CollisionPartsFilterBase;
}
class CollisionShapeKeeper;
class CollisionMultiShape;

class PlayerCollider : public al::HioNode, al::IUseCollision
{
public:
  al::CollisionDirector *mCollisionDirector;
  const sead::Matrix34f *mMtxPtr;
  const sead::Vector3f *mTransPtr;
  const sead::Vector3f *mGravityPtr;
  sead::Vector3f mTrans;
  float mSize;
  sead::Matrix34f mMtx;
  al::HitInfo *info1;
  float val1;
  int pad1;
  al::HitInfo *info2;
  float val2;
  int pad2;
  al::HitInfo *info3;
  float val3;
  sead::Vector3f unk3;
  bool flag1;
  bool flag2;
  bool pads[2];
  sead::Vector3f mCollisionHitNormal;
  sead::Vector3f mCollisionHitPos;
  int mTimeInAir;
  sead::Matrix34f mCollidePosMtx;
  CollisionShapeKeeper *mCollisionShapeKeeper;
  float mCollisionShapeScale;
  int pad;
  CollisionMultiShape *mCollisionMultiShape;
  int someBitField;
  bool mIsInFastMoveCollisionArea;
  bool mIsValidGroundSupport;
  bool mIsDuringRecovery;
  bool pad3[1];
  sead::Vector3f mCutCollideAffectDir;
  int mWallBorderCheckType;
  const al::CollisionPartsFilterBase *mCollisionPartsFilter;
  sead::PtrArray<al::HitInfo const> ptrArraysChangeMe[3];
  al::HitInfo *someHitInfos;
  unsigned int numHitInfosAbove;
  int pad5;
  sead::PtrArray<al::HitInfo> anotherPtrArray;
  int sizeOfArrayBelowIs3;
  int pad4;
  float *someThreeFloats;
  int sizeOfArrayBelowIs3_2;
  int pad4_2;
  float *anotherThreeFloats;
  sead::Vector3f unk9;
  sead::Vector3f unk10;
  float unk11;
};
