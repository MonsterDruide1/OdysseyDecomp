#pragma once

#include <math/seadVector.h>

namespace al {
class HitSensor;

class EventFlowScareCtrlBase {
public:
    virtual bool isScare() const = 0;
    virtual bool tryGetScareEnemyPos(sead::Vector3f*) const = 0;
    virtual const char16* getScareMessage() const = 0;
    virtual void update() = 0;
    virtual void attackSensor(HitSensor* self, HitSensor* other) = 0;
};
}  // namespace al
