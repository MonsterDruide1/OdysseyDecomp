#pragma once

namespace al {
class LiveActor;
class SensorMsg;
class HitSensor;
}

namespace rs {

void setAppearItemFactorAndOffsetByMsg(const al::LiveActor* actor, const al::SensorMsg* msg, const al::HitSensor* sensor);
void setAppearItemFactorAndOffsetForCombo(const al::LiveActor* actor, const al::SensorMsg* msg, const al::HitSensor* sensor, bool);

}
