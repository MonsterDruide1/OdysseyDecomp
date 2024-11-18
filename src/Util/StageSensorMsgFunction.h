#pragma once

#include <math/seadVector.h>

namespace al {
class HitSensor;
class SensorMsg;
}  // namespace al

namespace rs {

void sendMsgKuriboFlick(al::HitSensor*, al::HitSensor*);

bool isMsgNpcScareByEnemy(const al::SensorMsg*);
bool isMsgTankLookOn(const al::SensorMsg*);
bool isMsgPlayerDisregardHomingAttack(const al::SensorMsg*);
bool isMsgPlayerDisregardTargetMarker(const al::SensorMsg*);
bool isMsgKillByShineGet(const al::SensorMsg*);
bool isMsgYoshiTongueEatBind(const al::SensorMsg*);
bool isMsgYoshiEatBindFinish(const al::SensorMsg*);
bool isMsgYoshiEatBindCancel(const al::SensorMsg*);
bool isMsgPressDown(const al::SensorMsg*);
bool isMsgBlowDown(const al::SensorMsg*);
bool isMsgGrowerAttack(const al::SensorMsg*);
bool isMsgCapTrampolineAttack(const al::SensorMsg*);
bool isMsgFireDamageAll(const al::SensorMsg*);
bool isMsgHackAttack(const al::SensorMsg*);
bool isMsgKuriboFlick(const al::SensorMsg*);

void setMsgYoshiTongueEatBindRadiusAndOffset(const al::SensorMsg*, f32, f32);

void sendMsgPushToPlayer(al::HitSensor*, al::HitSensor*);

void requestHitReactionToAttacker(const al::SensorMsg*, al::HitSensor*, al::HitSensor*);
void requestHitReactionToAttacker(const al::SensorMsg*, const al::HitSensor*,
                                  const sead::Vector3f&);

void calcBlowDownDir(sead::Vector3f*, const al::SensorMsg*, al::HitSensor*, al::HitSensor*);

bool tryIncrementComboCount(int*, const al::SensorMsg*);

}  // namespace rs
