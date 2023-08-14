#pragma once

namespace al {
class HitSensor;

class HitSensorKeeper {
public:
    HitSensorKeeper(int);

    HitSensor* getSensor(const char*) const;

    HitSensor* getSensor(int) const;

private:
    int _0;
    int mSensorCount;
    HitSensor** mSensors;
};
};  // namespace al
