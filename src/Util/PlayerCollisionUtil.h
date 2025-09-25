#pragma once

#include <math/seadVector.h>

namespace al {
class LiveActor;
class CollisionParts;
class CollisionPartsFilterBase;
class HitSensor;
}  // namespace al

class IUsePlayerCollision;
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
bool isOnGroundSlopeSlideStart(const al::LiveActor*, const IUsePlayerCollision*,
                               const PlayerConst*);
bool isOnGroundForceSlideCode(const al::LiveActor*, const IUsePlayerCollision*, const PlayerConst*);
bool isOnGroundForceRollingCode(const al::LiveActor*, const IUsePlayerCollision*);
bool isOnGroundSkateCode(const al::LiveActor*, const IUsePlayerCollision*);
bool isOnGroundLessAngle(const al::LiveActor*, const IUsePlayerCollision*, f32);
bool isPlayerOnGround(const al::LiveActor*);
bool isOnGround(const al::LiveActor*, const IUsePlayerCollision*);
bool isJustLand(const IUsePlayerCollision*);
void calcGroundNormalOrGravityDir(sead::Vector3f*, const al::LiveActor*,
                                  const IUsePlayerCollision*);
bool isCollisionCodeSandSink(const IUsePlayerCollision*);
bool isCollidedWall(const IUsePlayerCollision*);
bool isCollidedWallFace(const IUsePlayerCollision*);
bool isCollidedCeiling(const IUsePlayerCollision*);
bool isActionCodeNoWallGrab(const IUsePlayerCollision*);
bool isActionCodeNoActionWall(const IUsePlayerCollision*);
bool isActionCodeNoWallKeepWall(const IUsePlayerCollision*);

bool isCollisionCodeGrabCeilAny(const IUsePlayerCollision*);
bool isCollisionCodeGrabCeilWall(const IUsePlayerCollision*);
bool isCollisionCodeGrabCeilCeiling(const IUsePlayerCollision*);
bool isCollisionCodeGrabCeilGrround(const IUsePlayerCollision*);

void setColliderFilterCollisionParts(IUsePlayerCollision*, const al::CollisionPartsFilterBase*);

bool reboundVelocityFromCollision(al::LiveActor*, const IUsePlayerCollision*, f32, f32, f32);
al::HitSensor* tryGetCollidedCeilingSensor(const IUsePlayerCollision*);

bool isOnGroundSlopeSlideEnd(const al::LiveActor*, const IUsePlayerCollision*, const PlayerConst*);
bool isAutoRunOnGroundSkateCode(const al::LiveActor*, const IUsePlayerCollision*, f32);

void startHitReactionLandIfLanding(const al::LiveActor*, const IUsePlayerCollision*, bool);

}  // namespace rs
