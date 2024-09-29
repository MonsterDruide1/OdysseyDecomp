#pragma once

namespace al {
class HitSensor;
}

class PlayerBindKeeper {
public:
    al::HitSensor* getSecondHitSensor() const { return mSecondHitSensor; }

private:
    char padding[8];
    al::HitSensor* mSecondHitSensor;
};
