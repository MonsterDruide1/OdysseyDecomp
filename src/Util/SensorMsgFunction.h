#pragma once

#include <math/seadVector.h>

namespace al {
class HitSensor;
class SensorMsg;
}  // namespace al

class CapTargetInfo;

namespace rs {

bool sendMsgEnemyAttack2D(al::HitSensor* source, al::HitSensor* target);
bool sendMsgNoticePlayerDamage(al::HitSensor* source, al::HitSensor* target);
bool sendMsgTouchFireDrum2D(al::HitSensor* source, al::HitSensor* target);
bool sendMsgItemAmiiboKoopa(al::HitSensor* source, al::HitSensor* target);
bool sendMsgPushToPlayer(al::HitSensor* source, al::HitSensor* target);
bool sendMsgHackUpperPunch(al::HitSensor*, al::HitSensor*);
bool sendMsgHackObjUpperPunch(al::HitSensor*, al::HitSensor*);

bool isMsgCapTouchWall(const al::SensorMsg*);
bool isMsgCapHipDrop(const al::SensorMsg*);
bool isMsgPlayerAndCapHipDropAll(const al::SensorMsg*);
bool isMsgNpcCapReactionAll(const al::SensorMsg*);
bool isMsgCapTouchWall(const al::SensorMsg*);
bool isMsgPlayerRollingWallHitDown(const al::SensorMsg*);
bool tryReceiveMsgInitCapTargetAndSetCapTargetInfo(const al::SensorMsg*, const CapTargetInfo*);
bool isMsgPlayerDisregardHomingAttack(const al::SensorMsg*);
bool isMsgPlayerDisregardTargetMarker(const al::SensorMsg*);
bool isMsgTargetMarkerPosition(const al::SensorMsg*);
void setMsgTargetMarkerPosition(const al::SensorMsg*, const sead::Vector3f&);
bool isMsgCapKeepLockOn(const al::SensorMsg*);
bool isMsgCapCancelLockOn(const al::SensorMsg*);
bool isMsgStartHack(const al::SensorMsg*);
bool isMsgCapStartLockOn(const al::SensorMsg*);
bool isMsgCancelHack(const al::SensorMsg*);
bool isMsgHackMarioDead(const al::SensorMsg*);
bool isMsgHackMarioDemo(const al::SensorMsg*);
bool isMsgHackMarioInWater(const al::SensorMsg*);
bool isMsgHackMarioCheckpointFlagWarp(const al::SensorMsg*);
bool isMsgPlayerDisregard(const al::SensorMsg*);
bool isMsgCapAttackCollide(const al::SensorMsg*);
bool isMsgFrogHackTrample(const al::SensorMsg*);
bool isMsgCapAttack(const al::SensorMsg*);
bool isMsgPressDown(const al::SensorMsg*);
bool isMsgAttackDirect(const al::SensorMsg*);
bool isMsgTankBullet(const al::SensorMsg*);
bool isMsgTankExplosion(const al::SensorMsg*);
bool isMsgSeedAttackHold(const al::SensorMsg*);
bool isMsgSeedTouch(const al::SensorMsg*);
bool isMsgWanwanEnemyAttack(const al::SensorMsg*);
bool isMsgBlowDown(const al::SensorMsg*);
bool isMsgNpcScareByEnemy(const al::SensorMsg*);
bool isMsgKillByShineGet(const al::SensorMsg*);
bool isMsgKillByHomeDemo(const al::SensorMsg*);
bool isMsgAirExplosion(const al::SensorMsg*);
bool isMsgByugoBlow(const al::SensorMsg*);
bool isMsgFishingItemGet(const al::SensorMsg*);
bool isMsgFishingLineTouch(const al::SensorMsg*);
bool isMsgItemGet2D(const al::SensorMsg*);
bool isMsgItemGetByWeapon(const al::SensorMsg*);
bool isMsgItemGetAll(const al::SensorMsg*);
bool isMsgCapIgnoreCancelLockOn(const al::SensorMsg*);
bool isMsgCapReflect(const al::SensorMsg*);
bool isMsgDonsukeAttack(const al::SensorMsg*);
bool isMsgPechoSpot(const al::SensorMsg*);
bool isMsgDamageBallAttack(const al::SensorMsg*);
bool isMsgKoopaHackPunchCollide(const al::SensorMsg*);
bool isMsgCapEnableLockIn(const al::SensorMsg*);
bool isVisibleChameleon(const al::SensorMsg*);

bool tryGetAirExplosionForce(sead::Vector3f* force, const al::SensorMsg*);
bool tryGetByugoBlowForce(sead::Vector3f* force, const al::SensorMsg*);

void requestHitReactionToAttacker(const al::SensorMsg*, const al::HitSensor*, const al::HitSensor*);
void requestHitReactionToAttacker(const al::SensorMsg*, const al::HitSensor*,
                                  const sead::Vector3f&);

}  // namespace rs
