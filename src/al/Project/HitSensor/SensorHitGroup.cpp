#include "al/Project/HitSensor/SensorHitGroup.h"

namespace al {

SensorHitGroup::SensorHitGroup(s32 maxSensors, const char* groupName)
    : mMaxSensors(maxSensors), mSensors(new HitSensor*[maxSensors]) {
    for (s32 i = 0; i < mMaxSensors; i++)
        mSensors[i] = nullptr;
}

void SensorHitGroup::add(al::HitSensor* pSensor) {
    mSensors[mSensorCount] = pSensor;
    mSensorCount++;
}

al::HitSensor* SensorHitGroup::getSensor(int idx) const {
    return mSensors[idx];
}

}  // namespace al
