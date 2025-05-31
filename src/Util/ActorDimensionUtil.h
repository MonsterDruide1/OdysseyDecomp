#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
class LiveActor;
class MtxConnector;
class CollisionPartsFilterBase;
}  // namespace al

class IUseDimension;
class ActorDimensionKeeper;
class IUsePlayerCollision;

namespace rs {

ActorDimensionKeeper* createDimensionKeeper(const al::LiveActor*);
void updateDimensionKeeper(ActorDimensionKeeper*);
const char* getSpecialPurposeName2DOnly();
void createAndSetFilter2DOnly(al::LiveActor*);
al::CollisionPartsFilterBase* createCollisionPartsFilter2DOnly();
bool is2D(const IUseDimension*);
bool isIn2DArea(const IUseDimension*);
bool is3D(const IUseDimension*);
bool isChange2D(const IUseDimension*);
bool isChange3D(const IUseDimension*);
bool isNearSnapSurface(const IUseDimension*, f32);
void calcLockDirection(sead::Vector3f*, const IUseDimension*);
void calcDimensionGravity(sead::Vector3f*, const IUseDimension*, const sead::Vector3f&);
void setDimensionGravity(al::LiveActor*, const IUseDimension*);
void syncDimensionPoseGravity(al::LiveActor*, const IUseDimension*);
void calcLockedMoveVec(sead::Vector3f*, const IUseDimension*, f32);
void pushOutFrom2DArea(al::LiveActor*, const IUseDimension*, f32, f32);
void snap2D(al::LiveActor*, const IUseDimension*, f32);
void snap2DGravity(al::LiveActor*, const IUseDimension*, f32);
void snap2DUp(al::LiveActor*, const IUseDimension*, f32);
void snap2DParallelizeFront(al::LiveActor*, const IUseDimension*, f32);
bool snap2DGravityPoseWithRotateCenter(al::LiveActor*, IUsePlayerCollision*, const IUseDimension*,
                                       f32, f32, const sead::Vector3f&);
bool calcSnap2DPosition(sead::Vector3f*, const IUseDimension*, const sead::Vector3f&, f32);
void attachMtxConnectorTo2DCollision(al::MtxConnector*, const al::LiveActor*, const sead::Vector3f&,
                                     const sead::Vector3f&);
void attachMtxConnectorTo2DCollision(al::MtxConnector*, const al::LiveActor*, bool);
void attachMtxConnectorTo2DCollision(al::MtxConnector*, const al::LiveActor*, f32, f32);
void syncVisAnimFrame2D(al::LiveActor*, const char*, bool);

}  // namespace rs
