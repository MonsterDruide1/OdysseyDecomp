#pragma once

namespace al {
class HitSensor;
class SensorMsg;
}  // namespace al

namespace rs {

bool sendMsgEnemyAttack2D(al::HitSensor* source, al::HitSensor* target);
bool sendMsgNoticePlayerDamage(al::HitSensor* source, al::HitSensor* target);
bool sendMsgTouchFireDrum2D(al::HitSensor* source, al::HitSensor* target);

bool isMsgCapTouchWall(const al::SensorMsg*);

}  // namespace rs
