#include "al/sensor/SensorHitGroup.h"

namespace al {
void SensorHitGroup::add(al::HitSensor* pSensor) {
    mSensors[mSensorCount] = pSensor;
    mSensorCount++;
}

al::HitSensor* SensorHitGroup::getSensor(int idx) const {
    return mSensors[idx];
}
};  // namespace al