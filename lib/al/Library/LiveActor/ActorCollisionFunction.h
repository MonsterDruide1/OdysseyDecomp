#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class ActorCollisionController;
class CollisionPartsFilterBase;
class HitSensor;
class TriangleFilterBase;

Collider* getActorCollider(const LiveActor* actor);
bool isExistActorCollider(const LiveActor* actor);
const HitSensor* getActorCollisionPartsSensor(const LiveActor* actor);
bool isExistCollisionParts(const LiveActor* actor);
bool isEqualCollisionParts(const LiveActor* actor, const CollisionParts* collisionParts);
void validateCollisionParts(LiveActor* actor);
void invalidateCollisionParts(LiveActor* actor);
void validateCollisionPartsBySystem(LiveActor* actor);
void invalidateCollisionPartsBySystem(LiveActor* actor);
bool isValidCollisionParts(const LiveActor* actor);
void setCollisionPartsSpecialPurposeName(LiveActor* actor, const char* name);
void resetAllCollisionMtx(LiveActor* actor);
void syncCollisionMtx(LiveActor* actor, const sead::Matrix34f* mtx);
void syncCollisionMtx(LiveActor* actor, CollisionParts* collisionParts, const sead::Matrix34f* mtx);
void setSyncCollisionMtxPtr(LiveActor* actor, const sead::Matrix34f* mtx);
bool isOnGround(const LiveActor* actor, u32 coyoteTime);
bool isOnGroundFace(const LiveActor* actor);
bool isCollidedGroundEdgeOrCorner(const LiveActor* actor);
bool isOnGroundNoVelocity(const LiveActor* actor, u32 coyoteTime);
bool isOnGroundDegree(const LiveActor* actor, f32 angle, u32 coyoteTime);
bool isOnGroundFaceDegree(const LiveActor* actor, f32 angle);
bool isOnGroundNoVelocityDegree(const LiveActor* actor, f32 angle, u32 coyoteTime);
const sead::Vector3f& getOnGroundNormal(const LiveActor* actor, u32 offset);
void setColliderRadius(LiveActor* actor, f32 radius);
void setColliderOffsetY(LiveActor* actor, f32 offsety);
f32 getColliderRadius(const LiveActor* actor);
f32 getColliderOffsetY(const LiveActor* actor);
const sead::Vector3f& getColliderFixReaction(const LiveActor* actor);
void calcColliderPos(sead::Vector3f* outPos, const LiveActor* actor);
void validateColliderRobustCheck(LiveActor* actor);
void invalidateColliderRobustCheck(LiveActor* actor);
void setColliderReactMovePower(LiveActor* actor, bool isEnabled);
void calcColliderFloorRotatePower(sead::Quatf* outRotatePower, LiveActor* actor);
void calcJumpInertia(sead::Vector3f* outJumpInertia, LiveActor* actor,
                     const sead::Vector3f& velocityDir, f32 force);
bool isCollidedGround(const LiveActor* actor);
void calcJumpInertiaWall(sead::Vector3f* outJumpInertia, LiveActor* actor, f32 force);
bool isCollidedWall(const LiveActor* actor);
void scaleVelocityInertiaWallHit(LiveActor* actor, f32 velocityScaleH, f32 maxVelocityH,
                                 f32 maxScaledVelocityH);
const sead::Vector3f& getCollidedWallNormal(const LiveActor* actor);
void calcCollidedNormalSum(const LiveActor* actor, sead::Vector3f* outNormal);
void calcGroundNormalOrUpDir(sead::Vector3f* outDir, const LiveActor* actor);
const sead::Vector3f& getCollidedGroundNormal(const LiveActor* actor);
void calcGroundNormalOrGravityDir(sead::Vector3f* outDir, const LiveActor* actor);
void setColliderFilterTriangle(LiveActor* actor, const TriangleFilterBase* triangleFilter);
void setColliderFilterCollisionParts(LiveActor* actor,
                                     const CollisionPartsFilterBase* collisionPartsFilter);
void createAndSetColliderFilterExistActor(LiveActor* actor);
void createAndSetColliderFilterExistActor(LiveActor* actor, LiveActor* filterActor);
void createAndSetColliderFilterSubActor(LiveActor* actor);
void createAndSetColliderSpecialPurpose(LiveActor* actor, const char* name);
void createAndSetColliderSpecialPurposeForCollisionActor(LiveActor* actor, const char* name);
void createAndSetColliderIgnoreOptionalPurpose(LiveActor* actor, const char* name);
void createAndSetColliderSpecialAndIgnoreOptionalPurpose(LiveActor* actor, const char* name,
                                                         const char* ignoreName);
ActorCollisionController* createActorCollisionController(LiveActor* actor);
void setColliderRadius(ActorCollisionController* collisionController, f32 radius);
void setColliderRadiusScale(ActorCollisionController* collisionController, f32 scale);
void setColliderOffsetY(ActorCollisionController* collisionController, f32 offsetY);
void resetActorCollisionController(ActorCollisionController* collisionController, s32 delay);
void updateActorCollisionController(ActorCollisionController* collisionController);
const char* getCollidedFloorMaterialCodeName(const LiveActor* actor);
const char* getCollidedFloorMaterialCodeName(const Collider* collider);
const char* getCollidedWallMaterialCodeName(const LiveActor* actor);
const char* getCollidedCeilingMaterialCodeName(const LiveActor* actor);
bool isCollidedFloorCode(const LiveActor* actor, const char* name);
bool isCollidedCollisionCode(const LiveActor* actor, const char* sensorName, const char* name);
bool isCollidedWallCode(const LiveActor* actor, const char* name);
bool isCollidedCameraCode(const LiveActor* actor, const char* name);
bool isCollidedMaterialCode(const LiveActor* actor, const char* name);
bool isCollidedGroundFloorCode(const LiveActor* actor, const char* name);
bool isCollidedGroundCollisionCode(const LiveActor* actor, const char* sensorName,
                                   const char* name);
bool isCollided(const LiveActor* actor);
bool isCollidedCeiling(const LiveActor* actor);
bool isCollidedWallFace(const LiveActor* actor);
bool isCollidedVelocity(const LiveActor* actor);
bool isCollidedWallVelocity(const LiveActor* actor);
bool isCollidedCeilingVelocity(const LiveActor* actor);
const sead::Vector3f& getCollidedCeilingNormal(const LiveActor* actor);
const sead::Vector3f& getCollidedGroundPos(const LiveActor* actor);
const sead::Vector3f& getCollidedWallPos(const LiveActor* actor);
const sead::Vector3f& getCollidedCeilingPos(const LiveActor* actor);
f32 calcSpeedCollideWall(const LiveActor* actor);
f32 calcSpeedCollideGround(const LiveActor* actor);
f32 calcSpeedCollideCeiling(const LiveActor* actor);
const CollisionParts* getCollidedGroundCollisionParts(const LiveActor* actor);
const CollisionParts* tryGetCollidedGroundCollisionParts(const LiveActor* actor);
const CollisionParts* getCollidedWallCollisionParts(const LiveActor* actor);
const CollisionParts* tryGetCollidedWallCollisionParts(const LiveActor* actor);
const CollisionParts* getCollidedCeilingCollisionParts(const LiveActor* actor);
const CollisionParts* tryGetCollidedCeilingCollisionParts(const LiveActor* actor);
HitSensor* getCollidedGroundSensor(const LiveActor* actor);
HitSensor* tryGetCollidedGroundSensor(const LiveActor* actor);
HitSensor* getCollidedWallSensor(const LiveActor* actor);
HitSensor* tryGetCollidedWallSensor(const LiveActor* actor);
HitSensor* getCollidedCeilingSensor(const LiveActor* actor);
HitSensor* tryGetCollidedCeilingSensor(const LiveActor* actor);
HitSensor* tryGetCollidedSensor(const LiveActor* actor);
bool tryGetCollidedPos(sead::Vector3f* outPos, const LiveActor* actor);
void setForceCollisionScaleOne(const LiveActor* actor);
void followRotateFrontAxisUpGround(LiveActor* actor);
void followRotateFrontAxisUp(LiveActor* actor, const CollisionParts* collisionParts);

}  // namespace al
