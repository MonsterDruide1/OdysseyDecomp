#pragma once

#include <math/seadVector.h>
#include <prim/seadRuntimeTypeInfo.h>

// TODO: This defines the class but the sead decomp doesn't have anything inside the RTTI functions,
// causing the functions in the vtable to be exported
#define SENSOR_MSG(Type)                                                                           \
    class SensorMsg##Type : public al::SensorMsg {                                                 \
        SEAD_RTTI_OVERRIDE(SensorMsg##Type, al::SensorMsg)                                         \
    };                                                                                             \
    bool isMsg##Type(const al::SensorMsg* msg) {                                                   \
        return SensorMsg##Type::checkDerivedRuntimeTypeInfoStatic(msg->getRuntimeTypeInfo());      \
    }

namespace al {
class SensorMsg {
    SEAD_RTTI_BASE(SensorMsg);
};

class HitSensor;
class ComboCounter;
class LiveActor;
class SensorMsg;

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
                             const sead::Vector3f&, const f32, sead::Vector3f&, f32);
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
bool sendMsgAskSafetyPoint(HitSensor* receiver, HitSensor* sender, sead::Vector3f**);
bool sendMsgAskSafetyPointToColliderGround(LiveActor* receiver, HitSensor* sender,
                                           sead::Vector3f**);
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
bool isMsgPlayerTrampleForCrossoverSensor(const SensorMsg* msg, const HitSensor*, const HitSensor*);
bool isMsgPlayerTrampleReflectForCrossoverSensor(const SensorMsg* msg, const HitSensor*,
                                                 const HitSensor*);
bool isMsgPlayerUpperPunchForCrossoverSensor(const SensorMsg* msg, const HitSensor*,
                                             const HitSensor*, f32);
bool isMsgKickStoneTrampleForCrossoverSensor(const SensorMsg* msg, const HitSensor*,
                                             const HitSensor*);

bool sendMsgPushAndKillVelocityToTarget(LiveActor*, HitSensor*, HitSensor*);
bool tryReceiveMsgPushAndAddVelocity(LiveActor*, const SensorMsg*, const HitSensor*,
                                     const HitSensor*, f32);
bool tryReceiveMsgPushAndAddVelocityH(LiveActor*, const SensorMsg*, const HitSensor*,
                                      const HitSensor*, f32);
bool pushAndAddVelocity(LiveActor*, const HitSensor*, const HitSensor*, f32);
bool pushAndAddVelocityH(LiveActor*, const HitSensor*, const HitSensor*, f32);
bool pushAndAddVelocityV(LiveActor*, const HitSensor*, const HitSensor*, f32);
bool isMySensor(const HitSensor* sensor, const LiveActor* actor);
}  // namespace al
