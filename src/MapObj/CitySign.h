#pragma once

#include "Library/LiveActor/LiveActor.h"

class CitySign : public al::LiveActor {
public:
    CitySign(const char* name);

    void init(const al::ActorInitInfo& info) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void exeWait();
    void exeLightOff();
};

static_assert(sizeof(CitySign) == 0x108);
