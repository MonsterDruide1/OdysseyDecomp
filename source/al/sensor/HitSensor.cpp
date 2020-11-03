#include "al/sensor/HitSensor.h"

namespace al
{
    void HitSensor::addHitSensor(al::HitSensor *pSensor)
    {
        if (mSensorCount < mMaxSensorCount)
        {
            mSensors[mSensorCount] = pSensor;
            mSensorCount++;
        }
    }
};