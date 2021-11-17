#pragma once

#include "al/sensor/HitSensor.h"

namespace al {
class HitSensorKeeper {
public:
    HitSensorKeeper(int);

    al::HitSensor* getSensor(char const*) const;

    al::HitSensor* getSensor(int) const;

    int _0;
    int mSensorCount;          // _04
    al::HitSensor** mSensors;  // _8
};
};  // namespace al