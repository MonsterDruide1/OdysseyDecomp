#pragma once

#include "Library/LiveActor/LiveActor.h"

class HomeInside : public al::LiveActor {
public:
    HomeInside(const char* name);

    void init(const al::ActorInitInfo& info) override;
};

class HomeShip : public al::LiveActor {
public:
    HomeShip(const char* name);

    void init(const al::ActorInitInfo& info) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void exeWait();

private:
    al::LiveActor* mGoalObj = nullptr;
};

static_assert(sizeof(HomeInside) == 0x108);
static_assert(sizeof(HomeShip) == 0x110);
