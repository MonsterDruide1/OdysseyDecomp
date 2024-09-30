#pragma once

namespace al {
class HitSensor;
}

class PlayerBindKeeper {
public:
    al::HitSensor* getBindSensor() const { return mBindSensor; }

private:
    char padding[8];
    al::HitSensor* mBindSensor;
};
