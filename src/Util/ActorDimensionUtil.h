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

ActorDimensionKeeper* createDimensionKeeper(const al::LiveActor* actor);
void updateDimensionKeeper(ActorDimensionKeeper*);
const char* getSpecialPurposeName2DOnly();
void createAndSetFilter2DOnly(al::LiveActor*);
al::CollisionPartsFilterBase* createCollisionPartsFilter2DOnly();
bool is2D(const IUseDimension* dimension);
bool isIn2DArea(const IUseDimension* dimension);
bool is3D(const IUseDimension* dimension);
bool isChange2D(const IUseDimension* dimension);
bool isChange3D(const IUseDimension* dimension);
bool isNearSnapSurface(const IUseDimension* dimension, f32);
void calcLockDirection(sead::Vector3f*, const IUseDimension* dimension);
void calcDimensionGravity(sead::Vector3f*, const IUseDimension* dimension, const sead::Vector3f&);
void setDimensionGravity(al::LiveActor*, const IUseDimension* dimension);
void syncDimensionPoseGravity(al::LiveActor*, const IUseDimension* dimension);
void calcLockedMoveVec(sead::Vector3f*, const IUseDimension* dimension, f32);
void pushOutFrom2DArea(al::LiveActor*, const IUseDimension* dimension, f32, f32);
void snap2D(al::LiveActor*, const IUseDimension* dimension, f32);
void snap2DGravity(al::LiveActor*, const IUseDimension* dimension, f32);
void snap2DUp(al::LiveActor*, const IUseDimension* dimension, f32);
void snap2DParallelizeFront(al::LiveActor*, const IUseDimension* dimension, f32);
bool snap2DGravityPoseWithRotateCenter(al::LiveActor*, IUsePlayerCollision*,
                                       const IUseDimension* dimension, f32, f32,
                                       const sead::Vector3f&);
bool calcSnap2DPosition(sead::Vector3f*, const IUseDimension* dimension, const sead::Vector3f&,
                        f32);
void attachMtxConnectorTo2DCollision(al::MtxConnector*, const al::LiveActor*, const sead::Vector3f&,
                                     const sead::Vector3f&);
void attachMtxConnectorTo2DCollision(al::MtxConnector*, const al::LiveActor*, bool);
void attachMtxConnectorTo2DCollision(al::MtxConnector*, const al::LiveActor*, f32, f32);
void syncVisAnimFrame2D(al::LiveActor*, const char*, bool);

}  // namespace rs
