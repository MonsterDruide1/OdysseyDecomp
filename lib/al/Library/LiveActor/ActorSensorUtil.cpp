#include "Library/LiveActor/ActorSensorUtil.h"

#include "Library/HitSensor/SensorMsgSetupUtil.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorSensorFunction.h"
#include "math/seadVectorFwd.h"

namespace al {

// Create the SensorMsg classes

SENSOR_MSG_CBC(PlayerAttackTrample);
SENSOR_MSG_CBC(PlayerTrampleReflect);
SENSOR_MSG_CBC(PlayerAttackHipDrop);
SENSOR_MSG_CBC(PlayerAttackObjHipDrop);
SENSOR_MSG_CBC(PlayerAttackObjHipDropReflect);
SENSOR_MSG_CBC(PlayerAttackObjHipDropHighJump);
SENSOR_MSG(PlayerAttackHipDropKnockDown);
SENSOR_MSG_CBC(PlayerAttackStatueDrop);
SENSOR_MSG_CBC(PlayerAttackObjStatueDrop);
SENSOR_MSG_CBC(PlayerAttackObjStatueDropReflect);
SENSOR_MSG(PlayerAttackObjStatueDropReflectNoCondition);
SENSOR_MSG(PlayerAttackStatueTouch);
SENSOR_MSG(PlayerAttackUpperPunch);
SENSOR_MSG(PlayerAttackObjUpperPunch);
SENSOR_MSG(PlayerAttackRollingAttack);
SENSOR_MSG(PlayerAttackRollingReflect);
SENSOR_MSG(PlayerAttackObjRollingAttack);
SENSOR_MSG(PlayerAttackObjRollingAttackFailure);
SENSOR_MSG_CBC(PlayerAttackInvincibleAttack);
SENSOR_MSG(PlayerAttackFireBallAttack);
SENSOR_MSG(PlayerAttackRouteDokanFireBallAttack);
SENSOR_MSG_CBC(PlayerAttackTailAttack);
SENSOR_MSG(PlayerAttackKick);
SENSOR_MSG(PlayerAttackCatch);
SENSOR_MSG_CBC(PlayerAttackSlidingAttack);
SENSOR_MSG_CBC(PlayerAttackBoomerangAttack);
SENSOR_MSG(PlayerAttackBoomerangAttackCollide);
SENSOR_MSG(PlayerAttackBoomerangReflect);
SENSOR_MSG(PlayerAttackBoomerangBreak);
SENSOR_MSG_CBC(PlayerAttackBodyAttack);
SENSOR_MSG_CBC(PlayerAttackBodyLanding);
SENSOR_MSG_CBC(PlayerAttackBodyAttackReflect);
SENSOR_MSG_CBC(PlayerAttackClimbAttack);
SENSOR_MSG_CBC(PlayerAttackClimbSliding);
SENSOR_MSG_CBC(PlayerAttackClimbRolling);
SENSOR_MSG_CBC(PlayerAttackSpinAttack);
SENSOR_MSG_CBC(PlayerAttackGiant);

SENSOR_MSG_CBC(PlayerCooperationHipDrop);
SENSOR_MSG_CBC(PlayerGiantHipDrop);
SENSOR_MSG(PlayerDisregard);
SENSOR_MSG(PlayerDamageTouch);
SENSOR_MSG(PlayerFloorTouchBind);  // This msg is referenced by al::isMsgFloorTouchBind, but doesn't
                                   // appear in the executable because it's never used

SENSOR_MSG(PlayerFloorTouch);
SENSOR_MSG(PlayerTouch);
SENSOR_MSG_CBC(PlayerInvincibleTouch);
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
                     (const char*, FireMaterialCode));  // Usually null, sometimes "LavaRed"
SENSOR_MSG(EnemyAttackKnockDown);
SENSOR_MSG(EnemyAttackBoomerang);
SENSOR_MSG(EnemyAttackNeedle);
SENSOR_MSG(EnemyFloorTouch);
SENSOR_MSG(EnemyItemGet);
SENSOR_MSG(EnemyRouteDokanAttack);
SENSOR_MSG(EnemyRouteDokanFire);
SENSOR_MSG_CBC(Explosion);
SENSOR_MSG_CBC(ExplosionCollide);
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
//Impulse
SENSOR_MSG(EnemyTouch);
SENSOR_MSG(EnemyUpperPunch);
SENSOR_MSG(EnemyTrample);
SENSOR_MSG(MapObjTrample);
SENSOR_MSG(NeedleBallAttack);
SENSOR_MSG(PunpunFloorTouch);
SENSOR_MSG(InvalidateFootPrint);
SENSOR_MSG_CBC(KickKouraAttack);
SENSOR_MSG_CBC(KickKouraAttackCollide);
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
SENSOR_MSG_CBC(BallAttack);
SENSOR_MSG_CBC(BallRouteDokanAttack);
SENSOR_MSG(BallAttackHold);
SENSOR_MSG(BallAttackDRCHold);
SENSOR_MSG(BallAttackCollide);
SENSOR_MSG_CBC(BallTrample);
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
SENSOR_MSG_CBC(BlockUpperPunch);
SENSOR_MSG_CBC(BlockLowerPunch);
SENSOR_MSG(BlockItemGet);
SENSOR_MSG_CBC(PlayerKouraAttack);
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
                     (HitSensor*, Sender));
SENSOR_MSG_WITH_DATA(StringVoidPtr, (const char*, Str), (void*, Ptr));

// SensorMsg related function implementations

SEND_MSG_IMPL(Push);
SEND_MSG_IMPL(PushStrong);
SEND_MSG_IMPL(PushVeryStrong);

SEND_MSG_CBC_IMPL(PlayerAttackTrample);
SEND_MSG_CBC_IMPL(PlayerTrampleReflect);
SEND_MSG_CBC_IMPL_(PlayerHipDrop, PlayerAttackHipDrop);
SEND_MSG_CBC_IMPL_(PlayerObjHipDrop, PlayerAttackObjHipDrop);
SEND_MSG_CBC_IMPL_(PlayerObjHipDropReflect, PlayerAttackObjHipDropReflect);
SEND_MSG_CBC_IMPL_(PlayerObjHipDropHighJump, PlayerAttackObjHipDropHighJump);
SEND_MSG_IMPL_(PlayerHipDropKnockDown, PlayerAttackHipDropKnockDown);
SEND_MSG_CBC_IMPL_(PlayerStatueDrop, PlayerAttackStatueDrop);
SEND_MSG_CBC_IMPL_(PlayerObjStatueDrop, PlayerAttackObjStatueDrop);
SEND_MSG_CBC_IMPL_(PlayerObjStatueDropReflect, PlayerAttackObjStatueDropReflect);
SEND_MSG_IMPL_(PlayerObjStatueDropReflectNoCondition, PlayerAttackObjStatueDropReflectNoCondition);
SEND_MSG_IMPL_(PlayerStatueTouch, PlayerAttackStatueTouch);
SEND_MSG_IMPL_(PlayerUpperPunch, PlayerAttackUpperPunch);
SEND_MSG_IMPL_(PlayerObjUpperPunch, PlayerAttackObjUpperPunch);
SEND_MSG_IMPL_(PlayerRollingAttack, PlayerAttackRollingAttack);
SEND_MSG_IMPL_(PlayerRollingReflect, PlayerAttackRollingReflect);
SEND_MSG_IMPL_(PlayerObjRollingAttack, PlayerAttackObjRollingAttack);
SEND_MSG_IMPL_(PlayerObjRollingAttackFailure, PlayerAttackObjRollingAttackFailure);
SEND_MSG_CBC_IMPL_(PlayerInvincibleAttack, PlayerAttackInvincibleAttack);
SEND_MSG_IMPL_(PlayerFireBallAttack, PlayerAttackFireBallAttack);
SEND_MSG_IMPL_(PlayerRouteDokanFireBallAttack, PlayerAttackRouteDokanFireBallAttack);
SEND_MSG_CBC_IMPL_(PlayerTailAttack, PlayerAttackTailAttack);
SEND_MSG_IMPL_(PlayerKick, PlayerAttackKick);
SEND_MSG_IMPL_(PlayerCatch, PlayerAttackCatch);
SEND_MSG_CBC_IMPL_(PlayerSlidingAttack, PlayerAttackSlidingAttack);
SEND_MSG_CBC_IMPL_(PlayerBoomerangAttack, PlayerAttackBoomerangAttack);
SEND_MSG_IMPL_(PlayerBoomerangAttackCollide, PlayerAttackBoomerangAttackCollide);
SEND_MSG_IMPL_(PlayerBoomerangReflect, PlayerAttackBoomerangReflect);
SEND_MSG_IMPL_(PlayerBoomerangBreak, PlayerAttackBoomerangBreak);
SEND_MSG_CBC_IMPL_(PlayerBodyAttack, PlayerAttackBodyAttack);
SEND_MSG_CBC_IMPL_(PlayerBodyLanding, PlayerAttackBodyLanding);
SEND_MSG_CBC_IMPL_(PlayerBodyAttackReflect, PlayerAttackBodyAttackReflect);
SEND_MSG_CBC_IMPL_(PlayerClimbAttack, PlayerAttackClimbAttack);
SEND_MSG_CBC_IMPL_(PlayerClimbSlidingAttack, PlayerAttackClimbSliding);
SEND_MSG_CBC_IMPL_(PlayerClimbRollingAttack, PlayerAttackClimbRolling);
SEND_MSG_CBC_IMPL_(PlayerSpinAttack, PlayerAttackSpinAttack);
SEND_MSG_CBC_IMPL_(PlayerGiantAttack, PlayerAttackGiant);

SEND_MSG_CBC_IMPL(PlayerCooperationHipDrop);
SEND_MSG_CBC_IMPL(PlayerGiantHipDrop);
SEND_MSG_IMPL(PlayerDisregard);
SEND_MSG_IMPL(PlayerDamageTouch);
IS_MSG_MULTIPLE_IMPL(FloorTouch, PlayerFloorTouch, EnemyFloorTouch);
SEND_MSG_IMPL(PlayerFloorTouch);
SEND_MSG_IMPL(EnemyFloorTouch);
IS_MSG_MULTIPLE_IMPL(UpperPunch, PlayerAttackUpperPunch, EnemyUpperPunch);
SEND_MSG_IMPL(EnemyUpperPunch);
SEND_MSG_IMPL(PlayerTouch);
SEND_MSG_CBC_IMPL(PlayerInvincibleTouch);
SEND_MSG_IMPL(PlayerPutOnEquipment);
SEND_MSG_IMPL(PlayerReleaseEquipment);
SEND_MSG_DATA_IMPL(PlayerReleaseEquipmentGoal, u32);
SEND_MSG_IMPL(PlayerCarryFront);
SEND_MSG_IMPL(PlayerCarryFrontWallKeep);
SEND_MSG_IMPL(PlayerCarryUp);
SEND_MSG_IMPL(PlayerCarryKeepDemo);
SEND_MSG_IMPL(PlayerCarryWarp);
SEND_MSG_IMPL(PlayerLeave);
SEND_MSG_IMPL(PlayerToss);
SEND_MSG_IMPL(PlayerRelease);
SEND_MSG_IMPL(PlayerReleaseBySwing);
SEND_MSG_IMPL(PlayerReleaseDead);
SEND_MSG_IMPL(PlayerReleaseDamage);
SEND_MSG_IMPL(PlayerReleaseDemo);

SEND_MSG_IMPL(KillerItemGet);
SEND_MSG_IMPL(PlayerItemGet);
SEND_MSG_IMPL(RideAllPlayerItemGet);

SEND_MSG_IMPL(EnemyAttack);
SEND_MSG_DATA_IMPL(EnemyAttackFire, const char*);
SEND_MSG_IMPL(EnemyAttackKnockDown);
SEND_MSG_IMPL(EnemyAttackBoomerang);
SEND_MSG_IMPL(EnemyAttackNeedle);
SEND_MSG_IMPL(EnemyItemGet);
SEND_MSG_IMPL(EnemyRouteDokanAttack);
SEND_MSG_IMPL(EnemyRouteDokanFire);
SEND_MSG_CBC_IMPL(Explosion);
SEND_MSG_CBC_IMPL(ExplosionCollide);
SEND_MSG_IMPL(BindStart);
SEND_MSG_DATA_IMPL(BindInit, u32);
SEND_MSG_IMPL(BindEnd);
SEND_MSG_IMPL(BindCancel);
SEND_MSG_IMPL(BindCancelByDemo);
SEND_MSG_IMPL(BindDamage);
SEND_MSG_IMPL(BindSteal);
SEND_MSG_IMPL(BindGiant);
SEND_MSG_IMPL(PressureDeath);
SEND_MSG_IMPL(NpcTouch);
SEND_MSG_IMPL(Hit);
SEND_MSG_IMPL(HitStrong);
SEND_MSG_IMPL(HitVeryStrong);
SEND_MSG_IMPL(KnockDown);
SEND_MSG_IMPL(MapPush);
SEND_MSG_IMPL(Vanish);
SEND_MSG_DATA_TO_ACTOR_IMPL(ChangeAlpha, f32);
SEND_MSG_IMPL(ShowModel);
SEND_MSG_IMPL(HideModel);
SEND_MSG_IMPL(Restart);
SEND_MSG_IMPL(EnemyTouch);
SEND_MSG_IMPL(EnemyTrample);
SEND_MSG_IMPL(MapObjTrample);
SEND_MSG_IMPL(NeedleBallAttack);
SEND_MSG_IMPL(PunpunFloorTouch);
SEND_MSG_IMPL(InvalidateFootPrint);
SEND_MSG_CBC_IMPL(KickKouraAttack);
SEND_MSG_CBC_IMPL(KickKouraAttackCollide);
SEND_MSG_IMPL(KickKouraGetItem);
SEND_MSG_IMPL(KickKouraReflect);
SEND_MSG_IMPL(KickKouraCollideNoReflect);
SEND_MSG_IMPL(KickKouraBreak);
SEND_MSG_IMPL(KickKouraBlow);
SEND_MSG_IMPL(KickStoneAttack);
SEND_MSG_IMPL(KickStoneAttackCollide);
SEND_MSG_IMPL(KickStoneAttackHold);
SEND_MSG_IMPL(KickStoneAttackReflect);
SEND_MSG_IMPL(KickStoneTrample);
SEND_MSG_IMPL(KillerAttack);
SEND_MSG_IMPL(LiftGeyser);
SEND_MSG_IMPL(WarpStart);
SEND_MSG_IMPL(WarpEnd);
SEND_MSG_IMPL(HoleIn);
SEND_MSG_IMPL(HoldCancel);
SEND_MSG_IMPL(JumpInhibit);
SEND_MSG_IMPL(GoalKill);
SEND_MSG_IMPL(Goal);
SEND_MSG_CBC_IMPL(BallAttack);
SEND_MSG_CBC_IMPL(BallRouteDokanAttack);
SEND_MSG_IMPL(BallAttackHold);
SEND_MSG_IMPL(BallAttackDRCHold);
SEND_MSG_IMPL(BallAttackCollide);
SEND_MSG_CBC_IMPL(BallTrample);
SEND_MSG_IMPL(BallTrampleCollide);
SEND_MSG_IMPL(BallItemGet);
SEND_MSG_IMPL_(FireBalCollide, FireBallCollide);  // Nintendo made a funny typo here
SEND_MSG_IMPL(FireBallFloorTouch);
SEND_MSG_IMPL(DokanBazookaAttack);

SEND_MSG_TO_ACTOR_IMPL(SwitchOn);
SEND_MSG_TO_ACTOR_IMPL(SwitchOnInit);
SEND_MSG_TO_ACTOR_IMPL(SwitchOffInit);
SEND_MSG_TO_ACTOR_IMPL(SwitchKillOn);
SEND_MSG_TO_ACTOR_IMPL(SwitchKillOnInit);
SEND_MSG_TO_ACTOR_IMPL(SwitchKillOffInit);

bool sendMsgPlayerFloorTouchToColliderGround(LiveActor* receiver, HitSensor* sender){
    HitSensor* collidedSensor = tryGetCollidedGroundSensor(receiver);
    return alActorSensorFunction::sendMsgSensorToSensor(SensorMsgPlayerFloorTouch(), sender, collidedSensor);
}

bool sendMsgPlayerUpperPunchToColliderCeiling(LiveActor* receiver, HitSensor* sender){
    HitSensor* collidedSensor = tryGetCollidedCeilingSensor(receiver);
    return alActorSensorFunction::sendMsgSensorToSensor(SensorMsgPlayerAttackUpperPunch(), sender, collidedSensor);
}

bool sendMsgEnemyFloorTouchToColliderGround(LiveActor* receiver, HitSensor* sender){
    HitSensor* collidedSensor = tryGetCollidedGroundSensor(receiver);
    return alActorSensorFunction::sendMsgSensorToSensor(SensorMsgEnemyFloorTouch(), sender, collidedSensor);
}

bool sendMsgEnemyUpperPunchToColliderCeiling(LiveActor* receiver, HitSensor* sender){
    HitSensor* collidedSensor = tryGetCollidedCeilingSensor(receiver);
    return alActorSensorFunction::sendMsgSensorToSensor(SensorMsgEnemyUpperPunch(), sender, collidedSensor);
}

bool sendMsgAskSafetyPointToColliderGround(LiveActor* receiver, HitSensor* sender,
                                           sead::Vector3f** safetyPointAccessor){
    HitSensor* collidedSensor = tryGetCollidedGroundSensor(receiver);
    return alActorSensorFunction::sendMsgSensorToSensor(SensorMsgAskSafetyPoint(safetyPointAccessor), sender, collidedSensor);
}

SEND_MSG_DATA_IMPL(AskSafetyPoint, sead::Vector3f**);
SEND_MSG_IMPL(TouchAssist);
SEND_MSG_IMPL(TouchAssistTrig);
SEND_MSG_IMPL(TouchStroke);
SEND_MSG_IMPL(IsNerveSupportFreeze);
SEND_MSG_IMPL(OnSyncSupportFreeze);
SEND_MSG_IMPL(OffSyncSupportFreeze);
SEND_MSG_IMPL(ScreenPointInvalidCollisionParts);
SEND_MSG_CBC_IMPL(BlockUpperPunch);
SEND_MSG_CBC_IMPL(BlockLowerPunch);
SEND_MSG_IMPL(BlockItemGet);
SEND_MSG_CBC_IMPL(PlayerKouraAttack);
SEND_MSG_IMPL(LightFlash);
SEND_MSG_IMPL(ForceAbyss);
SEND_MSG_IMPL(SwordAttackHighLeft);
SEND_MSG_IMPL(SwordAttackHighRight);
SEND_MSG_IMPL(SwordAttackLowLeft);
SEND_MSG_IMPL(SwordAttackLowRight);
SEND_MSG_IMPL(SwordBeamAttack);
SEND_MSG_IMPL(SwordBeamReflectAttack);
SEND_MSG_IMPL(SwordAttackJumpUnder);
SEND_MSG_IMPL(ShieldGuard);
SEND_MSG_IMPL(AskMultiPlayerEnemy);
SEND_MSG_IMPL(ItemGettable);
SEND_MSG_IMPL(KikkiThrow);
SEND_MSG_IMPL(IsKikkiThrowTarget);
SEND_MSG_IMPL(PlayerCloudGet);
SEND_MSG_IMPL(AutoJump);
SEND_MSG_IMPL(PlayerTouchShadow);
SEND_MSG_IMPL(PlayerPullOutShadow);
SEND_MSG_IMPL(PlayerAttackShadow);
SEND_MSG_IMPL(PlayerAttackShadowStrong);
SEND_MSG_DATA_IMPL(PlayerAttackChangePos, sead::Vector3f*);
SEND_MSG_IMPL(AtmosOnlineLight);
SEND_MSG_IMPL(LightBurn);
SEND_MSG_IMPL(MoonLightBurn);

SEND_MSG_DATA_IMPL(String, const char*);
SEND_MSG_DATA_MULTI_IMPL(StringV4fPtr, const char*, (sead::Vector4f*, Vec));

bool sendMsgStringV4fSensorPtr(HitSensor* receiver, HitSensor* sender, const char* str,
                               sead::Vector4f* vec) {
    SensorMsgStringV4fSensorPtr msg(str, vec, sender);
    return alActorSensorFunction::sendMsgSensorToSensor(msg, sender, receiver);
}

SEND_MSG_DATA_MULTI_IMPL(StringVoidPtr, const char*, (void*, Ptr));

SEND_MSG_TO_ACTOR_IMPL(HideModel);
SEND_MSG_TO_ACTOR_IMPL(ShowModel);
SEND_MSG_TO_ACTOR_IMPL(Restart);

bool sendMsgPlayerReflectOrTrample(HitSensor* receiver, HitSensor* sender,
                                  ComboCounter* comboCounter) {
    //return A || B mismatches, but this matches
    if(alActorSensorFunction::sendMsgSensorToSensor(SensorMsgPlayerTrampleReflect(comboCounter), sender, receiver))
        return true;
    return alActorSensorFunction::sendMsgSensorToSensor(SensorMsgPlayerAttackTrample(comboCounter), sender, receiver);
}

IS_MSG_IMPL_(PlayerTrample, PlayerAttackTrample);
IS_MSG_IMPL(PlayerTrampleReflect);
IS_MSG_IMPL_(PlayerObjHipDropHighJump, PlayerAttackObjHipDropHighJump);
IS_MSG_IMPL_(PlayerHipDropKnockDown, PlayerAttackHipDropKnockDown);
IS_MSG_IMPL_(PlayerStatueDrop, PlayerAttackStatueDrop);
IS_MSG_IMPL_(PlayerObjStatueDrop, PlayerAttackObjStatueDrop);
IS_MSG_IMPL_(PlayerObjStatueDropReflect, PlayerAttackObjStatueDropReflect);
IS_MSG_IMPL_(PlayerObjStatueDropReflectNoCondition, PlayerAttackObjStatueDropReflectNoCondition);
IS_MSG_IMPL_(PlayerStatueTouch, PlayerAttackStatueTouch);
IS_MSG_IMPL_(PlayerObjUpperPunch, PlayerAttackObjUpperPunch);
IS_MSG_IMPL_(PlayerUpperPunch, PlayerAttackUpperPunch);
IS_MSG_IMPL_(PlayerRollingAttack, PlayerAttackRollingAttack);
IS_MSG_IMPL_(PlayerRollingReflect, PlayerAttackRollingReflect);
IS_MSG_IMPL_(PlayerObjRollingAttack, PlayerAttackObjRollingAttack);
IS_MSG_IMPL_(PlayerObjRollingAttackFailure, PlayerAttackObjRollingAttackFailure);
IS_MSG_IMPL_(PlayerInvincibleAttack, PlayerAttackInvincibleAttack);
IS_MSG_IMPL_(PlayerFireBallAttack, PlayerAttackFireBallAttack);
IS_MSG_IMPL_(PlayerRouteDokanFireBallAttack, PlayerAttackRouteDokanFireBallAttack);
IS_MSG_IMPL_(PlayerTailAttack, PlayerAttackTailAttack);
IS_MSG_IMPL_(PlayerKick, PlayerAttackKick);
IS_MSG_IMPL_(PlayerCatch, PlayerAttackCatch);
IS_MSG_IMPL_(PlayerSlidingAttack, PlayerAttackSlidingAttack);
IS_MSG_IMPL_(PlayerBoomerangAttack, PlayerAttackBoomerangAttack);
IS_MSG_IMPL_(PlayerBoomerangAttackCollide, PlayerAttackBoomerangAttackCollide);
IS_MSG_IMPL_(PlayerBoomerangReflect, PlayerAttackBoomerangReflect);
IS_MSG_IMPL_(PlayerBoomerangBreak, PlayerAttackBoomerangBreak);
IS_MSG_IMPL_(PlayerBodyAttack, PlayerAttackBodyAttack);
IS_MSG_IMPL_(PlayerBodyLanding, PlayerAttackBodyLanding);
IS_MSG_IMPL_(PlayerBodyAttackReflect, PlayerAttackBodyAttackReflect);
IS_MSG_IMPL_(PlayerClimbAttack, PlayerAttackClimbAttack);
IS_MSG_IMPL_(PlayerClimbSlidingAttack, PlayerAttackClimbSliding);
IS_MSG_IMPL_(PlayerClimbRollingAttack, PlayerAttackClimbRolling);
IS_MSG_IMPL_(PlayerSpinAttack, PlayerAttackSpinAttack);
IS_MSG_IMPL_(PlayerGiantAttack, PlayerAttackGiant);

IS_MSG_IMPL(PlayerCooperationHipDrop);
IS_MSG_IMPL(PlayerGiantHipDrop);
IS_MSG_IMPL(PlayerDisregard);
IS_MSG_IMPL_(PlayerDash, PlayerAttackDash);
IS_MSG_IMPL(PlayerDamageTouch);
IS_MSG_IMPL(PlayerFloorTouchBind);
IS_MSG_IMPL(PlayerFloorTouch);
IS_MSG_IMPL(EnemyFloorTouch);
IS_MSG_IMPL(PlayerTouch);
IS_MSG_IMPL(PlayerInvincibleTouch);
IS_MSG_IMPL(PlayerGiantTouch);
IS_MSG_IMPL_(PlayerObjTouch, PlayerItemGet);
IS_MSG_IMPL(PlayerPutOnEquipment);
IS_MSG_IMPL(PlayerReleaseEquipment);
IS_MSG_IMPL(PlayerReleaseEquipmentGoal);
IS_MSG_IMPL(PlayerCarryFront);
IS_MSG_IMPL(PlayerCarryFrontWallKeep);
IS_MSG_IMPL(PlayerCarryUp);
IS_MSG_IMPL(PlayerCarryKeepDemo);
IS_MSG_IMPL(PlayerCarryWarp);
IS_MSG_IMPL(PlayerLeave);
IS_MSG_IMPL(PlayerToss);
IS_MSG_IMPL(PlayerRelease);
IS_MSG_IMPL(PlayerReleaseBySwing);
IS_MSG_IMPL(PlayerReleaseDead);
IS_MSG_IMPL(PlayerReleaseDamage);
IS_MSG_IMPL(PlayerReleaseDemo);

IS_MSG_IMPL(KillerItemGet);
IS_MSG_IMPL(PlayerItemGet);
IS_MSG_IMPL(RideAllPlayerItemGet);

IS_MSG_IMPL(EnemyAttackFire);
IS_MSG_IMPL(EnemyAttackKnockDown);
IS_MSG_IMPL(EnemyAttackBoomerang);
IS_MSG_IMPL(EnemyAttackNeedle);
IS_MSG_IMPL(EnemyItemGet);
IS_MSG_IMPL(EnemyRouteDokanAttack);
IS_MSG_IMPL(EnemyRouteDokanFire);
IS_MSG_IMPL(Explosion);
IS_MSG_IMPL(ExplosionCollide);
IS_MSG_IMPL(BindStart);
IS_MSG_IMPL(BindInit);
IS_MSG_IMPL(BindEnd);
IS_MSG_IMPL(BindCancel);
IS_MSG_IMPL(BindCancelByDemo);
IS_MSG_IMPL(BindDamage);
IS_MSG_IMPL(BindSteal);
IS_MSG_IMPL(BindGiant);
IS_MSG_IMPL(PressureDeath);
IS_MSG_IMPL(NpcTouch);
IS_MSG_IMPL(Hit);
IS_MSG_IMPL(HitStrong);
IS_MSG_IMPL(HitVeryStrong);
IS_MSG_IMPL(KnockDown);
IS_MSG_IMPL(MapPush);
IS_MSG_IMPL(Vanish);
IS_MSG_IMPL(ChangeAlpha);
IS_MSG_IMPL(ShowModel);
IS_MSG_IMPL(HideModel);
IS_MSG_IMPL(Restart);
IS_MSG_IMPL(EnemyTouch);
IS_MSG_IMPL(EnemyUpperPunch);
IS_MSG_IMPL(EnemyTrample);
IS_MSG_IMPL(MapObjTrample);
IS_MSG_IMPL(NeedleBallAttack);
IS_MSG_IMPL(PunpunFloorTouch);
IS_MSG_IMPL(InvalidateFootPrint);
IS_MSG_IMPL(KickKouraAttack);
IS_MSG_IMPL(KickKouraAttackCollide);
IS_MSG_IMPL_(KickKouraItemGet, KickKouraGetItem);
IS_MSG_IMPL(KickKouraReflect);
IS_MSG_IMPL(KickKouraCollideNoReflect);
IS_MSG_IMPL(KickKouraBreak);
IS_MSG_IMPL(KickKouraBlow);
IS_MSG_IMPL(KickStoneAttack);
IS_MSG_IMPL(KickStoneAttackCollide);
IS_MSG_IMPL(KickStoneAttackHold);
IS_MSG_IMPL(KickStoneAttackReflect);
IS_MSG_IMPL(KickStoneTrample);
IS_MSG_IMPL(KillerAttack);
IS_MSG_IMPL(LiftGeyser);
IS_MSG_IMPL(WarpStart);
IS_MSG_IMPL(WarpEnd);
IS_MSG_IMPL(HoleIn);
IS_MSG_IMPL(HoldCancel);
IS_MSG_IMPL(JumpInhibit);
IS_MSG_IMPL(GoalKill);
IS_MSG_IMPL(Goal);
IS_MSG_IMPL(BallAttack);
IS_MSG_IMPL(BallRouteDokanAttack);
IS_MSG_IMPL(BallAttackHold);
IS_MSG_IMPL(BallAttackDRCHold);
IS_MSG_IMPL(BallAttackCollide);
IS_MSG_IMPL(BallTrample);
IS_MSG_IMPL(BallTrampleCollide);
IS_MSG_IMPL(BallItemGet);
IS_MSG_IMPL(FireBallCollide);
IS_MSG_IMPL(FireBallFloorTouch);
IS_MSG_IMPL(DokanBazookaAttack);
IS_MSG_IMPL(TouchAssistNoPat);
IS_MSG_IMPL(TouchAssistTrig);
IS_MSG_IMPL(TouchAssistTrigOff);
IS_MSG_IMPL(TouchAssistTrigNoPat);
IS_MSG_IMPL(TouchAssistBurn);

IS_MSG_IMPL(SwitchOn);
IS_MSG_IMPL(SwitchOnInit);
IS_MSG_IMPL(SwitchOffInit);
IS_MSG_IMPL(SwitchKillOn);
IS_MSG_IMPL(SwitchKillOnInit);
IS_MSG_IMPL(SwitchKillOffInit);

IS_MSG_IMPL(AskSafetyPoint);
IS_MSG_IMPL(TouchAssist);
IS_MSG_IMPL(TouchStroke);
IS_MSG_IMPL(IsNerveSupportFreeze);
IS_MSG_IMPL(OnSyncSupportFreeze);
IS_MSG_IMPL(OffSyncSupportFreeze);
IS_MSG_IMPL(ScreenPointInvalidCollisionParts);
IS_MSG_IMPL(BlockUpperPunch);
IS_MSG_IMPL(BlockLowerPunch);
IS_MSG_IMPL(BlockItemGet);
IS_MSG_IMPL(PlayerKouraAttack);
IS_MSG_IMPL(LightFlash);
IS_MSG_IMPL(ForceAbyss);
IS_MSG_MULTIPLE_IMPL(SwordAttackHigh, SwordAttackHighLeft, SwordAttackHighRight);
IS_MSG_IMPL(SwordAttackHighLeft);
IS_MSG_IMPL(SwordAttackHighRight);
IS_MSG_MULTIPLE_IMPL(SwordAttackLow, SwordAttackLowLeft, SwordAttackLowRight);
IS_MSG_IMPL(SwordAttackLowLeft);
IS_MSG_IMPL(SwordAttackLowRight);
IS_MSG_IMPL(SwordBeamReflectAttack);
IS_MSG_IMPL(SwordAttackJumpUnder);
IS_MSG_IMPL(ShieldGuard);
IS_MSG_IMPL(AskMultiPlayerEnemy);
IS_MSG_IMPL(ItemGettable);
IS_MSG_IMPL(KikkiThrow);
IS_MSG_IMPL(IsKikkiThrowTarget);
IS_MSG_IMPL(PlayerCloudGet);
IS_MSG_IMPL(AutoJump);
IS_MSG_IMPL(PlayerTouchShadow);
IS_MSG_IMPL(PlayerPullOutShadow);
IS_MSG_IMPL(PlayerAttackShadow);
IS_MSG_IMPL(PlayerAttackShadowStrong);
IS_MSG_IMPL(PlayerAttackChangePos);
IS_MSG_IMPL(AtmosOnlineLight);
IS_MSG_IMPL(LightBurn);
IS_MSG_IMPL(MoonLightBurn);

IS_MSG_IMPL(String);
IS_MSG_IMPL(StringV4fPtr);
IS_MSG_IMPL(StringV4fSensorPtr);
IS_MSG_IMPL(StringVoidPtr);

IS_MSG_MULTIPLE_IMPL(PushAll, Push, PushStrong, PushVeryStrong);
IS_MSG_IMPL(Push);
IS_MSG_IMPL(PushStrong);
IS_MSG_IMPL(PushVeryStrong);
IS_MSG_MULTIPLE_IMPL(SwordBeamAttack, SwordBeamAttack, SwordBeamReflectAttack);
IS_MSG_MULTIPLE_IMPL(HoldReleaseAll, HoldCancel, PlayerRelease, PlayerReleaseBySwing,
                     PlayerReleaseDead, PlayerReleaseDamage, PlayerReleaseDemo);
IS_MSG_MULTIPLE_IMPL(ItemGetDirectAll, PlayerItemGet, RideAllPlayerItemGet, PlayerAttackTailAttack);
IS_MSG_MULTIPLE_IMPL(ItemGetByObjAll, BallItemGet, KickKouraGetItem, KillerItemGet);

//This could probably also be implemented with the IS_MSG_MULTIPLE_IMPL macro, but that would need to be given all messages checked by both of these
bool isMsgItemGetAll(const SensorMsg* msg) {
    return isMsgItemGetDirectAll(msg) || isMsgItemGetByObjAll(msg);
}

IS_MSG_MULTIPLE_IMPL(PlayerHipDropAll, PlayerAttackHipDrop, PlayerAttackStatueDrop);
IS_MSG_MULTIPLE_IMPL(PlayerObjHipDropAll, PlayerAttackObjHipDrop, PlayerAttackObjStatueDrop);
IS_MSG_MULTIPLE_IMPL(PlayerObjHipDropReflectAll, PlayerAttackObjHipDropReflect,
                     PlayerAttackObjStatueDropReflect);

IS_MSG_MULTIPLE_IMPL(TouchAssistAll, TouchAssist, TouchAssistTrig);
IS_MSG_IMPL(TouchCarryItem);
IS_MSG_IMPL(TouchReleaseItem);
IS_MSG_MULTIPLE_IMPL(EnemyAttack, EnemyAttack, EnemyAttackFire, EnemyAttackKnockDown);

SENSOR_MSG_WITH_DATA_CUSTOM_CTOR(CollidePush, (sead::Vector3f, Vec))
    inline SensorMsgCollidePush(const sead::Vector3f& pVec){
        mVec.set(pVec);
    }
};

SEND_MSG_DATA_IMPL(CollidePush, const sead::Vector3f&);

SENSOR_MSG_WITH_DATA_CUSTOM_CTOR(CollisionImpulse, (sead::Vector3f*, VecPtr), (const sead::Vector3f*, ConstVec),
                     (f32, FloatVal), (const sead::Vector3f*, ConstVec2), (f32, FloatVal2))
    inline SensorMsgCollisionImpulse(sead::Vector3f* pVecPtr,  const sead::Vector3f& pVecRef, f32 pFloatVal, const sead::Vector3f& pVecRef2, f32 pFloatVal2){
        mVecPtr = pVecPtr;
        mConstVec = &pVecRef;
        mFloatVal = pFloatVal;
        mConstVec2 = &pVecRef2;
        mFloatVal2 = pFloatVal2;
    }
};

SEND_MSG_DATA_MULTI_IMPL(CollisionImpulse, sead::Vector3f*, (const sead::Vector3f&, ConstVec),
                         (f32, FloatVal), (const sead::Vector3f&, ConstVec2), (f32, FloatVal2));

}  // namespace al
