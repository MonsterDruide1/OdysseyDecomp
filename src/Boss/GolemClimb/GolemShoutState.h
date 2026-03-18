#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class SensorMsg;
class HitSensor;
}  // namespace al

class IUseGolemState;

class GolemShoutState : public al::NerveStateBase {
public:
    GolemShoutState(const char* name, IUseGolemState* golemState);
    void appear();
    void kill();
    void control();
    void receiveMsg(const al::SensorMsg*, al::HitSensor* self, al::HitSensor* other);
    void attackSensor(al::HitSensor* self, al::HitSensor* other);
};
