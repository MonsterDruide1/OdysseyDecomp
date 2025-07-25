#pragma once

#include <math/seadBoundBox.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>
#include <prim/seadRuntimeTypeInfo.h>

#include "Library/HitSensor/SensorMsgSetupUtil.h"

namespace al {

class SensorMsg;
class LiveActor;
class HitSensor;
class SensorMsg;
struct ActorInitInfo;
class SensorSortCmpFuncBase;
class ActorSensorController;
class ComboCounter;

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
bool calcDirBetweenSensors(sead::Vector3f*, const HitSensor*, const HitSensor*);
bool calcDirBetweenSensorsH(sead::Vector3f*, const HitSensor*, const HitSensor*);
bool calcDirBetweenSensorsNormal(sead::Vector3f*, const HitSensor*, const HitSensor*,
                                 sead::Vector3f);
void calcVecBetweenSensors(sead::Vector3f*, const HitSensor*, const HitSensor*);
void calcVecBetweenSensorsH(sead::Vector3f*, const HitSensor*, const HitSensor*);
void calcVecBetweenSensorsNormal(sead::Vector3f*, const HitSensor*, const HitSensor*,
                                 sead::Vector3f);
s32 calcStrikeArrowCollideWallAndCeilingBetweenAttackSensor(const LiveActor*, const HitSensor*,
                                                            const HitSensor*, const sead::Vector3f&,
                                                            f32);
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
bool isSensorPlayerAll(const HitSensor*);
void validateHitSensorRideAll(LiveActor*);
bool isSensorRide(const HitSensor*);
void invalidateHitSensorEyeAll(LiveActor*);
void invalidateHitSensorPlayerAll(LiveActor*);
void invalidateHitSensorPlayerAttackAll(LiveActor*);
bool isSensorPlayerAttack(const HitSensor*);

bool sendMsgPlayerAttackTrample(HitSensor* receiver, HitSensor* sender, ComboCounter* comboCounter);
bool sendMsgPlayerTrampleReflect(HitSensor* receiver, HitSensor* sender,
                                 ComboCounter* comboCounter);
bool sendMsgPlayerReflectOrTrample(HitSensor* receiver, HitSensor* sender,
                                   ComboCounter* comboCounter);
bool sendMsgPlayerHipDrop(HitSensor* receiver, HitSensor* sender, ComboCounter* comboCounter);
bool sendMsgPlayerObjHipDrop(HitSensor* receiver, HitSensor* sender, ComboCounter* comboCounter);
bool sendMsgPlayerObjHipDropReflect(HitSensor* receiver, HitSensor* sender,
                                    ComboCounter* comboCounter);
bool sendMsgPlayerObjHipDropHighJump(HitSensor* receiver, HitSensor* sender,
                                     ComboCounter* comboCounter);
bool sendMsgPlayerHipDropKnockDown(HitSensor* receiver, HitSensor* sender);
bool sendMsgPlayerStatueDrop(HitSensor* receiver, HitSensor* sender, ComboCounter* comboCounter);
bool sendMsgPlayerObjStatueDrop(HitSensor* receiver, HitSensor* sender, ComboCounter* comboCounter);
bool sendMsgPlayerObjStatueDropReflect(HitSensor* receiver, HitSensor* sender,
                                       ComboCounter* comboCounter);
bool sendMsgPlayerObjStatueDropReflectNoCondition(HitSensor* receiver, HitSensor* sender);
bool sendMsgPlayerStatueTouch(HitSensor* receiver, HitSensor* sender);
bool sendMsgPlayerUpperPunch(HitSensor* receiver, HitSensor* sender);
bool sendMsgPlayerObjUpperPunch(HitSensor* receiver, HitSensor* sender);
bool sendMsgPlayerRollingAttack(HitSensor* receiver, HitSensor* sender);
bool sendMsgPlayerRollingReflect(HitSensor* receiver, HitSensor* sender);
bool sendMsgPlayerObjRollingAttack(HitSensor* receiver, HitSensor* sender);
bool sendMsgPlayerObjRollingAttackFailure(HitSensor* receiver, HitSensor* sender);
bool sendMsgPlayerInvincibleAttack(HitSensor* receiver, HitSensor* sender,
                                   ComboCounter* comboCounter);
bool sendMsgPlayerFireBallAttack(HitSensor* receiver, HitSensor* sender);
bool sendMsgPlayerRouteDokanFireBallAttack(HitSensor* receiver, HitSensor* sender);
bool sendMsgPlayerTailAttack(HitSensor* receiver, HitSensor* sender, ComboCounter* comboCounter);
bool sendMsgPlayerTouch(HitSensor* receiver, HitSensor* sender);
bool sendMsgPlayerKick(HitSensor* receiver, HitSensor* sender);
bool sendMsgPlayerCatch(HitSensor* receiver, HitSensor* sender);
bool sendMsgPlayerSlidingAttack(HitSensor* receiver, HitSensor* sender, ComboCounter* comboCounter);
bool sendMsgPlayerBoomerangAttack(HitSensor* receiver, HitSensor* sender,
                                  ComboCounter* comboCounter);
bool sendMsgPlayerBoomerangAttackCollide(HitSensor* receiver, HitSensor* sender);
bool sendMsgPlayerBoomerangReflect(HitSensor* receiver, HitSensor* sender);
bool sendMsgPlayerBoomerangBreak(HitSensor* receiver, HitSensor* sender);
bool sendMsgPlayerBodyAttack(HitSensor* receiver, HitSensor* sender, ComboCounter* comboCounter);
bool sendMsgPlayerBodyLanding(HitSensor* receiver, HitSensor* sender, ComboCounter* comboCounter);
bool sendMsgPlayerBodyAttackReflect(HitSensor* receiver, HitSensor* sender,
                                    ComboCounter* comboCounter);
bool sendMsgPlayerClimbAttack(HitSensor* receiver, HitSensor* sender, ComboCounter* comboCounter);
bool sendMsgPlayerSpinAttack(HitSensor* receiver, HitSensor* sender, ComboCounter* comboCounter);
bool sendMsgPlayerGiantAttack(HitSensor* receiver, HitSensor* sender, ComboCounter* comboCounter);
bool sendMsgPlayerCooperationHipDrop(HitSensor* receiver, HitSensor* sender,
                                     ComboCounter* comboCounter);
bool sendMsgPlayerClimbSlidingAttack(HitSensor* receiver, HitSensor* sender,
                                     ComboCounter* comboCounter);
bool sendMsgPlayerClimbRollingAttack(HitSensor* receiver, HitSensor* sender,
                                     ComboCounter* comboCounter);
bool sendMsgPlayerGiantHipDrop(HitSensor* receiver, HitSensor* sender, ComboCounter* comboCounter);
bool sendMsgPlayerDisregard(HitSensor* receiver, HitSensor* sender);
bool sendMsgPlayerItemGet(HitSensor* receiver, HitSensor* sender);
bool sendMsgPlayerPutOnEquipment(HitSensor* receiver, HitSensor* sender);
bool sendMsgPlayerReleaseEquipment(HitSensor* receiver, HitSensor* sender);
bool sendMsgPlayerReleaseEquipmentGoal(HitSensor* receiver, HitSensor* sender, u32);
bool sendMsgPlayerFloorTouch(HitSensor* receiver, HitSensor* sender);
bool sendMsgPlayerDamageTouch(HitSensor* receiver, HitSensor* sender);
bool sendMsgPlayerCarryFront(HitSensor* receiver, HitSensor* sender);
bool sendMsgPlayerCarryFrontWallKeep(HitSensor* receiver, HitSensor* sender);
bool sendMsgPlayerCarryUp(HitSensor* receiver, HitSensor* sender);
bool sendMsgPlayerCarryKeepDemo(HitSensor* receiver, HitSensor* sender);
bool sendMsgPlayerCarryWarp(HitSensor* receiver, HitSensor* sender);
bool sendMsgPlayerLeave(HitSensor* receiver, HitSensor* sender);
bool sendMsgPlayerRelease(HitSensor* receiver, HitSensor* sender);
bool sendMsgPlayerReleaseBySwing(HitSensor* receiver, HitSensor* sender);
bool sendMsgPlayerReleaseDamage(HitSensor* receiver, HitSensor* sender);
bool sendMsgPlayerReleaseDead(HitSensor* receiver, HitSensor* sender);
bool sendMsgPlayerReleaseDemo(HitSensor* receiver, HitSensor* sender);
bool sendMsgPlayerToss(HitSensor* receiver, HitSensor* sender);
bool sendMsgPlayerInvincibleTouch(HitSensor* receiver, HitSensor* sender,
                                  ComboCounter* comboCounter);
bool sendMsgEnemyAttack(HitSensor* receiver, HitSensor* sender);
bool sendMsgEnemyAttackBoomerang(HitSensor* receiver, HitSensor* sender);
bool sendMsgEnemyAttackFire(HitSensor* receiver, HitSensor* sender, const char*);
bool sendMsgEnemyAttackNeedle(HitSensor* receiver, HitSensor* sender);
bool sendMsgEnemyFloorTouch(HitSensor* receiver, HitSensor* sender);
bool sendMsgEnemyItemGet(HitSensor* receiver, HitSensor* sender);
bool sendMsgEnemyRouteDokanAttack(HitSensor* receiver, HitSensor* sender);
bool sendMsgEnemyRouteDokanFire(HitSensor* receiver, HitSensor* sender);
bool sendMsgEnemyTouch(HitSensor* receiver, HitSensor* sender);
bool sendMsgEnemyUpperPunch(HitSensor* receiver, HitSensor* sender);
bool sendMsgEnemyTrample(HitSensor* receiver, HitSensor* sender);
bool sendMsgMapObjTrample(HitSensor* receiver, HitSensor* sender);
bool sendMsgPressureDeath(HitSensor* receiver, HitSensor* sender);
bool sendMsgNpcTouch(HitSensor* receiver, HitSensor* sender);
bool sendMsgExplosion(HitSensor* receiver, HitSensor* sender, ComboCounter* comboCounter);
bool sendMsgExplosionCollide(HitSensor* receiver, HitSensor* sender, ComboCounter* comboCounter);
bool sendMsgPush(HitSensor* receiver, HitSensor* sender);
bool sendMsgPushStrong(HitSensor* receiver, HitSensor* sender);
bool sendMsgPushVeryStrong(HitSensor* receiver, HitSensor* sender);
bool sendMsgHit(HitSensor* receiver, HitSensor* sender);
bool sendMsgHitStrong(HitSensor* receiver, HitSensor* sender);
bool sendMsgHitVeryStrong(HitSensor* receiver, HitSensor* sender);
bool sendMsgKnockDown(HitSensor* receiver, HitSensor* sender);
bool sendMsgMapPush(HitSensor* receiver, HitSensor* sender);
bool sendMsgVanish(HitSensor* receiver, HitSensor* sender);
bool sendMsgChangeAlpha(LiveActor* receiver, f32 alpha);
bool sendMsgShowModel(HitSensor* receiver, HitSensor* sender);
bool sendMsgHideModel(HitSensor* receiver, HitSensor* sender);
bool sendMsgRestart(HitSensor* receiver, HitSensor* sender);
bool sendMsgNeedleBallAttack(HitSensor* receiver, HitSensor* sender);
bool sendMsgPunpunFloorTouch(HitSensor* receiver, HitSensor* sender);
bool sendMsgInvalidateFootPrint(HitSensor* receiver, HitSensor* sender);
bool sendMsgKickKouraAttack(HitSensor* receiver, HitSensor* sender, ComboCounter* comboCounter);
bool sendMsgKickKouraAttackCollide(HitSensor* receiver, HitSensor* sender,
                                   ComboCounter* comboCounter);
bool sendMsgKickKouraGetItem(HitSensor* receiver, HitSensor* sender);
bool sendMsgKickKouraReflect(HitSensor* receiver, HitSensor* sender);
bool sendMsgKickKouraCollideNoReflect(HitSensor* receiver, HitSensor* sender);
bool sendMsgKickKouraBreak(HitSensor* receiver, HitSensor* sender);
bool sendMsgKickKouraBlow(HitSensor* receiver, HitSensor* sender);
bool sendMsgKickStoneAttack(HitSensor* receiver, HitSensor* sender);
bool sendMsgKickStoneAttackCollide(HitSensor* receiver, HitSensor* sender);
bool sendMsgKickStoneAttackHold(HitSensor* receiver, HitSensor* sender);
bool sendMsgKickStoneAttackReflect(HitSensor* receiver, HitSensor* sender);
bool sendMsgKickStoneTrample(HitSensor* receiver, HitSensor* sender);
bool sendMsgKillerAttack(HitSensor* receiver, HitSensor* sender);
bool sendMsgLiftGeyser(HitSensor* receiver, HitSensor* sender);
bool sendMsgWarpStart(HitSensor* receiver, HitSensor* sender);
bool sendMsgWarpEnd(HitSensor* receiver, HitSensor* sender);
bool sendMsgHoldCancel(HitSensor* receiver, HitSensor* sender);
bool sendMsgHoleIn(HitSensor* receiver, HitSensor* sender);
bool sendMsgJumpInhibit(HitSensor* receiver, HitSensor* sender);
bool sendMsgGoalKill(HitSensor* receiver, HitSensor* sender);
bool sendMsgGoal(HitSensor* receiver, HitSensor* sender);
bool sendMsgBindStart(HitSensor* receiver, HitSensor* sender);
bool sendMsgBindInit(HitSensor* receiver, HitSensor* sender, u32);
bool sendMsgBindEnd(HitSensor* receiver, HitSensor* sender);
bool sendMsgBindCancel(HitSensor* receiver, HitSensor* sender);
bool sendMsgBindCancelByDemo(HitSensor* receiver, HitSensor* sender);
bool sendMsgBindDamage(HitSensor* receiver, HitSensor* sender);
bool sendMsgBindSteal(HitSensor* receiver, HitSensor* sender);
bool sendMsgBindGiant(HitSensor* receiver, HitSensor* sender);
bool sendMsgBallAttack(HitSensor* receiver, HitSensor* sender, ComboCounter* comboCounter);
bool sendMsgBallRouteDokanAttack(HitSensor* receiver, HitSensor* sender,
                                 ComboCounter* comboCounter);
bool sendMsgBallAttackHold(HitSensor* receiver, HitSensor* sender);
bool sendMsgBallAttackDRCHold(HitSensor* receiver, HitSensor* sender);
bool sendMsgBallAttackCollide(HitSensor* receiver, HitSensor* sender);
bool sendMsgBallTrample(HitSensor* receiver, HitSensor* sender, ComboCounter* comboCounter);
bool sendMsgBallTrampleCollide(HitSensor* receiver, HitSensor* sender);
bool sendMsgBallItemGet(HitSensor* receiver, HitSensor* sender);
bool sendMsgFireBalCollide(HitSensor* receiver, HitSensor* sender);
bool sendMsgFireBallFloorTouch(HitSensor* receiver, HitSensor* sender);
bool sendMsgDokanBazookaAttack(HitSensor* receiver, HitSensor* sender);
bool sendMsgRideAllPlayerItemGet(HitSensor* receiver, HitSensor* sender);
bool sendMsgHideModel(LiveActor* receiver);
bool sendMsgShowModel(LiveActor* receiver);
bool sendMsgRestart(LiveActor* receiver);
bool sendMsgCollisionImpulse(HitSensor* receiver, HitSensor* sender, sead::Vector3f*,
                             const sead::Vector3f&, f32, const sead::Vector3f&, f32);
bool sendMsgSwitchOn(LiveActor* receiver);
bool sendMsgSwitchOnInit(LiveActor* receiver);
bool sendMsgSwitchOffInit(LiveActor* receiver);
bool sendMsgSwitchKillOn(LiveActor* receiver);
bool sendMsgSwitchKillOnInit(LiveActor* receiver);
bool sendMsgSwitchKillOffInit(LiveActor* receiver);
bool sendMsgPlayerFloorTouchToColliderGround(LiveActor* receiver, HitSensor* sender);
bool sendMsgPlayerUpperPunchToColliderCeiling(LiveActor* receiver, HitSensor* sender);
bool sendMsgEnemyFloorTouchToColliderGround(LiveActor* receiver, HitSensor* sender);
bool sendMsgEnemyUpperPunchToColliderCeiling(LiveActor* receiver, HitSensor* sender);
bool sendMsgAskSafetyPoint(HitSensor* receiver, HitSensor* sender,
                           sead::Vector3f** safetyPointAccessor);
bool sendMsgAskSafetyPointToColliderGround(LiveActor* receiver, HitSensor* sender,
                                           sead::Vector3f** safetyPointAccessor);
bool sendMsgTouchAssist(HitSensor* receiver, HitSensor* sender);
bool sendMsgTouchAssistTrig(HitSensor* receiver, HitSensor* sender);
bool sendMsgTouchStroke(HitSensor* receiver, HitSensor* sender);
bool sendMsgScreenPointInvalidCollisionParts(HitSensor* receiver, HitSensor* sender);
bool sendMsgBlockUpperPunch(HitSensor* receiver, HitSensor* sender, ComboCounter* comboCounter);
bool sendMsgBlockLowerPunch(HitSensor* receiver, HitSensor* sender, ComboCounter* comboCounter);
bool sendMsgBlockItemGet(HitSensor* receiver, HitSensor* sender);
bool sendMsgKillerItemGet(HitSensor* receiver, HitSensor* sender);
bool sendMsgPlayerKouraAttack(HitSensor* receiver, HitSensor* sender, ComboCounter* comboCounter);
bool sendMsgLightFlash(HitSensor* receiver, HitSensor* sender);
bool sendMsgForceAbyss(HitSensor* receiver, HitSensor* sender);
bool sendMsgIsNerveSupportFreeze(HitSensor* receiver, HitSensor* sender);
bool sendMsgOnSyncSupportFreeze(HitSensor* receiver, HitSensor* sender);
bool sendMsgOffSyncSupportFreeze(HitSensor* receiver, HitSensor* sender);
bool sendMsgSwordAttackHighLeft(HitSensor* receiver, HitSensor* sender);
bool sendMsgSwordAttackLowLeft(HitSensor* receiver, HitSensor* sender);
bool sendMsgSwordAttackHighRight(HitSensor* receiver, HitSensor* sender);
bool sendMsgSwordAttackLowRight(HitSensor* receiver, HitSensor* sender);
bool sendMsgSwordAttackJumpUnder(HitSensor* receiver, HitSensor* sender);
bool sendMsgSwordBeamAttack(HitSensor* receiver, HitSensor* sender);
bool sendMsgSwordBeamReflectAttack(HitSensor* receiver, HitSensor* sender);
bool sendMsgShieldGuard(HitSensor* receiver, HitSensor* sender);
bool sendMsgEnemyAttackKnockDown(HitSensor* receiver, HitSensor* sender);
bool sendMsgAskMultiPlayerEnemy(HitSensor* receiver, HitSensor* sender);
bool sendMsgItemGettable(HitSensor* receiver, HitSensor* sender);
bool sendMsgKikkiThrow(HitSensor* receiver, HitSensor* sender);
bool sendMsgIsKikkiThrowTarget(HitSensor* receiver, HitSensor* sender);
bool sendMsgPlayerCloudGet(HitSensor* receiver, HitSensor* sender);
bool sendMsgAutoJump(HitSensor* receiver, HitSensor* sender);
bool sendMsgPlayerTouchShadow(HitSensor* receiver, HitSensor* sender);
bool sendMsgPlayerPullOutShadow(HitSensor* receiver, HitSensor* sender);
bool sendMsgPlayerAttackShadow(HitSensor* receiver, HitSensor* sender);
bool sendMsgPlayerAttackShadowStrong(HitSensor* receiver, HitSensor* sender);
bool sendMsgPlayerAttackChangePos(HitSensor* receiver, HitSensor* sender, sead::Vector3f* pos);
bool sendMsgAtmosOnlineLight(HitSensor* receiver, HitSensor* sender);
bool sendMsgLightBurn(HitSensor* receiver, HitSensor* sender);
bool sendMsgMoonLightBurn(HitSensor* receiver, HitSensor* sender);
bool sendMsgString(HitSensor* receiver, HitSensor* sender, const char* str);
bool sendMsgStringV4fPtr(HitSensor* receiver, HitSensor* sender, const char* str,
                         sead::Vector4f* vec);
bool sendMsgStringV4fSensorPtr(HitSensor* receiver, HitSensor* sender, const char* str,
                               sead::Vector4f* vec);
bool sendMsgStringVoidPtr(HitSensor* receiver, HitSensor* sender, const char* str, void* ptr);

bool isMsgPushAll(const SensorMsg* msg);
bool isMsgPush(const SensorMsg* msg);
bool isMsgPushStrong(const SensorMsg* msg);
bool isMsgPushVeryStrong(const SensorMsg* msg);
bool isMsgHoldReleaseAll(const SensorMsg* msg);
bool isMsgHoldCancel(const SensorMsg* msg);
bool isMsgPlayerRelease(const SensorMsg* msg);
bool isMsgPlayerReleaseBySwing(const SensorMsg* msg);
bool isMsgPlayerReleaseDead(const SensorMsg* msg);
bool isMsgPlayerReleaseDamage(const SensorMsg* msg);
bool isMsgPlayerReleaseDemo(const SensorMsg* msg);
bool isMsgItemGetDirectAll(const SensorMsg* msg);
bool isMsgPlayerItemGet(const SensorMsg* msg);
bool isMsgRideAllPlayerItemGet(const SensorMsg* msg);
bool isMsgPlayerTailAttack(const SensorMsg* msg);
bool isMsgItemGetByObjAll(const SensorMsg* msg);
bool isMsgBallItemGet(const SensorMsg* msg);
bool isMsgKickKouraItemGet(const SensorMsg* msg);
bool isMsgKillerItemGet(const SensorMsg* msg);
bool isMsgItemGetAll(const SensorMsg* msg);
bool isMsgFloorTouch(const SensorMsg* msg);
bool isMsgPlayerFloorTouch(const SensorMsg* msg);
bool isMsgEnemyFloorTouch(const SensorMsg* msg);
bool isMsgUpperPunch(const SensorMsg* msg);
bool isMsgPlayerUpperPunch(const SensorMsg* msg);
bool isMsgEnemyUpperPunch(const SensorMsg* msg);
bool isMsgPlayerTrample(const SensorMsg* msg);
bool isMsgPlayerTrampleReflect(const SensorMsg* msg);
bool isMsgPlayerHipDropAll(const SensorMsg* msg);
bool isMsgPlayerStatueDrop(const SensorMsg* msg);
bool isMsgPlayerObjHipDropAll(const SensorMsg* msg);
bool isMsgPlayerObjStatueDrop(const SensorMsg* msg);
bool isMsgPlayerObjHipDropReflectAll(const SensorMsg* msg);
bool isMsgPlayerObjStatueDropReflect(const SensorMsg* msg);
bool isMsgPlayerObjHipDropHighJump(const SensorMsg* msg);
bool isMsgPlayerHipDropKnockDown(const SensorMsg* msg);
bool isMsgPlayerObjStatueDropReflectNoCondition(const SensorMsg* msg);
bool isMsgPlayerStatueTouch(const SensorMsg* msg);
bool isMsgPlayerObjUpperPunch(const SensorMsg* msg);
bool isMsgPlayerRollingAttack(const SensorMsg* msg);
bool isMsgPlayerRollingReflect(const SensorMsg* msg);
bool isMsgPlayerObjRollingAttack(const SensorMsg* msg);
bool isMsgPlayerObjRollingAttackFailure(const SensorMsg* msg);
bool isMsgPlayerInvincibleAttack(const SensorMsg* msg);
bool isMsgPlayerFireBallAttack(const SensorMsg* msg);
bool isMsgPlayerRouteDokanFireBallAttack(const SensorMsg* msg);
bool isMsgPlayerKick(const SensorMsg* msg);
bool isMsgPlayerCatch(const SensorMsg* msg);
bool isMsgPlayerSlidingAttack(const SensorMsg* msg);
bool isMsgPlayerBoomerangAttack(const SensorMsg* msg);
bool isMsgPlayerBoomerangAttackCollide(const SensorMsg* msg);
bool isMsgPlayerBoomerangReflect(const SensorMsg* msg);
bool isMsgPlayerBoomerangBreak(const SensorMsg* msg);
bool isMsgPlayerBodyAttack(const SensorMsg* msg);
bool isMsgPlayerBodyLanding(const SensorMsg* msg);
bool isMsgPlayerBodyAttackReflect(const SensorMsg* msg);
bool isMsgPlayerClimbAttack(const SensorMsg* msg);
bool isMsgPlayerSpinAttack(const SensorMsg* msg);
bool isMsgPlayerGiantAttack(const SensorMsg* msg);
bool isMsgPlayerCooperationHipDrop(const SensorMsg* msg);
bool isMsgPlayerClimbSlidingAttack(const SensorMsg* msg);
bool isMsgPlayerClimbRollingAttack(const SensorMsg* msg);
bool isMsgPlayerGiantHipDrop(const SensorMsg* msg);
bool isMsgPlayerDisregard(const SensorMsg* msg);
bool isMsgPlayerDash(const SensorMsg* msg);
bool isMsgPlayerDamageTouch(const SensorMsg* msg);
bool isMsgPlayerFloorTouchBind(const SensorMsg* msg);
bool isMsgPlayerTouch(const SensorMsg* msg);
bool isMsgPlayerInvincibleTouch(const SensorMsg* msg);
bool isMsgPlayerGiantTouch(const SensorMsg* msg);
bool isMsgPlayerObjTouch(const SensorMsg* msg);
bool isMsgPlayerPutOnEquipment(const SensorMsg* msg);
bool isMsgPlayerReleaseEquipment(const SensorMsg* msg);
bool isMsgPlayerReleaseEquipmentGoal(const SensorMsg* msg);
bool isMsgPlayerCarryFront(const SensorMsg* msg);
bool isMsgPlayerCarryFrontWallKeep(const SensorMsg* msg);
bool isMsgPlayerCarryUp(const SensorMsg* msg);
bool isMsgPlayerCarryKeepDemo(const SensorMsg* msg);
bool isMsgPlayerCarryWarp(const SensorMsg* msg);
bool isMsgPlayerLeave(const SensorMsg* msg);
bool isMsgPlayerToss(const SensorMsg* msg);
bool isMsgEnemyAttack(const SensorMsg* msg);
bool isMsgEnemyAttackFire(const SensorMsg* msg);
bool isMsgEnemyAttackKnockDown(const SensorMsg* msg);
bool isMsgEnemyAttackBoomerang(const SensorMsg* msg);
bool isMsgEnemyAttackNeedle(const SensorMsg* msg);
bool isMsgEnemyItemGet(const SensorMsg* msg);
bool isMsgEnemyRouteDokanAttack(const SensorMsg* msg);
bool isMsgEnemyRouteDokanFire(const SensorMsg* msg);
bool isMsgExplosion(const SensorMsg* msg);
bool isMsgExplosionCollide(const SensorMsg* msg);
bool isMsgBindStart(const SensorMsg* msg);
bool isMsgBindInit(const SensorMsg* msg);
bool isMsgBindEnd(const SensorMsg* msg);
bool isMsgBindCancel(const SensorMsg* msg);
bool isMsgBindCancelByDemo(const SensorMsg* msg);
bool isMsgBindDamage(const SensorMsg* msg);
bool isMsgBindSteal(const SensorMsg* msg);
bool isMsgBindGiant(const SensorMsg* msg);
bool isMsgPressureDeath(const SensorMsg* msg);
bool isMsgNpcTouch(const SensorMsg* msg);
bool isMsgHit(const SensorMsg* msg);
bool isMsgHitStrong(const SensorMsg* msg);
bool isMsgHitVeryStrong(const SensorMsg* msg);
bool isMsgKnockDown(const SensorMsg* msg);
bool isMsgMapPush(const SensorMsg* msg);
bool isMsgVanish(const SensorMsg* msg);
bool isMsgChangeAlpha(const SensorMsg* msg);
bool isMsgShowModel(const SensorMsg* msg);
bool isMsgHideModel(const SensorMsg* msg);
bool isMsgRestart(const SensorMsg* msg);
bool isMsgEnemyTouch(const SensorMsg* msg);
bool isMsgEnemyTrample(const SensorMsg* msg);
bool isMsgMapObjTrample(const SensorMsg* msg);
bool isMsgNeedleBallAttack(const SensorMsg* msg);
bool isMsgPunpunFloorTouch(const SensorMsg* msg);
bool isMsgInvalidateFootPrint(const SensorMsg* msg);
bool isMsgKickKouraAttack(const SensorMsg* msg);
bool isMsgKickKouraAttackCollide(const SensorMsg* msg);
bool isMsgKickKouraReflect(const SensorMsg* msg);
bool isMsgKickKouraCollideNoReflect(const SensorMsg* msg);
bool isMsgKickKouraBreak(const SensorMsg* msg);
bool isMsgKickKouraBlow(const SensorMsg* msg);
bool isMsgKickStoneAttack(const SensorMsg* msg);
bool isMsgKickStoneAttackCollide(const SensorMsg* msg);
bool isMsgKickStoneAttackHold(const SensorMsg* msg);
bool isMsgKickStoneAttackReflect(const SensorMsg* msg);
bool isMsgKickStoneTrample(const SensorMsg* msg);
bool isMsgKillerAttack(const SensorMsg* msg);
bool isMsgLiftGeyser(const SensorMsg* msg);
bool isMsgWarpStart(const SensorMsg* msg);
bool isMsgWarpEnd(const SensorMsg* msg);
bool isMsgHoleIn(const SensorMsg* msg);
bool isMsgJumpInhibit(const SensorMsg* msg);
bool isMsgGoalKill(const SensorMsg* msg);
bool isMsgGoal(const SensorMsg* msg);
bool isMsgBallAttack(const SensorMsg* msg);
bool isMsgBallRouteDokanAttack(const SensorMsg* msg);
bool isMsgBallAttackHold(const SensorMsg* msg);
bool isMsgBallAttackDRCHold(const SensorMsg* msg);
bool isMsgBallAttackCollide(const SensorMsg* msg);
bool isMsgBallTrample(const SensorMsg* msg);
bool isMsgBallTrampleCollide(const SensorMsg* msg);
bool isMsgFireBallCollide(const SensorMsg* msg);
bool isMsgFireBallFloorTouch(const SensorMsg* msg);
bool isMsgDokanBazookaAttack(const SensorMsg* msg);
bool isMsgSwitchOn(const SensorMsg* msg);
bool isMsgSwitchOnInit(const SensorMsg* msg);
bool isMsgSwitchOffInit(const SensorMsg* msg);
bool isMsgSwitchKillOn(const SensorMsg* msg);
bool isMsgSwitchKillOnInit(const SensorMsg* msg);
bool isMsgSwitchKillOffInit(const SensorMsg* msg);
bool isMsgAskSafetyPoint(const SensorMsg* msg);
bool trySetAskSafetyPointFollowPos(const SensorMsg* msg, sead::Vector3f*);
bool isMsgTouchAssist(const SensorMsg* msg);
bool isMsgTouchAssistNoPat(const SensorMsg* msg);
bool isMsgTouchAssistTrig(const SensorMsg* msg);
bool isMsgTouchAssistTrigOff(const SensorMsg* msg);
bool isMsgTouchAssistTrigNoPat(const SensorMsg* msg);
bool isMsgTouchAssistBurn(const SensorMsg* msg);
bool isMsgTouchAssistAll(const SensorMsg* msg);
bool isMsgTouchCarryItem(const SensorMsg* msg);
bool isMsgTouchReleaseItem(const SensorMsg* msg);
bool isMsgTouchStroke(const SensorMsg* msg);
bool isMsgIsNerveSupportFreeze(const SensorMsg* msg);
bool isMsgOnSyncSupportFreeze(const SensorMsg* msg);
bool isMsgOffSyncSupportFreeze(const SensorMsg* msg);
bool isMsgScreenPointInvalidCollisionParts(const SensorMsg* msg);
bool isMsgBlockUpperPunch(const SensorMsg* msg);
bool isMsgBlockLowerPunch(const SensorMsg* msg);
bool isMsgBlockItemGet(const SensorMsg* msg);
bool isMsgPlayerKouraAttack(const SensorMsg* msg);
bool isMsgLightFlash(const SensorMsg* msg);
bool isMsgForceAbyss(const SensorMsg* msg);
bool isMsgSwordAttackHigh(const SensorMsg* msg);
bool isMsgSwordAttackHighLeft(const SensorMsg* msg);
bool isMsgSwordAttackHighRight(const SensorMsg* msg);
bool isMsgSwordAttackLow(const SensorMsg* msg);
bool isMsgSwordAttackLowLeft(const SensorMsg* msg);
bool isMsgSwordAttackLowRight(const SensorMsg* msg);
bool isMsgSwordBeamAttack(const SensorMsg* msg);
bool isMsgSwordBeamReflectAttack(const SensorMsg* msg);
bool isMsgSwordAttackJumpUnder(const SensorMsg* msg);
bool isMsgShieldGuard(const SensorMsg* msg);
bool isMsgAskMultiPlayerEnemy(const SensorMsg* msg);
bool isMsgItemGettable(const SensorMsg* msg);
bool isMsgKikkiThrow(const SensorMsg* msg);
bool isMsgIsKikkiThrowTarget(const SensorMsg* msg);
bool isMsgPlayerCloudGet(const SensorMsg* msg);
bool isMsgAutoJump(const SensorMsg* msg);
bool isMsgPlayerTouchShadow(const SensorMsg* msg);
bool isMsgPlayerPullOutShadow(const SensorMsg* msg);
bool isMsgPlayerAttackShadow(const SensorMsg* msg);
bool isMsgPlayerAttackShadowStrong(const SensorMsg* msg);
bool isMsgPlayerAttackChangePos(const SensorMsg* msg);
bool isMsgAtmosOnlineLight(const SensorMsg* msg);
bool isMsgLightBurn(const SensorMsg* msg);
bool isMsgMoonLightBurn(const SensorMsg* msg);
bool isMsgString(const SensorMsg* msg);
bool isMsgStringV4fPtr(const SensorMsg* msg);
bool isMsgStringV4fSensorPtr(const SensorMsg* msg);
bool isMsgStringVoidPtr(const SensorMsg* msg);
bool isMsgPlayerTrampleForCrossoverSensor(const SensorMsg* msg, const HitSensor* sender,
                                          const HitSensor* receiver);
bool isMsgPlayerTrampleReflectForCrossoverSensor(const SensorMsg* msg, const HitSensor* sender,
                                                 const HitSensor* receiver);
bool isMsgPlayerUpperPunchForCrossoverSensor(const SensorMsg* msg, const HitSensor* sender,
                                             const HitSensor* receiver, f32 threshold);
bool isMsgKickStoneTrampleForCrossoverSensor(const SensorMsg* msg, const HitSensor* sender,
                                             const HitSensor* receiver);
bool sendMsgEnemyAttackForCrossoverSensor(HitSensor* receiver, HitSensor* sender);
bool sendMsgEnemyAttackForCrossoverCylinderSensor(HitSensor* receiver, HitSensor* sender,
                                                  const sead::Vector3f& basePoint,
                                                  const sead::Vector3f& upAxis, f32 radius);

bool isSensorPlayer(const HitSensor*);
bool isSensorPlayerFoot(const HitSensor*);
bool isSensorPlayerDecoration(const HitSensor*);
bool isSensorPlayerEye(const HitSensor*);
bool isSensorPlayerOrPlayerWeapon(const HitSensor*);
bool isSensorCollision(const HitSensor*);
bool isSensorPlayerFireBall(const HitSensor*);
bool isSensorHoldObj(const HitSensor*);
bool isSensorLookAt(const HitSensor*);
bool isSensorBindableGoal(const HitSensor*);
bool isSensorBindableAllPlayer(const HitSensor*);
bool isSensorBindableBubbleOutScreen(const HitSensor*);
bool isSensorBindableKoura(const HitSensor*);
bool isSensorBindableRouteDokan(const HitSensor*);
bool isSensorBindableBubblePadInput(const HitSensor*);
bool isSensorBindable(const HitSensor*);
bool isSensorSimple(const HitSensor*);
void updateHitSensorsAll(LiveActor*);
bool isMySensor(const HitSensor*, const LiveActor*);
bool isSensorHitAnyPlane(const HitSensor*, const HitSensor*, const sead::Vector3f&);
bool isSensorHitRingShape(const HitSensor*, const HitSensor*, f32);
bool tryGetEnemyAttackFireMaterialCode(const char**, const SensorMsg*);
bool sendMsgPushAndKillVelocityToTarget(LiveActor* actor, HitSensor* receiver, HitSensor* sender);
bool sendMsgPushAndKillVelocityToTargetH(LiveActor* actor, HitSensor* receiver, HitSensor* sender);
bool pushAndAddVelocity(LiveActor*, const HitSensor*, const HitSensor*, f32);
bool pushAndAddVelocityH(LiveActor*, const HitSensor*, const HitSensor*, f32);
bool pushAndAddVelocityV(LiveActor*, const HitSensor*, const HitSensor*, f32);
bool tryReceiveMsgPushAndAddVelocity(LiveActor*, const SensorMsg*, const HitSensor*,
                                     const HitSensor*, f32);
bool tryReceiveMsgPushAndAddVelocityH(LiveActor*, const SensorMsg*, const HitSensor*,
                                      const HitSensor*, f32);
bool tryReceiveMsgPushAndCalcPushTrans(sead::Vector3f*, const SensorMsg*, const LiveActor*,
                                       const HitSensor*, const HitSensor*, f32);
bool sendMsgCollidePush(HitSensor*, HitSensor*, const sead::Vector3f&);
bool tryReceiveMsgCollidePush(sead::Vector3f*, const SensorMsg*);
f32 getChangeAlphaValue(const SensorMsg*);
u32 getBindInitType(const SensorMsg*);
const char* getMsgString(const SensorMsg*);
const char* getMsgStringV4fPtr(const SensorMsg*, sead::Vector4f**);
const char* getMsgStringV4fSensorPtr(const SensorMsg*, sead::Vector4f, HitSensor**);
const char* getMsgStringVoidPtr(const SensorMsg*, void**);
u32 getPlayerReleaseEquipmentGoalType(const SensorMsg*);
}  // namespace al

namespace AttackSensorFunction {
u16 getAttackSensorNum(const al::HitSensor*);
u16 getAttackSensorNumMax(const al::HitSensor*);
al::HitSensor* getAttackSensor(const al::HitSensor*, s32);
al::HitSensor* findNearestAttackSensor(const al::HitSensor*);
}  // namespace AttackSensorFunction

// Unnamed function at 8FEB0C here

namespace al {
SENSOR_MSG_COMBO(PlayerAttackTrample);
SENSOR_MSG_COMBO(PlayerTrampleReflect);
SENSOR_MSG_COMBO(PlayerAttackHipDrop);
SENSOR_MSG_COMBO(PlayerAttackObjHipDrop);
SENSOR_MSG_COMBO(PlayerAttackObjHipDropReflect);
SENSOR_MSG_COMBO(PlayerAttackObjHipDropHighJump);
SENSOR_MSG(PlayerAttackHipDropKnockDown);
SENSOR_MSG_COMBO(PlayerAttackStatueDrop);
SENSOR_MSG_COMBO(PlayerAttackObjStatueDrop);
SENSOR_MSG_COMBO(PlayerAttackObjStatueDropReflect);
SENSOR_MSG(PlayerAttackObjStatueDropReflectNoCondition);
SENSOR_MSG(PlayerAttackStatueTouch);
SENSOR_MSG(PlayerAttackUpperPunch);
SENSOR_MSG(PlayerAttackObjUpperPunch);
SENSOR_MSG(PlayerAttackRollingAttack);
SENSOR_MSG(PlayerAttackRollingReflect);
SENSOR_MSG(PlayerAttackObjRollingAttack);
SENSOR_MSG(PlayerAttackObjRollingAttackFailure);
SENSOR_MSG_COMBO(PlayerAttackInvincibleAttack);
SENSOR_MSG(PlayerAttackFireBallAttack);
SENSOR_MSG(PlayerAttackRouteDokanFireBallAttack);
SENSOR_MSG_COMBO(PlayerAttackTailAttack);
SENSOR_MSG(PlayerAttackKick);
SENSOR_MSG(PlayerAttackCatch);
SENSOR_MSG_COMBO(PlayerAttackSlidingAttack);
SENSOR_MSG_COMBO(PlayerAttackBoomerangAttack);
SENSOR_MSG(PlayerAttackBoomerangAttackCollide);
SENSOR_MSG(PlayerAttackBoomerangReflect);
SENSOR_MSG(PlayerAttackBoomerangBreak);
SENSOR_MSG_COMBO(PlayerAttackBodyAttack);
SENSOR_MSG_COMBO(PlayerAttackBodyLanding);
SENSOR_MSG_COMBO(PlayerAttackBodyAttackReflect);
SENSOR_MSG_COMBO(PlayerAttackClimbAttack);
SENSOR_MSG_COMBO(PlayerAttackClimbSliding);
SENSOR_MSG_COMBO(PlayerAttackClimbRolling);
SENSOR_MSG_COMBO(PlayerAttackSpinAttack);
SENSOR_MSG_COMBO(PlayerAttackGiant);

SENSOR_MSG_COMBO(PlayerCooperationHipDrop);
SENSOR_MSG_COMBO(PlayerGiantHipDrop);
SENSOR_MSG(PlayerDisregard);
SENSOR_MSG(PlayerDamageTouch);
SENSOR_MSG(PlayerFloorTouchBind);  // This msg is referenced by al::isMsgFloorTouchBind, but doesn't
                                   // appear in the executable because it's never used

SENSOR_MSG(PlayerFloorTouch);
SENSOR_MSG(PlayerTouch);
SENSOR_MSG_COMBO(PlayerInvincibleTouch);
SENSOR_MSG(PlayerPutOnEquipment);
SENSOR_MSG(PlayerReleaseEquipment);
SENSOR_MSG_WITH_DATA(PlayerReleaseEquipmentGoal, (u32, Type));
SENSOR_MSG(PlayerCarryFront);
SENSOR_MSG(PlayerCarryFrontWallKeep);
SENSOR_MSG(PlayerCarryUp);
SENSOR_MSG(PlayerCarryKeepDemo);
SENSOR_MSG(PlayerCarryWarp);
SENSOR_MSG(PlayerLeave);
SENSOR_MSG(PlayerRelease);
SENSOR_MSG(PlayerReleaseBySwing);
SENSOR_MSG(PlayerReleaseDead);
SENSOR_MSG(PlayerReleaseDamage);
SENSOR_MSG(PlayerReleaseDemo);
SENSOR_MSG(PlayerToss);

SENSOR_MSG(PlayerItemGet);
SENSOR_MSG(RideAllPlayerItemGet);
SENSOR_MSG(KillerItemGet);

SENSOR_MSG(EnemyAttack);
SENSOR_MSG_WITH_DATA(EnemyAttackFire,
                     (const char*, MaterialCode));  // Usually null, sometimes "LavaRed"
SENSOR_MSG(EnemyAttackKnockDown);
SENSOR_MSG(EnemyAttackBoomerang);
SENSOR_MSG(EnemyAttackNeedle);
SENSOR_MSG(EnemyFloorTouch);
SENSOR_MSG(EnemyItemGet);
SENSOR_MSG(EnemyRouteDokanAttack);
SENSOR_MSG(EnemyRouteDokanFire);
SENSOR_MSG_COMBO(Explosion);
SENSOR_MSG_COMBO(ExplosionCollide);
SENSOR_MSG(Push);
SENSOR_MSG(PushStrong);
SENSOR_MSG(PushVeryStrong);
SENSOR_MSG(BindStart);
SENSOR_MSG_WITH_DATA(BindInit, (u32, BindType));
SENSOR_MSG(BindEnd);
SENSOR_MSG(BindCancel);
SENSOR_MSG(BindCancelByDemo);
SENSOR_MSG(BindDamage);
SENSOR_MSG(BindSteal);
SENSOR_MSG(BindGiant);
SENSOR_MSG(PressureDeath);
SENSOR_MSG(NpcTouch);
SENSOR_MSG(Hit);
SENSOR_MSG(HitStrong);
SENSOR_MSG(HitVeryStrong);
SENSOR_MSG(KnockDown);
SENSOR_MSG(MapPush);
SENSOR_MSG(Vanish);
SENSOR_MSG_WITH_DATA(ChangeAlpha, (f32, Alpha));
SENSOR_MSG(ShowModel);
SENSOR_MSG(HideModel);
SENSOR_MSG(Restart);
// Impulse
SENSOR_MSG(EnemyTouch);
SENSOR_MSG(EnemyUpperPunch);
SENSOR_MSG(EnemyTrample);
SENSOR_MSG(MapObjTrample);
SENSOR_MSG(NeedleBallAttack);
SENSOR_MSG(PunpunFloorTouch);
SENSOR_MSG(InvalidateFootPrint);
SENSOR_MSG_COMBO(KickKouraAttack);
SENSOR_MSG_COMBO(KickKouraAttackCollide);
SENSOR_MSG(KickKouraGetItem);
SENSOR_MSG(KickKouraReflect);
SENSOR_MSG(KickKouraCollideNoReflect);
SENSOR_MSG(KickKouraBreak);
SENSOR_MSG(KickKouraBlow);
SENSOR_MSG(KickStoneAttack);
SENSOR_MSG(KickStoneAttackCollide);
SENSOR_MSG(KickStoneAttackHold);
SENSOR_MSG(KickStoneAttackReflect);
SENSOR_MSG(KickStoneTrample);
SENSOR_MSG(KillerAttack);
SENSOR_MSG(LiftGeyser);
SENSOR_MSG(WarpStart);
SENSOR_MSG(WarpEnd);
SENSOR_MSG(HoldCancel);
SENSOR_MSG(HoleIn);
SENSOR_MSG(JumpInhibit);
SENSOR_MSG(GoalKill);
SENSOR_MSG(Goal);
SENSOR_MSG_COMBO(BallAttack);
SENSOR_MSG_COMBO(BallRouteDokanAttack);
SENSOR_MSG(BallAttackHold);
SENSOR_MSG(BallAttackDRCHold);
SENSOR_MSG(BallAttackCollide);
SENSOR_MSG_COMBO(BallTrample);
SENSOR_MSG(BallTrampleCollide);
SENSOR_MSG(BallItemGet);
SENSOR_MSG(FireBallCollide);
SENSOR_MSG(FireBallFloorTouch);

SENSOR_MSG(DokanBazookaAttack);
SENSOR_MSG(SwitchOn);
SENSOR_MSG(SwitchOnInit);
SENSOR_MSG(SwitchOffInit);
SENSOR_MSG(SwitchKillOn);
SENSOR_MSG(SwitchKillOnInit);
SENSOR_MSG(SwitchKillOffInit);
SENSOR_MSG_WITH_DATA(AskSafetyPoint, (sead::Vector3f**, SafetyPoint));
SENSOR_MSG(TouchAssist);
SENSOR_MSG(TouchAssistTrig);

// These ten are also referenced by isMsgs but don't appear in the executable
SENSOR_MSG(PlayerGiantTouch);
SENSOR_MSG(PlayerAttackDash);
SENSOR_MSG(TouchAssistNoPat);
SENSOR_MSG(TouchAssistTrigOff);
SENSOR_MSG(TouchAssistTrigNoPat);
SENSOR_MSG(TouchAssistBurn);
SENSOR_MSG(TouchReleaseItem);
SENSOR_MSG(TouchCarryItem);
SENSOR_MSG(KickKouraItemGet);

SENSOR_MSG(TouchStroke);
SENSOR_MSG(IsNerveSupportFreeze);
SENSOR_MSG(OnSyncSupportFreeze);
SENSOR_MSG(OffSyncSupportFreeze);
SENSOR_MSG(ScreenPointInvalidCollisionParts);
SENSOR_MSG_COMBO(BlockUpperPunch);
SENSOR_MSG_COMBO(BlockLowerPunch);
SENSOR_MSG(BlockItemGet);
SENSOR_MSG_COMBO(PlayerKouraAttack);
SENSOR_MSG(LightFlash);
SENSOR_MSG(ForceAbyss);
SENSOR_MSG(SwordAttackHighLeft);
SENSOR_MSG(SwordAttackHighRight);
SENSOR_MSG(SwordAttackLow);
SENSOR_MSG(SwordAttackLowLeft);
SENSOR_MSG(SwordAttackLowRight);
SENSOR_MSG(SwordBeamAttack);
SENSOR_MSG(SwordBeamReflectAttack);
SENSOR_MSG(SwordAttackJumpUnder);
SENSOR_MSG(ShieldGuard);
SENSOR_MSG(AskMultiPlayerEnemy);
SENSOR_MSG(ItemGettable);
SENSOR_MSG(KikkiThrow);
SENSOR_MSG(IsKikkiThrowTarget);
SENSOR_MSG(PlayerCloudGet);
SENSOR_MSG(AutoJump);
SENSOR_MSG(PlayerTouchShadow);
SENSOR_MSG(PlayerPullOutShadow);
SENSOR_MSG(PlayerAttackShadow);
SENSOR_MSG(PlayerAttackShadowStrong);
SENSOR_MSG_WITH_DATA(PlayerAttackChangePos, (sead::Vector3f*, Pos));
SENSOR_MSG(AtmosOnlineLight);
SENSOR_MSG(LightBurn);
SENSOR_MSG(MoonLightBurn);

SENSOR_MSG_WITH_DATA(String, (const char*, Str));
SENSOR_MSG_WITH_DATA(StringV4fPtr, (const char*, Str), (sead::Vector4f*, Vec));
SENSOR_MSG_WITH_DATA(StringV4fSensorPtr, (const char*, Str), (sead::Vector4f*, Vec),
                     (HitSensor*, Sensor));
SENSOR_MSG_WITH_DATA(StringVoidPtr, (const char*, Str), (void*, Ptr));

// TODO: rename variables
SENSOR_MSG_WITH_DATA_CUSTOM_CTOR(CollidePush, ((sead::Vector3f, Vec)),
                                 ((const sead::Vector3f&, Vec))) {
    mVec.set(pVec);
}

// TODO: rename variables
SENSOR_MSG_WITH_DATA_CUSTOM_CTOR(CollisionImpulse,
                                 ((sead::Vector3f*, VecPtr), (const sead::Vector3f*, ConstVec),
                                  (f32, FloatVal), (const sead::Vector3f*, ConstVec2),
                                  (f32, FloatVal2)),
                                 ((sead::Vector3f*, VecPtr), (const sead::Vector3f&, VecRef),
                                  (f32, FloatVal), (const sead::Vector3f&, VecRef2),
                                  (f32, FloatVal2))) {
    mVecPtr = pVecPtr;
    mConstVec = &pVecRef;
    mFloatVal = pFloatVal;
    mConstVec2 = &pVecRef2;
    mFloatVal2 = pFloatVal2;
}

}  // namespace al
