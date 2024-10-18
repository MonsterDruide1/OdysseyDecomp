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

bool isMsgCapTouchWall(const al::SensorMsg*);
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

}  // namespace rs
