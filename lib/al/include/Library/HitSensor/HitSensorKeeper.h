#pragma once

namespace al {
class HitSensor;

class HitSensorKeeper {
public:
    HitSensorKeeper(u32);

    HitSensor* getSensor(const char*) const;
    HitSensor* getSensor(u32) const;
    u32 getSensorNum();

private:
    u32 mSensorCount;
    HitSensor** mSensors;
};
}  // namespace al
