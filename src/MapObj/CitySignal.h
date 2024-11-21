#pragma once

#include "Library/LiveActor/LiveActor.h"

class CitySignal : public al::LiveActor {
public:
    CitySignal(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void movement() override;
    void calcAnim() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void exeWaitRed();
    void exeWaitBlue();
    void exeWaitOff();
};

static_assert(sizeof(CitySignal) == 0x108);
