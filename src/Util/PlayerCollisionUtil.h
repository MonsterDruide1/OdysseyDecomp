#pragma once

#include <math/seadVector.h>
#include "Library/Collision/KTriangle.h"

namespace al {
class LiveActor;
class CollisionParts;
}  // namespace al
class IUsePlayerCollision;
class PlayerConst;
class IUsePlayerHeightCheck;
class PlayerConst;

namespace rs {

f32 getGroundHeight(const IUsePlayerHeightCheck*);
bool isAboveGround(const IUsePlayerHeightCheck*);

const sead::Vector3f& getCollidedWallPos(const IUsePlayerCollision*);
const sead::Vector3f& getCollidedWallNormal(const IUsePlayerCollision*);
const al::CollisionParts* getCollidedWallCollisionParts(const IUsePlayerCollision*);

const sead::Vector3f& getCollidedGroundPos(const IUsePlayerCollision*);
const sead::Vector3f& getCollidedGroundNormal(const IUsePlayerCollision*);
const al::CollisionParts* getCollidedGroundCollisionParts(const IUsePlayerCollision*);

const sead::Vector3f& getCollidedCeilingPos(const IUsePlayerCollision*);
const sead::Vector3f& getCollidedCeilingNormal(const IUsePlayerCollision*);
const al::CollisionParts* getCollidedCeilingCollisionParts(const IUsePlayerCollision*);

bool isCollidedGround(const IUsePlayerCollision*);
bool isCollidedGroundRunAngle(const al::LiveActor*, const IUsePlayerCollision*, const PlayerConst*);
f32 getGroundHeight(const IUsePlayerHeightCheck*);

const sead::Vector3f& getCollidedWallPos(const IUsePlayerCollision*);
const sead::Vector3f& getCollidedWallNormal(const IUsePlayerCollision*);
const al::CollisionParts* getCollidedWallCollisionParts(const IUsePlayerCollision*);

const sead::Vector3f& getCollidedGroundPos(const IUsePlayerCollision*);
const sead::Vector3f& getCollidedGroundNormal(const IUsePlayerCollision*);
const al::CollisionParts* getCollidedGroundCollisionParts(const IUsePlayerCollision*);

const sead::Vector3f& getCollidedCeilingPos(const IUsePlayerCollision*);
const sead::Vector3f& getCollidedCeilingNormal(const IUsePlayerCollision*);
const al::CollisionParts* getCollidedCeilingCollisionParts(const IUsePlayerCollision*);

bool isCollidedGround(const IUsePlayerCollision*);
bool isOnGroundForceSlideCode(const al::LiveActor*, const IUsePlayerCollision*, const PlayerConst*);
bool isOnGroundForceRollingCode(const al::LiveActor*, const IUsePlayerCollision*);
bool isOnGroundLessAngle(const al::LiveActor*, const IUsePlayerCollision*, f32);
bool isPlayerOnGround(const al::LiveActor*);
bool isOnGround(const al::LiveActor*, const IUsePlayerCollision*);
bool isJustLand(const IUsePlayerCollision*);
void calcGroundNormalOrGravityDir(sead::Vector3f*, const al::LiveActor*,
                                  const IUsePlayerCollision*);
bool isCollisionCodeSandSink(const IUsePlayerCollision*);
bool isCollidedWall(const IUsePlayerCollision*);
bool isCollidedWallFace(const IUsePlayerCollision*);
bool isActionCodeNoWallGrab(const IUsePlayerCollision*);
bool isActionCodeNoWallKeepWall(const IUsePlayerCollision*);
bool isActionCodeNoActionWall(const IUsePlayerCollision*);
bool isActionCodeNoActionCeiling(const IUsePlayerCollision*);

bool isCollisionCodeGrabCeilAny(const IUsePlayerCollision*);
bool isCollisionCodeGrabCeilWall(const IUsePlayerCollision*);
bool isCollisionCodeGrabCeilCeiling(const IUsePlayerCollision*);
bool isCollisionCodeGrabCeilGrround(const IUsePlayerCollision*);

bool isAutoRunOnGroundSkateCode(const al::LiveActor*, const IUsePlayerCollision*, float);
                                  
bool isActionCodeNoWallGrab(const al::HitInfo*);
bool isActionCodeNoWallPopUp(const al::HitInfo*);

bool isCollisionCodeGrabCeilAny(const IUsePlayerCollision*);
bool isCollisionCodeGrabCeilWall(const IUsePlayerCollision*);
bool isCollisionCodeGrabCeilCeiling(const IUsePlayerCollision*);
bool isCollisionCodeGrabCeilGrround(const IUsePlayerCollision*);

}  // namespace rs
