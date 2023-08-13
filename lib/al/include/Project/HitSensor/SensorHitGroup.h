#pragma once

#include <basis/seadTypes.h>

namespace al {
class HitSensor;

class SensorHitGroup {
public:
    SensorHitGroup(s32 maxSensors, const char* groupName);

    void add(HitSensor* sensor);
    void remove(HitSensor* sensor);
    HitSensor* getSensor(s32 index) const;

private:
    s32 mMaxSensors;
    s32 mSensorCount = 0;
    HitSensor** mSensors;
};
};  // namespace al
