#pragma once

namespace al {

class HitSensor;
class SensorMsg;

void sendMsgPlayerReleaseEquipment(al::HitSensor*, al::HitSensor*);

bool isMsgPlayerDisregard(al::SensorMsg const* message);

}  // namespace al
