#pragma once

#include <math/seadBoundBox.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {

class LiveActor;
class HitSensor;
class SensorMsg;
class ActorInitInfo;
class SensorSortCmpFuncBase;
class ActorSensorController;

HitSensor* addHitSensor(LiveActor*, const ActorInitInfo&, const char*, u32, f32, u16,
                        const sead::Vector3f&);

HitSensor* addHitSensorPlayer(LiveActor*, const ActorInitInfo&, const char*, f32, u16,
                              const sead::Vector3f&);

HitSensor* addHitSensorPlayerAttack(LiveActor*, const ActorInitInfo&, const char*, f32, u16,
                                    const sead::Vector3f&);
HitSensor* addHitSensorPlayerEye(LiveActor*, const ActorInitInfo&, const char*, f32, u16,
                                 const sead::Vector3f&);
HitSensor* addHitSensorEnemy(LiveActor*, const ActorInitInfo&, const char*, f32, u16,
                             const sead::Vector3f&);
HitSensor* addHitSensorEnemyBody(LiveActor*, const ActorInitInfo&, const char*, f32, u16,
                                 const sead::Vector3f&);
HitSensor* addHitSensorEnemyAttack(LiveActor*, const ActorInitInfo&, const char*, f32, u16,
                                   const sead::Vector3f&);
HitSensor* addHitSensorMapObj(LiveActor*, const ActorInitInfo&, const char*, f32, u16,
                              const sead::Vector3f&);
HitSensor* addHitSensorBindable(LiveActor*, const ActorInitInfo&, const char*, f32, u16,
                                const sead::Vector3f&);
HitSensor* addHitSensorBindableGoal(LiveActor*, const ActorInitInfo&, const char*, f32, u16,
                                    const sead::Vector3f&);
HitSensor* addHitSensorBindableAllPlayer(LiveActor*, const ActorInitInfo&, const char*, f32, u16,
                                         const sead::Vector3f&);
HitSensor* addHitSensorBindableBubbleOutScreen(LiveActor*, const ActorInitInfo&, const char*, f32,
                                               u16, const sead::Vector3f&);
HitSensor* addHitSensorBindableKoura(LiveActor*, const ActorInitInfo&, const char*, f32, u16,
                                     const sead::Vector3f&);
HitSensor* addHitSensorBindableRouteDokan(LiveActor*, const ActorInitInfo&, const char*, f32, u16,
                                          const sead::Vector3f&);
HitSensor* addHitSensorBindableBubblePadInput(LiveActor*, const ActorInitInfo&, const char*, f32,
                                              u16, const sead::Vector3f&);
HitSensor* addHitSensorCollisionParts(LiveActor*, const ActorInitInfo&, const char*, f32, u16,
                                      const sead::Vector3f&);
HitSensor* addHitSensorEye(LiveActor*, const ActorInitInfo&, const char*, f32, u16,
                           const sead::Vector3f&);
void setHitSensorSort(LiveActor*, const char*, const SensorSortCmpFuncBase*);
void setHitSensorPosPtr(LiveActor*, const char*, const sead::Vector3f*);
HitSensor* getHitSensor(const LiveActor*, const char*);
void setHitSensorMtxPtr(LiveActor*, const char*, const sead::Matrix34f*);
void setHitSensorJointMtx(LiveActor*, const char*, const char*);
void setSensorRadius(LiveActor*, const char*, f32);
void setSensorRadius(LiveActor*, f32);
f32 getSensorRadius(const LiveActor*, const char*);
f32 getSensorRadius(const LiveActor*);
const sead::Vector3f& getSensorPos(const LiveActor*, const char*);
const sead::Vector3f& getSensorPos(const LiveActor*);
void setSensorFollowPosOffset(LiveActor*, const char*, const sead::Vector3f&);
void setSensorFollowPosOffset(LiveActor*, const sead::Vector3f&);
const sead::Vector3f& getSensorFollowPosOffset(const LiveActor*, const char*);
const sead::Vector3f& getSensorFollowPosOffset(const LiveActor*);
ActorSensorController* createActorSensorController(LiveActor*, const char*);
void setSensorRadius(ActorSensorController*, f32);
void setSensorScale(ActorSensorController*, f32);
void setSensorFollowPosOffset(ActorSensorController*, const sead::Vector3f&);
f32 getOriginalSensorRadius(const ActorSensorController*);
const sead::Vector3f& getOriginalSensorFollowPosOffset(const ActorSensorController*);
void resetActorSensorController(ActorSensorController*);
void calcPosBetweenSensors(sead::Vector3f*, const HitSensor*, const HitSensor*, f32);
f32 calcDistance(const HitSensor*, const HitSensor*);
const sead::Vector3f& getSensorPos(const HitSensor*);
f32 calcDistanceV(const sead::Vector3f&, const HitSensor*, const HitSensor*);
f32 calcDistanceH(const sead::Vector3f&, const HitSensor*, const HitSensor*);
void calcDirBetweenSensors(sead::Vector3f*, const HitSensor*, const HitSensor*);
void calcDirBetweenSensorsH(sead::Vector3f*, const HitSensor*, const HitSensor*);
void calcDirBetweenSensorsNormal(sead::Vector3f*, const HitSensor*, const HitSensor*,
                                 sead::Vector3f);
void calcVecBetweenSensors(sead::Vector3f*, const HitSensor*, const HitSensor*);
void calcVecBetweenSensorsH(sead::Vector3f*, const HitSensor*, const HitSensor*);
void calcVecBetweenSensorsNormal(sead::Vector3f*, const HitSensor*, const HitSensor*,
                                 sead::Vector3f);
void calcStrikeArrowCollideWallAndCeilingBetweenAttackSensor(const LiveActor*, const HitSensor*,
                                                             const HitSensor*,
                                                             const sead::Vector3f&, f32);
LiveActor* getSensorHost(const HitSensor*);
bool isFaceBetweenSensors(const sead::Vector3f&, const HitSensor*, const HitSensor*);
bool isFaceBetweenSensorsH(const sead::Vector3f&, const HitSensor*, const HitSensor*);
bool isEnableLookAtTargetSensor(const HitSensor*, const sead::Vector3f&, f32);
bool isSensorValid(const HitSensor*);
bool isHitBoxSensor(const HitSensor*, const sead::Vector3f&, const sead::BoundBox3f&);
f32 getSensorRadius(const HitSensor*);
bool isHitBoxSensor(const HitSensor*, const sead::Matrix34f&, const sead::BoundBox3f&);
bool isHitCylinderSensor(const HitSensor*, const sead::Vector3f&, const sead::Vector3f&, f32);
bool isHitCylinderSensor(const HitSensor*, const HitSensor*, const sead::Vector3f&, f32);
bool isHitCylinderSensor(sead::Vector3f*, sead::Vector3f*, const HitSensor*, const sead::Vector3f&,
                         const sead::Vector3f&, f32);
bool isHitCylinderSensor(sead::Vector3f*, sead::Vector3f*, const HitSensor*, const HitSensor*,
                         const sead::Vector3f&, f32);
bool isHitCylinderSensorHeight(const HitSensor*, const HitSensor*, const sead::Vector3f&, f32, f32);
bool isHitCircleSensor(sead::Vector3f*, sead::Vector3f*, const HitSensor*, const sead::Vector3f&,
                       const sead::Vector3f&, f32, f32);
bool isHitCircleSensor(sead::Vector3f*, sead::Vector3f*, const HitSensor*, const HitSensor*,
                       const sead::Vector3f&, f32, f32);
bool isHitCircleSensor(const HitSensor*, const sead::Vector3f&, const sead::Vector3f&, f32, f32);
bool isHitCircleSensor(const HitSensor*, const HitSensor*, const sead::Vector3f&, f32, f32);
bool isHitPlaneSensor(const HitSensor*, const sead::Vector3f&, const sead::Vector3f&, f32);
bool isHitPlaneSensor(const HitSensor*, const HitSensor*, const sead::Vector3f&, f32);
/* ------------------- */

const sead::Vector3f& getActorTrans(const HitSensor*);
const sead::Vector3f& getActorVelocity(const HitSensor*);
const sead::Vector3f& getActorGravity(const HitSensor*);
bool isSensorName(const HitSensor*, const char*);
bool isSensorHostName(const HitSensor*, const char*);
bool isSensorHost(const HitSensor*, const LiveActor*);
void validateHitSensors(LiveActor*);
void invalidateHitSensors(LiveActor*);
bool isSensorValid(const LiveActor*, const char*);
void validateHitSensor(LiveActor*, const char*);
void invalidateHitSensor(LiveActor*, const char*);
void validateHitSensorBindableAll(LiveActor*);
bool isSensorBindableAll(const HitSensor*);
void validateHitSensorEnemyAll(LiveActor*);
bool isSensorEnemy(const HitSensor*);
void validateHitSensorEnemyAttackAll(LiveActor*);
bool isSensorEnemyAttack(const HitSensor*);
void validateHitSensorEnemyBodyAll(LiveActor*);
bool isSensorEnemyBody(const HitSensor*);
void validateHitSensorEyeAll(LiveActor*);
bool isSensorEye(const HitSensor*);
void validateHitSensorMapObjAll(LiveActor*);
bool isSensorMapObj(const HitSensor*);
void validateHitSensorNpcAll(LiveActor*);
bool isSensorNpc(const HitSensor*);
void validateHitSensorPlayerAll(LiveActor*);
bool isSensorPlayer(const HitSensor*);
bool isSensorPlayerAll(const HitSensor*);
void validateHitSensorRideAll(LiveActor*);
bool isSensorRide(const HitSensor*);
bool isSensorSimple(const HitSensor*);
bool isSensorLookAt(const HitSensor*);
void invalidateHitSensorEyeAll(LiveActor*);
void invalidateHitSensorPlayerAll(LiveActor*);
void invalidateHitSensorPlayerAttackAll(LiveActor*);
bool isSensorPlayerAttack(const HitSensor*);
bool isSensorPlayerEye(const HitSensor*);

bool isSensorBindableGoal(const HitSensor*);
bool isSensorBindableAllPlayer(const HitSensor*);
bool isSensorBindableBubbleOutScreen(const HitSensor*);
bool isSensorBindableKoura(const HitSensor*);
bool isSensorBindableRouteDokan(const HitSensor*);
bool isSensorBindableBubblePadInput(const HitSensor*);
bool isSensorBindable(const HitSensor*);

}  // namespace al
