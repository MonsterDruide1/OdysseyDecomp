#pragma once

#include <math/seadVector.h>

namespace al {
class HitSensor;
class SensorMsg;
}  // namespace al

class ExternalForceKeeper {
public:
    ExternalForceKeeper();

    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self);
    void reset();
    void calcForce(sead::Vector3f* force) const;

private:
    sead::Vector3f mMinForce = sead::Vector3f::zero;
    sead::Vector3f mMaxForce = sead::Vector3f::zero;
};
