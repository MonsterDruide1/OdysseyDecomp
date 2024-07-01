#pragma once

#include "Library/Collision/IUseCollision.h"
#include "Library/Collision/KTriangle.h"
#include "MapObj/AnagramAlphabetCharacter.h"
#include "Player/IUsePlayerCollision.h"
#include "Player/PlayerActionGroundMoveControl.h"
#include "Player/PlayerCollider.h"
#include "Player/PlayerConst.h"
#include "Player/PlayerTrigger.h"

namespace rs {

bool isCollisionCodePress(al::HitInfo*);
bool calcExistCollisionBorder(const al::IUseCollision*, const sead::Vector3f&,
                              const sead::Vector3f&);
bool isPlayer2D(const al::LiveActor*);
bool isPlayer3D(const al::LiveActor*);

void calcJumpInertia(sead::Vector3f*, al::LiveActor*, const IUsePlayerCollision*,
                     const sead::Vector3f&, f32);
void calcJumpInertiaWall(sead::Vector3f*, al::LiveActor*, const IUsePlayerCollision*, f32);

bool isCollidedGround(const IUsePlayerCollision*);
bool isCollidedWall(const IUsePlayerCollision*);
bool isCollidedNoScaleVelocityWall(const IUsePlayerCollision*);

bool isOnGround(al::LiveActor const*,IUsePlayerCollision const*);
bool isOnGroundRunAngle(al::LiveActor const*,IUsePlayerCollision const*,PlayerConst const*);

void calcMovePowerGround(sead::Vector3f*, const IUsePlayerCollision*, const sead::Vector3f&);
void calcMovePowerWall(sead::Vector3f*, const IUsePlayerCollision*, const sead::Vector3f&);

void calcGroundNormalOrUpDir(sead::Vector3f*, const al::LiveActor*, const IUsePlayerCollision*);
void calcGroundNormalOrGravityDir(sead::Vector3f*, const al::LiveActor*,
                                  const IUsePlayerCollision*);

const sead::Vector3f& getCollidedGroundPos(const IUsePlayerCollision*);
const sead::Vector3f& getCollidedGroundNormal(const IUsePlayerCollision*);
const sead::Vector3f& getCollidedWallNormal(const IUsePlayerCollision*);

bool isLandGroundRunAngle(al::LiveActor const*,IUsePlayerCollision const*,PlayerConst const*);
bool isJustLand(const IUsePlayerCollision*);

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

void cutVerticalVelocityGroundNormal(al::LiveActor*, const IUsePlayerCollision*);

bool tryFindSnapMoveAreaDir(sead::Vector3<float> *,al::LiveActor const*,IUsePlayerCollision const*);

void addPlayerJumpCount(const al::LiveActor*);

void waitGround(al::LiveActor *,IUsePlayerCollision const*,float,float,float,float);
void landGroundPoseAndSnap(al::LiveActor *,PlayerTrigger *,float *,IUsePlayerCollision const*,PlayerConst const*);
bool reboundVelocityFromCollision(al::LiveActor *,IUsePlayerCollision const*,float,float,float);
void calcCollidedNormalSum(sead::Vector3<float> *,IUsePlayerCollision const*);

al::CollisionPartsFilterBase* createCollisionPartsFilter2DOnly();

al::HitSensor* tryGetCollidedWallSensor(IUsePlayerCollision const*);
al::HitSensor* tryGetCollidedGroundSensor(IUsePlayerCollision const*);
bool calcGroundHeight(float *,sead::Vector3<float> *,al::IUseCollision const*,sead::Vector3<float> const&,sead::Vector3<float> const&,float,float);

bool convergeOnGroundCount(int *,al::LiveActor const*,IUsePlayerCollision const*,int,int);

void startHitReactionHipDropLand(al::LiveActor *,bool);

void moveInertiaSlide(sead::Vector3<float> *,al::LiveActor *,IUsePlayerCollision const*,sead::Vector3<float> const&,float,float,float,float,float,float,float);
void moveParallelJump(al::LiveActor *,sead::Vector3<float> const&,float,float,float,float,float,float,float);
f32 moveBrakeRun(float *,sead::Vector3<float> *,al::LiveActor *,PlayerActionGroundMoveControl *,float,int,float,float,float);
void moveDivingJump(al::LiveActor *,sead::Vector3<float> const&,float,float,float,float,float,float,float,float);
bool isOnGroundSlopeSlideEnd(al::LiveActor const*,IUsePlayerCollision const*,PlayerConst const*);

void controlDirectionalVelocity(sead::Vector3<float> *,al::LiveActor const*,sead::Vector3<float> const&,float,float,float,float,float);
bool calcExistCollisionBorderWallCatch(al::IUseCollision const*,sead::Vector3<float> const&,sead::Vector3<float> const&,sead::Vector3<float> const&);

void sendMsgPlayerStartWallJump(al::HitSensor*,al::HitSensor*);

void resetCollisionExpandCheck(IUsePlayerCollision *);

void calcLockDirection(sead::Vector3f *,const IUseDimension *);

bool tryGetPlayerInputWallAlongWallNormal(sead::Vector3<float> *,al::LiveActor const*);
void faceToCamera(al::LiveActor *);

}  // namespace rs
