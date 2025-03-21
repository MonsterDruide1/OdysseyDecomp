#pragma once

#include <math/seadBoundBox.h>
#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {
class LiveActor;
class HitSensor;
struct ActorParamMove;

void resetPosition(LiveActor* actor);
void resetPosition(LiveActor* actor, const sead::Vector3f& trans);
void resetRotatePosition(LiveActor* actor, const sead::Vector3f& rot, const sead::Vector3f& trans);
void resetQuatPosition(LiveActor* actor, const sead::Quatf& quat, const sead::Vector3f& trans);
void resetMtxPosition(LiveActor* actor, const sead::Matrix34f& mtx);
void resetActorPosition(LiveActor* actor, const LiveActor* target);
bool trySetPosOnGround(LiveActor* actor);
const sead::Vector3f& getVelocity(const LiveActor* actor);
sead::Vector3f* getVelocityPtr(LiveActor* actor);
void separateVelocityHV(sead::Vector3f* horizontal, sead::Vector3f* vertical, const LiveActor* actor);
void separateVelocityDirHV(sead::Vector3f* horizontal, sead::Vector3f* vertical, const LiveActor* actor,
                           const sead::Vector3f& dir);
void separateVelocityParallelVertical(sead::Vector3f* parallel, sead::Vector3f* vertical, const LiveActor* actor,
                                      const sead::Vector3f& dir);
void setVelocity(LiveActor* actor, const sead::Vector3f& vel);
void setVelocity(LiveActor* actor, f32 x, f32 y, f32 z);
void setVelocityX(LiveActor* actor, f32 x);
void setVelocityY(LiveActor* actor, f32 y);
void setVelocityZ(LiveActor* actor, f32 z);
void setVelocityZero(LiveActor* actor);
void setVelocityZeroX(LiveActor* actor);
void setVelocityZeroY(LiveActor* actor);
void setVelocityZeroZ(LiveActor* actor);
void setVelocityZeroH(LiveActor* actor);
void setVelocityZeroH(LiveActor* actor, const sead::Vector3f& gravity);
void setVelocityZeroV(LiveActor* actor);
void setVelocityZeroV(LiveActor* actor, const sead::Vector3f& gravity);
void setVelocityJump(LiveActor* actor, f32 speed);
void setVelocityToFront(LiveActor* actor, f32 speed);
void setVelocityToUp(LiveActor* actor, f32 speed);
void setVelocityToSide(LiveActor* actor, f32 speed);
void setVelocityToDirection(LiveActor* actor, const sead::Vector3f& dir, f32 speed);
void setVelocityToGravity(LiveActor* actor, f32 speed);
void setVelocitySeparateHV(LiveActor* actor, const sead::Vector3f& h, const sead::Vector3f& v, f32 speedH,
                           f32 speedV);
void setVelocitySeparateHV(LiveActor* actor, const sead::Vector3f& h, f32 speedH, f32 speedV);
void calcVelocitySeparateHV(sead::Vector3f* velocity, const LiveActor* actor, const sead::Vector3f& h, f32 speedH,
                            f32 speedV);
void setVelocitySeparateUp(LiveActor* actor, const sead::Vector3f& h, f32 speedH, f32 speedV);
void setVelocityOnlyDir(LiveActor* actor, const sead::Vector3f& dir, f32 speed);
void setVelocityOnlyV(LiveActor* actor, f32 speed);
void setVelocityOnlyGravity(LiveActor* actor, f32 speed);
void addVelocity(LiveActor* actor, const sead::Vector3f& vel);
void addVelocity(LiveActor* actor, f32 x, f32 y, f32 z);
void addVelocityX(LiveActor* actor, f32 x);
void addVelocityY(LiveActor* actor, f32 y);
void addVelocityZ(LiveActor* actor, f32 z);
void addVelocityDump(LiveActor* actor, const sead::Vector3f& dir, f32 force);
void addVelocityJump(LiveActor* actor, f32 force);
void addVelocityToFront(LiveActor* actor, f32 force);
void addVelocityToUp(LiveActor* actor, f32 force);
void addVelocityToSide(LiveActor* actor, f32 force);
void addVelocityToDown(LiveActor* actor, f32 force);
void addVelocityToDirection(LiveActor* actor, const sead::Vector3f& dir, f32 force);
void addVelocityToGravity(LiveActor* actor, f32 force);
void addVelocityToGravityLimit(LiveActor* actor, f32 force, f32 limit);
void addVelocityToGravityFittedGround(LiveActor* actor, f32 force, u32 maxAirTime);
void addVelocityToGravityNaturalOrFittedGround(LiveActor* actor, f32 force);
void calcGravityDir(sead::Vector3f* gravity, const LiveActor* actor);
void addVelocityToTarget(LiveActor* actor, const sead::Vector3f& target, f32 force);
void addVelocityToTarget(LiveActor* actor, const sead::Vector3f& target, f32 minForce, f32 maxForce, f32 minDistance, f32 maxDistance);
void addVelocityToTargetH(LiveActor* actor, const sead::Vector3f& target, f32 force);
void addVelocityToTargetHV(LiveActor* actor, const sead::Vector3f& target, f32 forceH, f32 forceV);
void addVelocityDampToTarget(LiveActor* actor, const sead::Vector3f& target, f32 forceDamp);
void addVelocityDampToTarget(LiveActor* actor, const sead::Vector3f& target, f32 force, f32 damp);
bool addVelocityToPlayer(LiveActor* actor, f32 force, const sead::Vector3f& offset);
bool addVelocityToPlayerHV(LiveActor* actor, f32 forceH, f32 forceV, const sead::Vector3f& offset);
void addVelocityFromTarget(LiveActor* actor, const sead::Vector3f& target, f32 force);
void addVelocityFromTargetHV(LiveActor* actor, const sead::Vector3f& target, f32 forceH, f32 forceV);
bool addVelocityFromPlayer(LiveActor* actor, f32 force, const sead::Vector3f& offset);
bool addVelocityFromPlayerHV(LiveActor* actor, f32 forceH, f32 forceV, const sead::Vector3f& offset);
void addVelocityClockwiseToDirection(LiveActor* actor, const sead::Vector3f& dir, f32 force);
bool calcVelocityClockwiseToDirection(LiveActor* actor, sead::Vector3f* dirVelocity, const sead::Vector3f& dir);
void addVelocityClockwiseToTarget(LiveActor* actor, const sead::Vector3f& target, f32 force);
void addVelocityJumpGroundInertia(LiveActor* actor, const sead::Vector3f& velocity, f32 force);
void tryAddVelocityLimit(LiveActor* actor, const sead::Vector3f& velocity, f32 limit);
void subVelocityExceptDirectionLimit(LiveActor* actor, const sead::Vector3f& direction, f32 subVel, f32 limit);
void scaleVelocity(LiveActor* actor, f32 factor);
void scaleVelocityLimit(LiveActor* actor, f32 factor, f32 limit);
void scaleVelocityX(LiveActor* actor, f32 factorX);
void scaleVelocityY(LiveActor* actor, f32 factorY);
void scaleVelocityZ(LiveActor* actor, f32 factorZ);
void scaleVelocityHV(LiveActor* actor, f32 factorH, f32 factorV);
void scaleVelocityDirection(LiveActor* actor, const sead::Vector3f& direction, f32 factor);
void scaleVelocityExceptDirection(LiveActor* actor, const sead::Vector3f& direction, f32 factor);
void scaleVelocityParallelVertical(LiveActor* actor, const sead::Vector3f& direction, f32 parallel, f32 vertical);
void limitVelocity(LiveActor* actor, f32 limit);
f32 calcSpeed(const LiveActor* actor);
void limitVelocityX(LiveActor* actor, f32 limitX);
void limitVelocityY(LiveActor* actor, f32 limitY);
void limitVelocityZ(LiveActor* actor, f32 limitZ);
void limitVelocityH(LiveActor* actor, f32 limitH);
void limitVelocityHV(LiveActor* actor, f32 limitH, f32 limitV);
void limitVelocityUpGravityH(LiveActor* actor, f32 limitDown, f32 limitUp, f32 limitH);
void limitVelocityDir(LiveActor* actor, const sead::Vector3f& dir, f32 limit);
void limitVelocityDirSign(LiveActor* actor, const sead::Vector3f& dir, f32 limit);
void limitVelocityDirV(LiveActor* actor, const sead::Vector3f& dir, f32 limit);
void limitVelocityDirVRate(LiveActor* actor, const sead::Vector3f& dir, f32 limit, f32 rate);
void limitVelocityParallelVertical(LiveActor* actor, const sead::Vector3f& dir, f32 parallel, f32 vertical);
void limitVelocitySeparateHV(LiveActor* actor, const sead::Vector3f& dir, f32 horizontal, f32 vertical);
u32 reboundVelocityPart(LiveActor* actor, f32 rebound, f32 threshold);
u32 reboundVelocityPart(LiveActor* actor, f32 ground, f32 wall, f32 ceiling, f32 threshold);
bool reboundVelocityFromEachCollision(LiveActor* actor, f32 ground, f32 wall, f32 ceiling, f32 threshold);
bool reboundVelocityFromCollision(LiveActor* actor, f32, f32, f32);
bool reboundVelocityFromTriangles(LiveActor* actor, f32, f32);
bool reboundVelocityFromActor(LiveActor* actor, const LiveActor* target, f32);
bool reboundVelocityFromActor(LiveActor* actor, const LiveActor* target, const sead::Vector3f&,
                              f32);
bool reboundVelocityFromSensor(LiveActor* actor, const HitSensor*, f32);
bool reboundVelocityFromSensor(LiveActor* actor, const HitSensor*, const sead::Vector3f&, f32);
bool calcDirToActor(sead::Vector3f* dir, const LiveActor* actor, const LiveActor* target);
bool reboundVelocityBetweenActor(LiveActor* actor, LiveActor* target, f32);
bool reboundVelocityBetweenSensor(HitSensor*, HitSensor*, f32);
void calcVelocityKeepLengthBetweenActor(sead::Vector3f*, const LiveActor* actor,
                                        const LiveActor* target, f32, f32);
void addVelocityKeepLengthBetweenActor(LiveActor* actor, LiveActor* target, f32, f32);
void addVelocityDumpKeepLengthBetweenActor(LiveActor* actor, LiveActor* target, f32, f32, f32);
void calcVelocityBlowAttack(sead::Vector3f* velocity, const LiveActor* actor, const sead::Vector3f& trans, f32 speedH, f32 speedV);
void addVelocityBlowAttack(LiveActor* actor, const sead::Vector3f& trans, f32 speedH, f32 speedV);
void addVelocityBlowAttack(LiveActor* actor, const HitSensor* sensor, f32 speedH, f32 speedV);
void setVelocityBlowAttack(LiveActor* actor, const sead::Vector3f& trans, f32 speedH, f32 speedV);
void setVelocityBlowAttack(LiveActor* actor, const HitSensor* sensor, f32 speedH, f32 speedV);
void setVelocityBlowAttackAndTurnToTarget(LiveActor* actor, const sead::Vector3f&, f32, f32);
bool isVelocityFast(const LiveActor* actor, f32 threshold);
bool isVelocityFastH(const LiveActor* actor, f32 threshold);
bool isVelocitySlow(const LiveActor* actor, f32 threshold);
bool isVelocitySlowH(const LiveActor* actor, f32 threshold);
f32 calcSpeedH(const LiveActor* actor);
f32 calcSpeedV(const LiveActor* actor);
f32 calcSpeedDirection(const LiveActor* actor, const sead::Vector3f& dir);
f32 calcSpeedExceptDir(const LiveActor* actor, const sead::Vector3f& dir);
bool isNear(const LiveActor* actor, const LiveActor* target, f32 threshold);
bool isNear(const LiveActor* actor, const sead::Vector3f& trans, f32 threshold);
bool isNearXZ(const LiveActor* actor, const sead::Vector3f& trans, f32 threshold);
bool isNearH(const LiveActor* actor, const sead::Vector3f& trans, f32 threshold);
f32 calcDistanceH(const LiveActor* actor, const sead::Vector3f& trans);
bool isNearV(const LiveActor* actor, const sead::Vector3f& trans, f32 threshold);
f32 calcDistanceV(const LiveActor* actor, const sead::Vector3f& trans);
bool isNearHV(const LiveActor* actor, const sead::Vector3f& trans, f32 threshH, f32 threshV);
bool isNearHV(const LiveActor* actor, const sead::Vector3f& trans, f32 threshH, f32 minV, f32 maxV);
f32 calcHeight(const LiveActor* actor, const sead::Vector3f& trans);
bool isFar(const LiveActor* actor, const LiveActor* target, f32 threshold);
bool isFar(const LiveActor* actor, const sead::Vector3f& trans, f32 threshold);
f32 calcDistance(const LiveActor* actor, const LiveActor* target);
f32 calcDistance(const LiveActor* actor, const sead::Vector3f& trans);
f32 calcDistanceV(const LiveActor* actor, const LiveActor* target);
f32 calcDistanceH(const LiveActor* actor, const LiveActor* target);
f32 calcDistanceH(const LiveActor* actor, const sead::Vector3f& trans1, const sead::Vector3f& trans2);
f32 calcHeight(const LiveActor* actor, const LiveActor* target);
f32 calcDistanceFront(const LiveActor* actor, const sead::Vector3f& trans);
f32 calcDistanceFront(const LiveActor* actor, const LiveActor* target);
void addRotateAndRepeatX(LiveActor* actor, f32);
void addRotateAndRepeatY(LiveActor* actor, f32);
void addRotateAndRepeatZ(LiveActor* actor, f32);
void addRandomRotateY(LiveActor* actor);
void calcQuatSide(sead::Vector3f*, const LiveActor* actor);
void calcQuatUp(sead::Vector3f*, const LiveActor* actor);
void calcQuatUp(sead::Vector3f*, const sead::Quatf& quat);
void calcQuatFront(sead::Vector3f*, const LiveActor* actor);
void calcQuatLocalAxis(sead::Vector3f*, const LiveActor* actor, s32);
void calcQuatLocalAxis(sead::Vector3f*, const sead::Quatf&, s32);
void calcTransOffsetFront(sead::Vector3f*, const LiveActor* actor, f32);
void calcTransOffsetUp(sead::Vector3f*, const LiveActor* actor, f32);
void calcTransOffsetSide(sead::Vector3f*, const LiveActor* actor, f32);
void setTransOffsetLocalDir(LiveActor* actor, const sead::Quatf&, const sead::Vector3f&, f32, s32);
void addTransOffsetLocal(LiveActor* actor, const sead::Vector3f&);
void addTransOffsetLocalDir(LiveActor* actor, f32, s32);
void rotateQuatXDirDegree(LiveActor* actor, f32);
void rotateQuatXDirDegree(LiveActor* actor, const sead::Quatf&, f32);
void rotateQuatYDirDegree(LiveActor* actor, f32);
void rotateQuatYDirDegree(LiveActor* actor, const sead::Quatf&, f32);
void rotateQuatZDirDegree(LiveActor* actor, f32);
void rotateQuatZDirDegree(LiveActor* actor, const sead::Quatf&, f32);
void rotateQuatLocalDirDegree(LiveActor* actor, s32, f32);
void rotateQuatLocalDirDegree(LiveActor* actor, const sead::Quatf&, s32, f32);
void rotateQuatLocalDirDegree(sead::Quatf* out, const sead::Quatf&, s32, f32);
void rotateQuatYDirRandomDegree(LiveActor* actor);
void rotateQuatYDirRandomDegree(LiveActor* actor, const sead::Quatf&);
void turnQuatFrontToDirDegreeH(LiveActor* actor, const sead::Vector3f&, f32);
void turnQuatFrontToPosDegreeH(LiveActor* actor, const sead::Vector3f&, f32);
void turnQuatFrontFromPosDegreeH(LiveActor* actor, const sead::Vector3f&, f32);
void turnFront(LiveActor* actor, f32);
void turnFront(LiveActor* actor, const sead::Vector3f&, f32);
void turnFrontToPos(LiveActor* actor, const sead::Vector3f&, f32);
void turnFrontToDir(LiveActor* actor, const sead::Vector3f&, f32);
void turnFrontToDirGetIsFinished(LiveActor* actor, const sead::Vector3f&, f32);
void turnDirectionDegree(const LiveActor* actor, sead::Vector3f*, const sead::Vector3f&, f32);
void turnFrontToTarget(LiveActor* actor, const LiveActor* target, f32);
void turnFrontFromTarget(LiveActor* actor, const LiveActor* target, f32);
void turnFrontToPlayer(LiveActor* actor, f32);
void turnFrontFromPlayer(LiveActor* actor, f32);
void turnDirection(const LiveActor* actor, sead::Vector3f*, const sead::Vector3f&, f32);
void turnDirectionToTarget(const LiveActor* actor, sead::Vector3f*, const sead::Vector3f&, f32);
void turnDirectionToTargetDegree(const LiveActor* actor, sead::Vector3f*, const sead::Vector3f&,
                                 f32);
void turnDirectionFromTargetDegree(const LiveActor* actor, sead::Vector3f*, const sead::Vector3f&,
                                   f32);
void turnDirectionAlongGround(const LiveActor* actor, sead::Vector3f*);
void turnDirectionAlongGround(LiveActor* actor);
void turnToDirectionAxis(LiveActor* actor, const sead::Vector3f&, const sead::Vector3f&, f32);
void turnFrontSpherical(const LiveActor* actor, sead::Vector3f*, const sead::Vector3f&, f32);
void turnFrontSphericalToTarget(const LiveActor* actor, sead::Vector3f*, const sead::Vector3f&,
                                f32);
void turnFrontSphericalToTargetDegree(const LiveActor* actor, sead::Vector3f*,
                                      const sead::Vector3f&, f32);
void turnToDirection(LiveActor* actor, const sead::Vector3f&, f32);
void turnLocalDirToDirection(LiveActor* actor, const sead::Vector3f&, const sead::Vector3f&, f32);
void turnToTarget(LiveActor* actor, const sead::Vector3f&, f32);
void turnToTarget(LiveActor* actor, const LiveActor* target, f32);
void faceToDirection(LiveActor* actor, const sead::Vector3f&);
void faceToDirectionSupportUp(LiveActor* actor, const sead::Vector3f&);
void faceToTarget(LiveActor* actor, const sead::Vector3f&);
void faceToTarget(LiveActor* actor, const LiveActor* target);
void faceToSensor(LiveActor* actor, const HitSensor*);
void faceToVelocity(LiveActor* actor);
void calcDirClockwiseToDir(sead::Vector3f*, const LiveActor* actor, const sead::Vector3f&);
void calcDirClockwiseToPos(sead::Vector3f*, const LiveActor* actor, const sead::Vector3f&);
void calcDirToActorH(sead::Vector3f*, const LiveActor* actor, const LiveActor* target);
void calcDirToActorH(sead::Vector3f*, const LiveActor* actor, const sead::Vector3f&);
void calcAngleToTargetH(const LiveActor* actor, const sead::Vector3f&);
void calcAngleToTargetV(const LiveActor* actor, const sead::Vector3f&);
bool isFaceToTargetDegree(const LiveActor* actor, const sead::Vector3f&, const sead::Vector3f&,
                          f32);
bool isFaceToTargetDegree(const LiveActor* actor, const sead::Vector3f&, f32);
bool isFaceToTargetDegreeHV(const LiveActor* actor, const sead::Vector3f&, const sead::Vector3f&,
                            f32, f32);
bool isFaceToTargetDegreeH(const LiveActor* actor, const sead::Vector3f&, const sead::Vector3f&,
                           f32);
bool isInSightCone(const LiveActor* actor, const sead::Vector3f&, const sead::Vector3f&, f32, f32);
bool isInSightConeTarget(const LiveActor* actor, const LiveActor* target, f32, f32);
bool isInSightConePlayer(const LiveActor* actor, f32, f32);
bool isInSightFan(const LiveActor* actor, const sead::Vector3f&, const sead::Vector3f&, f32, f32,
                  f32);
bool isInSightFanTarget(const LiveActor* actor, const LiveActor* target, f32, f32, f32);
bool isInSightBox(const LiveActor* actor, const sead::Vector3f&, const sead::BoundBox3f&);
void walkAndTurnToDirection(LiveActor* actor, const sead::Vector3f&, f32, f32, f32, f32, bool);
void walkAndTurnToDirection(LiveActor* actor, sead::Vector3f*, const sead::Vector3f&, f32, f32, f32,
                            f32, bool);
void walkAndTurnPoseToDirection(LiveActor* actor, const sead::Vector3f&, const ActorParamMove&,
                                bool);
void walkAndTurnToTarget(LiveActor* actor, const sead::Vector3f&, f32, f32, f32, f32, bool);
void flyAndTurnToDirection(LiveActor* actor, sead::Vector3f*, const sead::Vector3f&, f32, f32, f32,
                           f32);
void flyAndTurnToDirection(LiveActor* actor, const sead::Vector3f&, f32, f32, f32, f32);
void flyAndTurnToTarget(LiveActor* actor, const sead::Vector3f&, f32, f32, f32, f32);
void walkAndTurnToDirectionFittedGroundGravity(LiveActor* actor, sead::Vector3f*,
                                               const sead::Vector3f&, f32, f32, f32, f32, bool);
void walkAndTurnToDirectionFittedGroundGravity(LiveActor* actor, const sead::Vector3f&, f32, f32,
                                               f32, f32, bool);
void walkAndTurnToTargetFittedGroundGravity(LiveActor* actor, const sead::Vector3f&, f32, f32, f32,
                                            f32, bool);
bool tryKillByDeathArea(LiveActor* actor);
void rotateAndKeepColliderPosRate(LiveActor* actor, const sead::Vector3f&, const sead::Vector3f&,
                                  f32);
void calcSpringMovement(LiveActor* actor, const sead::Vector3f&, f32, f32, const sead::Vector3f&,
                        f32, f32);
void addVelocityClockwiseToPlayer(LiveActor* actor, f32);
void calcDirClockwiseToPlayer(sead::Vector3f*, const LiveActor* actor);
void flyAndTurnToPlayer(LiveActor* actor, const ActorParamMove&);
void escapeFromPlayer(LiveActor* actor, f32, f32, f32, f32);
void escapeFromPlayer(LiveActor* actor, sead::Vector3f*, f32, f32, f32, f32);
void walkAndTurnToPlayer(LiveActor* actor, f32, f32, f32, f32, bool);
bool isPlayerInSightFan(const LiveActor* actor, f32, f32, f32);
bool isFallOrDamageCodeNextMove(const LiveActor* actor, const sead::Vector3f&, f32, f32);
bool isFallNextMove(const LiveActor* actor, const sead::Vector3f&, f32, f32);

}  // namespace al
