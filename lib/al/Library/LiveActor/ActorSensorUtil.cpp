#include "Library/LiveActor/ActorSensorUtil.h"

#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorFunction.h"
#include "Library/Math/MathUtil.h"
#include "Project/HitSensor/HitSensor.h"

namespace al {

SEND_MSG_IMPL(Push);
SEND_MSG_IMPL(PushStrong);
SEND_MSG_IMPL(PushVeryStrong);

SEND_MSG_COMBO_IMPL(PlayerAttackTrample);
SEND_MSG_COMBO_IMPL(PlayerTrampleReflect);
SEND_MSG_COMBO_IMPL_(PlayerHipDrop, PlayerAttackHipDrop);
SEND_MSG_COMBO_IMPL_(PlayerObjHipDrop, PlayerAttackObjHipDrop);
SEND_MSG_COMBO_IMPL_(PlayerObjHipDropReflect, PlayerAttackObjHipDropReflect);
SEND_MSG_COMBO_IMPL_(PlayerObjHipDropHighJump, PlayerAttackObjHipDropHighJump);
SEND_MSG_IMPL_(PlayerHipDropKnockDown, PlayerAttackHipDropKnockDown);
SEND_MSG_COMBO_IMPL_(PlayerStatueDrop, PlayerAttackStatueDrop);
SEND_MSG_COMBO_IMPL_(PlayerObjStatueDrop, PlayerAttackObjStatueDrop);
SEND_MSG_COMBO_IMPL_(PlayerObjStatueDropReflect, PlayerAttackObjStatueDropReflect);
SEND_MSG_IMPL_(PlayerObjStatueDropReflectNoCondition, PlayerAttackObjStatueDropReflectNoCondition);
SEND_MSG_IMPL_(PlayerStatueTouch, PlayerAttackStatueTouch);
SEND_MSG_IMPL_(PlayerUpperPunch, PlayerAttackUpperPunch);
SEND_MSG_IMPL_(PlayerObjUpperPunch, PlayerAttackObjUpperPunch);
SEND_MSG_IMPL_(PlayerRollingAttack, PlayerAttackRollingAttack);
SEND_MSG_IMPL_(PlayerRollingReflect, PlayerAttackRollingReflect);
SEND_MSG_IMPL_(PlayerObjRollingAttack, PlayerAttackObjRollingAttack);
SEND_MSG_IMPL_(PlayerObjRollingAttackFailure, PlayerAttackObjRollingAttackFailure);
SEND_MSG_COMBO_IMPL_(PlayerInvincibleAttack, PlayerAttackInvincibleAttack);
SEND_MSG_IMPL_(PlayerFireBallAttack, PlayerAttackFireBallAttack);
SEND_MSG_IMPL_(PlayerRouteDokanFireBallAttack, PlayerAttackRouteDokanFireBallAttack);
SEND_MSG_COMBO_IMPL_(PlayerTailAttack, PlayerAttackTailAttack);
SEND_MSG_IMPL_(PlayerKick, PlayerAttackKick);
SEND_MSG_IMPL_(PlayerCatch, PlayerAttackCatch);
SEND_MSG_COMBO_IMPL_(PlayerSlidingAttack, PlayerAttackSlidingAttack);
SEND_MSG_COMBO_IMPL_(PlayerBoomerangAttack, PlayerAttackBoomerangAttack);
SEND_MSG_IMPL_(PlayerBoomerangAttackCollide, PlayerAttackBoomerangAttackCollide);
SEND_MSG_IMPL_(PlayerBoomerangReflect, PlayerAttackBoomerangReflect);
SEND_MSG_IMPL_(PlayerBoomerangBreak, PlayerAttackBoomerangBreak);
SEND_MSG_COMBO_IMPL_(PlayerBodyAttack, PlayerAttackBodyAttack);
SEND_MSG_COMBO_IMPL_(PlayerBodyLanding, PlayerAttackBodyLanding);
SEND_MSG_COMBO_IMPL_(PlayerBodyAttackReflect, PlayerAttackBodyAttackReflect);
SEND_MSG_COMBO_IMPL_(PlayerClimbAttack, PlayerAttackClimbAttack);
SEND_MSG_COMBO_IMPL_(PlayerClimbSlidingAttack, PlayerAttackClimbSliding);
SEND_MSG_COMBO_IMPL_(PlayerClimbRollingAttack, PlayerAttackClimbRolling);
SEND_MSG_COMBO_IMPL_(PlayerSpinAttack, PlayerAttackSpinAttack);
SEND_MSG_COMBO_IMPL_(PlayerGiantAttack, PlayerAttackGiant);

SEND_MSG_COMBO_IMPL(PlayerCooperationHipDrop);
SEND_MSG_COMBO_IMPL(PlayerGiantHipDrop);
SEND_MSG_IMPL(PlayerDisregard);
SEND_MSG_IMPL(PlayerDamageTouch);
IS_MSG_MULTIPLE_IMPL(FloorTouch, PlayerFloorTouch, EnemyFloorTouch);
SEND_MSG_IMPL(PlayerFloorTouch);
SEND_MSG_IMPL(EnemyFloorTouch);
IS_MSG_MULTIPLE_IMPL(UpperPunch, PlayerAttackUpperPunch, EnemyUpperPunch);
SEND_MSG_IMPL(EnemyUpperPunch);
SEND_MSG_IMPL(PlayerTouch);
SEND_MSG_COMBO_IMPL(PlayerInvincibleTouch);
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
SEND_MSG_COMBO_IMPL(Explosion);
SEND_MSG_COMBO_IMPL(ExplosionCollide);
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
SEND_MSG_DATA_MULTI_IMPL(CollisionImpulse, sead::Vector3f*, (const sead::Vector3f&, ConstVec),
                         (f32, FloatVal), (const sead::Vector3f&, ConstVec2), (f32, FloatVal2));
SEND_MSG_IMPL(EnemyTouch);
SEND_MSG_IMPL(EnemyTrample);
SEND_MSG_IMPL(MapObjTrample);
SEND_MSG_IMPL(NeedleBallAttack);
SEND_MSG_IMPL(PunpunFloorTouch);
SEND_MSG_IMPL(InvalidateFootPrint);
SEND_MSG_COMBO_IMPL(KickKouraAttack);
SEND_MSG_COMBO_IMPL(KickKouraAttackCollide);
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
SEND_MSG_COMBO_IMPL(BallAttack);
SEND_MSG_COMBO_IMPL(BallRouteDokanAttack);
SEND_MSG_IMPL(BallAttackHold);
SEND_MSG_IMPL(BallAttackDRCHold);
SEND_MSG_IMPL(BallAttackCollide);
SEND_MSG_COMBO_IMPL(BallTrample);
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

bool sendMsgPlayerFloorTouchToColliderGround(LiveActor* receiver, HitSensor* sender) {
    HitSensor* collidedSensor = tryGetCollidedGroundSensor(receiver);
    return alActorSensorFunction::sendMsgSensorToSensor(SensorMsgPlayerFloorTouch(), sender,
                                                        collidedSensor);
}

bool sendMsgPlayerUpperPunchToColliderCeiling(LiveActor* receiver, HitSensor* sender) {
    HitSensor* collidedSensor = tryGetCollidedCeilingSensor(receiver);
    return alActorSensorFunction::sendMsgSensorToSensor(SensorMsgPlayerAttackUpperPunch(), sender,
                                                        collidedSensor);
}

bool sendMsgEnemyFloorTouchToColliderGround(LiveActor* receiver, HitSensor* sender) {
    HitSensor* collidedSensor = tryGetCollidedGroundSensor(receiver);
    return alActorSensorFunction::sendMsgSensorToSensor(SensorMsgEnemyFloorTouch(), sender,
                                                        collidedSensor);
}

bool sendMsgEnemyUpperPunchToColliderCeiling(LiveActor* receiver, HitSensor* sender) {
    HitSensor* collidedSensor = tryGetCollidedCeilingSensor(receiver);
    return alActorSensorFunction::sendMsgSensorToSensor(SensorMsgEnemyUpperPunch(), sender,
                                                        collidedSensor);
}

bool sendMsgAskSafetyPointToColliderGround(LiveActor* receiver, HitSensor* sender,
                                           sead::Vector3f** safetyPointAccessor) {
    HitSensor* collidedSensor = tryGetCollidedGroundSensor(receiver);
    return alActorSensorFunction::sendMsgSensorToSensor(
        SensorMsgAskSafetyPoint(safetyPointAccessor), sender, collidedSensor);
}

SEND_MSG_DATA_IMPL(AskSafetyPoint, sead::Vector3f**);
SEND_MSG_IMPL(TouchAssist);
SEND_MSG_IMPL(TouchAssistTrig);
SEND_MSG_IMPL(TouchStroke);
SEND_MSG_IMPL(IsNerveSupportFreeze);
SEND_MSG_IMPL(OnSyncSupportFreeze);
SEND_MSG_IMPL(OffSyncSupportFreeze);
SEND_MSG_IMPL(ScreenPointInvalidCollisionParts);
SEND_MSG_COMBO_IMPL(BlockUpperPunch);
SEND_MSG_COMBO_IMPL(BlockLowerPunch);
SEND_MSG_IMPL(BlockItemGet);
SEND_MSG_COMBO_IMPL(PlayerKouraAttack);
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
    // return A || B mismatches, but this matches
    if (alActorSensorFunction::sendMsgSensorToSensor(SensorMsgPlayerTrampleReflect(comboCounter),
                                                     sender, receiver))
        return true;
    return alActorSensorFunction::sendMsgSensorToSensor(SensorMsgPlayerAttackTrample(comboCounter),
                                                        sender, receiver);
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

bool isCrossoverSensor(const HitSensor* sensor1, const HitSensor* sensor2) {
    sead::Vector3f diff = sensor1->getPos() - sensor2->getPos();
    sead::Vector3f sensor2ParentGravityN = getGravity(sensor2->getParentActor());
    sead::Vector3f sensor2ParentGravity = -sensor2ParentGravityN;
    if (!tryNormalizeOrZero(&diff))
        return false;
    if (diff.dot(sensor2ParentGravity) < sead::Mathf::deg2rad(20.0f))
        return false;
    sead::Vector3f velocityDir;
    tryNormalizeOrZero(&velocityDir, getVelocity(sensor1->getParentActor()));
    if (diff.y < 0.0f)
        return false;
    f32 dot = velocityDir.dot(sensor2ParentGravity);
    return !isNearZero(sead::Mathf::abs(dot), 0.001f) && !isNearZero(dot - 1.0f, 0.001f);
}

bool isMsgPlayerTrampleForCrossoverSensor(const SensorMsg* msg, const HitSensor* sensor1,
                                          const HitSensor* sensor2) {
    return MSG_TYPE_CHECK_(PlayerAttackTrample, msg) && isCrossoverSensor(sensor1, sensor2);
}

bool isMsgPlayerTrampleReflectForCrossoverSensor(const SensorMsg* msg, const HitSensor* sensor1,
                                                 const HitSensor* sensor2) {
    return MSG_TYPE_CHECK_(PlayerTrampleReflect, msg) && isCrossoverSensor(sensor1, sensor2);
}

bool isMsgPlayerUpperPunchForCrossoverSensor(const SensorMsg* msg, const HitSensor* sensor1,
                                             const HitSensor* sensor2, f32 threshold) {
    if (!MSG_TYPE_CHECK_(PlayerAttackObjUpperPunch, msg))
        return false;
    sead::Vector3f diff = sensor1->getPos() - sensor2->getPos();
    sead::Vector3f sensor2ParentGravity = getGravity(sensor2->getParentActor());
    normalize(&diff);
    if (sensor2ParentGravity.dot(diff) < 0.34202f)
        return false;
    // Has to be written like this, return A || B doesn't match
    if (sensor2ParentGravity.dot(getVelocity(sensor1->getParentActor())) >= -threshold)
        return false;
    return true;
}

bool isMsgKickStoneTrampleForCrossoverSensor(const SensorMsg* msg, const HitSensor* sensor1,
                                             const HitSensor* sensor2) {
    return MSG_TYPE_CHECK_(KickStoneTrample, msg) && isCrossoverSensor(sensor1, sensor2);
}

bool sendMsgEnemyAttackForCrossoverSensor(HitSensor* receiver, HitSensor* sender) {
    // Not sure why this isn't checking !isCrossoverSensor. Either I named it incorrectly, I
    // misunderstand how the function works or the devs accidentaly forgot the `!` (which is very
    // possible since this is an unused function)
    if (isCrossoverSensor(receiver, sender))
        return false;
    return alActorSensorFunction::sendMsgSensorToSensor(SensorMsgEnemyAttack(), sender, receiver);
}

// NON_MATCHING: Minor mismatch in stack usage (https://decomp.me/scratch/8y80i)
bool sendMsgEnemyAttackForCrossoverCylinderSensor(HitSensor* receiver, HitSensor* sender,
                                                  const sead::Vector3f& vec1,
                                                  const sead::Vector3f& vec2, f32 unk) {
    f32 v6 = sead::Mathf::clamp(unk - 20.0f, 0.0f, unk);

    sead::Vector3f diff = receiver->getPos() - sender->getPos();
    sead::Vector3f senderParentGravity = getGravity(sender->getParentActor());
    verticalizeVec(&diff, vec2, diff);

    if (!(diff.squaredLength() < sead::Mathf::square(v6))) {
        normalize(&diff);
        sead::Vector3f velocityDir;
        tryNormalizeOrZero(&velocityDir, getVelocity(sender->getParentActor()));
        if (!(diff.y < 0.0f)) {
            f32 dot =
                senderParentGravity.x * -velocityDir.x + senderParentGravity.y * -velocityDir.y +
                senderParentGravity.z *
                    -velocityDir.z;  // senderParentGravity.dot(-velocityDir) causes worse mismatch
            if (!isNearZero(sead::Mathf::abs(dot), 0.001f) && !isNearZero(dot - 1.0f, 0.001f))
                return false;
        }
    }

    sead::Vector3f diff2 = vec1 - receiver->getPos();
    verticalizeVec(&diff, vec2, diff2);
    if (diff.length() <= receiver->getRadius() + unk)
        return alActorSensorFunction::sendMsgSensorToSensor(SensorMsgEnemyAttack(), sender,
                                                            receiver);
    return false;
}

IS_MSG_MULTIPLE_IMPL(PushAll, Push, PushStrong, PushVeryStrong);
IS_MSG_IMPL(Push);
IS_MSG_IMPL(PushStrong);
IS_MSG_IMPL(PushVeryStrong);
IS_MSG_MULTIPLE_IMPL(SwordBeamAttack, SwordBeamAttack, SwordBeamReflectAttack);
IS_MSG_MULTIPLE_IMPL(HoldReleaseAll, HoldCancel, PlayerRelease, PlayerReleaseBySwing,
                     PlayerReleaseDead, PlayerReleaseDamage, PlayerReleaseDemo);
IS_MSG_MULTIPLE_IMPL(ItemGetDirectAll, PlayerItemGet, RideAllPlayerItemGet, PlayerAttackTailAttack);
IS_MSG_MULTIPLE_IMPL(ItemGetByObjAll, BallItemGet, KickKouraGetItem, KillerItemGet);

// This could probably also be implemented with the IS_MSG_MULTIPLE_IMPL macro, but that would need
// to be given all messages checked by both of these
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

SEND_MSG_DATA_IMPL(CollidePush, const sead::Vector3f&);

bool sendMsgPushAndKillVelocityToTarget(LiveActor* actor, HitSensor* receiver, HitSensor* sender) {
    if (!alActorSensorFunction::sendMsgSensorToSensor(SensorMsgPush(), receiver, sender))
        return false;
    sead::Vector3f diff = sender->getPos() - receiver->getPos();
    if (!tryNormalizeOrZero(&diff))
        diff.set(sead::Vector3f::ez);
    if (getVelocity(actor).dot(diff) > 0.0f)
        verticalizeVec(getVelocityPtr(actor), diff, getVelocity(actor));
    return true;
}

bool sendMsgPushAndKillVelocityToTargetH(LiveActor* actor, HitSensor* receiver, HitSensor* sender) {
    if (!alActorSensorFunction::sendMsgSensorToSensor(SensorMsgPush(), receiver, sender))
        return false;
    sead::Vector3f diff(sender->getPos().x - receiver->getPos().x, 0.0f,
                        sender->getPos().z - receiver->getPos().z);
    if (!tryNormalizeOrZero(&diff))
        diff.set(sead::Vector3f::ez);
    if (getVelocity(actor).dot(diff) > 0.0f)
        verticalizeVec(getVelocityPtr(actor), diff, getVelocity(actor));
    return true;
}

}  // namespace al
