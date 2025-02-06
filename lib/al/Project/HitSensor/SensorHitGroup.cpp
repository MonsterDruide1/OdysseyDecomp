#include "Project/HitSensor/SensorHitGroup.h"

#include "Project/HitSensor/HitSensor.h"

namespace al {

SensorHitGroup::SensorHitGroup(s32 maxSensors, const char* groupName) : mMaxSensors(maxSensors) {
    mSensors = new HitSensor*[maxSensors];
    for (s32 i = 0; i < mMaxSensors; i++)
        mSensors[i] = nullptr;
}

void SensorHitGroup::add(HitSensor* pSensor) {
    mSensors[mSensorCount] = pSensor;
    mSensorCount++;
}

void SensorHitGroup::remove(HitSensor* sensor) {
    for (s32 i = 0; i < mSensorCount; i++) {
        if (mSensors[i] == sensor) {
            mSensors[i] = mSensors[mSensorCount - 1];
            mSensorCount--;
            return;
        }
    }
}

HitSensor* SensorHitGroup::getSensor(s32 idx) const {
    return mSensors[idx];
}

void SensorHitGroup::clear() const {
    for (s32 i = 0; i < mSensorCount; i++)
        mSensors[i]->setSensorCountZero();
}

}  // namespace al
