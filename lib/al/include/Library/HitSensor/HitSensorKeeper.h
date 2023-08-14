#pragma once

namespace al {
class HitSensor;

class HitSensorKeeper {
public:
    HitSensorKeeper(s32);

    HitSensor* getSensor(const char*) const;

    HitSensor* getSensor(s32) const;

private:
    s32 _0;
    s32 mSensorCount;
    HitSensor** mSensors;
};
};  // namespace al
