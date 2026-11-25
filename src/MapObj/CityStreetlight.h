#pragma once

#include "Library/LiveActor/LiveActor.h"

class CityStreetlight : public al::LiveActor {
public:
    CityStreetlight(const char* name);

    void init(const al::ActorInitInfo& info) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void calcAnim() override;

    void exeWaitOn();
    void exeWaitOff();
};

static_assert(sizeof(CityStreetlight) == 0x108);
