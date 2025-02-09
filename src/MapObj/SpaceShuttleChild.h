#pragma once

#include "Library/LiveActor/LiveActor.h"

class SpaceShuttleChild : public al::LiveActor {
public:
    SpaceShuttleChild(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void exeWait();
};
