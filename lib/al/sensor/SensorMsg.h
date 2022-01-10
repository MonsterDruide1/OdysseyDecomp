#pragma once

namespace al {

class HitSensor;
class SensorMsg;

void sendMsgPlayerReleaseEquipment(HitSensor* source, HitSensor* target);

bool isMsgPlayerDisregard(al::SensorMsg const* message);

}  // namespace al
