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
void separateVelocityHV(sead::Vector3f* horizontal, sead::Vector3f* vertical,
                        const LiveActor* actor);
void separateVelocityDirHV(sead::Vector3f* horizontal, sead::Vector3f* vertical,
                           const LiveActor* actor, const sead::Vector3f& dir);
void separateVelocityParallelVertical(sead::Vector3f* parallel, sead::Vector3f* vertical,
                                      const LiveActor* actor, const sead::Vector3f& dir);
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
void setVelocitySeparateHV(LiveActor* actor, const sead::Vector3f& h, const sead::Vector3f& v,
                           f32 speedH, f32 speedV);
void setVelocitySeparateHV(LiveActor* actor, const sead::Vector3f& h, f32 speedH, f32 speedV);
void calcVelocitySeparateHV(sead::Vector3f* velocity, const LiveActor* actor,
                            const sead::Vector3f& h, f32 speedH, f32 speedV);
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
void addVelocityToTarget(LiveActor* actor, const sead::Vector3f& target, f32 minForce, f32 maxForce,
                         f32 minDistance, f32 maxDistance);
void addVelocityToTargetH(LiveActor* actor, const sead::Vector3f& target, f32 force);
void addVelocityToTargetHV(LiveActor* actor, const sead::Vector3f& target, f32 forceH, f32 forceV);
void addVelocityDampToTarget(LiveActor* actor, const sead::Vector3f& target, f32 forceDamp);
void addVelocityDampToTarget(LiveActor* actor, const sead::Vector3f& target, f32 force, f32 damp);
bool addVelocityToPlayer(LiveActor* actor, f32 force, const sead::Vector3f& offset);
bool addVelocityToPlayerHV(LiveActor* actor, f32 forceH, f32 forceV, const sead::Vector3f& offset);
void addVelocityFromTarget(LiveActor* actor, const sead::Vector3f& target, f32 force);
void addVelocityFromTargetHV(LiveActor* actor, const sead::Vector3f& target, f32 forceH,
                             f32 forceV);
bool addVelocityFromPlayer(LiveActor* actor, f32 force, const sead::Vector3f& offset);
bool addVelocityFromPlayerHV(LiveActor* actor, f32 forceH, f32 forceV,
                             const sead::Vector3f& offset);
void addVelocityClockwiseToDirection(LiveActor* actor, const sead::Vector3f& dir, f32 force);
bool calcVelocityClockwiseToDirection(LiveActor* actor, sead::Vector3f* dirVelocity,
                                      const sead::Vector3f& dir);
void addVelocityClockwiseToTarget(LiveActor* actor, const sead::Vector3f& target, f32 force);
void addVelocityJumpGroundInertia(LiveActor* actor, const sead::Vector3f& velocity, f32 force);
void tryAddVelocityLimit(LiveActor* actor, const sead::Vector3f& velocity, f32 limit);
void subVelocityExceptDirectionLimit(LiveActor* actor, const sead::Vector3f& direction, f32 subVel,
                                     f32 limit);
void scaleVelocity(LiveActor* actor, f32 factor);
void scaleVelocityLimit(LiveActor* actor, f32 factor, f32 limit);
void scaleVelocityX(LiveActor* actor, f32 factorX);
void scaleVelocityY(LiveActor* actor, f32 factorY);
void scaleVelocityZ(LiveActor* actor, f32 factorZ);
void scaleVelocityHV(LiveActor* actor, f32 factorH, f32 factorV);
void scaleVelocityDirection(LiveActor* actor, const sead::Vector3f& direction, f32 factor);
void scaleVelocityExceptDirection(LiveActor* actor, const sead::Vector3f& direction, f32 factor);
void scaleVelocityParallelVertical(LiveActor* actor, const sead::Vector3f& direction, f32 parallel,
                                   f32 vertical);
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
void limitVelocityParallelVertical(LiveActor* actor, const sead::Vector3f& dir, f32 parallel,
                                   f32 vertical);
void limitVelocitySeparateHV(LiveActor* actor, const sead::Vector3f& dir, f32 horizontal,
                             f32 vertical);
u32 reboundVelocityPart(LiveActor* actor, f32 rebound, f32 threshold);
u32 reboundVelocityPart(LiveActor* actor, f32 ground, f32 wall, f32 ceiling, f32 threshold);
bool reboundVelocityFromEachCollision(LiveActor* actor, f32 ground, f32 wall, f32 ceiling,
                                      f32 threshold);
bool reboundVelocityFromCollision(LiveActor* actor, f32 reboundStrength, f32 reboundMin,
                                  f32 friction);
bool reboundVelocityFromTriangles(LiveActor* actor, f32 reboundStrength, f32 reboundMin);
bool reboundVelocityFromActor(LiveActor* actor, const LiveActor* target, f32 reboundStrength);
bool reboundVelocityFromActor(LiveActor* actor, const LiveActor* target,
                              const sead::Vector3f& targetVelocity, f32 reboundStrength);
bool reboundVelocityFromSensor(LiveActor* actor, const HitSensor* sensor, f32 reboundStrength);
bool reboundVelocityFromSensor(LiveActor* actor, const HitSensor* sensor,
                               const sead::Vector3f& targetVelocity, f32 reboundStrength);
bool calcDirToActor(sead::Vector3f* dir, const LiveActor* actor, const LiveActor* target);
bool reboundVelocityBetweenActor(LiveActor* actor, LiveActor* target, f32 reboundStrength);
bool reboundVelocityBetweenSensor(HitSensor* sensorA, HitSensor* sensorB, f32 reboundStrength);
void calcVelocityKeepLengthBetweenActor(sead::Vector3f* vel, const LiveActor* actor,
                                        const LiveActor* target, f32 targetLength, f32 force);
void addVelocityKeepLengthBetweenActor(LiveActor* actor, LiveActor* target, f32 targetLength,
                                       f32 force);
void addVelocityDumpKeepLengthBetweenActor(LiveActor* actor, LiveActor* target, f32 targetLength,
                                           f32 lenForce, f32 dumpForce);
void calcVelocityBlowAttack(sead::Vector3f* velocity, const LiveActor* actor,
                            const sead::Vector3f& trans, f32 speedH, f32 speedV);
void addVelocityBlowAttack(LiveActor* actor, const sead::Vector3f& trans, f32 speedH, f32 speedV);
void addVelocityBlowAttack(LiveActor* actor, const HitSensor* sensor, f32 speedH, f32 speedV);
void setVelocityBlowAttack(LiveActor* actor, const sead::Vector3f& trans, f32 speedH, f32 speedV);
void setVelocityBlowAttack(LiveActor* actor, const HitSensor* sensor, f32 speedH, f32 speedV);
void setVelocityBlowAttackAndTurnToTarget(LiveActor* actor, const sead::Vector3f& target,
                                          f32 speedH, f32 speedV);
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
f32 calcDistanceH(const LiveActor* actor, const sead::Vector3f& trans1,
                  const sead::Vector3f& trans2);
f32 calcHeight(const LiveActor* actor, const LiveActor* target);
f32 calcDistanceFront(const LiveActor* actor, const sead::Vector3f& trans);
f32 calcDistanceFront(const LiveActor* actor, const LiveActor* target);
void addRotateAndRepeatX(LiveActor* actor, f32 deg);
void addRotateAndRepeatY(LiveActor* actor, f32 deg);
void addRotateAndRepeatZ(LiveActor* actor, f32 deg);
void addRandomRotateY(LiveActor* actor);
void calcQuatSide(sead::Vector3f* side, const LiveActor* actor);
void calcQuatUp(sead::Vector3f* up, const LiveActor* actor);
void calcQuatFront(sead::Vector3f* front, const LiveActor* actor);
void calcQuatLocalAxis(sead::Vector3f* local, const LiveActor* actor, s32 axis);
void calcTransOffsetFront(sead::Vector3f* offset, const LiveActor* actor, f32 len);
void calcTransOffsetUp(sead::Vector3f* offset, const LiveActor* actor, f32 len);
void calcTransOffsetSide(sead::Vector3f* offset, const LiveActor* actor, f32 len);
void setTransOffsetLocalDir(LiveActor* actor, const sead::Quatf& quat,
                            const sead::Vector3f& globalOffset, f32 localOffset, s32 axis);
void addTransOffsetLocal(LiveActor* actor, const sead::Vector3f& localOffset);
void addTransOffsetLocalDir(LiveActor* actor, f32 localOffset, s32 axis);
void rotateQuatXDirDegree(LiveActor* actor, f32 deg);
void rotateQuatXDirDegree(LiveActor* actor, const sead::Quatf& quat, f32 deg);
void rotateQuatYDirDegree(LiveActor* actor, f32 deg);
void rotateQuatYDirDegree(LiveActor* actor, const sead::Quatf& quat, f32 deg);
void rotateQuatZDirDegree(LiveActor* actor, f32 deg);
void rotateQuatZDirDegree(LiveActor* actor, const sead::Quatf& quat, f32 deg);
void rotateQuatLocalDirDegree(LiveActor* actor, s32 axis, f32 deg);
void rotateQuatLocalDirDegree(LiveActor* actor, const sead::Quatf& quat, s32 axis, f32 deg);
void rotateQuatYDirRandomDegree(LiveActor* actor);
void rotateQuatYDirRandomDegree(LiveActor* actor, const sead::Quatf& quat);
bool turnQuatFrontToDirDegreeH(LiveActor* actor, const sead::Vector3f& dir, f32 deg);
bool turnQuatFrontToPosDegreeH(LiveActor* actor, const sead::Vector3f& pos, f32 deg);
bool turnQuatFrontFromPosDegreeH(LiveActor* actor, const sead::Vector3f& pos, f32 deg);
void turnFront(LiveActor* actor, f32 deg);
void turnFront(LiveActor* actor, const sead::Vector3f& up, f32 deg);
void turnFrontToPos(LiveActor* actor, const sead::Vector3f& pos, f32 deg);
void turnFrontToDir(LiveActor* actor, const sead::Vector3f& dir, f32 deg);
bool turnFrontToDirGetIsFinished(LiveActor* actor, const sead::Vector3f& dir, f32 deg);
bool turnDirectionDegree(const LiveActor* actor, sead::Vector3f* vec, const sead::Vector3f& dir,
                         f32 deg);
void turnFrontToTarget(LiveActor* actor, const LiveActor* target, f32 deg);
void turnFrontFromTarget(LiveActor* actor, const LiveActor* target, f32 deg);
bool turnFrontToPlayer(LiveActor* actor, f32 deg);
bool turnFrontFromPlayer(LiveActor* actor, f32 deg);
bool turnDirection(const LiveActor* actor, sead::Vector3f* vec, const sead::Vector3f& dir, f32 cos);
bool turnDirectionToTarget(const LiveActor* actor, sead::Vector3f* vec,
                           const sead::Vector3f& target, f32 cos);
bool turnDirectionToTargetDegree(const LiveActor* actor, sead::Vector3f* vec,
                                 const sead::Vector3f& target, f32 deg);
bool turnDirectionFromTargetDegree(const LiveActor* actor, sead::Vector3f* vec,
                                   const sead::Vector3f& target, f32 deg);
void turnDirectionAlongGround(const LiveActor* actor, sead::Vector3f* dir);
void turnDirectionAlongGround(LiveActor* actor);
bool turnToDirectionAxis(LiveActor* actor, const sead::Vector3f& horizontal,
                         const sead::Vector3f& vertical, f32 deg);
bool turnFrontSpherical(const LiveActor* actor, sead::Vector3f* vec,
                        const sead::Vector3f& targetFront, f32 cos);
bool turnFrontSphericalToTarget(const LiveActor* actor, sead::Vector3f* vec,
                                const sead::Vector3f& target, f32 cos);
bool turnFrontSphericalToTargetDegree(const LiveActor* actor, sead::Vector3f* vec,
                                      const sead::Vector3f& target, f32 deg);
bool turnToDirection(LiveActor* actor, const sead::Vector3f& dir, f32 deg);
void turnLocalDirToDirection(LiveActor* actor, const sead::Vector3f& localDir,
                             const sead::Vector3f& targetDir, f32 deg);
bool turnToTarget(LiveActor* actor, const sead::Vector3f& target, f32 deg);
bool turnToTarget(LiveActor* actor, const LiveActor* target, f32 deg);
void faceToDirection(LiveActor* actor, const sead::Vector3f& dir);
void faceToDirectionSupportUp(LiveActor* actor, const sead::Vector3f& dir);
void faceToTarget(LiveActor* actor, const sead::Vector3f& target);
void faceToTarget(LiveActor* actor, const LiveActor* target);
void faceToSensor(LiveActor* actor, const HitSensor* sensor);
void faceToVelocity(LiveActor* actor);
void calcDirClockwiseToDir(sead::Vector3f* out, const LiveActor* actor, const sead::Vector3f& dir);
void calcDirClockwiseToPos(sead::Vector3f* out, const LiveActor* actor,
                           const sead::Vector3f& target);
void calcDirToActorH(sead::Vector3f* out, const LiveActor* actor, const LiveActor* target);
void calcDirToActorH(sead::Vector3f* out, const LiveActor* actor, const sead::Vector3f& target);
f32 calcAngleToTargetH(const LiveActor* actor, const sead::Vector3f& target);
f32 calcAngleToTargetV(const LiveActor* actor, const sead::Vector3f& target);
bool isFaceToTargetDegree(const LiveActor* actor, const sead::Vector3f& target,
                          const sead::Vector3f& face, f32 threshDeg);
bool isFaceToTargetDegree(const LiveActor* actor, const sead::Vector3f& target, f32 threshDeg);
bool isFaceToTargetDegreeHV(const LiveActor* actor, const sead::Vector3f& target,
                            const sead::Vector3f& face, f32 degH, f32 degV);
bool isFaceToTargetDegreeH(const LiveActor* actor, const sead::Vector3f& target,
                           const sead::Vector3f& face, f32 degH);
bool isInSightCone(const LiveActor* actor, const sead::Vector3f& target, const sead::Vector3f& face,
                   f32 maxDist, f32 threshDeg);
bool isInSightConeTarget(const LiveActor* actor, const LiveActor* target, f32 maxDist,
                         f32 threshDeg);
bool isInSightConePlayer(const LiveActor* actor, f32 maxDist, f32 threshDeg);
bool isInSightFan(const LiveActor* actor, const sead::Vector3f& target, const sead::Vector3f& face,
                  f32 maxDist, f32 angleH, f32 angleV);
bool isInSightFanTarget(const LiveActor* actor, const LiveActor* target, f32 maxDist, f32 angleH,
                        f32 angleV);
bool isInSightBox(const LiveActor* actor, const sead::Vector3f& pos, const sead::BoundBox3f& box);
void walkAndTurnToDirection(LiveActor* actor, const sead::Vector3f& dir, f32 forceFront,
                            f32 forceGravity, f32 decay, f32 deg, bool turnAlongGround);
void walkAndTurnToDirection(LiveActor* actor, sead::Vector3f* front, const sead::Vector3f& dir,
                            f32 forceFront, f32 forceGravity, f32 decay, f32 deg,
                            bool turnAlongGround);
void walkAndTurnPoseToDirection(LiveActor* actor, const sead::Vector3f& dir,
                                const ActorParamMove& param, bool turnAlongGround);
void walkAndTurnToTarget(LiveActor* actor, const sead::Vector3f& target, f32 forceFront,
                         f32 forceGravity, f32 decay, f32 deg, bool turnAlongGround);
void flyAndTurnToDirection(LiveActor* actor, sead::Vector3f* front, const sead::Vector3f& dir,
                           f32 forceFront, f32 forceGravity, f32 decay, f32 deg);
void flyAndTurnToDirection(LiveActor* actor, const sead::Vector3f& dir, f32 forceFront,
                           f32 forceGravity, f32 decay, f32 deg);
void flyAndTurnToTarget(LiveActor* actor, const sead::Vector3f& target, f32 forceFront,
                        f32 forceGravity, f32 decay, f32 deg);
bool walkAndTurnToDirectionFittedGroundGravity(LiveActor* actor, sead::Vector3f* front,
                                               const sead::Vector3f& dir, f32 forceFront,
                                               f32 forceGravity, f32 decay, f32 deg,
                                               bool turnAlongGround);
bool walkAndTurnToDirectionFittedGroundGravity(LiveActor* actor, const sead::Vector3f& dir,
                                               f32 forceFront, f32 forceGravity, f32 decay, f32 deg,
                                               bool turnAlongGround);
bool walkAndTurnToTargetFittedGroundGravity(LiveActor* actor, const sead::Vector3f& target,
                                            f32 forceFront, f32 forceGravity, f32 decay, f32 deg,
                                            bool turnAlongGround);
bool tryKillByDeathArea(LiveActor* actor);
void rotateAndKeepColliderPosRate(LiveActor* actor, const sead::Vector3f& up,
                                  const sead::Vector3f& front, f32 rate);
void calcSpringMovement(LiveActor* actor, const sead::Vector3f& pos, f32 springPos, f32 sinStrength,
                        const sead::Vector3f& offset, f32 constStrength, f32 sinAmpl);
bool addVelocityClockwiseToPlayer(LiveActor* actor, f32 force);
bool calcDirClockwiseToPlayer(sead::Vector3f* dir, const LiveActor* actor);
bool flyAndTurnToPlayer(LiveActor* actor, const ActorParamMove& param);
bool escapeFromPlayer(LiveActor* actor, f32 forceFront, f32 forceGravity, f32 decay, f32 deg);
bool escapeFromPlayer(LiveActor* actor, sead::Vector3f* front, f32 forceFront, f32 forceGravity,
                      f32 decay, f32 deg);
bool walkAndTurnToPlayer(LiveActor* actor, f32 forceFront, f32 forceGravity, f32 decay, f32 deg,
                         bool turnAlongGround);
bool isPlayerInSightFan(const LiveActor* actor, f32 maxDist, f32 angleH, f32 angleV);
bool isFallOrDamageCodeNextMove(const LiveActor* actor, const sead::Vector3f& velocity, f32 gravity,
                                f32 searchDist);
bool isFallNextMove(const LiveActor* actor, const sead::Vector3f& velocity, f32 gravity,
                    f32 searchDist);

}  // namespace al
