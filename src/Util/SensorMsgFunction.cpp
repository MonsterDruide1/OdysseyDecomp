#include "Util/SensorMsgFunction.h"

#include <math/seadMathCalcCommon.h>

#include "Library/Event/EventFlowExecutor.h"
#include "Library/HitSensor/SensorMsgSetupUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorFunction.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/LiveActor/ComboCounter.h"
#include "Library/Math/MathUtil.h"
#include "Library/Screen/ScreenPointer.h"
#include "Library/Screen/ScreenPointerUtil.h"
#include "Project/HitSensor/HitSensor.h"

#include "Enemy/GotogotonMark.h"
#include "MapObj/TouchTargetInfo.h"
#include "Player/WhipTargetInfo.h"
#include "Util/PlayerUtil.h"
#include "Util/ScenePlayerCapFunction.h"

namespace rs {

SEND_MSG_IMPL(AckCheckpoint);
SEND_MSG_IMPL(AckGetShine);
SEND_MSG_IMPL(AckLifeUp);
SEND_MSG_IMPL(AckLifeMaxUp);
SEND_MSG_IMPL(AskRailCollision);
SEND_MSG_IMPL(BreakPartsBreak);
SEND_MSG_IMPL(BirdFlyAway);
SEND_MSG_IMPL(CameraAngleElevationResetFast);
SEND_MSG_IMPL(ChorobonAttack);
SEND_MSG_IMPL(ConductLightning);
SEND_MSG_IMPL(DamageBallAttack);
SEND_MSG_IMPL(DamageBallBodyAttack);
SEND_MSG_IMPL(DonsukeAttack);
SEND_MSG_IMPL(DonsukeGroundAttack);
SEND_MSG_IMPL(DonsukePush);
SEND_MSG_IMPL(DragonAttack);
SEND_MSG_IMPL(EatExplosion);
SEND_MSG_IMPL(ElectricWireNoLimitDistance);
SEND_MSG_IMPL(EnemyAttack2D);
SEND_MSG_IMPL(EnemyAttack3D);
SEND_MSG_IMPL(EnemyAttackDash);
SEND_MSG_IMPL(HackAttackFire);
SEND_MSG_IMPL(EnemyAttackFireCollision);
SEND_MSG_IMPL(EnemyAttackPoison);
SEND_MSG_IMPL(EnemyAttackTRex);
SEND_MSG_IMPL(HackAttackPoison);
SEND_MSG_IMPL(ConfirmPaintObj);
SEND_MSG_IMPL(ConfirmPaintObjForSeed);
SEND_MSG_IMPL(PaintAttackPoison);
SEND_MSG_IMPL(EnemyKick);
SEND_MSG_IMPL(RabbitKick);
SEND_MSG_IMPL(FishingAttack);
SEND_MSG_IMPL(FishingCancel);
SEND_MSG_IMPL(FishingFishApproach);
SEND_MSG_IMPL(FishingFishFloatTouch);
SEND_MSG_IMPL(FishingItemGet);
SEND_MSG_IMPL(FishingLineTouch);
SEND_MSG_IMPL(FishingStart);
SEND_MSG_IMPL(FishingUpImmediatelyPrepare);
SEND_MSG_IMPL(FireBrosFireBallCollide);
SEND_MSG_IMPL(FireSwitchFire);
SEND_MSG_IMPL(FrogHackTrample);
SEND_MSG_IMPL(GhostRecordStart);
SEND_MSG_IMPL(GhostRecordEnd);
SEND_MSG_IMPL(GhostPlay);
SEND_MSG_IMPL(GhostStop);
SEND_MSG_IMPL(GiantWanderBossAttack);
SEND_MSG_IMPL(GiantWanderBossBulletAttack);
SEND_MSG_IMPL(GiantWanderBossBulletPush);
SEND_MSG_IMPL(GhostReverse);
SEND_MSG_IMPL(GhostCancel);
SEND_MSG_IMPL(GrowFlowerSeedDisablePush);
SEND_MSG_IMPL(GrowFlowerSeedNear);
SEND_MSG_IMPL(GrowPlantPush);
SEND_MSG_IMPL(GrowerAttack);
SEND_MSG_IMPL(GrowerWallAttack);
SEND_MSG_IMPL(GunetterAttack);
SEND_MSG_IMPL(GunetterBodyTouch);
SEND_MSG_IMPL(HammerAttackDown);
SEND_MSG_IMPL(HammerAttackSide);
SEND_MSG_IMPL(HammerAttackSideCollide);
SEND_MSG_IMPL(ItemAmiiboKoopa);
SEND_MSG_IMPL(IsExistPukupuku);
SEND_MSG_IMPL(JangoAttack);
SEND_MSG_IMPL(JangoRemoveCap);
SEND_MSG_IMPL(KakkuKick);
SEND_MSG_IMPL(KillByBossBattleDemo);
SEND_MSG_IMPL(KillByHackFirstDemo);
SEND_MSG_IMPL(KillByHomeDemo);
SEND_MSG_IMPL(EndHomeDemo);
SEND_MSG_IMPL(KillByMoonRockDemo);
SEND_MSG_IMPL(KillByShineGet);
SEND_MSG_TO_ACTOR_IMPL(KillBySwitchTimer);
SEND_MSG_IMPL(KoopaBindStart);
SEND_MSG_IMPL(KoopaCapPlayerFocusTarget);
SEND_MSG_IMPL(KoopaCapPunchFinishL);
SEND_MSG_IMPL(KoopaCapPunchFinishR);
SEND_MSG_IMPL(KoopaCapPunchInvincibleL);
SEND_MSG_IMPL(KoopaCapPunchInvincibleR);
SEND_MSG_IMPL(KoopaCapPunchKnockBackL);
SEND_MSG_IMPL(KoopaCapPunchKnockBackR);
SEND_MSG_IMPL(KoopaCapPunchL);
SEND_MSG_IMPL(KoopaCapPunchR);
SEND_MSG_IMPL(KoopaCapSpinAttack);
SEND_MSG_IMPL(KoopaCatchKoopaCap);
SEND_MSG_IMPL(KoopaDashPunchAttack);
SEND_MSG_IMPL(KoopaFire2D);
SEND_MSG_IMPL(KoopaFireBallAttack);
SEND_MSG_IMPL(KoopaHackDamage);
SEND_MSG_IMPL(KoopaHackPunch);
SEND_MSG_IMPL(KoopaHackPunchCollide);
SEND_MSG_IMPL(KoopaHackTrample);
SEND_MSG_IMPL(KoopaInvalidHackPunchFaceToCollision);
SEND_MSG_IMPL(KoopaRingBeamInvalidTouch);
SEND_MSG_IMPL(KoopaTailAttack);
SEND_MSG_IMPL(KoopaTouchFloor);
SEND_MSG_IMPL(KouraAttack2D);
SEND_MSG_IMPL(KouraItemGet2D);
SEND_MSG_IMPL(LaunchBlow);
SEND_MSG_IMPL(MeganeAttack);
SEND_MSG_IMPL(MeganeHackTrample);
SEND_MSG_IMPL(MofumofuBulletUnexplosion);
SEND_MSG_IMPL(MoonBasementAttackMeteor);
SEND_MSG_IMPL(MoonBasementBreakShockwaveMeteor);
SEND_MSG_IMPL(MoonBasementRockSyncClippingRegist);
SEND_MSG_IMPL(MoonBasementRockSyncClippingInvalidate);
SEND_MSG_IMPL(MoonBasementRockSyncClippingValidate);
SEND_MSG_IMPL(AttachCactusNeedle);
SEND_MSG_COMBO_IMPL(CactusNeedleAttack);
SEND_MSG_COMBO_IMPL(CactusNeedleAttackStrong);
SEND_MSG_IMPL(PlayerBallToss);
SEND_MSG_IMPL(PlayerCarryCameraSubjectiveStart);
SEND_MSG_IMPL(PlayerCarryCameraSubjectiveEnd);
SEND_MSG_IMPL(PlayerCarryShineGetStart);
SEND_MSG_IMPL(PlayerCarryShineGetEnd);
SEND_MSG_IMPL(PlayerCapCatch);
SEND_MSG_IMPL(PlayerCapHipDrop);
SEND_MSG_IMPL(PlayerCapPush);
SEND_MSG_IMPL(PlayerCapRecovery);
SEND_MSG_IMPL(PlayerCapTouchJump);
SEND_MSG_IMPL(PlayerCapTrample);
SEND_MSG_IMPL(PlayerCoinDashGet);
SEND_MSG_IMPL(PlayerEyePriorityTarget);
SEND_MSG_IMPL(PlayerDisregardHomingAttack);
SEND_MSG_IMPL(PlayerDisregardTargetMarker);
SEND_MSG_IMPL(PlayerEquipKoopaCap);
SEND_MSG_IMPL(PlayerFireBallAttack2D);
SEND_MSG_IMPL(PlayerFireBallAttack3D);
SEND_MSG_IMPL(PlayerHipDropDemoTrigger);
SEND_MSG_IMPL(PlayerHipDropHipDropSwitch);
SEND_MSG_IMPL(PlayerItemGet2D);
SEND_MSG_IMPL(PlayerJumpTakeOffFloor);
SEND_MSG_IMPL(PlayerObjectWallHit);
SEND_MSG_IMPL(PlayerObjLeapFrog);
SEND_MSG_IMPL(PlayerPenguinAttack);
SEND_MSG_IMPL(PlayerPenguinAttackReflect);
SEND_MSG_IMPL(PlayerPoleClimbKeep);
SEND_MSG_IMPL(PlayerPoleClimbReaction);
SEND_MSG_IMPL(PlayerRabbitGet);
SEND_MSG_IMPL(PlayerRollingObjHit);
SEND_MSG_IMPL(PlayerRollingWallHitDown);
SEND_MSG_IMPL(PlayerRollingWallHitMove);
SEND_MSG_IMPL(PlayerStartGrabCeil);
SEND_MSG_IMPL(PlayerStartWallJump);
SEND_MSG_IMPL(PlayerEndGrabCeil);
SEND_MSG_IMPL(PlayerSwordAttack);
SEND_MSG_IMPL(PlayerTouchFloorJumpCode);
SEND_MSG_IMPL(PlayerTrample2D);
SEND_MSG_IMPL(PlayerUpperPunch2D);
SEND_MSG_IMPL(PlayerObjUpperPunch2D);
SEND_MSG_IMPL(PukupukuDash);
SEND_MSG_IMPL(PukupukuKiss);
SEND_MSG_IMPL(PukupukuRollingAttack);
SEND_MSG_IMPL(Push2D);
SEND_MSG_IMPL(PushToFish);
SEND_MSG_IMPL(PushToMotorcycle);
SEND_MSG_IMPL(PushToPlayer);
SEND_MSG_IMPL(RadishAttack);
SEND_MSG_IMPL(RadishReflect);
SEND_MSG_IMPL(RequestChangeKinokoSuper);
SEND_MSG_IMPL(RequestChangeFireFlower);
SEND_MSG_IMPL(StartHack);
SEND_MSG_IMPL(SunshineAttack);
SEND_MSG_IMPL(CancelHack);
SEND_MSG_IMPL(CancelHackArea);
SEND_MSG_IMPL(CancelHackByDokan);
SEND_MSG_IMPL(PackunEatStart);
SEND_MSG_IMPL(HackAttack);
SEND_MSG_IMPL(HackAttackKick);
SEND_MSG_IMPL(HackAttackMapObj);
SEND_MSG_IMPL(HackDeathAreaSelfCheck);
SEND_MSG_IMPL(HackDemoEnd);
SEND_MSG_IMPL(HackDemoStart);
SEND_MSG_IMPL(HackInvalidEscape);
SEND_MSG_IMPL(HackInvalidEscapeNoReaction);
SEND_MSG_IMPL(HackMarioCheckpointFlagWarp);
SEND_MSG_IMPL(HackMarioDead);
SEND_MSG_IMPL(HackMarioDemo);
SEND_MSG_IMPL(HackMarioInWater);
SEND_MSG_IMPL(HackMoveRockForestPush);
SEND_MSG_IMPL(HackSelfCeilingCheckMiss);
SEND_MSG_IMPL(HackSyncDamageVisibility);
SEND_MSG_IMPL(WeaponItemGet);
SEND_MSG_IMPL(CapAttack);
SEND_MSG_IMPL(CapAttackCollide);
SEND_MSG_IMPL(CapAttackStayRolling);
SEND_MSG_IMPL(CapAttackStayRollingCollide);
SEND_MSG_IMPL(CapAttackRailMove);
SEND_MSG_IMPL(CapGiantAttack);
SEND_MSG_IMPL(CapReflect);
SEND_MSG_IMPL(CapReflectCollide);
SEND_MSG_IMPL(CapStartLockOn);
SEND_MSG_IMPL(CapKeepLockOn);
SEND_MSG_IMPL(CapCancelLockOn);
SEND_MSG_IMPL(CapHipDrop);
SEND_MSG_IMPL(CapObjHipDrop);
SEND_MSG_IMPL(CapObjHipDropReflect);
SEND_MSG_IMPL(CapIgnoreCancelLockOn);
SEND_MSG_IMPL(CapIgnoreCancelMissReaction);
SEND_MSG_IMPL(CapIgnoreCollisionCheck);
SEND_MSG_IMPL(CapItemGet);
SEND_MSG_IMPL(CapTrampolineAttack);
SEND_MSG_IMPL(CatchBombThrough);
SEND_MSG_IMPL(CheckCarObstacle);
SEND_MSG_IMPL_(CheckIsCardboardBox, IsCardboardBox);
SEND_MSG_IMPL(BullHackAttack);
SEND_MSG_IMPL(BullEnemyAttack);
SEND_MSG_IMPL(EnemyAttackStrong);
SEND_MSG_IMPL(EnemyObjBreak);
SEND_MSG_IMPL(WhipAttack);
SEND_MSG_DATA_IMPL(WhipBind, (WhipTargetInfo*, Info));
SEND_MSG_DATA_IMPL(WhipHold, (WhipTargetInfo*, Info));
SEND_MSG_DATA_IMPL(WhipThrow, (const sead::Vector3f&, Dir));
SEND_MSG_DATA_IMPL(BossMagmaBreathForce, (const sead::Vector3f&, Force));
SEND_MSG_DATA_IMPL(BossMagmaDeadDemoEnd, (const sead::Vector3f&, TargetPos));
SEND_MSG_DATA_IMPL(BossMagmaResetPos, (const sead::Vector3f&, ResetPos));
SEND_MSG_DATA_IMPL(HackBlowJump, (const sead::Vector3f&, End), (f32, Height));
SEND_MSG_DATA_IMPL(SphinxRideAttackTouchThrough, (const sead::Vector3f&, Pos),
                   (const sead::Vector3f&, Normal));
SEND_MSG_DATA_IMPL(SphinxRideAttackTouch, (const sead::Vector3f&, Pos),
                   (const sead::Vector3f&, Normal));
// TODO: Rename parameters
SEND_MSG_DATA_IMPL(SandGeyserRaise, (f32, Unk), (f32, Unk2));
SEND_MSG_DATA_IMPL(CheckFishingTarget, (const FishingFish*, Fish));
SEND_MSG_DATA_IMPL(GolemStampPushV, (f32, Velocity));
SEND_MSG_DATA_IMPL(GolemStampPushH, (f32, Velocity));
SEND_MSG_DATA_IMPL(GotogotonGoalMatch, (const GotogotonMark*, Mark));
SEND_MSG_DATA_IMPL(GotogotonGetJumpPath, (al::ParabolicPath*, Path));
SEND_MSG_DATA_IMPL(PlayerLookAtPosition, (sead::Vector3f*, Pos));
SEND_MSG_DATA_IMPL(TargetMarkerPosition, (sead::Vector3f*, Pos));
SEND_MSG_IMPL(NoLimitTouchJump);
SEND_MSG_IMPL(NoticePlayerDamage);
SEND_MSG_IMPL(StatueDrop);
SEND_MSG_IMPL(StatueTrampleReflect);
SEND_MSG_IMPL(StatuePush);
SEND_MSG_IMPL(StatueSnap);
SEND_MSG_IMPL(HitGrowFlowerPot);
SEND_MSG_IMPL(HitGrowPlantPot);
SEND_MSG_IMPL(ImplantGrowFlowerSeed);
SEND_MSG_IMPL(IcicleAttack);
SEND_MSG_IMPL(IgnoreMirrorWarp);
SEND_MSG_IMPL_(IgnoredByRunawayNpc, IsIgnoredByRunawayNpc);
SEND_MSG_IMPL(IgnorePushMotorcycle);
SEND_MSG_IMPL(SandSharkAttack);
SEND_MSG_IMPL(SeedAttack);
SEND_MSG_IMPL(SeedAttackBig);
SEND_MSG_IMPL(SeedAttackHold);
SEND_MSG_IMPL(SeedItemGet);
SEND_MSG_IMPL(SeedReflect);
SEND_MSG_IMPL(SenobiTrample);
SEND_MSG_IMPL(SenobiCancelStretch);
SEND_MSG_IMPL(SenobiPunchBlockTransparent);
SEND_MSG_IMPL(ShibakenApproach);
SEND_MSG_IMPL(ShibakenKick);
SEND_MSG_IMPL(HackUpperPunch);
SEND_MSG_IMPL(HackObjUpperPunch);
SEND_MSG_IMPL(ShineGet);
SEND_MSG_IMPL(ShineGet2D);
SEND_MSG_IMPL(SphinxJumpAttack);
// TODO: rename parameters
SEND_MSG_DATA_IMPL(NetworkShootingShot, (s32, Unk));
// TODO: rename parameters
SEND_MSG_DATA_IMPL(NetworkShootingChargeShot, (s32, Unk));
SEND_MSG_IMPL(RequestPlayerJumpBreakFloor);
SEND_MSG_DATA_IMPL(RequestPlayerJump, (f32, Power));
SEND_MSG_DATA_IMPL(RequestPlayerTrampleJump, (f32, Power));
SEND_MSG_DATA_IMPL(RequestPlayerSpinJump, (f32, Power));
SEND_MSG_DATA_IMPL(RequestSphinxJump, (f32, Power));
SEND_MSG_DATA_IMPL(InitCapTarget, (const CapTargetInfo**, Info));
SEND_MSG_DATA_IMPL(EndHack, (const HackEndParam*, Param));
SEND_MSG_IMPL(SphinxQuizRouteKill);
SEND_MSG_IMPL(SphinxRideAttack);
SEND_MSG_IMPL(SphinxRideAttackReflect);
SEND_MSG_IMPL(PechoSpot);
SEND_MSG_IMPL(BelowObjBroken);
SEND_MSG_IMPL(BindCollidedGround);
SEND_MSG_IMPL(BindKeepDemoStart);
SEND_MSG_IMPL(BindKeepDemoExecute);
SEND_MSG_IMPL(BindKeepDemoEnd);
SEND_MSG_IMPL(BindRecoveryLife);
SEND_MSG_IMPL(MayorItemReflect);
SEND_MSG_IMPL(MayorItemCollide);
SEND_MSG_IMPL(BlowObjAttack);
SEND_MSG_IMPL(BlowObjAttackReflect);
SEND_MSG_IMPL(ConfirmFrailBox);
SEND_MSG_IMPL(ConfirmBrokenFrailBox);
SEND_MSG_IMPL(TankLookOn);
SEND_MSG_IMPL(TankKickHack);
SEND_MSG_IMPL(TankKickEnemy);
SEND_MSG_IMPL(TankBullet);
SEND_MSG_IMPL(TankBulletNoReaction);
SEND_MSG_TO_ACTOR_IMPL(TimerAthleticDemoStart);
SEND_MSG_IMPL(RideOnStart);
SEND_MSG_IMPL(RideOnEnd);
SEND_MSG_IMPL(RideOnRelease);
SEND_MSG_IMPL(HipDropTransformTransform);
SEND_MSG_IMPL(HipDropTransformReverse);
SEND_MSG_IMPL(HipDropTransformingUp);
SEND_MSG_IMPL(HipDropTransformingDown);
SEND_MSG_IMPL(HipDropTransformingFinish);
SEND_MSG_IMPL(ClearFire);
SEND_MSG_IMPL(CollectAnimalTouchCollide);
SEND_MSG_IMPL(BossKnuckleCounter);
SEND_MSG_IMPL(BossKnuckleFallAttack);
SEND_MSG_IMPL(BossKnuckleHackAttack);
SEND_MSG_IMPL(BossKnuckleKillerAttack);
SEND_MSG_IMPL(BossKnuckleIceConflict);
SEND_MSG_IMPL(BossKnuckleIceFallToMummy);
SEND_MSG_IMPL(SkaterAttack);
SEND_MSG_DATA_IMPL(SpherePush, (const sead::Vector3f&, Center), (f32, Radius));
SEND_MSG_IMPL(EnableInSaucePan);
SEND_MSG_IMPL(EnableMapCheckPointWarp);
SEND_MSG_DATA_IMPL(StartInSaucePan, (bool, IsSpawnShine));
SEND_MSG_IMPL(EndInSaucePan);
SEND_MSG_IMPL(LineDancerLink);
SEND_MSG_IMPL(LongPushSensorHit);
SEND_MSG_IMPL(LongPushBoxHit);
SEND_MSG_IMPL(GoldHammerAttack);
SEND_MSG_IMPL(TrashBoxIn);
SEND_MSG_IMPL(TouchDoorDrumn);
SEND_MSG_IMPL(Kuribo2DTouch);
SEND_MSG_IMPL(KuriboCollisionDamage);
SEND_MSG_IMPL(KuriboCollisionKill);
SEND_MSG_IMPL(KuriboFlick);
SEND_MSG_DATA_IMPL(KuriboTowerOn, (u32, Num));
SEND_MSG_IMPL(PartyPopperSoundAttack);
SEND_MSG_IMPL(YokinBallAttack);
SEND_MSG_IMPL(YoshiDirectEat);
SEND_MSG_IMPL(YoshiTongueAttack);
SEND_MSG_DATA_IMPL(YoshiTongueEatBind, (f32*, RadiusPtr), (f32*, OffsetPtr), (f32*, ScalePtr));
SEND_MSG_IMPL(YoshiTongueEatBindCancel);
SEND_MSG_IMPL(YoshiTongueEatBindFinish);
SEND_MSG_IMPL(YoshiTongueEatHomingTarget);
SEND_MSG_IMPL(YukimaruPush);
SEND_MSG_IMPL(PunchMachinePunchStraight);
SEND_MSG_IMPL(PunchMachinePunchHook);
SEND_MSG_IMPL(PunchMachinePunchUpper);
SEND_MSG_IMPL(MorningStarWarpStart);
SEND_MSG_IMPL(MorningStarWarpEnd);
SEND_MSG_IMPL(GemyAim);
SEND_MSG_IMPL(HammerBrosHammerEnemyAttack);
SEND_MSG_IMPL(HammerBrosHammerHackAttack);
SEND_MSG_IMPL(HammerBrosHammerSearch);
SEND_MSG_IMPL(HackBrosContact);
SEND_MSG_IMPL(MotorcycleAttack);
SEND_MSG_IMPL(MotorcycleCollideParkingLot);
SEND_MSG_IMPL(MotorcycleDashAttack);
SEND_MSG_IMPL(MotorcycleDashCollide);
SEND_MSG_IMPL(WanwanEnemyAttack);
SEND_MSG_IMPL(WanwanPush);
SEND_MSG_IMPL(WanwanReboundAttack);
SEND_MSG_IMPL(WanwanBlockAttack);
SEND_MSG_IMPL(WanwanHoleIn);
SEND_MSG_IMPL(WaterRoadIn);
SEND_MSG_IMPL(WaterRoadNear);
SEND_MSG_IMPL(BreedaSlap);
SEND_MSG_IMPL(BreedaPush);
SEND_MSG_IMPL(GamaneBullet);
SEND_MSG_IMPL(GamaneBulletThrough);
SEND_MSG_IMPL(GamaneBulletForCoinFlower);
SEND_MSG_IMPL(VolleyballNpcScareByEnemy);
SEND_MSG_IMPL(RocketFlowerExtension);
SEND_MSG_IMPL(WanderBossCameraRange);
SEND_MSG_IMPL(KuriboGirlAttack);
SEND_MSG_IMPL(KuriboGirlLove);
SEND_MSG_IMPL(KuriboTop);
SEND_MSG_IMPL(TRexAttack);
SEND_MSG_IMPL(TRexAttackCollideBody);
SEND_MSG_IMPL(TRexAttackCollideHead);
SEND_MSG_IMPL(TRexDashAttack);
SEND_MSG_IMPL(TRexScrollPartsBreakWith);
SEND_MSG_IMPL(TsukkunForceCancelCollide);
SEND_MSG_IMPL(TsukkunHoldCollide);
SEND_MSG_IMPL(TsukkunThroughCollide);
SEND_MSG_IMPL(TsukkunNoTrace);
SEND_MSG_IMPL(TouchFireDrum2D);
SEND_MSG_IMPL(PropellerAttack);
SEND_MSG_IMPL(TankExplosion);
SEND_MSG_IMPL(TankHackTrample);
SEND_MSG_IMPL(UtsuboAttack);
SEND_MSG_IMPL(KillerAttackNoExplode);
SEND_MSG_IMPL(KillerMagnumAttack);
SEND_MSG_IMPL(KillerMagnumHackAttack);
SEND_MSG_IMPL(GabuzouAttack);
SEND_MSG_IMPL(StackerRollingAttack);
SEND_MSG_IMPL(StackerCapBoostAttack);
SEND_MSG_IMPL(IgnoreIgnitionBomb);
SEND_MSG_IMPL(ExplosionReflectBomb);
SEND_MSG_IMPL(GolemStampPress);
SEND_MSG_IMPL(CapBeamerBeam);
SEND_MSG_IMPL(HosuiAttack);
SEND_MSG_IMPL(HosuiAttackCollide);
SEND_MSG_IMPL(HosuiAttackNoEffect);
SEND_MSG_IMPL(HosuiAttackStrong);
SEND_MSG_IMPL(HosuiTouch);
SEND_MSG_IMPL(HosuiTrample);
SEND_MSG_IMPL(HosuiTrampleReflect);
SEND_MSG_IMPL(HosuiTrampleReflectHigh);
SEND_MSG_IMPL(BubbleAttack);
SEND_MSG_IMPL(BubbleAttackToPecho);
SEND_MSG_IMPL(BubbleReflectH);
SEND_MSG_IMPL(BubbleReflectV);
SEND_MSG_IMPL(BubbleWallTouch);
SEND_MSG_IMPL(BubbleGroundTouchTrigger);
SEND_MSG_IMPL(BubbleLauncherStart);
SEND_MSG_IMPL(BlockUpperPunch2D);
SEND_MSG_IMPL(GotogotonOn);
SEND_MSG_IMPL(GotogotonGoalExist);
SEND_MSG_IMPL(BossMagmaCatchPlayer);
SEND_MSG_IMPL(BossMagmaReleasePlayer);
SEND_MSG_IMPL(BossMagmaDeadDemoStart);
SEND_MSG_IMPL(BossMagmaQueryToBubble);
SEND_MSG_IMPL(TransferHack);
SEND_MSG_IMPL(RequestTransferHack);
SEND_MSG_IMPL(InitHack);
SEND_MSG_DATA_TO_ACTOR_IMPL(SwitchOnWithSaveRequest, SaveObjInfo*, Info);

// NOTE: This function is identical to al::sendMsgPushAndKillVelocityToTarget but with a different
// message
bool sendMsgPushToPlayerAndKillVelocityToTarget(al::LiveActor* actor, al::HitSensor* receiver,
                                                al::HitSensor* sender) {
    // BUG: Param ordering is wrong here
    if (!sendMsgPushToPlayer(sender, receiver))
        return false;
    sead::Vector3f diff = al::getSensorPos(sender) - al::getSensorPos(receiver);
    if (!al::tryNormalizeOrZero(&diff))
        diff.set(sead::Vector3f::ez);
    if (al::getVelocity(actor).dot(diff) > 0.0f)
        al::verticalizeVec(getVelocityPtr(actor), diff, getVelocity(actor));
    return true;
}

// NOTE: This function is identical to al::sendMsgPushAndKillVelocityToTargetH but with a different
// message
bool sendMsgPushToPlayerAndKillVelocityToTargetH(al::LiveActor* actor, al::HitSensor* receiver,
                                                 al::HitSensor* sender) {
    // BUG: Param ordering is wrong here
    if (!sendMsgPushToPlayer(sender, receiver))
        return false;
    sead::Vector3f diff = al::getSensorPos(sender) - al::getSensorPos(receiver);
    diff.y = 0;
    if (!al::tryNormalizeOrZero(&diff))
        diff.set(sead::Vector3f::ez);
    if (al::getVelocity(actor).dot(diff) > 0.0f)
        al::verticalizeVec(getVelocityPtr(actor), diff, getVelocity(actor));
    return true;
}

void sendMsgEventFlowScareCheck(al::HitSensor* receiver, al::HitSensor* sender,
                                al::EventFlowExecutor* executor) {
    executor->attackSensor(sender, receiver);
}

bool sendMsgRaceStart(al::LiveActor* receiver) {
    SensorMsgRaceStart msg;
    return receiver->receiveMsg(&msg, nullptr, nullptr);
}

bool sendMsgRaceStop(al::LiveActor* receiver) {
    SensorMsgRaceStop msg;
    return receiver->receiveMsg(&msg, nullptr, nullptr);
}

bool sendMsgRaceWait(al::LiveActor* receiver) {
    SensorMsgRaceWait msg;
    return receiver->receiveMsg(&msg, nullptr, nullptr);
}

bool sendMsgRaceReturnToCourse(al::LiveActor* receiver, const sead::Vector3f& pos,
                               const sead::Vector3f& front) {
    SensorMsgRaceReturnToCourse msg(pos, front);
    return receiver->receiveMsg(&msg, nullptr, nullptr);
}

bool sendMsgRequestPlayerWet(al::HitSensor* receiver, al::HitSensor* sender) {
    return alActorSensorFunction::sendMsgSensorToSensor(
        SensorMsgRequestPlayerStainWet(PlayerStainType::Wet), sender, receiver);
}

bool sendMsgRequestPlayerSnow(al::HitSensor* receiver, al::HitSensor* sender) {
    return alActorSensorFunction::sendMsgSensorToSensor(
        SensorMsgRequestPlayerStainWet(PlayerStainType::Snow), sender, receiver);
}

bool sendMsgRequestPlayerSandMoon(al::HitSensor* receiver, al::HitSensor* sender) {
    return alActorSensorFunction::sendMsgSensorToSensor(
        SensorMsgRequestPlayerStainWet(PlayerStainType::SandMoon), sender, receiver);
}

bool sendMsgWanwanReboundAttackToCollided(const al::LiveActor* actor, al::HitSensor* receiver) {
    al::HitSensor* collidedSensor = al::tryGetCollidedSensor(actor);
    return collidedSensor && alActorSensorFunction::sendMsgSensorToSensor(
                                 SensorMsgWanwanReboundAttack(), receiver, collidedSensor);
}

bool sendMsgWanwanBlockAttackToCollided(const al::LiveActor* actor, al::HitSensor* receiver) {
    al::HitSensor* collidedSensor = al::tryGetCollidedSensor(actor);
    return collidedSensor && alActorSensorFunction::sendMsgSensorToSensor(
                                 SensorMsgWanwanBlockAttack(), receiver, collidedSensor);
}

bool sendMsgFishingUpImmediately(al::HitSensor* receiver, al::HitSensor* sender,
                                 const sead::Matrix34f& mtx, const sead::Vector3f& trans,
                                 const char* materialCode) {
    // surfacePos = mtx.getBase(3) mismatches
    sead::Vector3f surfacePos;
    mtx.getBase(surfacePos, 3);
    return alActorSensorFunction::sendMsgSensorToSensor(
        SensorMsgFishingUpImmediately(&surfacePos, &trans, materialCode), sender, receiver);
}

bool sendMsgCapRethrow(al::HitSensor* receiver, al::HitSensor* sender, const sead::Vector3f& pos,
                       const sead::Vector3f& front, const sead::Vector3f& up) {
    return alActorSensorFunction::sendMsgSensorToSensor(SensorMsgCapRethrow(pos, front, up, false),
                                                        sender, receiver);
}

bool sendMsgCapRethrowReturnOnly(al::HitSensor* receiver, al::HitSensor* sender,
                                 const sead::Vector3f& pos, const sead::Vector3f& front,
                                 const sead::Vector3f& up) {
    return alActorSensorFunction::sendMsgSensorToSensor(SensorMsgCapRethrow(pos, front, up, true),
                                                        sender, receiver);
}

bool sendMsgRequestPlayerWaitAnimDigPoint(al::HitSensor* receiver, al::HitSensor* sender) {
    return alActorSensorFunction::sendMsgSensorToSensor(
        SensorMsgRequestPlayerWaitAnim("WaitDigPoint"), sender, receiver);
}

s32 tryGetKuriboTowerNum(al::HitSensor* receiver, al::HitSensor* sender) {
    s32 num = 0;
    SensorMsgKuriboTowerNum msg(&num);
    alActorSensorFunction::sendMsgSensorToSensor(msg, sender, receiver);
    return *msg.getNumPtr();
}

bool sendMsgIgnoreTouchTarget(al::ScreenPointer* pointer, al::ScreenPointTarget* sender) {
    return al::sendMsgScreenPointTarget(SensorMsgIgnoreTouchTarget(), pointer, sender);
}

bool sendMsgInitTouchTargetInfo(al::ScreenPointer* pointer, al::ScreenPointTarget* sender,
                                TouchTargetInfo* info, const sead::Vector3f* collisionTouchPos) {
    return al::sendMsgScreenPointTarget(SensorMsgTouchTargetInfo(info, collisionTouchPos), pointer,
                                        sender);
}

bool sendMsgPlayerItemGetAll(al::HitSensor* receiver, al::HitSensor* sender) {
    return al::sendMsgPlayerItemGet(receiver, sender) || sendMsgShineGet(receiver, sender);
}

bool sendMsgPlayerItemGetAll2D(al::HitSensor* receiver, al::HitSensor* sender) {
    return sendMsgPlayerItemGet2D(receiver, sender) || sendMsgShineGet2D(receiver, sender);
}

bool sendMsgTRexAttackAll(al::HitSensor* receiver, al::HitSensor* sender) {
    return sendMsgTRexAttack(receiver, sender) || sendMsgHackAttack(receiver, sender) ||
           sendMsgBreakPartsBreak(receiver, sender);
}

SEND_MSG_DATA_IMPL(DigPointSmell, (DigPoint*, DigPoint));
SEND_MSG_DATA_IMPL(MofumofuBodyChainExplode, (s32, DelayStep));
SEND_MSG_DATA_IMPL(MoonBasementRockThroughCollision, (bool, IsFallOrBreak));
SEND_MSG_DATA_IMPL(FishingWait, (al::HitSensor*, HookSensor));
SEND_MSG_DATA_IMPL(GhostRecordStartOk, (const char*, ObjId));
SEND_MSG_DATA_IMPL(GunetterPush, (const sead::Vector3f&, Center), (f32, Radius));
SEND_MSG_DATA_IMPL(TestPunch, (const sead::Vector3f&, Info), (s32, HitId), (s32, TeamId));
SEND_MSG_DATA_IMPL(TestPunchStrong, (const sead::Vector3f&, Info), (s32, HitId), (s32, TeamId));
SEND_MSG_DATA_IMPL(PunchGuard, (s32, PunchGuard), (s32, TeamId));
SEND_MSG_DATA_IMPL(TsukkunThrust, (const sead::Vector3f&, Dir), (s32, HitId), (bool, IsNonEnemy));
SEND_MSG_DATA_IMPL(TsukkunThrustSpin, (const sead::Vector3f&, Dir), (s32, HitId),
                   (bool, IsNonEnemy));
SEND_MSG_DATA_IMPL(TsukkunThrustReflect, (const sead::Vector3f&, Dir), (s32, HitId),
                   (bool, IsNonEnemy));
SEND_MSG_DATA_IMPL(TsukkunThrustCollide, (const sead::Vector3f&, Dir), (s32, HitId),
                   (bool, IsNonEnemy));
SEND_MSG_DATA_IMPL(TsukkunThrustHitReflectCollide, (const sead::Vector3f&, Dir), (s32, HitId),
                   (bool, IsNonEnemy));
SEND_MSG_DATA_IMPL(TsukkunThrustReflectCollide, (const sead::Vector3f&, Dir), (s32, HitId),
                   (bool, IsNonEnemy));
SEND_MSG_DATA_IMPL(TsukkunThrustHole, (const sead::Vector3f&, TsukkunPos),
                   (const sead::Vector3f&, BeakPos));

SEND_MSG_DATA_IMPL(AirExplosion, (const sead::Vector3f&, Force));
SEND_MSG_DATA_IMPL(ByugoBlow, (const sead::Vector3f&, Force));
// TODO: rename parameters
SEND_MSG_DATA_IMPL(CapChangeGiant, (f32, Unk), (s32, Unk2));
SEND_MSG_DATA_IMPL(CapTouchWall, (const sead::Vector3f&, HitPos), (const sead::Vector3f&, Normal));
// TODO: Rename variables
SEND_MSG_DATA_IMPL(CollisionImpulse, (sead::Vector3f*, VecPtr), (const sead::Vector3f&, ConstVec),
                   (f32, FloatVal), (const sead::Vector3f&, ConstVec2), (f32, FloatVal2));
SEND_MSG_DATA_IMPL(FireBlowerAttack, (const sead::Vector3f&, Pos), (const sead::Vector3f&, Dir),
                   (f32, Radius));
SEND_MSG_IMPL(IgnoreTouchTarget);
SEND_MSG_DATA_IMPL_(InitTouchTargetInfo, TouchTargetInfo, (TouchTargetInfo*, Info),
                    (const sead::Vector3f*, CollisionTouchPos));
SEND_MSG_DATA_IMPL(Magnet, (bool, IsPower));
// TODO: rename parameter
SEND_MSG_DATA_IMPL(MagnetBulletAttack, (f32, Unk));
// TODO: rename parameter
SEND_MSG_DATA_IMPL(DashPanel, (s32, Unk));
// TODO: rename parameters
SEND_MSG_DATA_IMPL(TrampolineCrackJump, (f32, Unk), (f32, Unk2));
// TODO: rename parameter
SEND_MSG_DATA_IMPL(NpcScareByEnemy, (s32, Unk));
SEND_MSG_DATA_IMPL(ObjSnapForce, (const sead::Vector3f&, Force));
SEND_MSG_DATA_IMPL(PackunEatCancel, (const sead::Vector3f&, Pos), (const sead::Vector3f&, Front));
SEND_MSG_DATA_IMPL(PackunEatEnd, (const sead::Vector3f&, Pos), (const sead::Vector3f&, Dir));
SEND_MSG_DATA_IMPL(PackunEatStartFollow, (const sead::Vector3f*, PosPtr));
// NOTE: The size field for these two msgs are taken in as s32s even though they are stored as f32s
SEND_MSG_DATA_IMPL(Paint, (const sead::Color4u8&, Color), (s32, Size), (s32, DrawType));
SEND_MSG_DATA_IMPL(PaintTexture, (s32, Size), (f32, Rotation), (s32, DrawType));
SEND_MSG_DATA_IMPL(HackDirectStageInit, (IUsePlayerHack*, Hack));
SEND_MSG_DATA_IMPL(CheckPaintClear, (const sead::Color4u8&, Color), (const sead::Vector3f&, Pos),
                   (s32, DrawType));
SEND_MSG_DATA_IMPL(CheckPaintAlpha, (const sead::Vector3f&, Pos));
// TODO: rename parameters
SEND_MSG_DATA_IMPL(SeedTouch, (const sead::Vector3f&, unk), (const sead::Vector3f&, unk2));
SEND_MSG_DATA_IMPL(SenobiPartsMove, (const sead::Vector3f&, CollidedNormal), (f32, Distance));
SEND_MSG_DATA_IMPL(StampTo2D, (const sead::Vector3f&, Force));
SEND_MSG_DATA_IMPL(PushVelocity, (const sead::Vector3f&, Velocity));

IS_MSG_IMPL(AckCheckpoint);
IS_MSG_IMPL(AckGetShine);
IS_MSG_IMPL(AckLifeUp);
IS_MSG_IMPL(AckLifeMaxUp);
IS_MSG_IMPL(AskRailCollision);
IS_MSG_IMPL(AttachCactusNeedle);
IS_MSG_IMPL(AirExplosion);
IS_MSG_IMPL(BelowObjBroken);
IS_MSG_IMPL(BindCollidedGround);
IS_MSG_IMPL(BindKeepDemoStart);
IS_MSG_IMPL(BindKeepDemoExecute);
IS_MSG_IMPL(BindKeepDemoEnd);
IS_MSG_IMPL(BindRecoveryLife);
IS_MSG_IMPL(BirdFlyAway);
IS_MSG_IMPL(BlowObjAttack);
IS_MSG_IMPL(MayorItemCollide);
IS_MSG_IMPL(MayorItemReflect);
IS_MSG_IMPL(BlowObjAttackReflect);
IS_MSG_IMPL(BossKnuckleCounter);
IS_MSG_IMPL(BossKnuckleFallAttack);
IS_MSG_IMPL(BossKnuckleHackAttack);
IS_MSG_IMPL(BossKnuckleIceConflict);
IS_MSG_IMPL(BossKnuckleIceFallToMummy);
IS_MSG_IMPL(BossKnuckleKillerAttack);
IS_MSG_IMPL(BreakPartsBreak);
IS_MSG_IMPL(BreedaSlap);
IS_MSG_IMPL(BreedaPush);
IS_MSG_IMPL(BubbleAttack);
IS_MSG_IMPL(BubbleAttackToPecho);
IS_MSG_IMPL(BubbleLauncherStart);
IS_MSG_IMPL(BubbleReflectH);
IS_MSG_IMPL(BubbleReflectV);
IS_MSG_IMPL(BubbleWallTouch);
IS_MSG_IMPL(BubbleGroundTouchTrigger);
IS_MSG_IMPL(BullHackAttack);
IS_MSG_IMPL(BullEnemyAttack);
IS_MSG_MULTIPLE_IMPL(BullAttack, BullHackAttack, BullEnemyAttack);
IS_MSG_IMPL(ByugoBlow);
IS_MSG_IMPL(CameraAngleElevationResetFast);
IS_MSG_IMPL(CancelHack);
IS_MSG_IMPL(CancelHackArea);
IS_MSG_IMPL(CancelHackByDokan);
IS_MSG_IMPL(CapAttack);
IS_MSG_IMPL(CapAttackCollide);
IS_MSG_IMPL(CapAttackRailMove);
IS_MSG_IMPL(CapAttackStayRolling);
IS_MSG_IMPL(CapAttackStayRollingCollide);
IS_MSG_IMPL(CapBeamerBeam);
IS_MSG_IMPL(CapCancelLockOn);
IS_MSG_IMPL(CapChangeGiant);
IS_MSG_IMPL(CapStartLockOn);
IS_MSG_IMPL(CapKeepLockOn);
IS_MSG_MULTIPLE_IMPL(CapEnableLockOn, CapStartLockOn, CapKeepLockOn);
IS_MSG_IMPL(CapGiantAttack);
IS_MSG_IMPL(CapHipDrop);
IS_MSG_IMPL(CapObjHipDrop);
IS_MSG_IMPL(CapObjHipDropReflect);
IS_MSG_IMPL(CapIgnoreCancelLockOn);
IS_MSG_IMPL(CapIgnoreCancelMissReaction);
IS_MSG_IMPL(CapIgnoreCollisionCheck);
IS_MSG_IMPL(CapItemGet);
IS_MSG_IMPL(CapReflect);
IS_MSG_IMPL(CapReflectCollide);
IS_MSG_IMPL(CapRethrow);
IS_MSG_IMPL(CapTouchWall);
IS_MSG_IMPL(CapTrampolineAttack);
IS_MSG_IMPL(CatchBombThrough);
IS_MSG_IMPL(CheckCarObstacle);
IS_MSG_IMPL_(CheckIsCardboardBox, IsCardboardBox);
IS_MSG_IMPL(ChorobonAttack);
IS_MSG_IMPL(ClearFire);
IS_MSG_IMPL(CollectAnimalTouchCollide);
IS_MSG_IMPL(CollisionImpulse);
IS_MSG_IMPL(ConductLightning);
IS_MSG_IMPL(ConfirmFrailBox);
IS_MSG_IMPL(ConfirmBrokenFrailBox);
IS_MSG_IMPL(DamageBallAttack);
IS_MSG_IMPL(DamageBallBodyAttack);
IS_MSG_IMPL(DigPointSmell);
IS_MSG_IMPL(DonsukeAttack);
IS_MSG_IMPL(DonsukeGroundAttack);
IS_MSG_IMPL(DonsukePush);
IS_MSG_IMPL(DragonAttack);
IS_MSG_IMPL(EatExplosion);
IS_MSG_IMPL(ElectricWireNoLimitDistance);
IS_MSG_IMPL(EnableInSaucePan);
IS_MSG_IMPL(EnableMapCheckPointWarp);
IS_MSG_IMPL(EndInSaucePan);
IS_MSG_IMPL(EnemyAttack2D);
IS_MSG_IMPL(EnemyAttack3D);
IS_MSG_IMPL(EnemyAttackDash);
IS_MSG_IMPL(FireBrosFireBallCollide);
IS_MSG_IMPL(HackAttackFire);
IS_MSG_IMPL(FireSwitchFire);
IS_MSG_IMPL(EnemyAttackFireCollision);
IS_MSG_IMPL(EnemyAttackTRex);
IS_MSG_MULTIPLE_IMPL(PoisonDamageAll, EnemyAttackPoison, HackAttackPoison, PaintAttackPoison);
IS_MSG_IMPL(EnemyAttackPoison);
IS_MSG_IMPL(HackAttackPoison);
IS_MSG_IMPL(PaintAttackPoison);
IS_MSG_IMPL(ConfirmPaintObj);
IS_MSG_IMPL(ConfirmPaintObjForSeed);
IS_MSG_IMPL(EnemyAttackStrong);
IS_MSG_IMPL(EnemyKick);
IS_MSG_IMPL(RabbitKick);
IS_MSG_IMPL(EnemyObjBreak);
IS_MSG_IMPL(FireBlowerAttack);
IS_MSG_IMPL(FishingAttack);
IS_MSG_IMPL(FishingCancel);
IS_MSG_IMPL(FishingFishApproach);
IS_MSG_IMPL(FishingFishFloatTouch);
IS_MSG_IMPL(FishingItemGet);
IS_MSG_IMPL(FishingLineTouch);
IS_MSG_IMPL(FishingStart);
IS_MSG_IMPL(FishingUpImmediately);
IS_MSG_IMPL(FishingUpImmediatelyPrepare);
IS_MSG_IMPL(FishingWait);
IS_MSG_IMPL(FrogHackTrample);
IS_MSG_IMPL(GamaneBullet);
IS_MSG_IMPL(GamaneBulletThrough);
IS_MSG_IMPL(GamaneBulletForCoinFlower);
IS_MSG_IMPL(GemyAim);
IS_MSG_IMPL(GhostCancel);
IS_MSG_IMPL(GhostPlay);
IS_MSG_IMPL(GhostRecordEnd);
IS_MSG_IMPL(GhostRecordStart);
IS_MSG_IMPL(GhostRecordStartOk);
IS_MSG_IMPL(GhostReverse);
IS_MSG_IMPL(GhostStop);
IS_MSG_IMPL(GiantWanderBossAttack);
IS_MSG_IMPL(GiantWanderBossBulletAttack);
IS_MSG_IMPL(GiantWanderBossBulletPush);
IS_MSG_IMPL(GoldHammerAttack);
IS_MSG_IMPL(GrowFlowerSeedDisablePush);
IS_MSG_IMPL(GrowFlowerSeedNear);
IS_MSG_IMPL(GrowPlantPush);
IS_MSG_IMPL(GrowerAttack);
IS_MSG_IMPL(GrowerWallAttack);
IS_MSG_IMPL(GunetterAttack);
IS_MSG_IMPL(GunetterBodyTouch);
IS_MSG_IMPL(GunetterPush);
IS_MSG_IMPL(HackAttack);
IS_MSG_IMPL(HackAttackKick);
IS_MSG_IMPL(HackAttackMapObj);
IS_MSG_IMPL(HackBrosContact);
IS_MSG_IMPL(HackDeathAreaSelfCheck);
IS_MSG_IMPL(HackDemoEnd);
IS_MSG_IMPL(HackDemoStart);
IS_MSG_IMPL(HackInvalidEscape);
IS_MSG_IMPL(HackInvalidEscapeNoReaction);
IS_MSG_IMPL(HackMarioCheckpointFlagWarp);
IS_MSG_IMPL(HackMarioDead);
IS_MSG_IMPL(HackMarioDemo);
IS_MSG_IMPL(HackMarioInWater);
IS_MSG_IMPL(HackMoveRockForestPush);
IS_MSG_IMPL(HackSelfCeilingCheckMiss);
IS_MSG_IMPL(HackSyncDamageVisibility);
IS_MSG_IMPL(HackUpperPunch);
IS_MSG_IMPL(HackObjUpperPunch);
IS_MSG_IMPL(HammerAttackDown);
IS_MSG_IMPL(HammerAttackSide);
IS_MSG_IMPL(HammerAttackSideCollide);
IS_MSG_IMPL(HammerBrosHammerEnemyAttack);
IS_MSG_IMPL(HammerBrosHammerHackAttack);
IS_MSG_IMPL(HammerBrosHammerSearch);
IS_MSG_IMPL(HipDropTransformReverse);
IS_MSG_IMPL(HipDropTransformTransform);
IS_MSG_IMPL(HipDropTransformingUp);
IS_MSG_IMPL(HipDropTransformingDown);
IS_MSG_IMPL(HipDropTransformingFinish);
IS_MSG_IMPL(HitGrowFlowerPot);
IS_MSG_IMPL(HitGrowPlantPot);
IS_MSG_IMPL(HosuiAttack);
IS_MSG_IMPL(HosuiAttackCollide);
IS_MSG_IMPL(HosuiAttackNoEffect);
IS_MSG_IMPL(HosuiAttackStrong);
IS_MSG_IMPL(HosuiTouch);
IS_MSG_IMPL(HosuiTrample);
IS_MSG_IMPL(HosuiTrampleReflect);
IS_MSG_IMPL(HosuiTrampleReflectHigh);
IS_MSG_IMPL(IgnoreTouchTarget);
IS_MSG_IMPL(IgnorePushMotorcycle);
IS_MSG_IMPL(IcicleAttack);
IS_MSG_IMPL(IgnoreMirrorWarp);
IS_MSG_IMPL_(IgnoredByRunawayNpc, IsIgnoredByRunawayNpc);
IS_MSG_IMPL(ImplantGrowFlowerSeed);
IS_MSG_IMPL_(InitTouchTargetInfo, TouchTargetInfo);
IS_MSG_IMPL(ItemAmiiboKoopa);
IS_MSG_IMPL(IsExistPukupuku);
IS_MSG_IMPL(JangoAttack);
IS_MSG_IMPL(JangoRemoveCap);
IS_MSG_IMPL(KakkuKick);
IS_MSG_IMPL(KoopaBindStart);
IS_MSG_IMPL(KoopaCapPlayerFocusTarget);
IS_MSG_IMPL(KoopaCapPunchFinishL);
IS_MSG_IMPL(KoopaCapPunchFinishR);
IS_MSG_IMPL(KoopaCapPunchInvincibleL);
IS_MSG_IMPL(KoopaCapPunchInvincibleR);
IS_MSG_IMPL(KoopaCapPunchKnockBackL);
IS_MSG_IMPL(KoopaCapPunchKnockBackR);
IS_MSG_IMPL(KoopaCapPunchL);
IS_MSG_IMPL(KoopaCapPunchR);
IS_MSG_IMPL(KoopaCapSpinAttack);
IS_MSG_IMPL(KoopaCatchKoopaCap);
IS_MSG_IMPL(KoopaDashPunchAttack);
IS_MSG_IMPL(KoopaFire2D);
IS_MSG_IMPL(KoopaFireBallAttack);
IS_MSG_IMPL(KoopaHackDamage);
IS_MSG_IMPL(KoopaHackPunch);
IS_MSG_IMPL(KoopaHackPunchCollide);
IS_MSG_IMPL(KoopaHackTrample);
IS_MSG_IMPL(KoopaInvalidHackPunchFaceToCollision);
IS_MSG_IMPL(KoopaRingBeamInvalidTouch);
IS_MSG_IMPL(KoopaTailAttack);
IS_MSG_IMPL(KoopaTouchFloor);
IS_MSG_IMPL(KouraAttack2D);
IS_MSG_IMPL(KouraItemGet2D);
IS_MSG_IMPL(Kuribo2DTouch);
IS_MSG_IMPL(KuriboCollisionDamage);
IS_MSG_IMPL(KuriboCollisionKill);
IS_MSG_IMPL(KuriboFlick);
IS_MSG_IMPL(KuriboGirlAttack);
IS_MSG_IMPL(KuriboGirlLove);
IS_MSG_IMPL(KuriboTop);
IS_MSG_IMPL(KuriboTowerNum);
IS_MSG_IMPL(LaunchBlow);
IS_MSG_IMPL(LineDancerLink);
IS_MSG_IMPL(LongPushSensorHit);
IS_MSG_IMPL(LongPushBoxHit);
IS_MSG_IMPL(MagnetBulletAttack);
IS_MSG_IMPL(MeganeAttack);
IS_MSG_IMPL(MeganeHackTrample);
IS_MSG_IMPL(MofumofuBodyChainExplode);
IS_MSG_IMPL(MofumofuBulletUnexplosion);
IS_MSG_IMPL(MoonBasementAttackMeteor);
IS_MSG_IMPL(MoonBasementBreakShockwaveMeteor);
IS_MSG_IMPL(MoonBasementRockSyncClippingRegist);
IS_MSG_IMPL(MoonBasementRockSyncClippingInvalidate);
IS_MSG_IMPL(MoonBasementRockSyncClippingValidate);
IS_MSG_IMPL(MoonBasementRockThroughCollision);
IS_MSG_IMPL(MorningStarWarpEnd);
IS_MSG_IMPL(MorningStarWarpStart);
IS_MSG_IMPL(MotorcycleAttack);
IS_MSG_IMPL(MotorcycleCollideParkingLot);
IS_MSG_IMPL(MotorcycleDashAttack);
IS_MSG_IMPL(MotorcycleDashCollide);
IS_MSG_IMPL(CactusNeedleAttack);
IS_MSG_IMPL(CactusNeedleAttackStrong);
IS_MSG_IMPL(NoLimitTouchJump);
IS_MSG_IMPL(NoticePlayerDamage);
IS_MSG_IMPL(NpcScareByEnemy);
IS_MSG_MULTIPLE_IMPL(VolleyballNpcScareByEnemy, VolleyballNpcScareByEnemy, NpcScareByEnemy);
IS_MSG_IMPL(ObjSnapForce);
IS_MSG_IMPL(PackunEatCancel);
IS_MSG_IMPL(PackunEatEnd);
IS_MSG_IMPL(PackunEatStart);
IS_MSG_IMPL(PackunEatStartFollow);
IS_MSG_IMPL(Paint);
IS_MSG_IMPL(PaintTexture);
IS_MSG_IMPL(CheckPaintClear);
IS_MSG_IMPL(CheckPaintAlpha);
IS_MSG_IMPL(PartyPopperSoundAttack);
IS_MSG_IMPL(PechoSpot);
IS_MSG_IMPL(PlayerBallToss);
IS_MSG_IMPL(PlayerCarryCameraSubjectiveStart);
IS_MSG_IMPL(PlayerCarryCameraSubjectiveEnd);
IS_MSG_IMPL(PlayerCarryShineGetStart);
IS_MSG_IMPL(PlayerCarryShineGetEnd);
IS_MSG_IMPL(PlayerCapCatch);
IS_MSG_IMPL(PlayerCapHipDrop);
IS_MSG_IMPL(PlayerCapPush);
IS_MSG_IMPL(PlayerCapRecovery);
IS_MSG_IMPL(PlayerCapTouchJump);
IS_MSG_IMPL(PlayerCapTrample);
IS_MSG_IMPL(PlayerCoinDashGet);
IS_MSG_IMPL(PlayerEyePriorityTarget);
IS_MSG_IMPL(PlayerDisregardHomingAttack);
IS_MSG_IMPL(PlayerDisregardTargetMarker);
IS_MSG_IMPL(PlayerEquipKoopaCap);
IS_MSG_IMPL(PlayerFireBallAttack2D);
IS_MSG_IMPL(PlayerFireBallAttack3D);
IS_MSG_IMPL(PlayerHipDropDemoTrigger);
IS_MSG_IMPL(PlayerHipDropHipDropSwitch);
IS_MSG_IMPL(PlayerItemGet2D);
IS_MSG_IMPL(PlayerJumpTakeOffFloor);
IS_MSG_IMPL(PlayerObjectWallHit);
IS_MSG_IMPL(PlayerObjLeapFrog);
IS_MSG_IMPL(PlayerPenguinAttack);
IS_MSG_IMPL(PlayerPenguinAttackReflect);
IS_MSG_IMPL(PlayerPoleClimbKeep);
IS_MSG_IMPL(PlayerPoleClimbReaction);
IS_MSG_IMPL(PlayerRabbitGet);
IS_MSG_IMPL(PlayerRollingObjHit);
IS_MSG_IMPL(PlayerRollingWallHitDown);
IS_MSG_IMPL(PlayerRollingWallHitMove);
IS_MSG_IMPL(PlayerStartGrabCeil);
IS_MSG_IMPL(PlayerStartWallJump);
IS_MSG_IMPL(PlayerEndGrabCeil);
IS_MSG_IMPL(PlayerSwordAttack);
IS_MSG_IMPL(PlayerTouchFloorJumpCode);
IS_MSG_IMPL(PlayerTrample2D);
IS_MSG_IMPL(PlayerUpperPunch2D);
IS_MSG_IMPL(PlayerObjUpperPunch2D);
IS_MSG_IMPL(PropellerAttack);
IS_MSG_IMPL(PukupukuDash);
IS_MSG_IMPL(PukupukuKiss);
IS_MSG_IMPL(PukupukuRollingAttack);
IS_MSG_IMPL(PunchMachinePunchHook);
IS_MSG_IMPL(PunchMachinePunchStraight);
IS_MSG_IMPL(PunchMachinePunchUpper);
IS_MSG_IMPL(Push2D);
IS_MSG_IMPL(PushToFish);
IS_MSG_IMPL(PushToMotorcycle);
IS_MSG_IMPL(PushToPlayer);
IS_MSG_IMPL(RadishAttack);
IS_MSG_IMPL(RadishReflect);
IS_MSG_IMPL(RaceStart);
IS_MSG_IMPL(RaceStop);
IS_MSG_IMPL(RaceWait);
IS_MSG_IMPL(RequestChangeFireFlower);
IS_MSG_IMPL(RequestChangeKinokoSuper);
IS_MSG_IMPL(RequestPlayerJumpBreakFloor);
IS_MSG_IMPL(RequestPlayerJump);
IS_MSG_IMPL(RequestPlayerTrampleJump);
IS_MSG_IMPL(RequestPlayerSpinJump);
IS_MSG_IMPL(RequestSphinxJump);
IS_MSG_IMPL(RideOnEnd);
IS_MSG_IMPL(RideOnRelease);
IS_MSG_IMPL(RideOnStart);
IS_MSG_IMPL(RocketFlowerExtension);
IS_MSG_IMPL(SandSharkAttack);
IS_MSG_IMPL(SeedAttack);
IS_MSG_IMPL(SeedAttackBig);
IS_MSG_IMPL(SeedAttackHold);
IS_MSG_IMPL(SeedItemGet);
IS_MSG_IMPL(SeedReflect);
IS_MSG_IMPL(SeedTouch);
IS_MSG_IMPL(SenobiCancelStretch);
IS_MSG_IMPL(SenobiPunchBlockTransparent);
IS_MSG_IMPL(SenobiPartsMove);
IS_MSG_IMPL(SenobiTrample);
IS_MSG_IMPL(ShibakenApproach);
IS_MSG_IMPL(ShibakenKick);
IS_MSG_IMPL(SkaterAttack);
IS_MSG_IMPL(SpherePush);
IS_MSG_IMPL(SphinxJumpAttack);
IS_MSG_IMPL(SphinxQuizRouteKill);
IS_MSG_IMPL(SphinxRideAttack);
IS_MSG_IMPL(SphinxRideAttackReflect);
IS_MSG_IMPL(SphinxRideAttackTouchThrough);
IS_MSG_IMPL(SphinxRideAttackTouch);
IS_MSG_IMPL(StampTo2D);
IS_MSG_IMPL(StartHack);
IS_MSG_IMPL(StartInSaucePan);
IS_MSG_IMPL(StatueDrop);
IS_MSG_IMPL(StatueTrampleReflect);
IS_MSG_IMPL(StatuePush);
IS_MSG_IMPL(StatueSnap);
IS_MSG_IMPL(SunshineAttack);
IS_MSG_IMPL(TankBullet);
IS_MSG_IMPL(TankBulletNoReaction);
IS_MSG_IMPL(TankExplosion);
IS_MSG_IMPL(TankHackTrample);
IS_MSG_IMPL(TankKickHack);
IS_MSG_IMPL(TankKickEnemy);
IS_MSG_IMPL(TankLookOn);
IS_MSG_IMPL(TestPunch);
IS_MSG_IMPL(TestPunchStrong);
IS_MSG_IMPL(TimerAthleticDemoStart);
IS_MSG_IMPL(TouchDoorDrumn);
IS_MSG_IMPL(TouchFireDrum2D);
IS_MSG_IMPL(TrashBoxIn);
IS_MSG_IMPL(TRexAttack);
IS_MSG_IMPL(TRexAttackCollideBody);
IS_MSG_IMPL(TRexAttackCollideHead);
IS_MSG_MULTIPLE_IMPL(TRexAttackCollideAll, TRexAttackCollideBody, TRexAttackCollideHead);
IS_MSG_IMPL(TRexDashAttack);
IS_MSG_IMPL(TRexScrollPartsBreakWith);
IS_MSG_IMPL(TsukkunForceCancelCollide);
IS_MSG_IMPL(TsukkunHoldCollide);
IS_MSG_IMPL(TsukkunThroughCollide);
IS_MSG_MULTIPLE_IMPL(TsukkunThrustAll, TsukkunThrust, TsukkunThrustCollide);
IS_MSG_IMPL(TsukkunNoTrace);
IS_MSG_IMPL(TsukkunThrustHole);
IS_MSG_IMPL(UtsuboAttack);
IS_MSG_IMPL(WanderBossCameraRange);
IS_MSG_IMPL(WanwanEnemyAttack);
IS_MSG_IMPL(WanwanBlockAttack);
IS_MSG_IMPL(WanwanHoleIn);
IS_MSG_IMPL(WaterRoadIn);
IS_MSG_IMPL(WaterRoadNear);
IS_MSG_IMPL(WanwanPush);
IS_MSG_IMPL(WanwanReboundAttack);
IS_MSG_IMPL(WeaponItemGet);
IS_MSG_IMPL(WhipAttack);
IS_MSG_IMPL(WhipBind);
IS_MSG_IMPL(WhipHold);
IS_MSG_IMPL(WhipThrow);
IS_MSG_IMPL(YokinBallAttack);
IS_MSG_IMPL(YoshiDirectEat);
IS_MSG_IMPL(YoshiTongueAttack);
IS_MSG_IMPL(YoshiTongueEatBind);
IS_MSG_IMPL(YoshiTongueEatBindCancel);
IS_MSG_IMPL(YoshiTongueEatBindFinish);
IS_MSG_IMPL(YoshiTongueEatHomingTarget);
IS_MSG_IMPL(YukimaruPush);
IS_MSG_IMPL(KillerAttackNoExplode);
IS_MSG_IMPL(KillerMagnumAttack);
IS_MSG_IMPL(KillerMagnumHackAttack);
IS_MSG_IMPL(GabuzouAttack);
IS_MSG_IMPL(StackerRollingAttack)
IS_MSG_IMPL(StackerCapBoostAttack);
IS_MSG_IMPL(IgnoreIgnitionBomb);
IS_MSG_IMPL(ExplosionReflectBomb);
IS_MSG_IMPL(GolemStampPress);
IS_MSG_IMPL(BlockUpperPunch2D);
IS_MSG_IMPL(ShineGet);
IS_MSG_IMPL(ShineGet2D);
IS_MSG_IMPL(KillByShineGet);
IS_MSG_IMPL(KillByHomeDemo);
IS_MSG_IMPL(EndHomeDemo);
IS_MSG_IMPL(BreakBySword);
IS_MSG_IMPL(GotogotonOn);
IS_MSG_IMPL(GotogotonGoalExist);
IS_MSG_IMPL(BossMagmaCatchPlayer);
IS_MSG_IMPL(BossMagmaReleasePlayer);
IS_MSG_IMPL(BossMagmaDeadDemoStart);
IS_MSG_IMPL(BossMagmaDeadDemoEnd);
IS_MSG_IMPL(BossMagmaQueryToBubble);
IS_MSG_IMPL(CheckFishingTarget);
IS_MSG_IMPL(PlayerLookAtPosition);
IS_MSG_IMPL(TargetMarkerPosition);
IS_MSG_IMPL(SandGeyserRaise);
IS_MSG_IMPL(InitCapTarget);
IS_MSG_IMPL(TransferHack);
IS_MSG_IMPL(RequestTransferHack);
IS_MSG_IMPL(InitHack);
IS_MSG_IMPL(KillByBossBattleDemo);
IS_MSG_IMPL(KillByHackFirstDemo);
IS_MSG_IMPL(KillByMoonRockDemo);
IS_MSG_IMPL(KillBySwitchTimer);
IS_MSG_IMPL_(MofumofuReflectAll, CapReflect);

IS_MSG_MULTIPLE_IMPL(ItemGet2D, PlayerItemGet2D, KouraItemGet2D, BlockUpperPunch2D);
IS_MSG_MULTIPLE_IMPL(BlockReaction2D, PlayerUpperPunch2D, KoopaFire2D, KouraAttack2D);
IS_MSG_MULTIPLE_IMPL(PlayerDamage2D, EnemyAttack2D, KouraAttack2D);
IS_MSG_MULTIPLE_IMPL(PlayerDamageBlowDown, KoopaCapSpinAttack, KoopaDashPunchAttack,
                     KoopaTailAttack, TRexDashAttack);
IS_MSG_MULTIPLE_IMPL(PlayerJumpRequestAll, RequestPlayerJump, RequestPlayerSpinJump,
                     RequestPlayerTrampleJump, RequestPlayerJumpBreakFloor);
IS_MSG_MULTIPLE_IMPL(KoopaCapPunchAll, KoopaCapPunchL, KoopaCapPunchR);
IS_MSG_MULTIPLE_IMPL(KoopaCapPunchInvincibleAll, KoopaCapPunchInvincibleL,
                     KoopaCapPunchInvincibleR);
IS_MSG_MULTIPLE_IMPL(KoopaCapPunchFinishAll, KoopaCapPunchFinishL, KoopaCapPunchFinishR);

// NOTE: Back to back al::isMsgExplosion for some reason
bool isMsgBlockReaction3D(const al::SensorMsg* msg) {
    return isMsgBlowObjAttack(msg) || isMsgCapAttack(msg) || isMsgCapAttackCollide(msg) ||
           isMsgCapAttackRailMove(msg) || isMsgCapAttackStayRollingCollide(msg) ||
           al::isMsgExplosion(msg) || al::isMsgExplosion(msg) || isMsgGrowerAttack(msg) ||
           isMsgHackAttack(msg) || isMsgHackObjUpperPunch(msg) || isMsgHackUpperPunch(msg) ||
           isMsgHammerBrosHammerEnemyAttack(msg) || isMsgHammerBrosHammerHackAttack(msg) ||
           isMsgHosuiAttackCollide(msg) || isMsgItemAmiiboKoopa(msg) ||
           al::isMsgKickKouraAttackCollide(msg) || isMsgKillerMagnumAttack(msg) ||
           isMsgGamaneBullet(msg) || isMsgMotorcycleDashAttack(msg) ||
           al::isMsgPlayerHipDropAll(msg) || isMsgPlayerRollingWallHitDown(msg) ||
           al::isMsgPlayerSpinAttack(msg) || isMsgTsukkunThrustAll(msg) ||
           al::isMsgUpperPunch(msg) || isMsgWanwanBlockAttack(msg) || isMsgYoshiTongueAttack(msg) ||
           isMsgHackAttackPoison(msg) || isMsgHackAttackFire(msg);
}

bool isMsgBlockReactionAll(const al::SensorMsg* msg) {
    return isMsgBlockReaction2D(msg) || isMsgBlockReaction3D(msg);
}

bool isMsgItemGetByWeapon(const al::SensorMsg* msg) {
    return isMsgWeaponItemGet(msg) || isMsgCapItemGet(msg) || al::isMsgItemGetByObjAll(msg) ||
           isMsgSeedItemGet(msg);
}

bool isMsgItemGet(const al::SensorMsg* msg) {
    return al::isMsgPlayerItemGet(msg);
}

bool isMsgItemGetAll(const al::SensorMsg* msg) {
    return isMsgItemGet(msg) || isMsgItemGetByWeapon(msg);
}

bool isMsgFireDamageAll(const al::SensorMsg* msg) {
    return al::isMsgEnemyAttackFire(msg) || isMsgHackAttackFire(msg) ||
           al::isMsgPlayerFireBallAttack(msg);
}

// NOTE: Some messages are checked multiple times here
bool isMsgShineReaction(const al::SensorMsg* msg) {
    return isMsgCapAttack(msg) || isMsgWanwanReboundAttack(msg) || isMsgFireDamageAll(msg) ||
           al::isMsgExplosion(msg) || isMsgSeedAttack(msg) || al::isMsgKickStoneAttack(msg) ||
           al::isMsgKickStoneAttackHold(msg) || isMsgFireDamageAll(msg) || isMsgHosuiAttack(msg) ||
           isMsgHammerBrosHammerHackAttack(msg) || al::isMsgPlayerFireBallAttack(msg) ||
           isMsgRadishAttack(msg) || isMsgByugoBlow(msg) || isMsgPukupukuRollingAttack(msg) ||
           isMsgFishingAttack(msg) || al::isMsgBlockUpperPunch(msg) ||
           isMsgYoshiTongueAttack(msg) || isMsgHackAttackPoison(msg) ||
           isMsgEnemyAttackPoison(msg) || isMsgGamaneBullet(msg);
}

bool isMsgNpcCapReactionAll(const al::SensorMsg* msg) {
    return isMsgBlowObjAttack(msg) || isMsgCapReflect(msg) || isMsgFireDamageAll(msg) ||
           isMsgFishingAttack(msg) || isMsgGamaneBullet(msg) ||
           isMsgGiantWanderBossBulletAttack(msg) || isMsgHammerBrosHammerHackAttack(msg) ||
           isMsgHosuiAttack(msg) || al::isMsgKickStoneAttackReflect(msg) ||
           isMsgMayorItemReflect(msg) || isMsgPlayerObjectWallHit(msg) || isMsgPukupukuDash(msg) ||
           isMsgRadishReflect(msg) || isMsgSeedReflect(msg) || isMsgTankBullet(msg) ||
           isMsgTsukkunThrustAll(msg) || isMsgYoshiTongueAttack(msg);
}

bool isMsgHackNpcCapReactionAll(const al::SensorMsg* msg) {
    return !isMsgCapReflect(msg) && isMsgNpcCapReactionAll(msg);
}

bool isMsgPressDown(const al::SensorMsg* msg) {
    return al::isMsgPlayerTrample(msg) || isMsgPlayerAndCapObjHipDropAll(msg) ||
           isMsgHosuiTrample(msg) || isMsgDonsukeAttack(msg) || isMsgHammerAttackDown(msg) ||
           isMsgMeganeHackTrample(msg) || isMsgTankHackTrample(msg) || isMsgGolemStampPress(msg);
}

bool isMsgBlowDown(const al::SensorMsg* msg) {
    return isMsgHackAttack(msg) || al::isMsgBlockUpperPunch(msg) || isMsgBlowObjAttack(msg) ||
           isMsgBubbleAttack(msg) || isMsgBullAttack(msg) || isMsgCactusNeedleAttack(msg) ||
           isMsgCactusNeedleAttackStrong(msg) || isMsgCapAttackRailMove(msg) ||
           isMsgDamageBallAttack(msg) || isMsgEnemyAttackDash(msg) || isMsgEnemyAttackStrong(msg) ||
           al::isMsgExplosion(msg) || isMsgGabuzouAttack(msg) || isMsgGamaneBullet(msg) ||
           isMsgGiantWanderBossBulletAttack(msg) || isMsgGunetterAttack(msg) ||
           isMsgHammerAttackSide(msg) || isMsgHammerBrosHammerEnemyAttack(msg) ||
           isMsgHosuiAttack(msg) || isMsgItemAmiiboKoopa(msg) || al::isMsgKickKouraAttack(msg) ||
           al::isMsgKickStoneAttack(msg) || al::isMsgKickStoneAttackHold(msg) ||
           isMsgLaunchBlow(msg) || isMsgMotorcycleDashAttack(msg) ||
           al::isMsgNeedleBallAttack(msg) || al::isMsgPlayerFireBallAttack(msg) ||
           isMsgPlayerSwordAttack(msg) || isMsgPropellerAttack(msg) || isMsgRadishAttack(msg) ||
           isMsgSeedAttack(msg) || isMsgSeedAttackHold(msg) || isMsgSphinxRideAttack(msg) ||
           isMsgTankBullet(msg) || isMsgTsukkunThrustAll(msg) || isMsgWanwanEnemyAttack(msg) ||
           isMsgWanwanReboundAttack(msg) || isMsgWhipAttack(msg) || isMsgYoshiTongueAttack(msg);
}

bool isMsgBreakFrailBox(const al::SensorMsg* msg) {
    return isMsgHackAttack(msg) || isMsgBullAttack(msg) || al::isMsgExplosion(msg) ||
           isMsgPlayerSwordAttack(msg) || al::isMsgKickKouraAttackCollide(msg) ||
           isMsgEnemyObjBreak(msg) || isMsgTsukkunThrustAll(msg) ||
           al::isMsgPlayerHipDropAll(msg) || isMsgEnemyAttackDash(msg) ||
           isMsgMotorcycleDashCollide(msg) || isMsgWanwanReboundAttack(msg) ||
           isMsgGamaneBullet(msg) || isMsgPlayerRollingWallHitDown(msg) ||
           isMsgItemAmiiboKoopa(msg) || isMsgTankExplosion(msg) ||
           isMsgTRexScrollPartsBreakWith(msg) || isMsgPukupukuDash(msg) || isMsgRadishAttack(msg) ||
           isMsgDonsukeAttack(msg) || al::isMsgPlayerFireBallAttack(msg) ||
           isMsgSphinxRideAttackTouchThrough(msg) || isMsgKillerMagnumAttack(msg);
}

bool isMsgDamageFrailBox(const al::SensorMsg* msg) {
    return al::isMsgPlayerSpinAttack(msg) || isMsgCapTouchWall(msg) || isMsgCapReflect(msg) ||
           isMsgCapAttackStayRolling(msg) || isMsgHosuiAttackCollide(msg) || isMsgSeedAttack(msg) ||
           isMsgGrowerAttack(msg) || isMsgYoshiTongueAttack(msg);
}

bool isMsgChorobonReaction(const al::SensorMsg* msg) {
    return isMsgBlowDown(msg) || isMsgCapAttack(msg) || al::isMsgKickStoneAttack(msg) ||
           al::isMsgKickStoneAttackHold(msg) || isMsgSeedAttack(msg) || isMsgSeedAttackHold(msg) ||
           al::isMsgPlayerObjTouch(msg);
}

bool isMsgAttackDirect(const al::SensorMsg* msg) {
    return isMsgPressDown(msg) || isMsgBlowDown(msg);
}

bool isMsgThrowObjHit(const al::SensorMsg* msg) {
    return al::isMsgKickStoneAttack(msg) || isMsgSeedAttack(msg) || isMsgRadishAttack(msg);
}

bool isMsgThrowObjHitReflect(const al::SensorMsg* msg) {
    return al::isMsgKickStoneAttackReflect(msg) || isMsgSeedReflect(msg) || isMsgRadishReflect(msg);
}

bool isMsgTrampleReflectAll(const al::SensorMsg* msg) {
    return al::isMsgPlayerTrampleReflect(msg) || isMsgPlayerAndCapObjHipDropReflectAll(msg) ||
           isMsgHosuiTrample(msg);
}

bool isMsgPlayerAndCapHipDropAll(const al::SensorMsg* msg) {
    return al::isMsgPlayerHipDropAll(msg) || isMsgCapHipDrop(msg);
}

bool isMsgPlayerAndCapObjHipDropAll(const al::SensorMsg* msg) {
    return al::isMsgPlayerObjHipDropAll(msg) || isMsgCapObjHipDrop(msg);
}

bool isMsgPlayerAndCapObjHipDropReflectAll(const al::SensorMsg* msg) {
    return al::isMsgPlayerObjHipDropReflectAll(msg) || isMsgCapObjHipDropReflect(msg);
}

bool isMsgBreakSignBoard(const al::SensorMsg* msg) {
    return isMsgBullAttack(msg) || isMsgEnemyAttackTRex(msg) || al::isMsgExplosion(msg) ||
           isMsgKillerMagnumAttack(msg) || isMsgMotorcycleDashAttack(msg) ||
           isMsgSphinxRideAttackTouchThrough(msg) || isMsgTRexAttack(msg) ||
           isMsgTRexAttackCollideAll(msg) || isMsgWanwanReboundAttack(msg);
}

bool isMsgBreakCollapseSandHill(const al::SensorMsg* msg) {
    return isMsgBullAttack(msg) || isMsgCapAttack(msg) || al::isMsgExplosion(msg) ||
           isMsgKillerMagnumAttack(msg) || isMsgMotorcycleDashAttack(msg) ||
           al::isMsgPlayerHipDropAll(msg) || isMsgPlayerRollingObjHit(msg) ||
           isMsgPlayerObjectWallHit(msg) || isMsgRocketFlowerExtension(msg) ||
           isMsgSphinxRideAttackTouch(msg) || isMsgTRexAttack(msg) ||
           isMsgTRexAttackCollideAll(msg) || isMsgWanwanReboundAttack(msg);
}

bool isMsgPlayerDamage(const al::SensorMsg* msg) {
    return al::isMsgEnemyAttack(msg) || isMsgEnemyAttackTRex(msg) || al::isMsgExplosion(msg) ||

           (isMsgFireDamageAll(msg) && !al::isMsgPlayerFireBallAttack(msg)) ||
           isMsgPoisonDamageAll(msg) || isMsgBullAttack(msg) || isMsgBossKnuckleFallAttack(msg) ||
           isMsgEnemyAttackFireCollision(msg) || isMsgEnemyAttack3D(msg) ||
           isMsgCactusNeedleAttack(msg) || isMsgCactusNeedleAttackStrong(msg) ||
           isMsgDamageBallAttack(msg) || isMsgAttachCactusNeedle(msg) ||
           isMsgHammerBrosHammerHackAttack(msg) || isMsgIcicleAttack(msg) ||
           isMsgDonsukeAttack(msg) || isMsgPlayerDamage2D(msg);
}

bool isMsgUpperPunchAll(const al::SensorMsg* msg) {
    return isMsgPlayerUpperPunch2D(msg) || isMsgHackUpperPunch(msg) || al::isMsgUpperPunch(msg);
}

bool isMsgTsukkunThrust(const al::SensorMsg* pMsg, bool* isNonEnemy) {
    auto* msg = sead::DynamicCast<const SensorMsgTsukkunThrust>(pMsg);
    if (!msg)
        return false;
    if (isNonEnemy)
        *isNonEnemy = msg->getIsNonEnemy();
    return true;
}

bool isMsgTsukkunThrustCollide(const al::SensorMsg* pMsg, bool* isNonEnemy) {
    auto* msg = sead::DynamicCast<const SensorMsgTsukkunThrustCollide>(pMsg);
    if (!msg)
        return false;
    if (isNonEnemy)
        *isNonEnemy = msg->getIsNonEnemy();
    return true;
}

bool isMsgTsukkunThrustSpin(const al::SensorMsg* pMsg, bool* isNonEnemy) {
    auto* msg = sead::DynamicCast<const SensorMsgTsukkunThrustSpin>(pMsg);
    if (!msg)
        return false;
    if (isNonEnemy)
        *isNonEnemy = msg->getIsNonEnemy();
    return true;
}

bool isMsgTsukkunThrustReflect(const al::SensorMsg* pMsg, bool* isNonEnemy) {
    auto* msg = sead::DynamicCast<const SensorMsgTsukkunThrustReflect>(pMsg);
    if (!msg)
        return false;
    if (isNonEnemy)
        *isNonEnemy = msg->getIsNonEnemy();
    return true;
}

bool isMsgTsukkunThrustHitReflectCollide(const al::SensorMsg* pMsg, bool* isNonEnemy) {
    auto* msg = sead::DynamicCast<const SensorMsgTsukkunThrustHitReflectCollide>(pMsg);
    if (!msg)
        return false;
    if (isNonEnemy)
        *isNonEnemy = msg->getIsNonEnemy();
    return true;
}

bool isMsgTsukkunThrustReflectCollide(const al::SensorMsg* pMsg, bool* isNonEnemy) {
    auto* msg = sead::DynamicCast<const SensorMsgTsukkunThrustReflectCollide>(pMsg);
    if (!msg)
        return false;
    if (isNonEnemy)
        *isNonEnemy = msg->getIsNonEnemy();
    return true;
}

bool isMsgSwitchOnWithSaveRequest(const al::SensorMsg* pMsg, SaveObjInfo** info) {
    auto* msg = sead::DynamicCast<const SensorMsgSwitchOnWithSaveRequest>(pMsg);
    if (!msg)
        return false;
    if (info)
        *info = msg->getInfo();
    return true;
}

bool isMsgDashPanel(const al::SensorMsg* pMsg, s32* unk) {
    auto* msg = sead::DynamicCast<const SensorMsgDashPanel>(pMsg);
    if (!msg)
        return false;
    if (unk)
        *unk = msg->getUnk();
    return true;
}

bool isMsgNetworkShootingShot(const al::SensorMsg* pMsg, s32 unk) {
    // NOTE: Combining conditions or doing an early-return here mismatches
    if (auto* msg = sead::DynamicCast<const SensorMsgNetworkShootingShot>(pMsg)) {
        if (msg->getUnk() != unk)
            return true;
    }
    return false;
}

bool isMsgNetworkShootingChargeShot(const al::SensorMsg* pMsg, s32 unk) {
    // NOTE: Combining conditions or doing an early-return here mismatches
    if (auto* msg = sead::DynamicCast<const SensorMsgNetworkShootingChargeShot>(pMsg)) {
        if (msg->getUnk() != unk)
            return true;
    }
    return false;
}

bool isMsgRaceReturnToCourse(const al::SensorMsg* pMsg, sead::Vector3f* pos,
                             sead::Vector3f* front) {
    auto* msg = sead::DynamicCast<const SensorMsgRaceReturnToCourse>(pMsg);
    if (!msg)
        return false;
    if (pos)
        pos->set(msg->getPos());
    if (front)
        front->set(msg->getFront());
    return true;
}

bool isMsgTrampolineCrackJump(f32* unk, f32* unk2, const al::SensorMsg* pMsg) {
    // NOTE: Doing an early-return here mismatches
    if (auto* msg = sead::DynamicCast<const SensorMsgTrampolineCrackJump>(pMsg)) {
        if (unk)
            *unk = msg->getUnk();
        if (unk2)
            *unk2 = msg->getUnk2();
        return true;
    }
    return false;
}

bool isMsgEndHack(const HackEndParam** param, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgEndHack>(pMsg);
    if (!msg)
        return false;
    *param = msg->getParam();
    return true;
}

bool isMsgHackDirectStageInit(IUsePlayerHack** hack, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgHackDirectStageInit>(pMsg);
    if (!msg)
        return false;
    *hack = msg->getHack();
    return true;
}

bool isMsgRequestPlayerStainWet(const al::SensorMsg* pMsg, s32* stainType) {
    auto* msg = sead::DynamicCast<const SensorMsgRequestPlayerStainWet>(pMsg);
    if (!msg)
        return false;
    *stainType = (s32)msg->getStainType();
    return true;
}

bool tryGetTestPunchInfo(sead::Vector3f* dir, s32* hitId, const al::SensorMsg* pMsg, s32 teamId) {
    if (auto* msg = sead::DynamicCast<const SensorMsgTestPunch>(pMsg)) {
        if ((msg->getHitId() == -1 || *hitId != msg->getHitId()) &&
            (msg->getTeamId() == -1 || msg->getTeamId() != teamId)) {
            dir->set(msg->getDir());
            *hitId = msg->getHitId();
            return true;
        }
    }

    if (auto* msg = sead::DynamicCast<const SensorMsgTestPunchStrong>(pMsg)) {
        if ((msg->getHitId() == -1 || *hitId != msg->getHitId()) &&
            (msg->getTeamId() == -1 || msg->getTeamId() != teamId)) {
            dir->set(msg->getDir());
            *hitId = msg->getHitId();
            return true;
        }
    }

    if (auto* msg = sead::DynamicCast<const SensorMsgTsukkunThrust>(pMsg)) {
        if (msg->getHitId() == -1 || *hitId != msg->getHitId()) {
            dir->set(msg->getDir());
            *hitId = msg->getHitId();
            return true;
        }
    }

    return false;
}

bool tryGetTestPunchTeamId(s32* teamId, const al::SensorMsg* pMsg) {
    if (auto* msg = sead::DynamicCast<const SensorMsgTestPunch>(pMsg)) {
        *teamId = msg->getTeamId();
        return true;
    }

    if (auto* msg = sead::DynamicCast<const SensorMsgTestPunchStrong>(pMsg)) {
        *teamId = msg->getTeamId();
        return true;
    }
    return false;
}

bool tryGetPunchGuard(s32* punchGuard, const al::SensorMsg* pMsg, s32 teamId) {
    auto* msg = sead::DynamicCast<const SensorMsgPunchGuard>(pMsg);
    if (!msg || (msg->getPunchGuard() != -1 && *punchGuard == msg->getPunchGuard()) ||
        (msg->getTeamId() != -1 && msg->getTeamId() == teamId))
        return false;
    *punchGuard = msg->getPunchGuard();
    return true;
}

bool tryGetTsukkunThrustInfo(sead::Vector3f* dir, s32* hitId, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgTsukkunThrust>(pMsg);
    if (!msg)
        return false;
    dir->set(msg->getDir());
    *hitId = msg->getHitId();
    return true;
}

bool tryGetTsukkunThrustDir(sead::Vector3f* dir, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgTsukkunThrust>(pMsg);
    if (!msg)
        return false;
    if (dir)
        dir->set(msg->getDir());
    return true;
}

bool tryGetTsukkunThrustReflectDir(sead::Vector3f* dir, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgTsukkunThrustReflect>(pMsg);
    if (!msg)
        return false;
    if (dir)
        dir->set(msg->getDir());
    return true;
}

bool tryGetTsukkunThrustHole(sead::Vector3f* tsukkunPos, sead::Vector3f* beakPos,
                             const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgTsukkunThrustHole>(pMsg);
    if (!msg)
        return false;
    if (tsukkunPos)
        tsukkunPos->set(msg->getTsukkunPos());
    if (beakPos)
        beakPos->set(msg->getBeakPos());
    return true;
}

bool tryGetTouchTargetInfo(TouchTargetInfo** info, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgTouchTargetInfo>(pMsg);
    if (!msg)
        return false;
    *info = msg->getInfo();
    return true;
}

bool tryGetTouchTargetInfoCollisionTouchPos(sead::Vector3f* pos, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgTouchTargetInfo>(pMsg);
    if (!msg || !msg->getCollisionTouchPos())
        return false;
    *pos = *msg->getCollisionTouchPos();
    return true;
}

bool tryGetWhipThrowDir(sead::Vector3f* dir, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgWhipThrow>(pMsg);
    if (!msg)
        return false;
    *dir = msg->getDir();
    return true;
}

bool tryGetRequestPlayerJumpInfo(f32* power, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgRequestPlayerJump>(pMsg);
    if (!msg)
        return false;
    *power = msg->getPower();
    return true;
}

bool tryGetRequestPlayerSpinJumpInfo(f32* power, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgRequestPlayerJump>(pMsg);
    if (!msg)
        return false;
    *power = msg->getPower();
    return true;
}

bool tryGetRequestPlayerTrampleJumpInfo(f32* power, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgRequestPlayerJump>(pMsg);
    if (!msg)
        return false;
    *power = msg->getPower();
    return true;
}

bool tryGetRequestSphinxJumpInfo(f32* power, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgRequestPlayerJump>(pMsg);
    if (!msg)
        return false;
    *power = msg->getPower();
    return true;
}

bool tryGetRequestPlayerWaitAnim(const char** anim, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgRequestPlayerWaitAnim>(pMsg);
    if (!msg)
        return false;
    *anim = msg->getAnim();
    return true;
}

bool tryGetCapTouchWallHitPos(sead::Vector3f* pos, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgCapTouchWall>(pMsg);
    if (!msg)
        return false;
    *pos = msg->getHitPos();
    return true;
}

bool tryGetCapTouchWallNormal(sead::Vector3f* normal, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgCapTouchWall>(pMsg);
    if (!msg)
        return false;
    *normal = msg->getNormal();
    return true;
}

bool tryGetCapRethrowPos(sead::Vector3f* pos, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgCapRethrow>(pMsg);
    if (!msg)
        return false;
    *pos = msg->getPos();
    return true;
}

bool tryGetCapRethrowUp(sead::Vector3f* up, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgCapRethrow>(pMsg);
    if (!msg)
        return false;
    *up = msg->getUp();
    return true;
}

bool tryGetCapRethrowFront(sead::Vector3f* front, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgCapRethrow>(pMsg);
    if (!msg)
        return false;
    *front = msg->getFront();
    return true;
}

bool tryGetGolemStampPushV(f32* velocity, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgGolemStampPushV>(pMsg);
    if (!msg)
        return false;
    *velocity = msg->getVelocity();
    return true;
}

bool tryGetGolemStampPushH(f32* velocity, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgGolemStampPushH>(pMsg);
    if (!msg)
        return false;
    *velocity = msg->getVelocity();
    return true;
}

// TODO: rename parameters
bool tryGetCapChangeGiant(f32* unk, s32* unk2, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgCapChangeGiant>(pMsg);
    if (!msg)
        return false;
    *unk = msg->getUnk();
    *unk2 = msg->getUnk2();
    return true;
}

bool tryGetByugoBlowForce(sead::Vector3f* force, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgByugoBlow>(pMsg);
    if (!msg)
        return false;
    *force = msg->getForce();
    return true;
}

bool tryGetObjSnapForce(sead::Vector3f* force, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgObjSnapForce>(pMsg);
    if (!msg)
        return false;
    *force = msg->getForce();
    return true;
}

bool tryGetPackunEatEndPos(sead::Vector3f* pos, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgPackunEatEnd>(pMsg);
    if (!msg)
        return false;
    *pos = msg->getPos();
    return true;
}

bool tryGetPackunEatEndDir(sead::Vector3f* dir, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgPackunEatEnd>(pMsg);
    if (!msg)
        return false;
    *dir = msg->getDir();
    return true;
}

bool tryGetPackunEatStartFollowPos(const sead::Vector3f** posPtr, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgPackunEatStartFollow>(pMsg);
    if (!msg)
        return false;
    *posPtr = msg->getPosPtr();
    return true;
}

bool tryGetHackBlowJump(sead::Vector3f* end, f32* height, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgHackBlowJump>(pMsg);
    if (!msg)
        return false;
    end->set(msg->getEnd());
    *height = msg->getHeight();
    return true;
}

bool tryGetFireBlowerAttackPos(sead::Vector3f* pos, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgFireBlowerAttack>(pMsg);
    if (!msg)
        return false;
    *pos = msg->getPos();
    return true;
}

bool tryGetFireBlowerAttackDir(sead::Vector3f* dir, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgFireBlowerAttack>(pMsg);
    if (!msg)
        return false;
    *dir = msg->getDir();
    return true;
}

bool tryGetFireBlowerAttackRadius(f32* radius, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgFireBlowerAttack>(pMsg);
    if (!msg)
        return false;
    *radius = msg->getRadius();
    return true;
}

bool tryGetPaintColor(sead::Color4u8* color, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgPaint>(pMsg);
    if (!msg)
        return false;
    *color = msg->getColor();
    return true;
}

bool tryGetPaintSize(f32* size, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgPaint>(pMsg);
    if (!msg)
        return false;
    *size = msg->getSize();
    return true;
}

bool tryGetPaintDrawType(s32* drawType, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgPaint>(pMsg);
    if (!msg)
        return false;
    *drawType = msg->getDrawType();
    return true;
}

bool tryGetPaintTextureSize(f32* size, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgPaintTexture>(pMsg);
    if (!msg)
        return false;
    *size = msg->getSize();
    return true;
}

bool tryGetPaintTextureRotation(f32* rotation, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgPaintTexture>(pMsg);
    if (!msg)
        return false;
    *rotation = msg->getRotation();
    return true;
}

bool tryGetPaintTextureDrawType(s32* drawType, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgPaintTexture>(pMsg);
    if (!msg)
        return false;
    *drawType = msg->getDrawType();
    return true;
}

bool tryGetCheckPaintClearColor(sead::Color4u8* color, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgCheckPaintClear>(pMsg);
    if (!msg)
        return false;
    *color = msg->getColor();
    return true;
}

bool tryGetCheckPaintClearPos(sead::Vector3f* pos, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgCheckPaintClear>(pMsg);
    if (!msg)
        return false;
    *pos = msg->getPos();
    return true;
}

bool tryGetCheckPaintClearDrawType(s32* drawType, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgCheckPaintClear>(pMsg);
    if (!msg)
        return false;
    *drawType = msg->getDrawType();
    return true;
}

bool tryGetCheckPaintAlphaPos(sead::Vector3f* pos, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgCheckPaintAlpha>(pMsg);
    if (!msg)
        return false;
    *pos = msg->getPos();
    return true;
}

bool tryGetSenobiPartsCollidedNormal(sead::Vector3f* collidedNormal, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgSenobiPartsMove>(pMsg);
    if (!msg)
        return false;
    collidedNormal->set(msg->getCollidedNormal());
    return true;
}

bool tryGetSenobiPartsMoveDistance(f32* distance, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgSenobiPartsMove>(pMsg);
    if (!msg)
        return false;
    *distance = msg->getDistance();
    return true;
}

bool tryGetStampTo2DForce(sead::Vector3f* force, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgStampTo2D>(pMsg);
    if (!msg)
        return false;
    *force = msg->getForce();
    return true;
}

bool tryGetAirExplosionForce(sead::Vector3f* force, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgAirExplosion>(pMsg);
    if (!msg)
        return false;
    *force = msg->getForce();
    return true;
}

bool tryGetGhostStartOkObjId(sead::BufferedSafeString* objId, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgGhostRecordStartOk>(pMsg);
    if (!msg)
        return false;
    objId->format("%s", msg->getObjId());
    return true;
}

bool tryGetBossMagmaDeadDemoEndTargetPos(const al::SensorMsg* pMsg, sead::Vector3f* targetPos) {
    auto* msg = sead::DynamicCast<const SensorMsgBossMagmaDeadDemoEnd>(pMsg);
    if (!msg)
        return false;
    targetPos->set(msg->getTargetPos());
    return true;
}

bool tryGetBossMagmaBreathForce(const al::SensorMsg* pMsg, sead::Vector3f* force) {
    auto* msg = sead::DynamicCast<const SensorMsgBossMagmaBreathForce>(pMsg);
    if (!msg)
        return false;
    force->set(msg->getForce());
    return true;
}

bool tryGetBossMagmaResetPos(const al::SensorMsg* pMsg, sead::Vector3f* resetPos) {
    auto* msg = sead::DynamicCast<const SensorMsgBossMagmaResetPos>(pMsg);
    if (!msg)
        return false;
    resetPos->set(msg->getResetPos());
    return true;
}

// TODO: Rename paramteres
bool tryGetSandGeyserRaise(const al::SensorMsg* pMsg, f32* unk, f32* unk2) {
    auto* msg = sead::DynamicCast<const SensorMsgSandGeyserRaise>(pMsg);
    if (!msg)
        return false;
    *unk = msg->getUnk();
    *unk2 = msg->getUnk2();
    return true;
}

const char* tryGetFishingUpFloatMaterialCode(const al::SensorMsg* pMsg) {
    if (!MSG_TYPE_CHECK_(pMsg, FishingUpImmediately))
        return nullptr;
    auto* msg = sead::DynamicCast<const SensorMsgFishingUpImmediately>(pMsg);
    return msg->getMaterialCode();
}

al::ComboCounter* tryGetMsgComboCount(const al::SensorMsg* pMsg) {
    if (auto* msg = sead::DynamicCast<const SensorMsgCactusNeedleAttack>(pMsg))
        return msg->getComboCounter();

    if (auto* msg = sead::DynamicCast<const al::SensorMsgPlayerAttackTrample>(pMsg))
        return msg->getComboCounter();

    if (auto* msg = sead::DynamicCast<const al::SensorMsgPlayerAttackHipDrop>(pMsg))
        return msg->getComboCounter();

    if (auto* msg = sead::DynamicCast<const al::SensorMsgPlayerAttackObjHipDrop>(pMsg))
        return msg->getComboCounter();

    if (auto* msg = sead::DynamicCast<const al::SensorMsgPlayerAttackObjHipDropReflect>(pMsg))
        return msg->getComboCounter();

    if (auto* msg = sead::DynamicCast<const al::SensorMsgPlayerTrampleReflect>(pMsg))
        return msg->getComboCounter();

    return nullptr;
}

al::ComboCounter* getMsgComboCount(const al::SensorMsg* pMsg) {
    return tryGetMsgComboCount(pMsg);
}

bool setMsgYoshiTongueEatBindRadiusAndOffset(const al::SensorMsg* pMsg, f32 radius, f32 offset) {
    auto* msg = sead::DynamicCast<const SensorMsgYoshiTongueEatBind>(pMsg);
    if (!msg)
        return false;
    *msg->getRadiusPtr() = radius;
    *msg->getOffsetPtr() = offset;
    return true;
}

bool setMsgYoshiTongueEatBindScale(const al::SensorMsg* pMsg, f32 scale) {
    auto* msg = sead::DynamicCast<const SensorMsgYoshiTongueEatBind>(pMsg);
    if (!msg)
        return false;
    *msg->getScalePtr() = scale;
    return true;
}

void calcBlowDownDir(sead::Vector3f* dir, const al::SensorMsg* pMsg, al::HitSensor* receiver,
                     al::HitSensor* sender) {
    if (!tryGetTsukkunThrustDir(dir, pMsg))
        al::calcDirBetweenSensorsH(dir, receiver, sender);
}

bool trySendMsgHackPunchToCeil(al::LiveActor* sender, al::HitSensor* receiver) {
    // NOTE: Writing this as one return statement causes a mismatch
    if (al::getVelocityPtr(sender)->y > 0.0f && al::isCollidedCeiling(sender) &&
        sendMsgHackUpperPunch(al::getCollidedCeilingSensor(sender), receiver))
        return true;
    return false;
}

bool trySendMsgHackPunchToSensor(al::LiveActor* actor, al::HitSensor* receiver,
                                 al::HitSensor* sender) {
    sead::Vector3f dir = {0.0f, 0.0f, 0.0f};
    if (!al::calcDirBetweenSensors(&dir, sender, receiver))
        return false;

    const sead::Vector3f& gravity = al::getGravity(actor);
    if (dir.dot(-gravity) <= 0.342020153f)  // cos(70°)
        return false;

    if ((-gravity).dot(al::getVelocity(actor)) > 0.0)
        if (sendMsgHackObjUpperPunch(receiver, sender))
            return true;

    return false;
}

bool isFallingTargetMoonBasementRock(const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgMoonBasementRockThroughCollision>(pMsg);
    return msg->getIsFallOrBreak();
}

// TODO: Rename unk and unk2
bool tryInitTouchTargetInfoBySensorOffset(const al::SensorMsg* pMsg, const al::HitSensor* sensor,
                                          const sead::Vector3f& unk, const sead::Vector3f& unk2) {
    auto* msg = sead::DynamicCast<const SensorMsgTouchTargetInfo>(pMsg);
    if (!msg)
        return false;
    msg->getInfo()->setInfoBySensor(sensor, unk, unk2);
    return true;
}

// TODO: Rename unk, unk2 and unk3
bool tryInitTouchTargetInfoByConnector(const al::SensorMsg* pMsg, const al::MtxConnector* connector,
                                       const sead::Vector3f& unk, const sead::Vector3f& unk2,
                                       bool unk3) {
    auto* msg = sead::DynamicCast<const SensorMsgTouchTargetInfo>(pMsg);
    if (!msg)
        return false;
    msg->getInfo()->setInfoByConnector(connector, unk, unk2, unk3);
    return true;
}

// TODO: Rename out unk2, unk3 and unk4
// TODO: Clean this up (https://decomp.me/scratch/NNnYg)
bool tryCalcCollisionImpulse(sead::Vector3f* out, f32 unk2, const sead::Vector3f& unk3, f32 unk4,
                             const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgCollisionImpulse>(pMsg);
    if (!msg)
        return false;

    const sead::Vector3f* const pConstVec = msg->getConstVec();

    f32 term1 = ((msg->getFloatVal() + unk2) * 0.5f) + 1.0f;
    f32 val2 = msg->getFloatVal2();
    f32 denominator = val2 + unk4;
    f32 numerator_part1 = val2 * unk4;
    f32 thing = (numerator_part1 * term1) / denominator;

    sead::Vector3f parellelized;
    sead::Vector3f parellelized2;

    al::parallelizeVec(&parellelized, *pConstVec, *msg->getConstVec2());
    al::parallelizeVec(&parellelized2, *pConstVec, unk3);

    sead::Vector3f diff = parellelized2 - parellelized;

    *msg->getVecPtr() = diff * thing;
    *out = -diff * thing;

    return true;
}

bool tryInitWhipTarget(const al::SensorMsg* pMsg, al::HitSensor* sensor,
                       const sead::Vector3f* trans) {
    if (auto* msg = sead::DynamicCast<const SensorMsgWhipHold>(pMsg)) {
        msg->getInfo()->initWhipTarget(sensor, trans);
        return true;
    }

    if (auto* msg = sead::DynamicCast<const SensorMsgWhipBind>(pMsg)) {
        msg->getInfo()->initWhipTarget(sensor, trans);
        return true;
    }

    return false;
}

bool tryMagnetPower(bool* isPower, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgMagnet>(pMsg);
    if (!msg)
        return false;
    *isPower = msg->getIsPower();
    return true;
}

bool tryMagnetBulletAttack(f32* unk, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgMagnetBulletAttack>(pMsg);
    if (!msg)
        return false;
    *unk = msg->getUnk();
    return true;
}

const char* tryGetHitReactionForMsg(bool* found, const al::SensorMsg* msg) {
    *found = true;
    if (isMsgCapAttack(msg) || isMsgCapAttackRailMove(msg) || isMsgCapAttackStayRolling(msg) ||
        isMsgCapAttackCollide(msg))
        return "帽子ヒット";
    if (isMsgCapAttackStayRollingCollide(msg))
        return "帽子停滞ヒット";
    if (isMsgCapHipDrop(msg) || isMsgCapObjHipDrop(msg) || isMsgCapObjHipDropReflect(msg))
        return "帽子ヒット";
    if (isMsgCapReflect(msg) || isMsgCapReflectCollide(msg) || isMsgCapTouchWall(msg))
        return "帽子反射ヒット";
    if (isMsgChorobonAttack(msg))
        return "チョロボンアタック";
    if (al::isMsgEnemyAttackFire(msg))
        return "炎攻撃[敵]ヒット";
    if (isMsgHackAttackFire(msg))
        return "炎攻撃[ひょうい]ヒット";
    if (isMsgFireBrosFireBallCollide(msg))
        return "炎攻撃ヒット";
    if (isMsgEnemyAttackFireCollision(msg))
        return "溶岩ダメージ[オブジェ]ヒット";
    if (al::isMsgEnemyAttack(msg) || isMsgEnemyAttackStrong(msg) || isMsgEnemyAttackDash(msg))
        return "敵攻撃ヒット";
    if (isMsgEnemyObjBreak(msg))
        return "敵オブジェ破壊";
    if (isMsgFishingAttack(msg))
        return "釣り針攻撃";
    if (isMsgGabuzouAttack(msg))
        return "ガブゾウ攻撃敵ヒット";
    if (isMsgEnemyAttackPoison(msg))
        return "毒攻撃[敵]ヒット";
    if (isMsgHackAttackPoison(msg))
        return "毒攻撃[ひょうい]ヒット";
    if (isMsgPaintAttackPoison(msg))
        return "毒攻撃[お絵かきオブジェ]ヒット";
    if (isMsgPukupukuDash(msg))
        return "プクプクダッシュアタック";
    if (al::isMsgExplosion(msg)) {
        *found = true;
        return nullptr;
    }
    if (isMsgGunetterAttack(msg) || isMsgGunetterBodyTouch(msg))
        return "グネッター攻撃ヒット";
    if (isMsgItemAmiiboKoopa(msg))
        return "クッパAmiibo攻撃ヒット";
    if (isMsgHackAttack(msg))
        return "憑依攻撃ヒット";
    if (isMsgHackAttackMapObj(msg))
        return "憑依攻撃ヒット[地形オブジェ]";
    if (isMsgHackAttackKick(msg))
        return "キックヒット";
    if (isMsgHackUpperPunch(msg) || isMsgHackObjUpperPunch(msg))
        return "突き上げパンチヒット";
    if (isMsgHammerBrosHammerHackAttack(msg))
        return "衝突";
    if (isMsgHammerBrosHammerEnemyAttack(msg))
        return "敵攻撃ヒット";
    if (al::isMsgKickStoneAttack(msg) || al::isMsgKickStoneAttackCollide(msg) ||
        al::isMsgKickStoneAttackHold(msg))
        return "キック小石ヒット";
    if (al::isMsgKickStoneAttackReflect(msg))
        return "キック小石反射";
    if (al::isMsgKickStoneTrample(msg))
        return "キック小石踏み";
    if (isMsgMotorcycleAttack(msg))
        return "バイクヒット";
    if (isMsgMotorcycleDashAttack(msg) || isMsgMotorcycleDashCollide(msg))
        return "バイク突進ヒット";
    if (isMsgCactusNeedleAttack(msg))
        return "とげヒット";
    if (isMsgCactusNeedleAttackStrong(msg))
        return "とげヒット[強]";
    if (al::isMsgPlayerFireBallAttack(msg))
        return "炎攻撃ヒット";
    if (al::isMsgPlayerKick(msg))
        return "キックヒット";
    if (isMsgPlayerRabbitGet(msg))
        return "ウサギゲット";
    if (isMsgEnemyKick(msg) || isMsgRabbitKick(msg))
        return "キックヒット";
    if (isMsgPlayerBallToss(msg))
        return "ボールトス";
    if (isMsgPlayerObjectWallHit(msg))
        return "オブジェ壁ヒット";
    if (al::isMsgPlayerObjHipDropAll(msg))
        return "ヒップドロップヒット";
    if (isMsgPlayerCapTrample(msg) || isMsgPlayerCapHipDrop(msg))
        return "踏みつけヒット[反射]";
    if (al::isMsgPlayerHipDropAll(msg))
        return "ヒップドロップヒット[コリジョン]";
    if (al::isMsgPlayerItemGet(msg))
        return "アイテムゲット";
    if (al::isMsgPlayerObjHipDropReflectAll(msg))
        return "踏みつけヒット";
    if (isMsgPlayerObjLeapFrog(msg))
        return "オブジェ馬跳びヒット";
    if (isMsgPlayerRollingWallHitDown(msg))
        return "壁衝突ヒット";
    if (isMsgPlayerRollingWallHitMove(msg))
        return "壁衝突ヒット[貫通]";
    if (isMsgPlayerRollingObjHit(msg))
        return "ローリングヒット";
    if (isMsgPlayerStartWallJump(msg)) {
        *found = true;
        return nullptr;
    }
    if (al::isMsgPlayerSpinAttack(msg))
        return "帽子ヒット";
    if (al::isMsgPlayerSlidingAttack(msg))
        return "スライディングアタックヒット";
    if (al::isMsgPlayerTrample(msg))
        return "踏みつけヒット";
    if (isMsgPlayerTrample2D(msg))
        return "踏みつけヒット[2D]";
    if (al::isMsgPlayerTrampleReflect(msg))
        return "踏みつけヒット[反射]";
    if (isMsgPlayerTouchFloorJumpCode(msg))
        return "踏みつけヒット[反射]";
    if (al::isMsgPlayerUpperPunch(msg))
        return "突き上げパンチヒット";
    if (isMsgPlayerUpperPunch2D(msg))
        return "突き上げパンチヒット[2D]";
    if (isMsgTRexAttack(msg))
        return "Tレックスヒット";
    if (isMsgTRexAttackCollideBody(msg))
        return "Tレックスコリジョン体ヒット";
    if (isMsgTRexAttackCollideHead(msg))
        return "Tレックスコリジョン頭ヒット";
    if (isMsgSeedAttack(msg) || isMsgSeedAttackBig(msg) || isMsgSeedAttackHold(msg))
        return "衝突ヒット";
    if (isMsgSeedReflect(msg))
        return "衝突反射";
    if (isMsgSeedTouch(msg))
        return "衝突反射";
    if (isMsgShibakenKick(msg))
        return "キックヒット";
    bool isNonEnemyTsukkunThrust;
    // NOTE: This needs to be seperate as putting it as the first condition in the if below it
    // mismatches
    if (isMsgTsukkunThrust(msg, &isNonEnemyTsukkunThrust))
        // [敵] = [Enemy]
        return isNonEnemyTsukkunThrust ? "突きヒット" : "突きヒット[敵]";
    if (isMsgTsukkunThrustSpin(msg, &isNonEnemyTsukkunThrust) ||
        isMsgTsukkunThrustCollide(msg, &isNonEnemyTsukkunThrust))
        return isNonEnemyTsukkunThrust ? "突きヒット" : "突きヒット[敵]";
    if (isMsgTsukkunThrustReflect(msg, &isNonEnemyTsukkunThrust) ||
        isMsgTsukkunThrustReflectCollide(msg, &isNonEnemyTsukkunThrust))
        return isNonEnemyTsukkunThrust ? "突き反射" : "突き反射[敵]";
    if (isMsgTsukkunThrustHitReflectCollide(msg, &isNonEnemyTsukkunThrust))
        return isNonEnemyTsukkunThrust ? "突き反射攻撃成功" : "突き反射攻撃成功[敵]";
    if (isMsgTsukkunThrustHole(msg))
        return "穴刺さり";
    if (isMsgSphinxRideAttack(msg))
        return "スフィンクス突進ヒット";
    if (isMsgSphinxRideAttackReflect(msg))
        return "スフィンクス突進反射";
    if (isMsgSphinxRideAttackTouch(msg) || isMsgSphinxRideAttackTouchThrough(msg))
        return "スフィンクス壁突進";
    if (isMsgPropellerAttack(msg))
        return "プロペラヒット";
    if (isMsgGrowerAttack(msg))
        return "グローワーアタック";
    if (isMsgBullHackAttack(msg))
        return "ブルアタック[Hack]";
    if (isMsgBullEnemyAttack(msg))
        return "ブルアタック[Enemy]";
    if (isMsgHosuiAttack(msg) || isMsgHosuiAttackCollide(msg))
        return "ホウスイアタック";
    if (isMsgKoopaCapPunchL(msg))
        return "クッパ左パンチアタック";
    if (isMsgKoopaCapPunchR(msg))
        return "クッパ右パンチアタック";
    if (isMsgKoopaCapPunchInvincibleL(msg))
        return "クッパ左パンチ無敵無効";
    if (isMsgKoopaCapPunchInvincibleR(msg))
        return "クッパ右パンチ無敵無効";
    if (isMsgKoopaCapPunchKnockBackL(msg))
        return "クッパ左パンチアタック";
    if (isMsgKoopaCapPunchKnockBackR(msg))
        return "クッパ右パンチアタック";
    if (isMsgKoopaCapPunchFinishL(msg))
        return "クッパ左とどめパンチアタック";
    if (isMsgKoopaCapPunchFinishR(msg))
        return "クッパ右とどめパンチアタック";
    if (isMsgKoopaHackPunch(msg))
        return "崩落クッパパンチアタック";
    if (isMsgKoopaHackPunchCollide(msg))
        return "崩落クッパパンチアタック";
    if (isMsgWanwanEnemyAttack(msg))
        return "ワンワンアタック";
    if (isMsgWanwanReboundAttack(msg))
        return "ワンワン反発アタック";
    if (isMsgWaterRoadIn(msg))
        return "水流ヒット";
    if (isMsgGamaneBullet(msg))
        return "ガマネー弾アタック";
    if (isMsgGamaneBulletThrough(msg))
        return "ガマネー弾アタック[貫通]";
    if (isMsgYoshiDirectEat(msg))
        return "ヨッシー食べる";
    if (isMsgYoshiTongueAttack(msg))
        return "ヨッシー舌ヒット";
    if (isMsgYoshiTongueEatBind(msg))
        return "ヨッシー舌接着";
    if (isMsgYoshiTongueEatBindFinish(msg))
        return "ヨッシー舌食べる";
    if (al::isMsgBlockUpperPunch(msg))
        return "ブロックアッパーパンチ";
    if (isMsgBlockUpperPunch2D(msg))
        return "ブロックアッパーパンチ[2D]";
    if (isMsgMayorItemReflect(msg))
        return "市長アイテムヒット";
    if (isMsgMayorItemCollide(msg))
        return "市長アイテムコリジョン接触";
    if (isMsgBlowObjAttack(msg))
        return "吹っ飛びオブジェヒット";
    if (isMsgBlowObjAttackReflect(msg))
        return "ヒットマーク小";
    if (isMsgBubbleAttack(msg))
        return "バブルアタック";
    if (isMsgTankBullet(msg))
        return "タンクロー弾ヒット";
    if (isMsgTankBulletNoReaction(msg))
        return "タンクロー弾ヒット[リアクションなし]";
    if (isMsgTankExplosion(msg))
        return "タンクロー弾爆発ヒット";
    if (isMsgDamageBallAttack(msg))
        return "ダメージボールアタック";
    if (isMsgDamageBallBodyAttack(msg))
        return "ダメージボールアタック";
    if (isMsgSenobiTrample(msg))
        return "セノビー踏みアタック";
    if (isMsgRadishAttack(msg))
        return "カブヒット";
    if (isMsgRadishReflect(msg))
        return "カブ反射ヒット";
    if (isMsgTankExplosion(msg))
        return "タンク弾ヒット";
    if (isMsgKouraAttack2D(msg))
        return "２Ｄコウラヒット";
    if (isMsgMeganeAttack(msg))
        return "メガネーアタック";
    if (isMsgHosuiTrample(msg))
        return "踏みつけヒット";
    if (isMsgHosuiTrampleReflect(msg) || isMsgHosuiTrampleReflectHigh(msg))
        return "踏みつけヒット[反射]";
    if (isMsgKillerAttackNoExplode(msg))
        return "爆発しないキラーヒット";
    if (isMsgKillerMagnumAttack(msg) || isMsgKillerMagnumHackAttack(msg))
        return "マグナムキラーヒット";
    if (isMsgDonsukeAttack(msg))
        return "ドンスケアタック";
    if (isMsgDonsukeGroundAttack(msg))
        return "ドンスケ地面アタック";
    if (isMsgStackerCapBoostAttack(msg))
        return "加速帽子ヒット";
    if (isMsgGiantWanderBossAttack(msg))
        return "徘徊ボス本体ヒット";
    if (isMsgGiantWanderBossBulletAttack(msg))
        return "徘徊ボス弾ヒット";
    if (isMsgIcicleAttack(msg))
        return "つららヒット";
    if (isMsgPechoSpot(msg))
        return "ペチョスポットヒット";
    if (isMsgCapTrampolineAttack(msg))
        return "帽子トランポリンヒット";
    if (isMsgBossKnuckleFallAttack(msg))
        return "拳攻撃ヒット";
    if (isMsgBossKnuckleIceFallToMummy(msg))
        return "氷マミーヒット";
    if (al::isMsgEnemyAttackNeedle(msg))
        return "とげヒット";
    if (al::isMsgNeedleBallAttack(msg))
        return "とげボールヒット";
    if (al::isMsgEnemyTrample(msg) || isMsgMeganeHackTrample(msg) || isMsgTankHackTrample(msg) ||
        isMsgGolemStampPress(msg))
        return "踏みつけヒット";
    if (isMsgKakkuKick(msg))
        return "キックヒット";
    if (isMsgStatueTrampleReflect(msg))
        return "地蔵踏み[反射]";
    if (isMsgCapBeamerBeam(msg) || isMsgKuriboGirlAttack(msg)) {
        *found = true;
        return nullptr;
    }
    *found = false;
    return nullptr;
}

void requestHitReactionToAttacker(const al::SensorMsg* msg, const al::HitSensor* other,
                                  const al::HitSensor* self) {
    al::LiveActor* host = al::getSensorHost(self);
    bool found = false;
    if (const char* hitReaction = tryGetHitReactionForMsg(&found, msg))
        al::startHitReactionHitEffect(host, hitReaction, other, self);
}

void requestHitReactionToAttacker(const al::SensorMsg* msg, const al::HitSensor* other,
                                  const sead::Vector3f& pos) {
    al::LiveActor* host = al::getSensorHost(other);
    bool found = false;
    if (const char* hitReaction = tryGetHitReactionForMsg(&found, msg))
        al::startHitReactionHitEffect(host, hitReaction, pos);
}

bool isCapRethrowReturnOnly(const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgCapRethrow>(pMsg);
    if (!msg)
        return false;
    return msg->getIsReturnOnly();
}

void getPackunEatCancelPosAndFront(sead::Vector3f* pos, sead::Vector3f* front,
                                   const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgPackunEatCancel>(pMsg);
    if (pos)
        pos->set(msg->getPos());
    if (front)
        front->set(msg->getFront());
}

bool isMatchGotogotonGoal(const al::SensorMsg* pMsg, const GotogotonMark* mark) {
    if (!mark)
        return false;
    auto* msg = sead::DynamicCast<const SensorMsgGotogotonGoalMatch>(pMsg);
    if (!msg)
        return false;
    return mark->isMatch(msg->getMark());
}

s32 getMofumofuBodyChainExplodeDelayStep(const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgMofumofuBodyChainExplode>(pMsg);
    return msg->getDelayStep();
}

void calcFishingUpJugemDir(sead::Vector3f* jugemDir, const al::SensorMsg* pMsg) {
    const sead::Vector3f* trans = &sead::Vector3f::zero;
    if (MSG_TYPE_CHECK_(pMsg, FishingUpImmediately)) {
        auto* msg = sead::DynamicCast<const SensorMsgFishingUpImmediately>(pMsg);
        trans = msg->getTrans();
    }
    *jugemDir = *trans - *getFishingUpFloatPos(pMsg);
    jugemDir->y = 0;
    al::tryNormalizeOrZero(jugemDir);
}

const sead::Vector3f* getFishingUpFloatPos(const al::SensorMsg* pMsg) {
    if (!MSG_TYPE_CHECK_(pMsg, FishingUpImmediately))
        return &sead::Vector3f::zero;
    auto* msg = sead::DynamicCast<const SensorMsgFishingUpImmediately>(pMsg);
    return msg->getFloatPos();
}

al::HitSensor* getFishingHookSensor(const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgFishingWait>(pMsg);
    return msg->getHookSensor();
}

// TODO: rename parameter `unk`
bool checkMsgNpcTrampleReactionAll(const al::SensorMsg* msg, const al::HitSensor* other,
                                   const al::HitSensor* self, bool unk) {
    if (isMsgHosuiTrampleReflect(msg))
        return true;
    if (unk && isMsgPlayerObjLeapFrog(msg) && !al::isNearZero(al::getActorVelocity(other)) &&
        al::getActorVelocity(other).dot(al::getActorGravity(self)) >= 0.0f)
        return true;
    if (al::isMsgPlayerObjHipDropReflectAll(msg) || isMsgCapObjHipDropReflect(msg))
        return true;
    if (al::isMsgPlayerTrampleReflectForCrossoverSensor(msg, other, self) &&
        !al::isNearZero(al::getActorVelocity(other)) &&
        al::getActorVelocity(other).dot(al::getActorGravity(self)) >= 0.0f)
        // There isn't a function in this object that this "velocity isn't near zero and the dot
        // product of velocity and gravity is more than or equal to zero" thing could be inlined
        // from, but maybe it should still be an inline function since it's done twice?
        return true;
    return al::isMsgKickStoneTrampleForCrossoverSensor(msg, other, self);
}

const FishingFish* getCheckFishingTarget(const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgCheckFishingTarget>(pMsg);
    return msg->getFish();
}

const sead::Vector3f& getGunetterPushCenter(const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgGunetterPush>(pMsg);
    return msg->getCenter();
}

f32 getGunetterPushRadius(const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgGunetterPush>(pMsg);
    return msg->getRadius();
}

u32 getNumKuriboTowerOn(const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgKuriboTowerOn>(pMsg);
    if (!msg)
        return 0;
    return msg->getNum();
}

void setMsgPlayerLookAtPosition(const al::SensorMsg* pMsg, const sead::Vector3f& pos) {
    auto* msg = sead::DynamicCast<const SensorMsgPlayerLookAtPosition>(pMsg);
    if (!msg)
        return;
    msg->getPos()->set(pos);
}

void setMsgTargetMarkerPosition(const al::SensorMsg* pMsg, const sead::Vector3f& pos) {
    auto* msg = sead::DynamicCast<const SensorMsgTargetMarkerPosition>(pMsg);
    if (!msg)
        return;
    msg->getPos()->set(pos);
}

void tryTreasureBoxPlayerTrampleJump(al::LiveActor* treasureBox, al::HitSensor* receiver,
                                     al::HitSensor* sender, f32 power) {
    if (isPlayerCollidedGround(treasureBox) && !isPlayerOnActor(treasureBox))
        return;
    sendMsgRequestPlayerTrampleJump(receiver, sender, power);
}

bool tryReceivePushAndAddVelocity(al::LiveActor* actor, const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgPushVelocity>(pMsg);
    if (!msg)
        return false;
    al::addVelocity(actor, msg->getVelocity());
    return true;
}

bool tryReceiveMsgPushToPlayerAndAddVelocity(al::LiveActor* actor, const al::SensorMsg* pMsg,
                                             const al::HitSensor* other, const al::HitSensor* self,
                                             f32 power) {
    auto* msg = sead::DynamicCast<const SensorMsgPushToPlayer>(pMsg);
    if (!msg)
        return false;
    al::pushAndAddVelocity(actor, other, self, power);
    return true;
}

bool tryReceiveMsgPushToPlayerAndAddVelocityH(al::LiveActor* actor, const al::SensorMsg* pMsg,
                                              const al::HitSensor* other, const al::HitSensor* self,
                                              f32 power) {
    auto* msg = sead::DynamicCast<const SensorMsgPushToPlayer>(pMsg);
    if (!msg)
        return false;
    al::pushAndAddVelocityH(actor, other, self, power);
    return true;
}

void calcPushTrans(sead::Vector3f* trans, const al::LiveActor* actor, const al::HitSensor* other,
                   const al::HitSensor* self, f32 maxPower) {
    f32 combinedRadius = al::getSensorRadius(self) + al::getSensorRadius(other);
    sead::Vector3f diff = al::getSensorPos(self) - al::getSensorPos(other);
    if (diff.squaredLength() > sead::Mathf::square(combinedRadius)) {
        *trans = {0.0f, 0.0f, 0.0f};
        return;
    }
    sead::Vector3f pushDir = {0.0f, 0.0f, 0.0f};
    f32 powerV = 1.0f;
    if (al::tryNormalizeOrZero(&pushDir, diff)) {
        f32 len = pushDir.cross(al::getGravity(actor)).length();
        powerV = sead::Mathf::clamp(len, 0.1f, 1.0f);
    }
    al::verticalizeVec(&diff, al::getGravity(actor), diff);
    f32 powerH = sead::Mathf::clampMin(combinedRadius * powerV - diff.length(), 0.0f);
    f32 power = sead::Mathf::clampMax(powerH, maxPower);

    if (!al::tryNormalizeOrZero(&diff)) {
        al::calcFrontDir(&diff, actor);
        diff.negate();
    }

    diff *= power;
    trans->set(diff);
}

bool tryReceiveMsgPushToPlayerAndCalcPushTrans(sead::Vector3f* trans, const al::SensorMsg* msg,
                                               const al::LiveActor* actor,
                                               const al::HitSensor* other,
                                               const al::HitSensor* self, f32 power) {
    if (!isMsgPushToPlayer(msg))
        return false;
    calcPushTrans(trans, actor, other, self, power);
    return true;
}

bool tryReceiveMsgPushToHackerAndCalcPushTrans(sead::Vector3f* trans, const al::SensorMsg* msg,
                                               const al::LiveActor* actor,
                                               const al::HitSensor* other,
                                               const al::HitSensor* self, f32 power, bool isYoshi) {
    if (isYoshi) {
        if (!isMsgPushToPlayer(msg))
            return false;
    } else if (!al::isMsgPushAll(msg))
        return false;
    calcPushTrans(trans, actor, other, self, power);
    return true;
}

bool tryReceiveMsgPushToYoshiNpcAndCalcPushTrans(sead::Vector3f* trans, const al::SensorMsg* msg,
                                                 const al::LiveActor* actor,
                                                 const al::HitSensor* other,
                                                 const al::HitSensor* self, f32 power) {
    return tryReceiveMsgPushToGrowPlantAndCalcPushTrans(trans, msg, actor, other, self, power);
}

bool tryReceiveMsgPushToCapAndCalcPushTrans(sead::Vector3f* trans, const al::SensorMsg* msg,
                                            const al::LiveActor* actor, const al::HitSensor* other,
                                            const al::HitSensor* self, f32 power) {
    if (!isMsgPlayerCapPush(msg))
        return false;
    calcPushTrans(trans, actor, other, self, power);
    return true;
}

bool tryReceiveMsgPushToGrowPlantAndCalcPushTrans(sead::Vector3f* trans, const al::SensorMsg* msg,
                                                  const al::LiveActor* actor,
                                                  const al::HitSensor* other,
                                                  const al::HitSensor* self, f32 power) {
    if (!isMsgGrowPlantPush(msg))
        return false;
    calcPushTrans(trans, actor, other, self, power);
    return true;
}

void setCapTargetInfo(const al::SensorMsg* pMsg, const CapTargetInfo* info) {
    auto* msg = sead::DynamicCast<const SensorMsgInitCapTarget>(pMsg);
    if (msg)
        *msg->getInfo() = info;
}

bool tryReceiveMsgInitCapTargetAndSetCapTargetInfo(const al::SensorMsg* msg,
                                                   const CapTargetInfo* info) {
    if (!isMsgInitCapTarget(msg))
        return false;
    setCapTargetInfo(msg, info);
    return true;
}

bool tryReceiveMsgNpcScareByEnemyIgnoreTargetHack(const al::SensorMsg* pMsg,
                                                  const CapTargetInfo* info) {
    if (!isMsgNpcScareByEnemy(pMsg))
        return false;
    auto* msg = sead::DynamicCast<const SensorMsgNpcScareByEnemy>(pMsg);
    if (msg && (msg->getUnk() < 1 || !EventFlowFunction::isCapTargetHackType(info, msg->getUnk())))
        return true;
    return false;
}

bool tryIncrementComboCount(s32* count, const al::SensorMsg* msg) {
    al::ComboCounter* counter = getMsgComboCount(msg);
    if (!counter)
        return false;
    counter->increment();
    *count = counter->getCount();
    return true;
}

bool isRideOn(const al::HitSensor* other, const al::HitSensor* self) {
    sead::Vector3f dir = al::getSensorPos(self) - al::getSensorPos(other);
    if (!al::tryNormalizeOrZero(&dir))
        return false;
    if (dir.dot(al::getActorGravity(self)) < 0.342020153f)  // cos(70°)
        return false;
    sead::Vector3f velocityDir = al::getActorVelocity(other);
    return al::tryNormalizeOrZero(&velocityDir) && !(velocityDir.y > 0.0f);
}

bool isVisibleChameleon(const al::SensorMsg* msg) {
    return isMsgDonsukeGroundAttack(msg) || al::isMsgPlayerHipDropKnockDown(msg);
}

void setKuriboTowerNum(const al::SensorMsg* pMsg, s32 num) {
    auto* msg = sead::DynamicCast<const SensorMsgKuriboTowerNum>(pMsg);
    *msg->getNumPtr() = num;
}

const sead::Vector3f& getSpherePushCenter(const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgSpherePush>(pMsg);
    return msg->getCenter();
}

f32 getSpherePushRadius(const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgSpherePush>(pMsg);
    return msg->getRadius();
}

DigPoint* getSmellDigPoint(const al::SensorMsg* pMsg) {
    auto* msg = sead::DynamicCast<const SensorMsgDigPointSmell>(pMsg);
    return msg->getDigPoint();
}

}  // namespace rs
