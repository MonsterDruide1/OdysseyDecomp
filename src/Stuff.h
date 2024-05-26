#pragma once

#include "Library/Collision/IUseCollision.h"
#include "Library/Collision/KTriangle.h"
#include "MapObj/AnagramAlphabetCharacter.h"
#include "Player/IUsePlayerCollision.h"

namespace rs {

bool isCollisionCodePress(al::HitInfo*);
bool calcExistCollisionBorder(const al::IUseCollision*, const sead::Vector3f&,
                              const sead::Vector3f&);
bool isPlayer2D(const al::LiveActor*);

void calcJumpInertia(sead::Vector3f*, al::LiveActor*, const IUsePlayerCollision*,
                     const sead::Vector3f&, f32);
void calcJumpInertiaWall(sead::Vector3f*, al::LiveActor*, const IUsePlayerCollision*, f32);

bool isCollidedGround(const IUsePlayerCollision*);
bool isCollidedWall(const IUsePlayerCollision*);
bool isCollidedNoScaleVelocityWall(const IUsePlayerCollision*);

void calcMovePowerGround(sead::Vector3f*, const IUsePlayerCollision*, const sead::Vector3f&);
void calcMovePowerWall(sead::Vector3f*, const IUsePlayerCollision*, const sead::Vector3f&);

void calcGroundNormalOrUpDir(sead::Vector3f*, const al::LiveActor*, const IUsePlayerCollision*);
void calcGroundNormalOrGravityDir(sead::Vector3f*, const al::LiveActor*,
                                  const IUsePlayerCollision*);

const sead::Vector3f& getCollidedGroundNormal(const IUsePlayerCollision*);
const sead::Vector3f& getCollidedWallNormal(const IUsePlayerCollision*);

void scaleVelocityInertiaWallHit(al::LiveActor*, const IUsePlayerCollision*, float, float, float);

bool isHoldHackJump(IUsePlayerHack*);

void slerpUpFront(al::LiveActor*, const sead::Vector3f&, const sead::Vector3f&, float, float);
void slerpUp(al::LiveActor*, const sead::Vector3f&, float, float);
void slerpSkyPoseAir(al::LiveActor*, const sead::Vector3f&, const sead::Vector3f&, float);
void slerpSkyPoseAirSnapSide(al::LiveActor*, const sead::Vector3f&, const sead::Vector3f&, float,
                             float);
void slerpGroundStandFront(al::LiveActor*, const IUsePlayerCollision*, const sead::Vector3f&, float,
                           float);
void slerpGroundStand(al::LiveActor*, const IUsePlayerCollision*, float, float);
void slerpGravity(al::LiveActor*, float);
void slerpGravityVelH(al::LiveActor*, float);

}  // namespace rs
