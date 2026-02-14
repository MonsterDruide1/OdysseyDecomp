#pragma once

#include <basis/seadTypes.h>
#include <gfx/seadColor.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>
#include <prim/seadSafeString.h>

#include "Library/HitSensor/SensorMsgSetupUtil.h"

namespace al {
class ComboCounter;
class EventFlowExecutor;
class HitSensor;
class LiveActor;
class MtxConnector;
class ParabolicPath;
class SensorMsg;
class ScreenPointer;
class ScreenPointTarget;
}  // namespace al

class CapTargetInfo;
class DigPoint;
class FishingFish;
class GotogotonMark;
class HackEndParam;
class IUsePlayerHack;
class SaveObjInfo;
class TouchTargetInfo;
class WhipTargetInfo;

namespace rs {

enum class PlayerStainType {
    Wet,
    Snow,
    SandMoon,
};

bool sendMsgAckCheckpoint(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgAckGetShine(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgAckLifeUp(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgAckLifeMaxUp(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgAskRailCollision(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgBreakPartsBreak(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgBirdFlyAway(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgCameraAngleElevationResetFast(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgChorobonAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgConductLightning(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgDamageBallAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgDamageBallBodyAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgDonsukeAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgDonsukeGroundAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgDonsukePush(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgDragonAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgEatExplosion(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgElectricWireNoLimitDistance(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgEnemyAttack2D(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgEnemyAttack3D(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgEnemyAttackDash(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgHackAttackFire(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgEnemyAttackFireCollision(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgEnemyAttackPoison(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgEnemyAttackTRex(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgHackAttackPoison(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgConfirmPaintObj(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgConfirmPaintObjForSeed(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPaintAttackPoison(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgEnemyKick(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgRabbitKick(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgFishingAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgFishingCancel(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgFishingFishApproach(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgFishingFishFloatTouch(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgFishingItemGet(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgFishingLineTouch(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgFishingStart(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgFishingUpImmediatelyPrepare(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgFireBrosFireBallCollide(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgFireSwitchFire(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgFrogHackTrample(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgGhostRecordStart(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgGhostRecordEnd(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgGhostPlay(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgGhostStop(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgGiantWanderBossAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgGiantWanderBossBulletAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgGiantWanderBossBulletPush(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgGhostReverse(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgGhostCancel(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgGrowFlowerSeedDisablePush(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgGrowFlowerSeedNear(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgGrowPlantPush(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgGrowerAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgGrowerWallAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgGunetterAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgGunetterBodyTouch(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgHammerAttackDown(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgHammerAttackSide(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgHammerAttackSideCollide(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgItemAmiiboKoopa(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgIsExistPukupuku(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgJangoAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgJangoRemoveCap(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgKakkuKick(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgKillByBossBattleDemo(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgKillByHackFirstDemo(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgKillByHomeDemo(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgEndHomeDemo(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgKillByMoonRockDemo(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgKillByShineGet(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgKillBySwitchTimer(al::LiveActor* receiver);
bool sendMsgKoopaBindStart(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgKoopaCapPlayerFocusTarget(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgKoopaCapPunchFinishL(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgKoopaCapPunchFinishR(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgKoopaCapPunchInvincibleL(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgKoopaCapPunchInvincibleR(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgKoopaCapPunchKnockBackL(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgKoopaCapPunchKnockBackR(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgKoopaCapPunchL(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgKoopaCapPunchR(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgKoopaCapSpinAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgKoopaCatchKoopaCap(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgKoopaDashPunchAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgKoopaFire2D(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgKoopaFireBallAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgKoopaHackDamage(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgKoopaHackPunch(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgKoopaHackPunchCollide(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgKoopaHackTrample(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgKoopaInvalidHackPunchFaceToCollision(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgKoopaRingBeamInvalidTouch(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgKoopaTailAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgKoopaTouchFloor(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgKouraAttack2D(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgKouraItemGet2D(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgLaunchBlow(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgMeganeAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgMeganeHackTrample(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgMofumofuBulletUnexplosion(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgMoonBasementAttackMeteor(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgMoonBasementBreakShockwaveMeteor(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgMoonBasementRockSyncClippingRegist(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgMoonBasementRockSyncClippingInvalidate(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgMoonBasementRockSyncClippingValidate(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgAttachCactusNeedle(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgCactusNeedleAttack(al::HitSensor* receiver, al::HitSensor* sender,
                               al::ComboCounter* pComboCounter);
bool sendMsgCactusNeedleAttackStrong(al::HitSensor* receiver, al::HitSensor* sender,
                                     al::ComboCounter* pComboCounter);
bool sendMsgPlayerBallToss(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPlayerCarryCameraSubjectiveStart(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPlayerCarryCameraSubjectiveEnd(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPlayerCarryShineGetStart(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPlayerCarryShineGetEnd(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPlayerCapCatch(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPlayerCapHipDrop(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPlayerCapPush(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPlayerCapRecovery(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPlayerCapTouchJump(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPlayerCapTrample(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPlayerCoinDashGet(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPlayerEyePriorityTarget(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPlayerDisregardHomingAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPlayerDisregardTargetMarker(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPlayerEquipKoopaCap(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPlayerFireBallAttack2D(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPlayerFireBallAttack3D(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPlayerHipDropDemoTrigger(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPlayerHipDropHipDropSwitch(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPlayerItemGet2D(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPlayerJumpTakeOffFloor(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPlayerObjectWallHit(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPlayerObjLeapFrog(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPlayerPenguinAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPlayerPenguinAttackReflect(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPlayerPoleClimbKeep(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPlayerPoleClimbReaction(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPlayerRabbitGet(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPlayerRollingObjHit(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPlayerRollingWallHitDown(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPlayerRollingWallHitMove(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPlayerStartGrabCeil(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPlayerStartWallJump(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPlayerEndGrabCeil(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPlayerSwordAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPlayerTouchFloorJumpCode(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPlayerTrample2D(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPlayerUpperPunch2D(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPlayerObjUpperPunch2D(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPukupukuDash(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPukupukuKiss(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPukupukuRollingAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPush2D(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPushToFish(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPushToMotorcycle(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPushToPlayer(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgRadishAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgRadishReflect(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgRequestChangeKinokoSuper(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgRequestChangeFireFlower(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgRequestPlayerSandMoon(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgRequestPlayerSnow(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgRequestPlayerWet(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgStartHack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgSunshineAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgCancelHack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgCancelHackArea(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgCancelHackByDokan(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPackunEatStart(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgHackAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgHackAttackKick(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgHackAttackMapObj(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgHackDeathAreaSelfCheck(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgHackDemoEnd(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgHackDemoStart(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgHackInvalidEscape(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgHackInvalidEscapeNoReaction(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgHackMarioCheckpointFlagWarp(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgHackMarioDead(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgHackMarioDemo(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgHackMarioInWater(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgHackMoveRockForestPush(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgHackSelfCeilingCheckMiss(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgHackSyncDamageVisibility(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgWeaponItemGet(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgCapAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgCapAttackCollide(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgCapAttackStayRolling(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgCapAttackStayRollingCollide(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgCapAttackRailMove(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgCapGiantAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgCapReflect(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgCapReflectCollide(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgCapStartLockOn(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgCapKeepLockOn(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgCapCancelLockOn(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgCapHipDrop(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgCapObjHipDrop(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgCapObjHipDropReflect(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgCapIgnoreCancelLockOn(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgCapIgnoreCancelMissReaction(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgCapIgnoreCollisionCheck(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgCapItemGet(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgCapTrampolineAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgCatchBombThrough(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgCheckCarObstacle(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgCheckIsCardboardBox(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgBullHackAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgBullEnemyAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgEnemyAttackStrong(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgEnemyObjBreak(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgWhipAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgNoLimitTouchJump(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgNoticePlayerDamage(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgStatueDrop(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgStatueTrampleReflect(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgStatuePush(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgStatueSnap(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgHitGrowFlowerPot(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgHitGrowPlantPot(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgImplantGrowFlowerSeed(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgIcicleAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgIgnoreMirrorWarp(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgIgnoredByRunawayNpc(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgIgnorePushMotorcycle(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgSandSharkAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgSeedAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgSeedAttackBig(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgSeedAttackHold(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgSeedItemGet(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgSeedReflect(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgSenobiTrample(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgSenobiCancelStretch(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgSenobiPunchBlockTransparent(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgShibakenApproach(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgShibakenKick(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgHackUpperPunch(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgHackObjUpperPunch(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgShineGet(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgShineGet2D(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgSpherePush(al::HitSensor* receiver, al::HitSensor* sender,
                       const sead::Vector3f& pCenter, f32 pRadius);
bool sendMsgSphinxJumpAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgSphinxQuizRouteKill(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgSphinxRideAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgSphinxRideAttackReflect(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPechoSpot(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgBelowObjBroken(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgBindCollidedGround(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgBindKeepDemoStart(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgBindKeepDemoExecute(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgBindKeepDemoEnd(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgBindRecoveryLife(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgMayorItemReflect(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgMayorItemCollide(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgBlowObjAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgBlowObjAttackReflect(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgConfirmFrailBox(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgConfirmBrokenFrailBox(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgTankLookOn(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgTankKickHack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgTankKickEnemy(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgTankBullet(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgTankBulletNoReaction(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgTimerAthleticDemoStart(al::LiveActor* receiver);
bool sendMsgRideOnStart(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgRideOnEnd(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgRideOnRelease(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgHipDropTransformTransform(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgHipDropTransformReverse(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgHipDropTransformingUp(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgHipDropTransformingDown(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgHipDropTransformingFinish(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgClearFire(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgCollectAnimalTouchCollide(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgBossKnuckleCounter(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgBossKnuckleFallAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgBossKnuckleHackAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgBossKnuckleKillerAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgBossKnuckleIceConflict(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgBossKnuckleIceFallToMummy(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgSkaterAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgEnableInSaucePan(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgEnableMapCheckPointWarp(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgStartInSaucePan(al::HitSensor* receiver, al::HitSensor* sender, bool pIsSpawnShine);
bool sendMsgEndInSaucePan(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgLineDancerLink(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgLongPushSensorHit(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgLongPushBoxHit(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgGoldHammerAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgTrashBoxIn(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgTouchDoorDrumn(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgKuribo2DTouch(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgKuriboCollisionDamage(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgKuriboCollisionKill(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgKuriboFlick(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgKuriboTowerOn(al::HitSensor* receiver, al::HitSensor* sender, u32 pNum);
bool sendMsgPartyPopperSoundAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgYokinBallAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgYoshiDirectEat(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgYoshiTongueAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgYoshiTongueEatBind(al::HitSensor* receiver, al::HitSensor* sender, f32* pRadiusPtr,
                               f32* pOffsetPtr, f32* pScalePtr);
bool sendMsgYoshiTongueEatBindCancel(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgYoshiTongueEatBindFinish(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgYoshiTongueEatHomingTarget(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgYukimaruPush(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPunchMachinePunchStraight(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPunchMachinePunchHook(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPunchMachinePunchUpper(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgMorningStarWarpStart(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgMorningStarWarpEnd(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgGemyAim(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgHammerBrosHammerEnemyAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgHammerBrosHammerHackAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgHammerBrosHammerSearch(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgHackBrosContact(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgMotorcycleAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgMotorcycleCollideParkingLot(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgMotorcycleDashAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgMotorcycleDashCollide(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgWanwanEnemyAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgWanwanPush(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgWanwanReboundAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgWanwanBlockAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgWanwanHoleIn(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgWaterRoadIn(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgWaterRoadNear(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgBreedaSlap(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgBreedaPush(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgGamaneBullet(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgGamaneBulletThrough(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgGamaneBulletForCoinFlower(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgVolleyballNpcScareByEnemy(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgRocketFlowerExtension(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgWanderBossCameraRange(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgKuriboGirlAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgKuriboGirlLove(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgKuriboTop(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgTRexAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgTRexAttackCollideBody(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgTRexAttackCollideHead(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgTRexDashAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgTRexScrollPartsBreakWith(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgTsukkunNoTrace(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgTouchFireDrum2D(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPropellerAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgTankExplosion(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgTankHackTrample(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgUtsuboAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgKillerAttackNoExplode(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgKillerMagnumAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgKillerMagnumHackAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgGabuzouAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgStackerRollingAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgStackerCapBoostAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgIgnoreIgnitionBomb(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgExplosionReflectBomb(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgGolemStampPress(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgSwitchOnWithSaveRequest(al::LiveActor* receiver, SaveObjInfo* Info);
bool sendMsgWanwanReboundAttackToCollided(const al::LiveActor* actor, al::HitSensor* receiver);
bool sendMsgWanwanBlockAttackToCollided(const al::LiveActor* actor, al::HitSensor* receiver);
bool sendMsgDigPointSmell(al::HitSensor* receiver, al::HitSensor* sender, DigPoint* pDigPoint);
bool sendMsgMofumofuBodyChainExplode(al::HitSensor* receiver, al::HitSensor* sender,
                                     s32 pDelayStep);
bool sendMsgMoonBasementRockThroughCollision(al::HitSensor* receiver, al::HitSensor* sender,
                                             bool pIsFallOrBreak);
bool sendMsgFishingWait(al::HitSensor* receiver, al::HitSensor* sender, al::HitSensor* pHookSensor);
bool sendMsgFishingUpImmediately(al::HitSensor* receiver, al::HitSensor* sender,
                                 const sead::Matrix34f& mtx, const sead::Vector3f& trans,
                                 const char* materialCode);
bool sendMsgGunetterPush(al::HitSensor* receiver, al::HitSensor* sender,
                         const sead::Vector3f& pCenter, f32 pRadius);
bool sendMsgTestPunch(al::HitSensor* receiver, al::HitSensor* sender, const sead::Vector3f& pInfo,
                      s32 pHitId, s32 pTeamId);
bool sendMsgTestPunchStrong(al::HitSensor* receiver, al::HitSensor* sender,
                            const sead::Vector3f& pInfo, s32 pHitId, s32 pTeamId);
bool sendMsgPunchGuard(al::HitSensor* receiver, al::HitSensor* sender, s32 pPunchGuard,
                       s32 pTeamId);
bool sendMsgTsukkunThrust(al::HitSensor* receiver, al::HitSensor* sender,
                          const sead::Vector3f& pDir, s32 pHitId, bool pIsNonEnemy);
bool sendMsgTsukkunThrustSpin(al::HitSensor* receiver, al::HitSensor* sender,
                              const sead::Vector3f& pDir, s32 pHitId, bool pIsNonEnemy);
bool sendMsgTsukkunThrustReflect(al::HitSensor* receiver, al::HitSensor* sender,
                                 const sead::Vector3f& pDir, s32 pHitId, bool pIsNonEnemy);
bool sendMsgTsukkunThrustCollide(al::HitSensor* receiver, al::HitSensor* sender,
                                 const sead::Vector3f& pDir, s32 pHitId, bool pIsNonEnemy);
bool sendMsgTsukkunThrustHitReflectCollide(al::HitSensor* receiver, al::HitSensor* sender,
                                           const sead::Vector3f& pDir, s32 pHitId,
                                           bool pIsNonEnemy);
bool sendMsgTsukkunThrustReflectCollide(al::HitSensor* receiver, al::HitSensor* sender,
                                        const sead::Vector3f& pDir, s32 pHitId, bool pIsNonEnemy);
bool sendMsgTsukkunThrustHole(al::HitSensor* receiver, al::HitSensor* sender,
                              const sead::Vector3f& pTsukkunPos, const sead::Vector3f& pBeakPos);
bool sendMsgTsukkunThroughCollide(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgTsukkunHoldCollide(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgTsukkunForceCancelCollide(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgNetworkShootingShot(al::HitSensor* receiver, al::HitSensor* sender, s32 pUnk);
bool sendMsgNetworkShootingChargeShot(al::HitSensor* receiver, al::HitSensor* sender, s32 pUnk);
bool sendMsgRequestPlayerJumpBreakFloor(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgRequestPlayerJump(al::HitSensor* receiver, al::HitSensor* sender, f32 pPower);
bool sendMsgRequestPlayerTrampleJump(al::HitSensor* receiver, al::HitSensor* sender, f32 pPower);
bool sendMsgRequestPlayerSpinJump(al::HitSensor* receiver, al::HitSensor* sender, f32 pPower);
bool sendMsgRequestSphinxJump(al::HitSensor* receiver, al::HitSensor* sender, f32 pPower);
bool sendMsgIgnoreTouchTarget(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgIgnoreTouchTarget(al::ScreenPointer* pointer, al::ScreenPointTarget* sender);
bool sendMsgInitTouchTargetInfo(al::HitSensor* receiver, al::HitSensor* sender,
                                TouchTargetInfo* pInfo, const sead::Vector3f* pCollisionTouchPos);
bool sendMsgInitTouchTargetInfo(al::ScreenPointer* pointer, al::ScreenPointTarget* sender,
                                TouchTargetInfo* info, const sead::Vector3f* collisionTouchPos);
bool sendMsgCollisionImpulse(al::HitSensor* receiver, al::HitSensor* sender,
                             sead::Vector3f* pVecPtr, const sead::Vector3f& pConstVec,
                             f32 pFloatVal, const sead::Vector3f& pConstVec2, f32 pFloatVal2);
bool sendMsgWhipHold(al::HitSensor* receiver, al::HitSensor* sender, WhipTargetInfo* pInfo);
bool sendMsgWhipBind(al::HitSensor* receiver, al::HitSensor* sender, WhipTargetInfo* pInfo);
bool sendMsgWhipThrow(al::HitSensor* receiver, al::HitSensor* sender, const sead::Vector3f& pDir);
bool sendMsgMagnet(al::HitSensor* receiver, al::HitSensor* sender, bool pIsPower);
bool sendMsgMagnetBulletAttack(al::HitSensor* receiver, al::HitSensor* sender, f32 pUnk);
bool sendMsgDashPanel(al::HitSensor* receiver, al::HitSensor* sender, s32 pUnk);
bool sendMsgTrampolineCrackJump(al::HitSensor* receiver, al::HitSensor* sender, f32 pUnk,
                                f32 pUnk2);
bool sendMsgCapTouchWall(al::HitSensor* receiver, al::HitSensor* sender,
                         const sead::Vector3f& pHitPos, const sead::Vector3f& pNormal);
bool sendMsgCapRethrow(al::HitSensor* receiver, al::HitSensor* sender, const sead::Vector3f& pos,
                       const sead::Vector3f& front, const sead::Vector3f& up);
bool sendMsgCapRethrowReturnOnly(al::HitSensor* receiver, al::HitSensor* sender,
                                 const sead::Vector3f& pos, const sead::Vector3f& front,
                                 const sead::Vector3f& up);
bool sendMsgCapChangeGiant(al::HitSensor* receiver, al::HitSensor* sender, f32 pUnk, s32 pUnk2);
bool sendMsgPackunEatCancel(al::HitSensor* receiver, al::HitSensor* sender,
                            const sead::Vector3f& pPos, const sead::Vector3f& pFront);
bool sendMsgPackunEatEnd(al::HitSensor* receiver, al::HitSensor* sender, const sead::Vector3f& pPos,
                         const sead::Vector3f& pDir);
bool sendMsgPackunEatStartFollow(al::HitSensor* receiver, al::HitSensor* sender,
                                 const sead::Vector3f* pPosPtr);
bool sendMsgFireBlowerAttack(al::HitSensor* receiver, al::HitSensor* sender,
                             const sead::Vector3f& pPos, const sead::Vector3f& pDir, f32 pRadius);
bool sendMsgPaint(al::HitSensor* receiver, al::HitSensor* sender, const sead::Color4u8& pColor,
                  s32 pSize, s32 pDrawType);
bool sendMsgPaintTexture(al::HitSensor* receiver, al::HitSensor* sender, s32 pSize, f32 pRotation,
                         s32 pDrawType);
bool sendMsgCheckPaintClear(al::HitSensor* receiver, al::HitSensor* sender,
                            const sead::Color4u8& pColor, const sead::Vector3f& pPos,
                            s32 pDrawType);
bool sendMsgCheckPaintAlpha(al::HitSensor* receiver, al::HitSensor* sender,
                            const sead::Vector3f& pPos);
bool sendMsgByugoBlow(al::HitSensor* receiver, al::HitSensor* sender, const sead::Vector3f& pForce);
bool sendMsgHackDirectStageInit(al::HitSensor* receiver, al::HitSensor* sender,
                                IUsePlayerHack* pHack);
bool sendMsgObjSnapForce(al::HitSensor* receiver, al::HitSensor* sender,
                         const sead::Vector3f& pForce);
bool sendMsgCapBeamerBeam(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgHosuiAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgHosuiAttackCollide(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgHosuiAttackNoEffect(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgHosuiAttackStrong(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgHosuiTouch(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgHosuiTrample(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgHosuiTrampleReflect(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgHosuiTrampleReflectHigh(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgBubbleAttack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgBubbleAttackToPecho(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgBubbleReflectH(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgBubbleReflectV(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgBubbleWallTouch(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgBubbleGroundTouchTrigger(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgBubbleLauncherStart(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgSenobiPartsMove(al::HitSensor* receiver, al::HitSensor* sender,
                            const sead::Vector3f& pCollidedNormal, f32 pDistance);
bool sendMsgStampTo2D(al::HitSensor* receiver, al::HitSensor* sender, const sead::Vector3f& pForce);
bool sendMsgAirExplosion(al::HitSensor* receiver, al::HitSensor* sender,
                         const sead::Vector3f& pForce);
bool sendMsgPushVelocity(al::HitSensor* receiver, al::HitSensor* sender,
                         const sead::Vector3f& pVelocity);
bool sendMsgGhostRecordStartOk(al::HitSensor* receiver, al::HitSensor* sender, const char* pObjId);
bool sendMsgSandGeyserRaise(al::HitSensor* receiver, al::HitSensor* sender, f32 pUnk, f32 pUnk2);
bool sendMsgRaceWait(al::LiveActor* receiver);
bool sendMsgRaceStart(al::LiveActor* receiver);
bool sendMsgRaceStop(al::LiveActor* receiver);
bool sendMsgRaceReturnToCourse(al::LiveActor* receiver, const sead::Vector3f& pos,
                               const sead::Vector3f& front);
bool sendMsgPlayerLookAtPosition(al::HitSensor* receiver, al::HitSensor* sender,
                                 sead::Vector3f* pPos);
bool sendMsgTargetMarkerPosition(al::HitSensor* receiver, al::HitSensor* sender,
                                 sead::Vector3f* pPos);
bool sendMsgHackBlowJump(al::HitSensor* receiver, al::HitSensor* sender, const sead::Vector3f& pEnd,
                         f32 pHeight);
bool sendMsgGolemStampPushV(al::HitSensor* receiver, al::HitSensor* sender, f32 pVelocity);
bool sendMsgGolemStampPushH(al::HitSensor* receiver, al::HitSensor* sender, f32 pVelocity);
bool sendMsgRequestPlayerWaitAnimDigPoint(al::HitSensor* receiver, al::HitSensor* sender);
void sendMsgEventFlowScareCheck(al::HitSensor* receiver, al::HitSensor* sender,
                                al::EventFlowExecutor* executor);
bool sendMsgPlayerItemGetAll(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgPlayerItemGetAll2D(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgTRexAttackAll(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgSeedTouch(al::HitSensor* receiver, al::HitSensor* sender, const sead::Vector3f& punk,
                      const sead::Vector3f& punk2);
bool sendMsgSphinxRideAttackTouchThrough(al::HitSensor* receiver, al::HitSensor* sender,
                                         const sead::Vector3f& pPos, const sead::Vector3f& pNormal);
bool sendMsgSphinxRideAttackTouch(al::HitSensor* receiver, al::HitSensor* sender,
                                  const sead::Vector3f& pPos, const sead::Vector3f& pNormal);

bool isMsgAckCheckpoint(const al::SensorMsg* msg);
bool isMsgAckGetShine(const al::SensorMsg* msg);
bool isMsgAckLifeUp(const al::SensorMsg* msg);
bool isMsgAckLifeMaxUp(const al::SensorMsg* msg);
bool isMsgAskRailCollision(const al::SensorMsg* msg);
bool isMsgAttachCactusNeedle(const al::SensorMsg* msg);
bool isMsgAirExplosion(const al::SensorMsg* msg);
bool isMsgBelowObjBroken(const al::SensorMsg* msg);
bool isMsgBindCollidedGround(const al::SensorMsg* msg);
bool isMsgBindKeepDemoStart(const al::SensorMsg* msg);
bool isMsgBindKeepDemoExecute(const al::SensorMsg* msg);
bool isMsgBindKeepDemoEnd(const al::SensorMsg* msg);
bool isMsgBindRecoveryLife(const al::SensorMsg* msg);
bool isMsgBirdFlyAway(const al::SensorMsg* msg);
bool isMsgBlowObjAttack(const al::SensorMsg* msg);
bool isMsgMayorItemCollide(const al::SensorMsg* msg);
bool isMsgMayorItemReflect(const al::SensorMsg* msg);
bool isMsgBlowObjAttackReflect(const al::SensorMsg* msg);
bool isMsgBossKnuckleCounter(const al::SensorMsg* msg);
bool isMsgBossKnuckleFallAttack(const al::SensorMsg* msg);
bool isMsgBossKnuckleHackAttack(const al::SensorMsg* msg);
bool isMsgBossKnuckleIceConflict(const al::SensorMsg* msg);
bool isMsgBossKnuckleIceFallToMummy(const al::SensorMsg* msg);
bool isMsgBossKnuckleKillerAttack(const al::SensorMsg* msg);
bool isMsgBreakPartsBreak(const al::SensorMsg* msg);
bool isMsgBreedaSlap(const al::SensorMsg* msg);
bool isMsgBreedaPush(const al::SensorMsg* msg);
bool isMsgBubbleAttack(const al::SensorMsg* msg);
bool isMsgBubbleAttackToPecho(const al::SensorMsg* msg);
bool isMsgBubbleLauncherStart(const al::SensorMsg* msg);
bool isMsgBubbleReflectH(const al::SensorMsg* msg);
bool isMsgBubbleReflectV(const al::SensorMsg* msg);
bool isMsgBubbleWallTouch(const al::SensorMsg* msg);
bool isMsgBubbleGroundTouchTrigger(const al::SensorMsg* msg);
bool isMsgBullAttack(const al::SensorMsg* msg);
bool isMsgBullHackAttack(const al::SensorMsg* msg);
bool isMsgBullEnemyAttack(const al::SensorMsg* msg);
bool isMsgByugoBlow(const al::SensorMsg* msg);
bool isMsgCameraAngleElevationResetFast(const al::SensorMsg* msg);
bool isMsgCancelHack(const al::SensorMsg* msg);
bool isMsgCancelHackArea(const al::SensorMsg* msg);
bool isMsgCancelHackByDokan(const al::SensorMsg* msg);
bool isMsgCapAttack(const al::SensorMsg* msg);
bool isMsgCapAttackCollide(const al::SensorMsg* msg);
bool isMsgCapAttackRailMove(const al::SensorMsg* msg);
bool isMsgCapAttackStayRolling(const al::SensorMsg* msg);
bool isMsgCapAttackStayRollingCollide(const al::SensorMsg* msg);
bool isMsgCapBeamerBeam(const al::SensorMsg* msg);
bool isMsgCapCancelLockOn(const al::SensorMsg* msg);
bool isMsgCapChangeGiant(const al::SensorMsg* msg);
bool isMsgCapEnableLockOn(const al::SensorMsg* msg);
bool isMsgCapStartLockOn(const al::SensorMsg* msg);
bool isMsgCapKeepLockOn(const al::SensorMsg* msg);
bool isMsgCapGiantAttack(const al::SensorMsg* msg);
bool isMsgCapHipDrop(const al::SensorMsg* msg);
bool isMsgCapObjHipDrop(const al::SensorMsg* msg);
bool isMsgCapObjHipDropReflect(const al::SensorMsg* msg);
bool isMsgCapIgnoreCancelLockOn(const al::SensorMsg* msg);
bool isMsgCapIgnoreCancelMissReaction(const al::SensorMsg* msg);
bool isMsgCapIgnoreCollisionCheck(const al::SensorMsg* msg);
bool isMsgCapItemGet(const al::SensorMsg* msg);
bool isMsgCapReflect(const al::SensorMsg* msg);
bool isMsgCapReflectCollide(const al::SensorMsg* msg);
bool isMsgCapRethrow(const al::SensorMsg* msg);
bool isMsgCapTouchWall(const al::SensorMsg* msg);
bool isMsgCapTrampolineAttack(const al::SensorMsg* msg);
bool isMsgCatchBombThrough(const al::SensorMsg* msg);
bool isMsgCheckCarObstacle(const al::SensorMsg* msg);
bool isMsgCheckIsCardboardBox(const al::SensorMsg* msg);
bool isMsgChorobonAttack(const al::SensorMsg* msg);
bool isMsgClearFire(const al::SensorMsg* msg);
bool isMsgCollectAnimalTouchCollide(const al::SensorMsg* msg);
bool isMsgCollisionImpulse(const al::SensorMsg* msg);
bool isMsgConductLightning(const al::SensorMsg* msg);
bool isMsgConfirmFrailBox(const al::SensorMsg* msg);
bool isMsgConfirmBrokenFrailBox(const al::SensorMsg* msg);
bool isMsgDamageBallAttack(const al::SensorMsg* msg);
bool isMsgDamageBallBodyAttack(const al::SensorMsg* msg);
bool isMsgDigPointSmell(const al::SensorMsg* msg);
bool isMsgDonsukeAttack(const al::SensorMsg* msg);
bool isMsgDonsukeGroundAttack(const al::SensorMsg* msg);
bool isMsgDonsukePush(const al::SensorMsg* msg);
bool isMsgDragonAttack(const al::SensorMsg* msg);
bool isMsgEatExplosion(const al::SensorMsg* msg);
bool isMsgElectricWireNoLimitDistance(const al::SensorMsg* msg);
bool isMsgEnableInSaucePan(const al::SensorMsg* msg);
bool isMsgEnableMapCheckPointWarp(const al::SensorMsg* msg);
bool isMsgEndInSaucePan(const al::SensorMsg* msg);
bool isMsgEnemyAttack2D(const al::SensorMsg* msg);
bool isMsgEnemyAttack3D(const al::SensorMsg* msg);
bool isMsgEnemyAttackDash(const al::SensorMsg* msg);
bool isMsgFireBrosFireBallCollide(const al::SensorMsg* msg);
bool isMsgFireDamageAll(const al::SensorMsg* msg);
bool isMsgHackAttackFire(const al::SensorMsg* msg);
bool isMsgFireSwitchFire(const al::SensorMsg* msg);
bool isMsgEnemyAttackFireCollision(const al::SensorMsg* msg);
bool isMsgEnemyAttackTRex(const al::SensorMsg* msg);
bool isMsgPoisonDamageAll(const al::SensorMsg* msg);
bool isMsgEnemyAttackPoison(const al::SensorMsg* msg);
bool isMsgHackAttackPoison(const al::SensorMsg* msg);
bool isMsgPaintAttackPoison(const al::SensorMsg* msg);
bool isMsgConfirmPaintObj(const al::SensorMsg* msg);
bool isMsgConfirmPaintObjForSeed(const al::SensorMsg* msg);
bool isMsgEnemyAttackStrong(const al::SensorMsg* msg);
bool isMsgEnemyKick(const al::SensorMsg* msg);
bool isMsgRabbitKick(const al::SensorMsg* msg);
bool isMsgEnemyObjBreak(const al::SensorMsg* msg);
bool isMsgFireBlowerAttack(const al::SensorMsg* msg);
bool isMsgFishingAttack(const al::SensorMsg* msg);
bool isMsgFishingCancel(const al::SensorMsg* msg);
bool isMsgFishingFishApproach(const al::SensorMsg* msg);
bool isMsgFishingFishFloatTouch(const al::SensorMsg* msg);
bool isMsgFishingItemGet(const al::SensorMsg* msg);
bool isMsgFishingLineTouch(const al::SensorMsg* msg);
bool isMsgFishingStart(const al::SensorMsg* msg);
bool isMsgFishingUpImmediately(const al::SensorMsg* msg);
bool isMsgFishingUpImmediatelyPrepare(const al::SensorMsg* msg);
bool isMsgFishingWait(const al::SensorMsg* msg);
bool isMsgFrogHackTrample(const al::SensorMsg* msg);
bool isMsgGamaneBullet(const al::SensorMsg* msg);
bool isMsgGamaneBulletThrough(const al::SensorMsg* msg);
bool isMsgGamaneBulletForCoinFlower(const al::SensorMsg* msg);
bool isMsgGemyAim(const al::SensorMsg* msg);
bool isMsgGhostCancel(const al::SensorMsg* msg);
bool isMsgGhostPlay(const al::SensorMsg* msg);
bool isMsgGhostRecordEnd(const al::SensorMsg* msg);
bool isMsgGhostRecordStart(const al::SensorMsg* msg);
bool isMsgGhostRecordStartOk(const al::SensorMsg* msg);
bool isMsgGhostReverse(const al::SensorMsg* msg);
bool isMsgGhostStop(const al::SensorMsg* msg);
bool isMsgGiantWanderBossAttack(const al::SensorMsg* msg);
bool isMsgGiantWanderBossBulletAttack(const al::SensorMsg* msg);
bool isMsgGiantWanderBossBulletPush(const al::SensorMsg* msg);
bool isMsgGoldHammerAttack(const al::SensorMsg* msg);
bool isMsgGrowFlowerSeedDisablePush(const al::SensorMsg* msg);
bool isMsgGrowFlowerSeedNear(const al::SensorMsg* msg);
bool isMsgGrowPlantPush(const al::SensorMsg* msg);
bool isMsgGrowerAttack(const al::SensorMsg* msg);
bool isMsgGrowerWallAttack(const al::SensorMsg* msg);
bool isMsgGunetterAttack(const al::SensorMsg* msg);
bool isMsgGunetterBodyTouch(const al::SensorMsg* msg);
bool isMsgGunetterPush(const al::SensorMsg* msg);
bool isMsgHackAttack(const al::SensorMsg* msg);
bool isMsgHackAttackKick(const al::SensorMsg* msg);
bool isMsgHackAttackMapObj(const al::SensorMsg* msg);
bool isMsgHackBrosContact(const al::SensorMsg* msg);
bool isMsgHackDeathAreaSelfCheck(const al::SensorMsg* msg);
bool isMsgHackDemoEnd(const al::SensorMsg* msg);
bool isMsgHackDemoStart(const al::SensorMsg* msg);
bool isMsgHackInvalidEscape(const al::SensorMsg* msg);
bool isMsgHackInvalidEscapeNoReaction(const al::SensorMsg* msg);
bool isMsgHackMarioCheckpointFlagWarp(const al::SensorMsg* msg);
bool isMsgHackMarioDead(const al::SensorMsg* msg);
bool isMsgHackMarioDemo(const al::SensorMsg* msg);
bool isMsgHackMarioInWater(const al::SensorMsg* msg);
bool isMsgHackMoveRockForestPush(const al::SensorMsg* msg);
bool isMsgHackSelfCeilingCheckMiss(const al::SensorMsg* msg);
bool isMsgHackSyncDamageVisibility(const al::SensorMsg* msg);
bool isMsgHackUpperPunch(const al::SensorMsg* msg);
bool isMsgHackObjUpperPunch(const al::SensorMsg* msg);
bool isMsgHammerAttackDown(const al::SensorMsg* msg);
bool isMsgHammerAttackSide(const al::SensorMsg* msg);
bool isMsgHammerAttackSideCollide(const al::SensorMsg* msg);
bool isMsgHammerBrosHammerEnemyAttack(const al::SensorMsg* msg);
bool isMsgHammerBrosHammerHackAttack(const al::SensorMsg* msg);
bool isMsgHammerBrosHammerSearch(const al::SensorMsg* msg);
bool isMsgHipDropTransformReverse(const al::SensorMsg* msg);
bool isMsgHipDropTransformTransform(const al::SensorMsg* msg);
bool isMsgHipDropTransformingUp(const al::SensorMsg* msg);
bool isMsgHipDropTransformingDown(const al::SensorMsg* msg);
bool isMsgHipDropTransformingFinish(const al::SensorMsg* msg);
bool isMsgHitGrowFlowerPot(const al::SensorMsg* msg);
bool isMsgHitGrowPlantPot(const al::SensorMsg* msg);
bool isMsgHosuiAttack(const al::SensorMsg* msg);
bool isMsgHosuiAttackCollide(const al::SensorMsg* msg);
bool isMsgHosuiAttackNoEffect(const al::SensorMsg* msg);
bool isMsgHosuiAttackStrong(const al::SensorMsg* msg);
bool isMsgHosuiTouch(const al::SensorMsg* msg);
bool isMsgHosuiTrample(const al::SensorMsg* msg);
bool isMsgHosuiTrampleReflect(const al::SensorMsg* msg);
bool isMsgHosuiTrampleReflectHigh(const al::SensorMsg* msg);
bool isMsgIgnoreTouchTarget(const al::SensorMsg* msg);
bool isMsgIgnorePushMotorcycle(const al::SensorMsg* msg);
bool isMsgIcicleAttack(const al::SensorMsg* msg);
bool isMsgIgnoreMirrorWarp(const al::SensorMsg* msg);
bool isMsgIgnoredByRunawayNpc(const al::SensorMsg* msg);
bool isMsgImplantGrowFlowerSeed(const al::SensorMsg* msg);
bool isMsgInitTouchTargetInfo(const al::SensorMsg* msg);
bool isMsgItemAmiiboKoopa(const al::SensorMsg* msg);
bool isMsgIsExistPukupuku(const al::SensorMsg* msg);
bool isMsgJangoAttack(const al::SensorMsg* msg);
bool isMsgJangoRemoveCap(const al::SensorMsg* msg);
bool isMsgKakkuKick(const al::SensorMsg* msg);
bool isMsgKoopaBindStart(const al::SensorMsg* msg);
bool isMsgKoopaCapPlayerFocusTarget(const al::SensorMsg* msg);
bool isMsgKoopaCapPunchFinishL(const al::SensorMsg* msg);
bool isMsgKoopaCapPunchFinishR(const al::SensorMsg* msg);
bool isMsgKoopaCapPunchInvincibleL(const al::SensorMsg* msg);
bool isMsgKoopaCapPunchInvincibleR(const al::SensorMsg* msg);
bool isMsgKoopaCapPunchKnockBackL(const al::SensorMsg* msg);
bool isMsgKoopaCapPunchKnockBackR(const al::SensorMsg* msg);
bool isMsgKoopaCapPunchL(const al::SensorMsg* msg);
bool isMsgKoopaCapPunchR(const al::SensorMsg* msg);
bool isMsgKoopaCapSpinAttack(const al::SensorMsg* msg);
bool isMsgKoopaCatchKoopaCap(const al::SensorMsg* msg);
bool isMsgKoopaDashPunchAttack(const al::SensorMsg* msg);
bool isMsgKoopaFire2D(const al::SensorMsg* msg);
bool isMsgKoopaFireBallAttack(const al::SensorMsg* msg);
bool isMsgKoopaHackDamage(const al::SensorMsg* msg);
bool isMsgKoopaHackPunch(const al::SensorMsg* msg);
bool isMsgKoopaHackPunchCollide(const al::SensorMsg* msg);
bool isMsgKoopaHackTrample(const al::SensorMsg* msg);
bool isMsgKoopaInvalidHackPunchFaceToCollision(const al::SensorMsg* msg);
bool isMsgKoopaRingBeamInvalidTouch(const al::SensorMsg* msg);
bool isMsgKoopaTailAttack(const al::SensorMsg* msg);
bool isMsgKoopaTouchFloor(const al::SensorMsg* msg);
bool isMsgKouraAttack2D(const al::SensorMsg* msg);
bool isMsgKouraItemGet2D(const al::SensorMsg* msg);
bool isMsgKuribo2DTouch(const al::SensorMsg* msg);
bool isMsgKuriboCollisionDamage(const al::SensorMsg* msg);
bool isMsgKuriboCollisionKill(const al::SensorMsg* msg);
bool isMsgKuriboFlick(const al::SensorMsg* msg);
bool isMsgKuriboGirlAttack(const al::SensorMsg* msg);
bool isMsgKuriboGirlLove(const al::SensorMsg* msg);
bool isMsgKuriboTop(const al::SensorMsg* msg);
bool isMsgKuriboTowerNum(const al::SensorMsg* msg);
bool isMsgLaunchBlow(const al::SensorMsg* msg);
bool isMsgLineDancerLink(const al::SensorMsg* msg);
bool isMsgLongPushSensorHit(const al::SensorMsg* msg);
bool isMsgLongPushBoxHit(const al::SensorMsg* msg);
bool isMsgMagnetBulletAttack(const al::SensorMsg* msg);
bool isMsgMeganeAttack(const al::SensorMsg* msg);
bool isMsgMeganeHackTrample(const al::SensorMsg* msg);
bool isMsgMofumofuBodyChainExplode(const al::SensorMsg* msg);
bool isMsgMofumofuBulletUnexplosion(const al::SensorMsg* msg);
bool isMsgMoonBasementAttackMeteor(const al::SensorMsg* msg);
bool isMsgMoonBasementBreakShockwaveMeteor(const al::SensorMsg* msg);
bool isMsgMoonBasementRockSyncClippingRegist(const al::SensorMsg* msg);
bool isMsgMoonBasementRockSyncClippingInvalidate(const al::SensorMsg* msg);
bool isMsgMoonBasementRockSyncClippingValidate(const al::SensorMsg* msg);
bool isMsgMoonBasementRockThroughCollision(const al::SensorMsg* msg);
bool isMsgMorningStarWarpEnd(const al::SensorMsg* msg);
bool isMsgMorningStarWarpStart(const al::SensorMsg* msg);
bool isMsgMotorcycleAttack(const al::SensorMsg* msg);
bool isMsgMotorcycleCollideParkingLot(const al::SensorMsg* msg);
bool isMsgMotorcycleDashAttack(const al::SensorMsg* msg);
bool isMsgMotorcycleDashCollide(const al::SensorMsg* msg);
bool isMsgCactusNeedleAttack(const al::SensorMsg* msg);
bool isMsgCactusNeedleAttackStrong(const al::SensorMsg* msg);
bool isMsgNoLimitTouchJump(const al::SensorMsg* msg);
bool isMsgNoticePlayerDamage(const al::SensorMsg* msg);
bool isMsgNpcScareByEnemy(const al::SensorMsg* msg);
bool isMsgVolleyballNpcScareByEnemy(const al::SensorMsg* msg);
bool isMsgObjSnapForce(const al::SensorMsg* msg);
bool isMsgPackunEatCancel(const al::SensorMsg* msg);
bool isMsgPackunEatEnd(const al::SensorMsg* msg);
bool isMsgPackunEatStart(const al::SensorMsg* msg);
bool isMsgPackunEatStartFollow(const al::SensorMsg* msg);
bool isMsgPaint(const al::SensorMsg* msg);
bool isMsgPaintTexture(const al::SensorMsg* msg);
bool isMsgCheckPaintClear(const al::SensorMsg* msg);
bool isMsgCheckPaintAlpha(const al::SensorMsg* msg);
bool isMsgPartyPopperSoundAttack(const al::SensorMsg* msg);
bool isMsgPechoSpot(const al::SensorMsg* msg);
bool isMsgPlayerBallToss(const al::SensorMsg* msg);
bool isMsgPlayerCarryCameraSubjectiveStart(const al::SensorMsg* msg);
bool isMsgPlayerCarryCameraSubjectiveEnd(const al::SensorMsg* msg);
bool isMsgPlayerCarryShineGetStart(const al::SensorMsg* msg);
bool isMsgPlayerCarryShineGetEnd(const al::SensorMsg* msg);
bool isMsgPlayerCapCatch(const al::SensorMsg* msg);
bool isMsgPlayerCapHipDrop(const al::SensorMsg* msg);
bool isMsgPlayerCapPush(const al::SensorMsg* msg);
bool isMsgPlayerCapRecovery(const al::SensorMsg* msg);
bool isMsgPlayerCapTouchJump(const al::SensorMsg* msg);
bool isMsgPlayerCapTrample(const al::SensorMsg* msg);
bool isMsgPlayerCoinDashGet(const al::SensorMsg* msg);
bool isMsgPlayerEyePriorityTarget(const al::SensorMsg* msg);
bool isMsgPlayerDisregardHomingAttack(const al::SensorMsg* msg);
bool isMsgPlayerDisregardTargetMarker(const al::SensorMsg* msg);
bool isMsgPlayerEquipKoopaCap(const al::SensorMsg* msg);
bool isMsgPlayerFireBallAttack2D(const al::SensorMsg* msg);
bool isMsgPlayerFireBallAttack3D(const al::SensorMsg* msg);
bool isMsgPlayerHipDropDemoTrigger(const al::SensorMsg* msg);
bool isMsgPlayerHipDropHipDropSwitch(const al::SensorMsg* msg);
bool isMsgPlayerItemGet2D(const al::SensorMsg* msg);
bool isMsgPlayerJumpTakeOffFloor(const al::SensorMsg* msg);
bool isMsgPlayerObjectWallHit(const al::SensorMsg* msg);
bool isMsgPlayerObjLeapFrog(const al::SensorMsg* msg);
bool isMsgPlayerPenguinAttack(const al::SensorMsg* msg);
bool isMsgPlayerPenguinAttackReflect(const al::SensorMsg* msg);
bool isMsgPlayerPoleClimbKeep(const al::SensorMsg* msg);
bool isMsgPlayerPoleClimbReaction(const al::SensorMsg* msg);
bool isMsgPlayerRabbitGet(const al::SensorMsg* msg);
bool isMsgPlayerRollingObjHit(const al::SensorMsg* msg);
bool isMsgPlayerRollingWallHitDown(const al::SensorMsg* msg);
bool isMsgPlayerRollingWallHitMove(const al::SensorMsg* msg);
bool isMsgPlayerStartGrabCeil(const al::SensorMsg* msg);
bool isMsgPlayerStartWallJump(const al::SensorMsg* msg);
bool isMsgPlayerEndGrabCeil(const al::SensorMsg* msg);
bool isMsgPlayerSwordAttack(const al::SensorMsg* msg);
bool isMsgPlayerTouchFloorJumpCode(const al::SensorMsg* msg);
bool isMsgPlayerTrample2D(const al::SensorMsg* msg);
bool isMsgPlayerUpperPunch2D(const al::SensorMsg* msg);
bool isMsgPlayerObjUpperPunch2D(const al::SensorMsg* msg);
bool isMsgPropellerAttack(const al::SensorMsg* msg);
bool isMsgPukupukuDash(const al::SensorMsg* msg);
bool isMsgPukupukuKiss(const al::SensorMsg* msg);
bool isMsgPukupukuRollingAttack(const al::SensorMsg* msg);
bool isMsgPunchMachinePunchHook(const al::SensorMsg* msg);
bool isMsgPunchMachinePunchStraight(const al::SensorMsg* msg);
bool isMsgPunchMachinePunchUpper(const al::SensorMsg* msg);
bool isMsgPush2D(const al::SensorMsg* msg);
bool isMsgPushToFish(const al::SensorMsg* msg);
bool isMsgPushToMotorcycle(const al::SensorMsg* msg);
bool isMsgPushToPlayer(const al::SensorMsg* msg);
bool isMsgRadishAttack(const al::SensorMsg* msg);
bool isMsgRadishReflect(const al::SensorMsg* msg);
bool isMsgRaceStart(const al::SensorMsg* msg);
bool isMsgRaceStop(const al::SensorMsg* msg);
bool isMsgRaceWait(const al::SensorMsg* msg);
bool isMsgRequestChangeFireFlower(const al::SensorMsg* msg);
bool isMsgRequestChangeKinokoSuper(const al::SensorMsg* msg);
bool isMsgRequestPlayerJumpBreakFloor(const al::SensorMsg* msg);
bool isMsgRequestPlayerJump(const al::SensorMsg* msg);
bool isMsgRequestPlayerTrampleJump(const al::SensorMsg* msg);
bool isMsgRequestPlayerSpinJump(const al::SensorMsg* msg);
bool isMsgRequestSphinxJump(const al::SensorMsg* msg);
bool isMsgRideOnEnd(const al::SensorMsg* msg);
bool isMsgRideOnRelease(const al::SensorMsg* msg);
bool isMsgRideOnStart(const al::SensorMsg* msg);
bool isMsgRocketFlowerExtension(const al::SensorMsg* msg);
bool isMsgSandSharkAttack(const al::SensorMsg* msg);
bool isMsgSeedAttack(const al::SensorMsg* msg);
bool isMsgSeedAttackBig(const al::SensorMsg* msg);
bool isMsgSeedAttackHold(const al::SensorMsg* msg);
bool isMsgSeedItemGet(const al::SensorMsg* msg);
bool isMsgSeedReflect(const al::SensorMsg* msg);
bool isMsgSeedTouch(const al::SensorMsg* msg);
bool isMsgSenobiCancelStretch(const al::SensorMsg* msg);
bool isMsgSenobiPunchBlockTransparent(const al::SensorMsg* msg);
bool isMsgSenobiPartsMove(const al::SensorMsg* msg);
bool isMsgSenobiTrample(const al::SensorMsg* msg);
bool isMsgShibakenApproach(const al::SensorMsg* msg);
bool isMsgShibakenKick(const al::SensorMsg* msg);
bool isMsgSkaterAttack(const al::SensorMsg* msg);
bool isMsgSpherePush(const al::SensorMsg* msg);
bool isMsgSphinxJumpAttack(const al::SensorMsg* msg);
bool isMsgSphinxQuizRouteKill(const al::SensorMsg* msg);
bool isMsgSphinxRideAttack(const al::SensorMsg* msg);
bool isMsgSphinxRideAttackReflect(const al::SensorMsg* msg);
bool isMsgSphinxRideAttackTouchThrough(const al::SensorMsg* msg);
bool isMsgSphinxRideAttackTouch(const al::SensorMsg* msg);
bool isMsgStampTo2D(const al::SensorMsg* msg);
bool isMsgStartHack(const al::SensorMsg* msg);
bool isMsgStartInSaucePan(const al::SensorMsg* msg);
bool isMsgStatueDrop(const al::SensorMsg* msg);
bool isMsgStatueTrampleReflect(const al::SensorMsg* msg);
bool isMsgStatuePush(const al::SensorMsg* msg);
bool isMsgStatueSnap(const al::SensorMsg* msg);
bool isMsgSunshineAttack(const al::SensorMsg* msg);
bool isMsgTankBullet(const al::SensorMsg* msg);
bool isMsgTankBulletNoReaction(const al::SensorMsg* msg);
bool isMsgTankExplosion(const al::SensorMsg* msg);
bool isMsgTankHackTrample(const al::SensorMsg* msg);
bool isMsgTankKickHack(const al::SensorMsg* msg);
bool isMsgTankKickEnemy(const al::SensorMsg* msg);
bool isMsgTankLookOn(const al::SensorMsg* msg);
bool isMsgTestPunch(const al::SensorMsg* msg);
bool isMsgTestPunchStrong(const al::SensorMsg* msg);
bool isMsgTimerAthleticDemoStart(const al::SensorMsg* msg);
bool isMsgTouchDoorDrumn(const al::SensorMsg* msg);
bool isMsgTouchFireDrum2D(const al::SensorMsg* msg);
bool isMsgTrashBoxIn(const al::SensorMsg* msg);
bool isMsgTRexAttack(const al::SensorMsg* msg);
bool isMsgTRexAttackCollideAll(const al::SensorMsg* msg);
bool isMsgTRexAttackCollideBody(const al::SensorMsg* msg);
bool isMsgTRexAttackCollideHead(const al::SensorMsg* msg);
bool isMsgTRexDashAttack(const al::SensorMsg* msg);
bool isMsgTRexScrollPartsBreakWith(const al::SensorMsg* msg);
bool isMsgTsukkunForceCancelCollide(const al::SensorMsg* msg);
bool isMsgTsukkunHoldCollide(const al::SensorMsg* msg);
bool isMsgTsukkunThroughCollide(const al::SensorMsg* msg);
bool isMsgTsukkunThrustAll(const al::SensorMsg* msg);
bool isMsgTsukkunThrust(const al::SensorMsg* pMsg, bool* isNonEnemy);
bool isMsgTsukkunThrustCollide(const al::SensorMsg* pMsg, bool* isNonEnemy);
bool isMsgTsukkunNoTrace(const al::SensorMsg* msg);
bool isMsgTsukkunThrustHole(const al::SensorMsg* msg);
bool isMsgUtsuboAttack(const al::SensorMsg* msg);
bool isMsgWanderBossCameraRange(const al::SensorMsg* msg);
bool isMsgWanwanEnemyAttack(const al::SensorMsg* msg);
bool isMsgWanwanBlockAttack(const al::SensorMsg* msg);
bool isMsgWanwanHoleIn(const al::SensorMsg* msg);
bool isMsgWaterRoadIn(const al::SensorMsg* msg);
bool isMsgWaterRoadNear(const al::SensorMsg* msg);
bool isMsgWanwanPush(const al::SensorMsg* msg);
bool isMsgWanwanReboundAttack(const al::SensorMsg* msg);
bool isMsgWeaponItemGet(const al::SensorMsg* msg);
bool isMsgWhipAttack(const al::SensorMsg* msg);
bool isMsgWhipBind(const al::SensorMsg* msg);
bool isMsgWhipHold(const al::SensorMsg* msg);
bool isMsgWhipThrow(const al::SensorMsg* msg);
bool isMsgYokinBallAttack(const al::SensorMsg* msg);
bool isMsgYoshiDirectEat(const al::SensorMsg* msg);
bool isMsgYoshiTongueAttack(const al::SensorMsg* msg);
bool isMsgYoshiTongueEatBind(const al::SensorMsg* msg);
bool isMsgYoshiTongueEatBindCancel(const al::SensorMsg* msg);
bool isMsgYoshiTongueEatBindFinish(const al::SensorMsg* msg);
bool isMsgYoshiTongueEatHomingTarget(const al::SensorMsg* msg);
bool isMsgYukimaruPush(const al::SensorMsg* msg);
bool isMsgKillerAttackNoExplode(const al::SensorMsg* msg);
bool isMsgKillerMagnumAttack(const al::SensorMsg* msg);
bool isMsgKillerMagnumHackAttack(const al::SensorMsg* msg);
bool isMsgGabuzouAttack(const al::SensorMsg* msg);
bool isMsgStackerRollingAttack(const al::SensorMsg* msg);
bool isMsgStackerCapBoostAttack(const al::SensorMsg* msg);
bool isMsgIgnoreIgnitionBomb(const al::SensorMsg* msg);
bool isMsgExplosionReflectBomb(const al::SensorMsg* msg);
bool isMsgGolemStampPress(const al::SensorMsg* msg);
bool setMsgYoshiTongueEatBindRadiusAndOffset(const al::SensorMsg* pMsg, f32 radius, f32 offset);
bool setMsgYoshiTongueEatBindScale(const al::SensorMsg* pMsg, f32 scale);
bool isMsgTsukkunThrustSpin(const al::SensorMsg* pMsg, bool* isNonEnemy);
bool isMsgTsukkunThrustReflect(const al::SensorMsg* pMsg, bool* isNonEnemy);
bool isMsgTsukkunThrustHitReflectCollide(const al::SensorMsg* pMsg, bool* isNonEnemy);
bool isMsgTsukkunThrustReflectCollide(const al::SensorMsg* pMsg, bool* isNonEnemy);
bool isMsgSwitchOnWithSaveRequest(const al::SensorMsg* pMsg, SaveObjInfo** info);
bool isMsgNpcCapReactionAll(const al::SensorMsg* msg);
bool isMsgHackNpcCapReactionAll(const al::SensorMsg* msg);
bool checkMsgNpcTrampleReactionAll(const al::SensorMsg* msg, const al::HitSensor* other,
                                   const al::HitSensor* self, bool unk);
bool isMsgPlayerAndCapObjHipDropReflectAll(const al::SensorMsg* msg);
bool isMsgKoopaCapPunchAll(const al::SensorMsg* msg);
bool isMsgKoopaCapPunchInvincibleAll(const al::SensorMsg* msg);
bool isMsgKoopaCapPunchFinishAll(const al::SensorMsg* msg);
bool isMsgItemGet(const al::SensorMsg* msg);
bool isMsgItemGetByWeapon(const al::SensorMsg* msg);
bool isMsgItemGet2D(const al::SensorMsg* msg);
bool isMsgBlockUpperPunch2D(const al::SensorMsg* msg);
bool isMsgItemGetAll(const al::SensorMsg* msg);
bool isMsgShineGet(const al::SensorMsg* msg);
bool isMsgShineGet2D(const al::SensorMsg* msg);
bool isMsgShineReaction(const al::SensorMsg* msg);
bool isMsgKillByShineGet(const al::SensorMsg* msg);
bool isMsgKillByHomeDemo(const al::SensorMsg* msg);
bool isMsgEndHomeDemo(const al::SensorMsg* msg);
bool isMsgBreakBySword(const al::SensorMsg* msg);
bool isMsgPressDown(const al::SensorMsg* msg);
bool isMsgPlayerAndCapObjHipDropAll(const al::SensorMsg* msg);
bool isMsgAttackDirect(const al::SensorMsg* msg);
bool isMsgBlowDown(const al::SensorMsg* msg);
bool isMsgTrampleReflectAll(const al::SensorMsg* msg);
bool isMsgThrowObjHit(const al::SensorMsg* msg);
bool isMsgThrowObjHitReflect(const al::SensorMsg* msg);
bool isMsgPlayerAndCapHipDropAll(const al::SensorMsg* msg);
bool isMsgUpperPunchAll(const al::SensorMsg* msg);
bool sendMsgBlockUpperPunch2D(al::HitSensor* receiver, al::HitSensor* sender);
bool isMsgBlockReaction2D(const al::SensorMsg* msg);
bool isMsgBlockReaction3D(const al::SensorMsg* msg);
bool isMsgBlockReactionAll(const al::SensorMsg* msg);
bool isMsgBreakFrailBox(const al::SensorMsg* msg);
bool isMsgDamageFrailBox(const al::SensorMsg* msg);
bool isMsgChorobonReaction(const al::SensorMsg* msg);
bool isMsgBreakSignBoard(const al::SensorMsg* msg);
bool isMsgBreakCollapseSandHill(const al::SensorMsg* msg);
void calcBlowDownDir(sead::Vector3f* dir, const al::SensorMsg* pMsg, al::HitSensor* receiver,
                     al::HitSensor* sender);
bool tryGetTsukkunThrustDir(sead::Vector3f* dir, const al::SensorMsg* pMsg);
bool isMsgPlayerDamage(const al::SensorMsg* msg);
bool isMsgPlayerDamage2D(const al::SensorMsg* msg);
bool isMsgPlayerDamageBlowDown(const al::SensorMsg* msg);
bool isMsgPlayerJumpRequestAll(const al::SensorMsg* msg);
bool trySendMsgHackPunchToCeil(al::LiveActor* sender, al::HitSensor* receiver);
bool trySendMsgHackPunchToSensor(al::LiveActor* actor, al::HitSensor* receiver,
                                 al::HitSensor* sender);
bool isMsgDashPanel(const al::SensorMsg* pMsg, s32* unk);
bool isMsgNetworkShootingShot(const al::SensorMsg* pMsg, s32 unk);
bool isMsgNetworkShootingChargeShot(const al::SensorMsg* pMsg, s32 unk);
bool isMsgRaceReturnToCourse(const al::SensorMsg* pMsg, sead::Vector3f* pos, sead::Vector3f* front);
bool isFallingTargetMoonBasementRock(const al::SensorMsg* pMsg);
bool tryGetTestPunchInfo(sead::Vector3f* dir, s32* hitId, const al::SensorMsg* pMsg, s32 teamId);
bool tryGetTestPunchTeamId(s32* teamId, const al::SensorMsg* pMsg);
bool tryGetPunchGuard(s32* punchGuard, const al::SensorMsg* pMsg, s32 teamId);
bool tryGetTsukkunThrustInfo(sead::Vector3f* dir, s32* hitId, const al::SensorMsg* pMsg);
bool tryGetTsukkunThrustReflectDir(sead::Vector3f* dir, const al::SensorMsg* pMsg);
bool tryGetTsukkunThrustHole(sead::Vector3f* tsukkunPos, sead::Vector3f* beakPos,
                             const al::SensorMsg* pMsg);
bool tryGetRequestPlayerJumpInfo(f32* power, const al::SensorMsg* pMsg);
bool tryGetRequestPlayerTrampleJumpInfo(f32* power, const al::SensorMsg* pMsg);
bool tryGetRequestPlayerSpinJumpInfo(f32* power, const al::SensorMsg* pMsg);
bool tryGetRequestSphinxJumpInfo(f32* power, const al::SensorMsg* pMsg);
bool tryGetTouchTargetInfo(TouchTargetInfo** info, const al::SensorMsg* pMsg);
bool tryGetTouchTargetInfoCollisionTouchPos(sead::Vector3f* pos, const al::SensorMsg* pMsg);
bool tryInitTouchTargetInfoBySensorOffset(const al::SensorMsg* pMsg, const al::HitSensor* sensor,
                                          const sead::Vector3f& unk, const sead::Vector3f& unk2);
bool tryInitTouchTargetInfoByConnector(const al::SensorMsg* pMsg, const al::MtxConnector* connector,
                                       const sead::Vector3f& unk, const sead::Vector3f& unk2,
                                       bool unk3);
bool tryCalcCollisionImpulse(sead::Vector3f* out, f32 unk2, const sead::Vector3f& unk3, f32 unk4,
                             const al::SensorMsg* pMsg);
bool tryInitWhipTarget(const al::SensorMsg* pMsg, al::HitSensor* sensor,
                       const sead::Vector3f* trans);
bool tryGetWhipThrowDir(sead::Vector3f* dir, const al::SensorMsg* pMsg);
bool tryMagnetPower(bool* isPower, const al::SensorMsg* pMsg);
bool tryMagnetBulletAttack(f32* unk, const al::SensorMsg* pMsg);
bool isMsgTrampolineCrackJump(f32* unk, f32* unk2, const al::SensorMsg* pMsg);
void requestHitReactionToAttacker(const al::SensorMsg* msg, const al::HitSensor* other,
                                  const al::HitSensor* self);
void requestHitReactionToAttacker(const al::SensorMsg* msg, const al::HitSensor* other,
                                  const sead::Vector3f& pos);
bool tryGetCapTouchWallHitPos(sead::Vector3f* pos, const al::SensorMsg* pMsg);
bool tryGetCapTouchWallNormal(sead::Vector3f* normal, const al::SensorMsg* pMsg);
bool isCapRethrowReturnOnly(const al::SensorMsg* pMsg);
bool tryGetCapRethrowPos(sead::Vector3f* pos, const al::SensorMsg* pMsg);
bool tryGetCapRethrowFront(sead::Vector3f* front, const al::SensorMsg* pMsg);
bool tryGetCapRethrowUp(sead::Vector3f* up, const al::SensorMsg* pMsg);
bool tryGetHackBlowJump(sead::Vector3f* end, f32* height, const al::SensorMsg* pMsg);
bool tryGetGolemStampPushV(f32* velocity, const al::SensorMsg* pMsg);
bool tryGetGolemStampPushH(f32* velocity, const al::SensorMsg* pMsg);
bool tryGetRequestPlayerWaitAnim(const char** anim, const al::SensorMsg* pMsg);
bool tryGetCapChangeGiant(f32* unk, s32* unk2, const al::SensorMsg* pMsg);
bool tryGetByugoBlowForce(sead::Vector3f* force, const al::SensorMsg* pMsg);
bool tryGetObjSnapForce(sead::Vector3f* force, const al::SensorMsg* pMsg);
void getPackunEatCancelPosAndFront(sead::Vector3f* pos, sead::Vector3f* front,
                                   const al::SensorMsg* pMsg);
bool tryGetPackunEatEndPos(sead::Vector3f* pos, const al::SensorMsg* pMsg);
bool tryGetPackunEatEndDir(sead::Vector3f* dir, const al::SensorMsg* pMsg);
bool tryGetPackunEatStartFollowPos(const sead::Vector3f** posPtr, const al::SensorMsg* pMsg);
bool tryGetFireBlowerAttackPos(sead::Vector3f* pos, const al::SensorMsg* pMsg);
bool tryGetFireBlowerAttackDir(sead::Vector3f* dir, const al::SensorMsg* pMsg);
bool tryGetFireBlowerAttackRadius(f32* radius, const al::SensorMsg* pMsg);
bool tryGetPaintColor(sead::Color4u8* color, const al::SensorMsg* pMsg);
bool tryGetPaintSize(f32* size, const al::SensorMsg* pMsg);
bool tryGetPaintDrawType(s32* drawType, const al::SensorMsg* pMsg);
bool tryGetPaintTextureSize(f32* size, const al::SensorMsg* pMsg);
bool tryGetPaintTextureRotation(f32* rotation, const al::SensorMsg* pMsg);
bool tryGetPaintTextureDrawType(s32* drawType, const al::SensorMsg* pMsg);
bool tryGetCheckPaintClearColor(sead::Color4u8* color, const al::SensorMsg* pMsg);
bool tryGetCheckPaintClearPos(sead::Vector3f* pos, const al::SensorMsg* pMsg);
bool tryGetCheckPaintClearDrawType(s32* drawType, const al::SensorMsg* pMsg);
bool tryGetCheckPaintAlphaPos(sead::Vector3f* pos, const al::SensorMsg* pMsg);
bool tryGetSenobiPartsMoveDistance(f32* distance, const al::SensorMsg* pMsg);
bool tryGetSenobiPartsCollidedNormal(sead::Vector3f* collidedNormal, const al::SensorMsg* pMsg);
bool tryGetStampTo2DForce(sead::Vector3f* force, const al::SensorMsg* pMsg);
bool tryGetGhostStartOkObjId(sead::BufferedSafeString* objId, const al::SensorMsg* pMsg);
bool tryGetAirExplosionForce(sead::Vector3f* force, const al::SensorMsg* pMsg);
bool sendMsgGotogotonOn(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgGotogotonGetJumpPath(al::HitSensor* receiver, al::HitSensor* sender,
                                 al::ParabolicPath* pPath);
bool sendMsgGotogotonGoalExist(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgGotogotonGoalMatch(al::HitSensor* receiver, al::HitSensor* sender,
                               const GotogotonMark* pMark);
bool isMatchGotogotonGoal(const al::SensorMsg* pMsg, const GotogotonMark* mark);
bool isMsgGotogotonOn(const al::SensorMsg* msg);
bool isMsgGotogotonGoalExist(const al::SensorMsg* msg);
bool sendMsgBossMagmaBreathForce(al::HitSensor* receiver, al::HitSensor* sender,
                                 const sead::Vector3f& pForce);
bool sendMsgBossMagmaCatchPlayer(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgBossMagmaReleasePlayer(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgBossMagmaDeadDemoStart(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgBossMagmaDeadDemoEnd(al::HitSensor* receiver, al::HitSensor* sender,
                                 const sead::Vector3f& pTargetPos);
bool sendMsgBossMagmaResetPos(al::HitSensor* receiver, al::HitSensor* sender,
                              const sead::Vector3f& pResetPos);
bool sendMsgBossMagmaQueryToBubble(al::HitSensor* receiver, al::HitSensor* sender);
bool isMsgBossMagmaCatchPlayer(const al::SensorMsg* msg);
bool isMsgBossMagmaReleasePlayer(const al::SensorMsg* msg);
bool isMsgBossMagmaDeadDemoStart(const al::SensorMsg* msg);
bool isMsgBossMagmaDeadDemoEnd(const al::SensorMsg* msg);
bool tryGetBossMagmaDeadDemoEndTargetPos(const al::SensorMsg* pMsg, sead::Vector3f* targetPos);
bool tryGetBossMagmaBreathForce(const al::SensorMsg* pMsg, sead::Vector3f* force);
bool tryGetBossMagmaResetPos(const al::SensorMsg* pMsg, sead::Vector3f* resetPos);
bool isMsgBossMagmaQueryToBubble(const al::SensorMsg* msg);
s32 getMofumofuBodyChainExplodeDelayStep(const al::SensorMsg* pMsg);
bool isMsgMofumofuReflectAll(const al::SensorMsg* msg);
void calcFishingUpJugemDir(sead::Vector3f* jugemDir, const al::SensorMsg* pMsg);
const sead::Vector3f* getFishingUpFloatPos(const al::SensorMsg* pMsg);
const char* tryGetFishingUpFloatMaterialCode(const al::SensorMsg* pMsg);
al::HitSensor* getFishingHookSensor(const al::SensorMsg* pMsg);
bool sendMsgCheckFishingTarget(al::HitSensor* receiver, al::HitSensor* sender,
                               const FishingFish* pFish);
bool isMsgCheckFishingTarget(const al::SensorMsg* msg);
const FishingFish* getCheckFishingTarget(const al::SensorMsg* pMsg);
const sead::Vector3f& getGunetterPushCenter(const al::SensorMsg* pMsg);
f32 getGunetterPushRadius(const al::SensorMsg* pMsg);
u32 getNumKuriboTowerOn(const al::SensorMsg* pMsg);
bool isMsgPlayerLookAtPosition(const al::SensorMsg* msg);
void setMsgPlayerLookAtPosition(const al::SensorMsg* pMsg, const sead::Vector3f& pos);
bool isMsgTargetMarkerPosition(const al::SensorMsg* msg);
void setMsgTargetMarkerPosition(const al::SensorMsg* pMsg, const sead::Vector3f& pos);
bool isMsgSandGeyserRaise(const al::SensorMsg* msg);
bool tryGetSandGeyserRaise(const al::SensorMsg* pMsg, f32* unk, f32* unk2);
void tryTreasureBoxPlayerTrampleJump(al::LiveActor* treasureBox, al::HitSensor* receiver,
                                     al::HitSensor* sender, f32 power);
bool sendMsgPushToPlayerAndKillVelocityToTarget(al::LiveActor* actor, al::HitSensor* receiver,
                                                al::HitSensor* sender);
bool sendMsgPushToPlayerAndKillVelocityToTargetH(al::LiveActor* actor, al::HitSensor* receiver,
                                                 al::HitSensor* sender);
bool tryReceivePushAndAddVelocity(al::LiveActor* actor, const al::SensorMsg* pMsg);
bool tryReceiveMsgPushToPlayerAndAddVelocity(al::LiveActor* actor, const al::SensorMsg* pMsg,
                                             const al::HitSensor* other, const al::HitSensor* self,
                                             f32 power);
bool tryReceiveMsgPushToPlayerAndAddVelocityH(al::LiveActor* actor, const al::SensorMsg* pMsg,
                                              const al::HitSensor* other, const al::HitSensor* self,
                                              f32 power);
bool tryReceiveMsgPushToPlayerAndCalcPushTrans(sead::Vector3f* trans, const al::SensorMsg* msg,
                                               const al::LiveActor* actor,
                                               const al::HitSensor* other,
                                               const al::HitSensor* self, f32 power);
bool tryReceiveMsgPushToHackerAndCalcPushTrans(sead::Vector3f* trans, const al::SensorMsg* msg,
                                               const al::LiveActor* actor,
                                               const al::HitSensor* other,
                                               const al::HitSensor* self, f32 power, bool isYoshi);
bool tryReceiveMsgPushToYoshiNpcAndCalcPushTrans(sead::Vector3f* trans, const al::SensorMsg* msg,
                                                 const al::LiveActor* actor,
                                                 const al::HitSensor* other,
                                                 const al::HitSensor* self, f32 power);
bool tryReceiveMsgPushToCapAndCalcPushTrans(sead::Vector3f* trans, const al::SensorMsg* msg,
                                            const al::LiveActor* actor, const al::HitSensor* other,
                                            const al::HitSensor* self, f32 power);
bool tryReceiveMsgPushToGrowPlantAndCalcPushTrans(sead::Vector3f* trans, const al::SensorMsg* msg,
                                                  const al::LiveActor* actor,
                                                  const al::HitSensor* other,
                                                  const al::HitSensor* self, f32 power);
bool sendMsgInitCapTarget(al::HitSensor* receiver, al::HitSensor* sender,
                          const CapTargetInfo** pInfo);
bool isMsgInitCapTarget(const al::SensorMsg* msg);
void setCapTargetInfo(const al::SensorMsg* pMsg, const CapTargetInfo* info);
bool tryReceiveMsgInitCapTargetAndSetCapTargetInfo(const al::SensorMsg* msg,
                                                   const CapTargetInfo* info);
bool sendMsgTransferHack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgRequestTransferHack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgInitHack(al::HitSensor* receiver, al::HitSensor* sender);
bool sendMsgEndHack(al::HitSensor* receiver, al::HitSensor* sender, const HackEndParam* pParam);
bool isMsgTransferHack(const al::SensorMsg* msg);
bool isMsgRequestTransferHack(const al::SensorMsg* msg);
bool isMsgInitHack(const al::SensorMsg* msg);
bool isMsgEndHack(const HackEndParam** param, const al::SensorMsg* pMsg);
bool isMsgHackDirectStageInit(IUsePlayerHack** hack, const al::SensorMsg* pMsg);
bool isMsgKillByBossBattleDemo(const al::SensorMsg* msg);
bool isMsgKillByHackFirstDemo(const al::SensorMsg* msg);
bool isMsgKillByMoonRockDemo(const al::SensorMsg* msg);
// TODO: rename parameter
bool sendMsgNpcScareByEnemy(al::HitSensor* receiver, al::HitSensor* sender, s32 pUnk);
bool tryReceiveMsgNpcScareByEnemyIgnoreTargetHack(const al::SensorMsg* pMsg,
                                                  const CapTargetInfo* info);
bool tryIncrementComboCount(s32* count, const al::SensorMsg* msg);
al::ComboCounter* tryGetMsgComboCount(const al::SensorMsg* pMsg);
al::ComboCounter* getMsgComboCount(const al::SensorMsg* pMsg);
bool isRideOn(const al::HitSensor* other, const al::HitSensor* self);
bool isMsgKillBySwitchTimer(const al::SensorMsg* msg);
bool isVisibleChameleon(const al::SensorMsg* msg);
bool isMsgRequestPlayerStainWet(const al::SensorMsg* pMsg, s32* stainType);
s32 tryGetKuriboTowerNum(al::HitSensor* receiver, al::HitSensor* sender);
void setKuriboTowerNum(const al::SensorMsg* pMsg, s32 num);
const sead::Vector3f& getSpherePushCenter(const al::SensorMsg* pMsg);
f32 getSpherePushRadius(const al::SensorMsg* pMsg);
DigPoint* getSmellDigPoint(const al::SensorMsg* pMsg);

SENSOR_MSG(AckCheckpoint);
SENSOR_MSG(AckGetShine);
SENSOR_MSG(AckLifeUp);
SENSOR_MSG(AckLifeMaxUp);
SENSOR_MSG(AskRailCollision);
SENSOR_MSG(BreakPartsBreak);
SENSOR_MSG(BirdFlyAway);
SENSOR_MSG(CameraAngleElevationResetFast);
SENSOR_MSG(ChorobonAttack);
SENSOR_MSG(ConductLightning);
SENSOR_MSG(DamageBallAttack);
SENSOR_MSG(DamageBallBodyAttack);
SENSOR_MSG(DonsukeAttack);
SENSOR_MSG(DonsukeGroundAttack);
SENSOR_MSG(DonsukePush);
SENSOR_MSG(DragonAttack);
SENSOR_MSG(EatExplosion);
SENSOR_MSG(ElectricWireNoLimitDistance);
SENSOR_MSG(EnemyAttack2D);
SENSOR_MSG(EnemyAttack3D);
SENSOR_MSG(EnemyAttackDash);
SENSOR_MSG(HackAttackFire);
SENSOR_MSG(EnemyAttackFireCollision);
SENSOR_MSG(EnemyAttackPoison);
SENSOR_MSG(EnemyAttackTRex);
SENSOR_MSG(HackAttackPoison);
SENSOR_MSG(ConfirmPaintObj);
SENSOR_MSG(ConfirmPaintObjForSeed);
SENSOR_MSG(PaintAttackPoison);
SENSOR_MSG(EnemyKick);
SENSOR_MSG(RabbitKick);
SENSOR_MSG(FishingAttack);
SENSOR_MSG(FishingCancel);
SENSOR_MSG(FishingFishApproach);
SENSOR_MSG(FishingFishFloatTouch);
SENSOR_MSG(FishingItemGet);
SENSOR_MSG(FishingLineTouch);
SENSOR_MSG(FishingStart);
SENSOR_MSG(FishingUpImmediatelyPrepare);
SENSOR_MSG(FireBrosFireBallCollide);
SENSOR_MSG(FireSwitchFire);
SENSOR_MSG(FrogHackTrample);
SENSOR_MSG(GhostRecordStart);
SENSOR_MSG(GhostRecordEnd);
SENSOR_MSG(GhostPlay);
SENSOR_MSG(GhostStop);
SENSOR_MSG(GiantWanderBossAttack);
SENSOR_MSG(GiantWanderBossBulletAttack);
SENSOR_MSG(GiantWanderBossBulletPush);
SENSOR_MSG(GhostReverse);
SENSOR_MSG(GhostCancel);
SENSOR_MSG(GrowFlowerSeedDisablePush);
SENSOR_MSG(GrowFlowerSeedNear);
SENSOR_MSG(GrowPlantPush);
SENSOR_MSG(GrowerAttack);
SENSOR_MSG(GrowerWallAttack);
SENSOR_MSG(GunetterAttack);
SENSOR_MSG(GunetterBodyTouch);
SENSOR_MSG(HammerAttackDown);
SENSOR_MSG(HammerAttackSide);
SENSOR_MSG(HammerAttackSideCollide);
SENSOR_MSG(ItemAmiiboKoopa);
SENSOR_MSG(IsExistPukupuku);
SENSOR_MSG(JangoAttack);
SENSOR_MSG(JangoRemoveCap);
SENSOR_MSG(KakkuKick);
SENSOR_MSG(KillByBossBattleDemo);
SENSOR_MSG(KillByHackFirstDemo);
SENSOR_MSG(KillByHomeDemo);
SENSOR_MSG(EndHomeDemo);
SENSOR_MSG(KillByMoonRockDemo);
SENSOR_MSG(KillByShineGet);
SENSOR_MSG(KillBySwitchTimer);
SENSOR_MSG(KoopaBindStart);
SENSOR_MSG(KoopaCapPlayerFocusTarget);
SENSOR_MSG(KoopaCapPunchFinishL);
SENSOR_MSG(KoopaCapPunchFinishR);
SENSOR_MSG(KoopaCapPunchInvincibleL);
SENSOR_MSG(KoopaCapPunchInvincibleR);
SENSOR_MSG(KoopaCapPunchKnockBackL);
SENSOR_MSG(KoopaCapPunchKnockBackR);
SENSOR_MSG(KoopaCapPunchL);
SENSOR_MSG(KoopaCapPunchR);
SENSOR_MSG(KoopaCapSpinAttack);
SENSOR_MSG(KoopaCatchKoopaCap);
SENSOR_MSG(KoopaDashPunchAttack);
SENSOR_MSG(KoopaFire2D);
SENSOR_MSG(KoopaFireBallAttack);
SENSOR_MSG(KoopaHackDamage);
SENSOR_MSG(KoopaHackPunch);
SENSOR_MSG(KoopaHackPunchCollide);
SENSOR_MSG(KoopaHackTrample);
SENSOR_MSG(KoopaInvalidHackPunchFaceToCollision);
SENSOR_MSG(KoopaRingBeamInvalidTouch);
SENSOR_MSG(KoopaTailAttack);
SENSOR_MSG(KoopaTouchFloor);
SENSOR_MSG(KouraAttack2D);
SENSOR_MSG(KouraItemGet2D);
SENSOR_MSG(LaunchBlow);
SENSOR_MSG(MeganeAttack);
SENSOR_MSG(MeganeHackTrample);
SENSOR_MSG(MofumofuBulletUnexplosion);
SENSOR_MSG(MoonBasementAttackMeteor);
SENSOR_MSG(MoonBasementBreakShockwaveMeteor);
SENSOR_MSG(MoonBasementRockSyncClippingRegist);
SENSOR_MSG(MoonBasementRockSyncClippingInvalidate);
SENSOR_MSG(MoonBasementRockSyncClippingValidate);
SENSOR_MSG(AttachCactusNeedle);
SENSOR_MSG_COMBO(CactusNeedleAttack);
SENSOR_MSG_COMBO(CactusNeedleAttackStrong);
SENSOR_MSG(PlayerBallToss);
SENSOR_MSG(PlayerCarryCameraSubjectiveStart);
SENSOR_MSG(PlayerCarryCameraSubjectiveEnd);
SENSOR_MSG(PlayerCarryShineGetStart);
SENSOR_MSG(PlayerCarryShineGetEnd);
SENSOR_MSG(PlayerCapCatch);
SENSOR_MSG(PlayerCapHipDrop);
SENSOR_MSG(PlayerCapPush);
SENSOR_MSG(PlayerCapRecovery);
SENSOR_MSG(PlayerCapTouchJump);
SENSOR_MSG(PlayerCapTrample);
SENSOR_MSG(PlayerCoinDashGet);
SENSOR_MSG(PlayerEyePriorityTarget);
SENSOR_MSG(PlayerDisregardHomingAttack);
SENSOR_MSG(PlayerDisregardTargetMarker);
SENSOR_MSG(PlayerEquipKoopaCap);
SENSOR_MSG(PlayerFireBallAttack2D);
SENSOR_MSG(PlayerFireBallAttack3D);
SENSOR_MSG(PlayerHipDropDemoTrigger);
SENSOR_MSG(PlayerHipDropHipDropSwitch);
SENSOR_MSG(PlayerItemGet2D);
SENSOR_MSG(PlayerJumpTakeOffFloor);
SENSOR_MSG(PlayerObjectWallHit);
SENSOR_MSG(PlayerObjLeapFrog);
SENSOR_MSG(PlayerPenguinAttack);
SENSOR_MSG(PlayerPenguinAttackReflect);
SENSOR_MSG(PlayerPoleClimbKeep);
SENSOR_MSG(PlayerPoleClimbReaction);
SENSOR_MSG(PlayerRabbitGet);
SENSOR_MSG(PlayerRollingObjHit);
SENSOR_MSG(PlayerRollingWallHitDown);
SENSOR_MSG(PlayerRollingWallHitMove);
SENSOR_MSG(PlayerStartGrabCeil);
SENSOR_MSG(PlayerStartWallJump);
SENSOR_MSG(PlayerEndGrabCeil);
SENSOR_MSG(PlayerSwordAttack);
SENSOR_MSG(PlayerTouchFloorJumpCode);
SENSOR_MSG(PlayerTrample2D);
SENSOR_MSG(PlayerUpperPunch2D);
SENSOR_MSG(PlayerObjUpperPunch2D);
SENSOR_MSG(PukupukuDash);
SENSOR_MSG(PukupukuKiss);
SENSOR_MSG(PukupukuRollingAttack);
SENSOR_MSG(Push2D);
SENSOR_MSG(PushToFish);
SENSOR_MSG(PushToMotorcycle);
SENSOR_MSG(PushToPlayer);
SENSOR_MSG(RadishAttack);
SENSOR_MSG(RadishReflect);
SENSOR_MSG(RaceStart);
SENSOR_MSG(RaceStop);
SENSOR_MSG(RaceWait);
SENSOR_MSG(RequestChangeKinokoSuper);
SENSOR_MSG(RequestChangeFireFlower);
SENSOR_MSG(RequestPlayerJumpBreakFloor);
SENSOR_MSG_WITH_DATA(RequestPlayerJump, (f32, Power));
SENSOR_MSG_WITH_DATA(RequestPlayerTrampleJump, (f32, Power));
SENSOR_MSG_WITH_DATA(RequestPlayerSpinJump, (f32, Power));
SENSOR_MSG_WITH_DATA(RequestSphinxJump, (f32, Power));
SENSOR_MSG_WITH_DATA(RequestPlayerStainWet, (PlayerStainType, StainType));
SENSOR_MSG(StartHack);
SENSOR_MSG(SunshineAttack);
SENSOR_MSG(CancelHack);
SENSOR_MSG(CancelHackArea);
SENSOR_MSG(CancelHackByDokan);
SENSOR_MSG(PackunEatStart);
SENSOR_MSG(HackAttack);
SENSOR_MSG(HackAttackKick);
SENSOR_MSG(HackAttackMapObj);
SENSOR_MSG(HackDeathAreaSelfCheck);
SENSOR_MSG(HackDemoEnd);
SENSOR_MSG(HackDemoStart);
SENSOR_MSG(HackInvalidEscape);
SENSOR_MSG(HackInvalidEscapeNoReaction);
SENSOR_MSG(HackMarioCheckpointFlagWarp);
SENSOR_MSG(HackMarioDead);
SENSOR_MSG(HackMarioDemo);
SENSOR_MSG(HackMarioInWater);
SENSOR_MSG(HackMoveRockForestPush);
SENSOR_MSG(HackSelfCeilingCheckMiss);
SENSOR_MSG(HackSyncDamageVisibility);
SENSOR_MSG(WeaponItemGet);
SENSOR_MSG(CapAttack);
SENSOR_MSG(CapAttackCollide);
SENSOR_MSG(CapAttackStayRolling);
SENSOR_MSG(CapAttackStayRollingCollide);
SENSOR_MSG(CapAttackRailMove);
SENSOR_MSG(CapGiantAttack);
SENSOR_MSG(CapReflect);
SENSOR_MSG(CapReflectCollide);
SENSOR_MSG(CapStartLockOn);
SENSOR_MSG(CapKeepLockOn);
SENSOR_MSG(CapCancelLockOn);
SENSOR_MSG(CapHipDrop);
SENSOR_MSG(CapObjHipDrop);
SENSOR_MSG(CapObjHipDropReflect);
SENSOR_MSG(CapIgnoreCancelLockOn);
SENSOR_MSG(CapIgnoreCancelMissReaction);
SENSOR_MSG(CapIgnoreCollisionCheck);
SENSOR_MSG(CapItemGet);
SENSOR_MSG(CapTrampolineAttack);
SENSOR_MSG(CatchBombThrough);
SENSOR_MSG(CheckCarObstacle);
SENSOR_MSG(IsCardboardBox);
SENSOR_MSG(BullHackAttack);
SENSOR_MSG(BullEnemyAttack);
SENSOR_MSG(EnemyAttackStrong);
SENSOR_MSG(EnemyObjBreak);
SENSOR_MSG(WhipAttack);
SENSOR_MSG_WITH_DATA(WhipBind, (WhipTargetInfo*, Info));
SENSOR_MSG_WITH_DATA(WhipHold, (WhipTargetInfo*, Info));
SENSOR_MSG(NoLimitTouchJump);
SENSOR_MSG(NoticePlayerDamage);
SENSOR_MSG(StatueDrop);
SENSOR_MSG(StatueTrampleReflect);
SENSOR_MSG(StatuePush);
SENSOR_MSG(StatueSnap);
SENSOR_MSG(HitGrowFlowerPot);
SENSOR_MSG(HitGrowPlantPot);
SENSOR_MSG(ImplantGrowFlowerSeed);
SENSOR_MSG(IcicleAttack);
SENSOR_MSG(IgnoreMirrorWarp);
SENSOR_MSG(IsIgnoredByRunawayNpc);
SENSOR_MSG(IgnorePushMotorcycle);
SENSOR_MSG(SandSharkAttack);
SENSOR_MSG(SeedAttack);
SENSOR_MSG(SeedAttackBig);
SENSOR_MSG(SeedAttackHold);
SENSOR_MSG(SeedItemGet);
SENSOR_MSG(SeedReflect);
SENSOR_MSG(SenobiTrample);
SENSOR_MSG(SenobiCancelStretch);
SENSOR_MSG(SenobiPunchBlockTransparent);
SENSOR_MSG(ShibakenApproach);
SENSOR_MSG(ShibakenKick);
SENSOR_MSG(HackUpperPunch);
SENSOR_MSG(HackObjUpperPunch);
SENSOR_MSG(ShineGet);
SENSOR_MSG(ShineGet2D);
SENSOR_MSG(SphinxJumpAttack);
SENSOR_MSG(SphinxQuizRouteKill);
SENSOR_MSG(SphinxRideAttack);
SENSOR_MSG(SphinxRideAttackReflect);
SENSOR_MSG(PechoSpot);
SENSOR_MSG(BelowObjBroken);
SENSOR_MSG(BindCollidedGround);
SENSOR_MSG(BindKeepDemoStart);
SENSOR_MSG(BindKeepDemoExecute);
SENSOR_MSG(BindKeepDemoEnd);
SENSOR_MSG(BindRecoveryLife);
SENSOR_MSG(MayorItemReflect);
SENSOR_MSG(MayorItemCollide);
SENSOR_MSG(BlowObjAttack);
SENSOR_MSG(BlowObjAttackReflect);
SENSOR_MSG(ConfirmFrailBox);
SENSOR_MSG(ConfirmBrokenFrailBox);
SENSOR_MSG(TankLookOn);
SENSOR_MSG(TankKickHack);
SENSOR_MSG(TankKickEnemy);
SENSOR_MSG(TankBullet);
SENSOR_MSG(TankBulletNoReaction);
SENSOR_MSG(TimerAthleticDemoStart);
SENSOR_MSG(RideOnStart);
SENSOR_MSG(RideOnEnd);
SENSOR_MSG(RideOnRelease);
SENSOR_MSG(HipDropTransformTransform);
SENSOR_MSG(HipDropTransformReverse);
SENSOR_MSG(HipDropTransformingUp);
SENSOR_MSG(HipDropTransformingDown);
SENSOR_MSG(HipDropTransformingFinish);
SENSOR_MSG(ClearFire);
SENSOR_MSG(CollectAnimalTouchCollide);
SENSOR_MSG(BossKnuckleCounter);
SENSOR_MSG(BossKnuckleFallAttack);
SENSOR_MSG(BossKnuckleHackAttack);
SENSOR_MSG(BossKnuckleKillerAttack);
SENSOR_MSG(BossKnuckleIceConflict);
SENSOR_MSG(BossKnuckleIceFallToMummy);
SENSOR_MSG(SkaterAttack);
SENSOR_MSG(EnableInSaucePan);
SENSOR_MSG(EnableMapCheckPointWarp);
SENSOR_MSG_WITH_DATA(StartInSaucePan, (bool, IsSpawnShine));
SENSOR_MSG(EndInSaucePan);
SENSOR_MSG(LineDancerLink);
SENSOR_MSG(LongPushSensorHit);
SENSOR_MSG(LongPushBoxHit);
SENSOR_MSG(GoldHammerAttack);
SENSOR_MSG(TrashBoxIn);
SENSOR_MSG(TouchDoorDrumn);
SENSOR_MSG(Kuribo2DTouch);
SENSOR_MSG(KuriboCollisionDamage);
SENSOR_MSG(KuriboCollisionKill);
SENSOR_MSG(KuriboFlick);
SENSOR_MSG_WITH_DATA(KuriboTowerOn, (u32, Num));
SENSOR_MSG_WITH_DATA(KuriboTowerNum, (s32*, NumPtr));
SENSOR_MSG(PartyPopperSoundAttack);
SENSOR_MSG(YokinBallAttack);
SENSOR_MSG(YoshiDirectEat);
SENSOR_MSG(YoshiTongueAttack);
SENSOR_MSG_WITH_DATA(YoshiTongueEatBind, (f32*, RadiusPtr), (f32*, OffsetPtr), (f32*, ScalePtr));
SENSOR_MSG(YoshiTongueEatBindCancel);
SENSOR_MSG(YoshiTongueEatBindFinish);
SENSOR_MSG(YoshiTongueEatHomingTarget);
SENSOR_MSG(YukimaruPush);
SENSOR_MSG(PunchMachinePunchStraight);
SENSOR_MSG(PunchMachinePunchHook);
SENSOR_MSG(PunchMachinePunchUpper);
SENSOR_MSG(MorningStarWarpStart);
SENSOR_MSG(MorningStarWarpEnd);
SENSOR_MSG(GemyAim);
SENSOR_MSG(HammerBrosHammerEnemyAttack);
SENSOR_MSG(HammerBrosHammerHackAttack);
SENSOR_MSG(HammerBrosHammerSearch);
SENSOR_MSG(HackBrosContact);
SENSOR_MSG(MotorcycleAttack);
SENSOR_MSG(MotorcycleCollideParkingLot);
SENSOR_MSG(MotorcycleDashAttack);
SENSOR_MSG(MotorcycleDashCollide);
SENSOR_MSG(WanwanEnemyAttack);
SENSOR_MSG(WanwanPush);
SENSOR_MSG(WanwanReboundAttack);
SENSOR_MSG(WanwanBlockAttack);
SENSOR_MSG(WanwanHoleIn);
SENSOR_MSG(WaterRoadIn);
SENSOR_MSG(WaterRoadNear);
SENSOR_MSG(BreedaSlap);
SENSOR_MSG(BreedaPush);
SENSOR_MSG(GamaneBullet);
SENSOR_MSG(GamaneBulletThrough);
SENSOR_MSG(GamaneBulletForCoinFlower);
SENSOR_MSG(VolleyballNpcScareByEnemy);
SENSOR_MSG(RocketFlowerExtension);
SENSOR_MSG(WanderBossCameraRange);
SENSOR_MSG(KuriboGirlAttack);
SENSOR_MSG(KuriboGirlLove);
SENSOR_MSG(KuriboTop);
SENSOR_MSG(TRexAttack);
SENSOR_MSG(TRexAttackCollideBody);
SENSOR_MSG(TRexAttackCollideHead);
SENSOR_MSG(TRexDashAttack);
SENSOR_MSG(TRexScrollPartsBreakWith);
SENSOR_MSG(TsukkunForceCancelCollide);
SENSOR_MSG(TsukkunHoldCollide);
SENSOR_MSG(TsukkunThroughCollide);
SENSOR_MSG(TsukkunNoTrace);
SENSOR_MSG(TouchFireDrum2D);
SENSOR_MSG(PropellerAttack);
SENSOR_MSG(TankExplosion);
SENSOR_MSG(TankHackTrample);
SENSOR_MSG(UtsuboAttack);
SENSOR_MSG(KillerAttackNoExplode);
SENSOR_MSG(KillerMagnumAttack);
SENSOR_MSG(KillerMagnumHackAttack);
SENSOR_MSG(GabuzouAttack);
SENSOR_MSG(StackerRollingAttack);
SENSOR_MSG(StackerCapBoostAttack);
SENSOR_MSG(IgnoreIgnitionBomb);
SENSOR_MSG(ExplosionReflectBomb);
SENSOR_MSG(GolemStampPress);
SENSOR_MSG(CapBeamerBeam);
SENSOR_MSG(HosuiAttack);
SENSOR_MSG(HosuiAttackCollide);
SENSOR_MSG(HosuiAttackNoEffect);
SENSOR_MSG(HosuiAttackStrong);
SENSOR_MSG(HosuiTouch);
SENSOR_MSG(HosuiTrample);
SENSOR_MSG(HosuiTrampleReflect);
SENSOR_MSG(HosuiTrampleReflectHigh);
SENSOR_MSG(BubbleAttack);
SENSOR_MSG(BubbleAttackToPecho);
SENSOR_MSG(BubbleReflectH);
SENSOR_MSG(BubbleReflectV);
SENSOR_MSG(BubbleWallTouch);
SENSOR_MSG(BubbleGroundTouchTrigger);
SENSOR_MSG(BubbleLauncherStart);
SENSOR_MSG(BlockUpperPunch2D);
SENSOR_MSG(GotogotonOn);
SENSOR_MSG(GotogotonGoalExist);
SENSOR_MSG(BossMagmaCatchPlayer);
SENSOR_MSG(BossMagmaReleasePlayer);
SENSOR_MSG(BossMagmaDeadDemoStart);
SENSOR_MSG(BossMagmaQueryToBubble);
SENSOR_MSG(TransferHack);
SENSOR_MSG(RequestTransferHack);
SENSOR_MSG(InitHack);
SENSOR_MSG(ShineReaction);
SENSOR_MSG(BreakBySword);
SENSOR_MSG_WITH_DATA(RequestPlayerWaitAnim, (const char*, Anim));
SENSOR_MSG_WITH_DATA(PunchGuard, (s32, PunchGuard), (s32, TeamId));
SENSOR_MSG_WITH_DATA(FishingUpImmediately, (const sead::Vector3f*, FloatPos),
                     (const sead::Vector3f*, Trans), (const char*, MaterialCode))
SENSOR_MSG_WITH_DATA(SwitchOnWithSaveRequest, (SaveObjInfo*, Info));
SENSOR_MSG_WITH_DATA(DigPointSmell, (DigPoint*, DigPoint));
SENSOR_MSG_WITH_DATA(MofumofuBodyChainExplode, (s32, DelayStep));
SENSOR_MSG_WITH_DATA(MoonBasementRockThroughCollision, (bool, IsFallOrBreak));
SENSOR_MSG_WITH_DATA(FishingWait, (al::HitSensor*, HookSensor));
// NOTE: as this message is unused, a proper name for the parameter cannot be determined
SENSOR_MSG_WITH_DATA(NetworkShootingShot, (s32, Unk));
// NOTE: as this message is unused, a proper name for the parameter cannot be determined
SENSOR_MSG_WITH_DATA(NetworkShootingChargeShot, (s32, Unk));
// NOTE: as this message is unused, proper names for the parameters cannot be determined
SENSOR_MSG_WITH_DATA(CapChangeGiant, (f32, Unk), (s32, Unk2));
SENSOR_MSG(IgnoreTouchTarget);
SENSOR_MSG_WITH_DATA(TouchTargetInfo, (TouchTargetInfo*, Info),
                     (const sead::Vector3f*, CollisionTouchPos));
SENSOR_MSG_WITH_DATA(Magnet, (bool, IsPower));
// NOTE: as this message is unused, proper names for the parameters cannot be determined
SENSOR_MSG_WITH_DATA(MagnetBulletAttack, (f32, Unk));
// NOTE: as this message is unused, proper names for the parameters cannot be determined
SENSOR_MSG_WITH_DATA(DashPanel, (s32, Unk));
// NOTE: as this message is unused, proper names for the parameters cannot be determined
SENSOR_MSG_WITH_DATA(TrampolineCrackJump, (f32, Unk), (f32, Unk2));
// TODO: rename parameter
SENSOR_MSG_WITH_DATA(NpcScareByEnemy, (s32, Unk));
SENSOR_MSG_WITH_DATA(PackunEatStartFollow, (const sead::Vector3f*, PosPtr));
SENSOR_MSG_WITH_DATA(PaintTexture, (f32, Size), (f32, Rotation), (s32, DrawType));
SENSOR_MSG_WITH_DATA(HackDirectStageInit, (IUsePlayerHack*, Hack));
// TODO: Rename parameters
SENSOR_MSG_WITH_DATA(SandGeyserRaise, (f32, Unk), (f32, Unk2));
SENSOR_MSG_WITH_DATA(CheckFishingTarget, (const FishingFish*, Fish));
SENSOR_MSG_WITH_DATA(GolemStampPushV, (f32, Velocity));
SENSOR_MSG_WITH_DATA(GolemStampPushH, (f32, Velocity));
SENSOR_MSG_WITH_DATA(GotogotonGoalMatch, (const GotogotonMark*, Mark));
SENSOR_MSG_WITH_DATA(GotogotonGetJumpPath, (al::ParabolicPath*, Path));
SENSOR_MSG_WITH_DATA(PlayerLookAtPosition, (sead::Vector3f*, Pos));
SENSOR_MSG_WITH_DATA(TargetMarkerPosition, (sead::Vector3f*, Pos));
SENSOR_MSG_WITH_DATA(InitCapTarget, (const CapTargetInfo**, Info));
SENSOR_MSG_WITH_DATA(EndHack, (const HackEndParam*, Param));

// TODO: rename variables
// NOTE: This SensorMsg is identical to al::SensorMsgCollisionImpulse
SENSOR_MSG_WITH_DATA_CUSTOM_CTOR_DIRECT_GETTERS(
    CollisionImpulse,
    ((sead::Vector3f*, VecPtr), (const sead::Vector3f*, ConstVec), (f32, FloatVal),
     (const sead::Vector3f*, ConstVec2), (f32, FloatVal2)),
    ((sead::Vector3f*, VecPtr), (const sead::Vector3f&, VecRef), (f32, FloatVal),
     (const sead::Vector3f&, VecRef2), (f32, FloatVal2))) {
    mVecPtr = pVecPtr;
    mConstVec = &pVecRef;
    mFloatVal = pFloatVal;
    mConstVec2 = &pVecRef2;
    mFloatVal2 = pFloatVal2;
}

SENSOR_MSG_WITH_DATA_CUSTOM_CTOR(PackunEatCancel, ((sead::Vector3f, Pos), (sead::Vector3f, Front)),
                                 ((const sead::Vector3f&, Pos), (const sead::Vector3f&, Front))) {
    mPos.set(pPos);
    mFront.set(pFront);
}

SENSOR_MSG_WITH_DATA_CUSTOM_CTOR(CapRethrow,
                                 ((sead::Vector3f, Pos), (sead::Vector3f, Front),
                                  (sead::Vector3f, Up), (bool, IsReturnOnly)),
                                 ((const sead::Vector3f&, Pos), (const sead::Vector3f&, Front),
                                  (const sead::Vector3f&, Up), (bool, IsReturnOnly))) {
    mPos.set(pPos);
    mFront.set(pFront);
    mUp.set(pUp);
    mIsReturnOnly = pIsReturnOnly;
}

SENSOR_MSG_WITH_DATA_CUSTOM_CTOR(CapTouchWall, ((sead::Vector3f, HitPos), (sead::Vector3f, Normal)),
                                 ((const sead::Vector3f&, HitPos),
                                  (const sead::Vector3f&, Normal))) {
    mHitPos.set(pHitPos);
    mNormal.set(pNormal);
}

SENSOR_MSG_WITH_DATA_CUSTOM_CTOR(PackunEatEnd, ((sead::Vector3f, Pos), (sead::Vector3f, Dir)),
                                 ((const sead::Vector3f&, Pos), (const sead::Vector3f&, Dir))) {
    mPos.set(pPos);
    mDir.set(pDir);
}

SENSOR_MSG_WITH_DATA_CUSTOM_CTOR(Paint, ((sead::Color4u8, Color), (f32, Size), (s32, DrawType)),
                                 ((const sead::Color4u8&, Color), (f32, Size), (s32, DrawType))) {
    mColor = pColor;
    mSize = pSize;
    mDrawType = pDrawType;
}

SENSOR_MSG_WITH_DATA_CUSTOM_CTOR(CheckPaintClear,
                                 ((sead::Color4u8, Color), (sead::Vector3f, Pos), (s32, DrawType)),
                                 ((const sead::Color4u8&, Color), (const sead::Vector3f&, Pos),
                                  (s32, DrawType))) {
    mColor = pColor;
    mPos.set(pPos);
    mDrawType = pDrawType;
}

SENSOR_MSG_WITH_DATA_CUSTOM_CTOR(CheckPaintAlpha, ((sead::Vector3f, Pos)),
                                 ((const sead::Vector3f&, Pos))) {
    mPos.set(pPos);
}

SENSOR_MSG_WITH_DATA_CUSTOM_CTOR(SeedTouch,
                                 ((sead::Vector3f, Pos), (sead::Vector3f, CollidedNormal)),
                                 ((const sead::Vector3f&, Pos),
                                  (const sead::Vector3f&, CollidedNormal))) {
    mPos.set(pPos);
    mCollidedNormal.set(pCollidedNormal);
}

// NOTE: Distance is stored as an s32 even though the ctor takes it in as an f32 and
// tryGetSenobiPartsMoveDistance extractcs it as an f32
SENSOR_MSG_WITH_DATA_CUSTOM_CTOR(SenobiPartsMove,
                                 ((sead::Vector3f, CollidedNormal), (s32, Distance)),
                                 ((const sead::Vector3f&, CollidedNormal), (f32, Distance))) {
    mCollidedNormal.set(pCollidedNormal);
    mDistance = pDistance;
};

SENSOR_MSG_WITH_DATA_CUSTOM_CTOR(RaceReturnToCourse,
                                 ((sead::Vector3f, Pos), (sead::Vector3f, Front)),
                                 ((const sead::Vector3f&, Pos), (const sead::Vector3f&, Front))) {
    mPos.set(pPos);
    mFront.set(pFront);
}

SENSOR_MSG_WITH_DATA_CUSTOM_CTOR(BossMagmaBreathForce, ((sead::Vector3f, Force)),
                                 ((const sead::Vector3f&, Force))) {
    mForce.set(pForce);
}

SENSOR_MSG_WITH_DATA_CUSTOM_CTOR(BossMagmaDeadDemoEnd, ((sead::Vector3f, TargetPos)),
                                 ((const sead::Vector3f&, TargetPos))) {
    mTargetPos.set(pTargetPos);
}

SENSOR_MSG_WITH_DATA_CUSTOM_CTOR(BossMagmaResetPos, ((sead::Vector3f, ResetPos)),
                                 ((const sead::Vector3f&, ResetPos))) {
    mResetPos.set(pResetPos);
}

SENSOR_MSG_WITH_DATA_CUSTOM_CTOR(StampTo2D, ((sead::Vector3f, Force)),
                                 ((const sead::Vector3f&, Force))) {
    mForce.set(pForce);
}

SENSOR_MSG_WITH_DATA_CUSTOM_CTOR(PushVelocity, ((sead::Vector3f, Velocity)),
                                 ((const sead::Vector3f&, Velocity))) {
    mVelocity.set(pVelocity);
}

SENSOR_MSG_WITH_DATA_CUSTOM_CTOR(AirExplosion, ((sead::Vector3f, Force)),
                                 ((const sead::Vector3f&, Force))) {
    mForce.set(pForce);
}

SENSOR_MSG_WITH_DATA_CUSTOM_CTOR(ObjSnapForce, ((sead::Vector3f, Force)),
                                 ((const sead::Vector3f&, Force))) {
    mForce.set(pForce);
}

SENSOR_MSG_WITH_DATA_CUSTOM_CTOR(WhipThrow, ((sead::Vector3f, Dir)),
                                 ((const sead::Vector3f&, Dir))) {
    mDir.set(pDir);
};

SENSOR_MSG_WITH_DATA_CUSTOM_CTOR(ByugoBlow, ((sead::Vector3f, Force)),
                                 ((const sead::Vector3f&, Force))) {
    mForce.set(pForce);
};

SENSOR_MSG_WITH_DATA_CUSTOM_CTOR(GunetterPush, ((sead::Vector3f, Center), (f32, Radius)),
                                 ((const sead::Vector3f&, Center), (f32, Radius))) {
    mCenter.set(pCenter);
    mRadius = pRadius;
};

SENSOR_MSG_WITH_DATA_CUSTOM_CTOR(HackBlowJump, ((sead::Vector3f, End), (f32, Height)),
                                 ((const sead::Vector3f&, End), (f32, Height))) {
    mEnd.set(pEnd);
    mHeight = pHeight;
};

SENSOR_MSG_WITH_DATA_CUSTOM_CTOR(SphinxRideAttackTouchThrough,
                                 ((sead::Vector3f, Pos), (sead::Vector3f, Normal)),
                                 ((const sead::Vector3f&, Pos), (const sead::Vector3f&, Normal))) {
    mPos.set(pPos);
    mNormal.set(pNormal);
}

SENSOR_MSG_WITH_DATA_CUSTOM_CTOR(SphinxRideAttackTouch,
                                 ((sead::Vector3f, Pos), (sead::Vector3f, Normal)),
                                 ((const sead::Vector3f&, Pos), (const sead::Vector3f&, Normal))) {
    mPos.set(pPos);
    mNormal.set(pNormal);
}

SENSOR_MSG_WITH_DATA_CUSTOM_CTOR(TestPunch, ((sead::Vector3f, Dir), (s32, HitId), (s32, TeamId)),
                                 ((const sead::Vector3f&, Dir), (s32, HitId), (s32, TeamId))) {
    mDir.set(pDir);
    mHitId = pHitId;
    mTeamId = pTeamId;
};

SENSOR_MSG_WITH_DATA_CUSTOM_CTOR(TestPunchStrong,
                                 ((sead::Vector3f, Dir), (s32, HitId), (s32, TeamId)),
                                 ((const sead::Vector3f&, Dir), (s32, HitId), (s32, TeamId))) {
    mDir.set(pDir);
    mHitId = pHitId;
    mTeamId = pTeamId;
};

SENSOR_MSG_WITH_DATA_CUSTOM_CTOR(TsukkunThrust,
                                 ((sead::Vector3f, Dir), (s32, HitId), (bool, IsNonEnemy)),
                                 ((const sead::Vector3f&, Dir), (s32, HitId), (bool, IsNonEnemy))) {
    mDir.set(pDir);
    mHitId = pHitId;
    mIsNonEnemy = pIsNonEnemy;
}

SENSOR_MSG_WITH_DATA_CUSTOM_CTOR(TsukkunThrustSpin,
                                 ((sead::Vector3f, Dir), (s32, HitId), (bool, IsNonEnemy)),
                                 ((const sead::Vector3f&, Dir), (s32, HitId), (bool, IsNonEnemy))) {
    mDir.set(pDir);
    mHitId = pHitId;
    mIsNonEnemy = pIsNonEnemy;
}

SENSOR_MSG_WITH_DATA_CUSTOM_CTOR(TsukkunThrustReflect,
                                 ((sead::Vector3f, Dir), (s32, HitId), (bool, IsNonEnemy)),
                                 ((const sead::Vector3f&, Dir), (s32, HitId), (bool, IsNonEnemy))) {
    mDir.set(pDir);
    mHitId = pHitId;
    mIsNonEnemy = pIsNonEnemy;
}

SENSOR_MSG_WITH_DATA_CUSTOM_CTOR(TsukkunThrustCollide,
                                 ((sead::Vector3f, Dir), (s32, HitId), (bool, IsNonEnemy)),
                                 ((const sead::Vector3f&, Dir), (s32, HitId), (bool, IsNonEnemy))) {
    mDir.set(pDir);
    mHitId = pHitId;
    mIsNonEnemy = pIsNonEnemy;
}

SENSOR_MSG_WITH_DATA_CUSTOM_CTOR(TsukkunThrustHitReflectCollide,
                                 ((sead::Vector3f, Dir), (s32, HitId), (bool, IsNonEnemy)),
                                 ((const sead::Vector3f&, Dir), (s32, HitId), (bool, IsNonEnemy))) {
    mDir.set(pDir);
    mHitId = pHitId;
    mIsNonEnemy = pIsNonEnemy;
}

SENSOR_MSG_WITH_DATA_CUSTOM_CTOR(TsukkunThrustReflectCollide,
                                 ((sead::Vector3f, Dir), (s32, HitId), (bool, IsNonEnemy)),
                                 ((const sead::Vector3f&, Dir), (s32, HitId), (bool, IsNonEnemy))) {
    mDir.set(pDir);
    mHitId = pHitId;
    mIsNonEnemy = pIsNonEnemy;
}

SENSOR_MSG_WITH_DATA_CUSTOM_CTOR(TsukkunThrustHole,
                                 ((sead::Vector3f, TsukkunPos), (sead::Vector3f, BeakPos)),
                                 ((const sead::Vector3f&, TsukkunPos),
                                  (const sead::Vector3f&, BeakPos))) {
    mTsukkunPos.set(pTsukkunPos);
    mBeakPos.set(pBeakPos);
}

SENSOR_MSG_WITH_DATA_CUSTOM_CTOR(SpherePush, ((sead::Vector3f, Center), (f32, Radius)),
                                 ((const sead::Vector3f&, Center), (f32, Radius))) {
    mCenter.set(pCenter);
    mRadius = pRadius;
}

SENSOR_MSG_WITH_DATA_CUSTOM_CTOR(FireBlowerAttack,
                                 ((sead::Vector3f, Pos), (sead::Vector3f, Dir), (f32, Radius)),
                                 ((const sead::Vector3f&, Pos), (const sead::Vector3f&, Dir),
                                  (f32, Radius))) {
    mPos.set(pPos);
    mDir.set(pDir);
    mRadius = pRadius;
}

// NOTE: This class needs to be manually declared because getObjId needs to either return a
// const sead::FixedSafeString<32>& or call cstr() and the ctor needs to use an initializer list
class SensorMsgGhostRecordStartOk : public al::SensorMsg {
    SEAD_RTTI_OVERRIDE(SensorMsgGhostRecordStartOk, al::SensorMsg);

public:
    inline SensorMsgGhostRecordStartOk(const char* pObjId) : mObjId(pObjId) {}

    const char* getObjId() const { return mObjId.cstr(); }

private:
    sead::FixedSafeString<32> mObjId;
};

}  // namespace rs
