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
void separateVelocityHV(sead::Vector3f*, sead::Vector3f*, const LiveActor* actor);
void separateVelocityDirHV(sead::Vector3f*, sead::Vector3f*, const LiveActor* actor,
                           const sead::Vector3f&);
void separateVelocityParallelVertical(sead::Vector3f*, sead::Vector3f*, const LiveActor* actor,
                                      const sead::Vector3f&);
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
void setVelocityZeroH(LiveActor* actor, const sead::Vector3f&);
void setVelocityZeroV(LiveActor* actor);
void setVelocityZeroV(LiveActor* actor, const sead::Vector3f&);
void setVelocityJump(LiveActor* actor, f32);
void setVelocityToFront(LiveActor* actor, f32);
void setVelocityToUp(LiveActor* actor, f32);
void setVelocityToSide(LiveActor* actor, f32);
void setVelocityToDirection(LiveActor* actor, const sead::Vector3f&, f32);
void setVelocityToGravity(LiveActor* actor, f32);
void setVelocitySeparateHV(LiveActor* actor, const sead::Vector3f&, const sead::Vector3f&, f32,
                           f32);
void setVelocitySeparateHV(LiveActor* actor, const sead::Vector3f&, f32, f32);
void calcVelocitySeparateHV(sead::Vector3f*, const LiveActor* actor, const sead::Vector3f&, f32,
                            f32);
void setVelocitySeparateUp(LiveActor* actor, const sead::Vector3f&, f32, f32);
void setVelocityOnlyDir(LiveActor* actor, const sead::Vector3f&, f32);
void setVelocityOnlyV(LiveActor* actor, f32);
void setVelocityOnlyGravity(LiveActor* actor, f32);
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
void addVelocityToGravity(LiveActor* actor, f32);
void addVelocityToGravityLimit(LiveActor* actor, f32, f32);
void addVelocityToGravityFittedGround(LiveActor* actor, f32, u32);
void addVelocityToGravityNaturalOrFittedGround(LiveActor* actor, f32);
void calcGravityDir(sead::Vector3f*, const LiveActor* actor);
void addVelocityToTarget(LiveActor* actor, const sead::Vector3f&, f32);
void addVelocityToTarget(LiveActor* actor, const sead::Vector3f&, f32, f32, f32, f32);
void addVelocityToTargetH(LiveActor* actor, const sead::Vector3f&, f32);
void addVelocityToTargetHV(LiveActor* actor, const sead::Vector3f&, f32, f32);
void addVelocityDampToTarget(LiveActor* actor, const sead::Vector3f&, f32);
void addVelocityDampToTarget(LiveActor* actor, const sead::Vector3f&, f32, f32);
void addVelocityToPlayer(LiveActor* actor, const f32, sead::Vector3f&);
void addVelocityToPlayerHV(LiveActor* actor, const f32, f32, sead::Vector3f&);
void addVelocityFromTarget(LiveActor* actor, const sead::Vector3f&, f32);
void addVelocityFromTargetHV(LiveActor* actor, const sead::Vector3f&, f32, f32);
void addVelocityFromPlayer(LiveActor* actor, const f32, sead::Vector3f&);
void addVelocityFromPlayerHV(LiveActor* actor, const f32, f32, sead::Vector3f&);
void addVelocityClockwiseToDirection(LiveActor* actor, const sead::Vector3f&, f32);
void calcVelocityClockwiseToDirection(LiveActor* actor, sead::Vector3f*, const sead::Vector3f&);
void addVelocityClockwiseToTarget(LiveActor* actor, const sead::Vector3f&, f32);
void addVelocityJumpGroundInertia(LiveActor* actor, const sead::Vector3f&, f32);
bool tryAddVelocityLimit(LiveActor* actor, const sead::Vector3f&, f32);
void subVelocityExceptDirectionLimit(LiveActor* actor, const sead::Vector3f&, f32, f32);
void scaleVelocity(LiveActor* actor, f32);
void scaleVelocityLimit(LiveActor* actor, f32, f32);
void scaleVelocityX(LiveActor* actor, f32);
void scaleVelocityY(LiveActor* actor, f32);
void scaleVelocityZ(LiveActor* actor, f32);
void scaleVelocityHV(LiveActor* actor, f32, f32);
void scaleVelocityDirection(LiveActor* actor, const sead::Vector3f&, f32);
void scaleVelocityExceptDirection(LiveActor* actor, const sead::Vector3f&, f32);
void scaleVelocityParallelVertical(LiveActor* actor, const sead::Vector3f&, f32, f32);
void limitVelocity(LiveActor* actor, f32);
f32 calcSpeed(const LiveActor* actor);
void limitVelocityX(LiveActor* actor, f32);
void limitVelocityY(LiveActor* actor, f32);
void limitVelocityZ(LiveActor* actor, f32);
void limitVelocityH(LiveActor* actor, f32);
void limitVelocityHV(LiveActor* actor, f32, f32);
void limitVelocityUpGravityH(LiveActor* actor, f32, f32, f32);
void limitVelocityDir(LiveActor* actor, const sead::Vector3f&, f32);
void limitVelocityDirSign(LiveActor* actor, const sead::Vector3f&, f32);
void limitVelocityDirV(LiveActor* actor, const sead::Vector3f&, f32);
void limitVelocityDirVRate(LiveActor* actor, const sead::Vector3f&, f32, f32);
void limitVelocityParallelVertical(LiveActor* actor, const sead::Vector3f&, f32, f32);
void limitVelocitySeparateHV(LiveActor* actor, const sead::Vector3f&, f32, f32);
void reboundVelocityPart(LiveActor* actor, f32, f32);
void reboundVelocityPart(LiveActor* actor, f32, f32, f32, f32);
bool reboundVelocityFromEachCollision(LiveActor* actor, f32, f32, f32, f32);
bool reboundVelocityFromCollision(LiveActor* actor, f32, f32, f32);
void reboundVelocityFromTriangles(LiveActor* actor, f32, f32);
void reboundVelocityFromActor(LiveActor* actor, const LiveActor* target, f32);
void reboundVelocityFromActor(LiveActor* actor, const LiveActor* target, const sead::Vector3f&,
                              f32);
void reboundVelocityFromSensor(LiveActor* actor, const HitSensor*, f32);
void reboundVelocityFromSensor(LiveActor* actor, const HitSensor*, const sead::Vector3f&, f32);
void calcDirToActor(sead::Vector3f*, const LiveActor* actor, const LiveActor* target);
void reboundVelocityBetweenActor(LiveActor* actor, LiveActor* target, f32);
void reboundVelocityBetweenSensor(HitSensor*, HitSensor*, f32);
void calcVelocityKeepLengthBetweenActor(sead::Vector3f*, const LiveActor* actor,
                                        const LiveActor* target, f32, f32);
void addVelocityKeepLengthBetweenActor(LiveActor* actor, LiveActor* target, f32, f32);
void addVelocityDumpKeepLengthBetweenActor(LiveActor* actor, LiveActor* target, f32, f32, f32);
void calcVelocityBlowAttack(sead::Vector3f*, const LiveActor* actor, const sead::Vector3f&, f32,
                            f32);
void addVelocityBlowAttack(LiveActor* actor, const sead::Vector3f&, f32, f32);
void addVelocityBlowAttack(LiveActor* actor, const HitSensor*, f32, f32);
void setVelocityBlowAttack(LiveActor* actor, const sead::Vector3f&, f32, f32);
void setVelocityBlowAttack(LiveActor* actor, const HitSensor*, f32, f32);
void setVelocityBlowAttackAndTurnToTarget(LiveActor* actor, const sead::Vector3f&, f32, f32);
bool isVelocityFast(const LiveActor* actor, f32);
bool isVelocityFastH(const LiveActor* actor, f32);
bool isVelocitySlow(const LiveActor* actor, f32);
bool isVelocitySlowH(const LiveActor* actor, f32);
f32 calcSpeedH(const LiveActor* actor);
f32 calcSpeedV(const LiveActor* actor);
void calcSpeedDirection(const LiveActor* actor, const sead::Vector3f&);
void calcSpeedExceptDir(const LiveActor* actor, const sead::Vector3f&);
bool isNear(const LiveActor* actor, const LiveActor* target, f32);
bool isNear(const LiveActor* actor, const sead::Vector3f&, f32);
bool isNearXZ(const LiveActor* actor, const sead::Vector3f&, f32);
bool isNearH(const LiveActor* actor, const sead::Vector3f&, f32);
f32 calcDistanceH(const LiveActor* actor, const sead::Vector3f&);
bool isNearV(const LiveActor* actor, const sead::Vector3f&, f32);
f32 calcDistanceV(const LiveActor* actor, const sead::Vector3f&);
bool isNearHV(const LiveActor* actor, const sead::Vector3f&, f32, f32);
bool isNearHV(const LiveActor* actor, const sead::Vector3f&, f32, f32, f32);
void calcHeight(const LiveActor* actor, const sead::Vector3f&);
bool isFar(const LiveActor* actor, const LiveActor* target, f32);
bool isFar(const LiveActor* actor, const sead::Vector3f&, f32);
f32 calcDistance(const LiveActor* actor, const LiveActor* target);
f32 calcDistance(const LiveActor* actor, const sead::Vector3f&);
f32 calcDistanceV(const LiveActor* actor, const LiveActor* target);
f32 calcDistanceH(const LiveActor* actor, const LiveActor* target);
f32 calcDistanceH(const LiveActor* actor, const sead::Vector3f&, const sead::Vector3f&);
f32 calcHeight(const LiveActor* actor, const LiveActor* target);
void calcDistanceFront(const LiveActor* actor, const sead::Vector3f&);
void calcDistanceFront(const LiveActor* actor, const LiveActor* target);
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
void calcSpringMovement(LiveActor* actor, const sead::Vector3f&, const f32, f32, sead::Vector3f&,
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
