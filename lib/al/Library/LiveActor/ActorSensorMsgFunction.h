#pragma once

namespace al {

class HitSensor;
class SensorMsg;

void sendMsgPlayerReleaseEquipment(HitSensor* source, HitSensor* target);

bool isMsgPlayerDisregard(const al::SensorMsg* message);
bool isMsgHideModel(const al::SensorMsg* message);
bool isMsgShowModel(const al::SensorMsg* message);
bool isMsgAskSafetyPoint(const al::SensorMsg* message);

}  // namespace al
