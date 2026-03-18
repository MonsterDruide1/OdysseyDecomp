#pragma once

#include "Library/LiveActor/LiveActor.h"

class WanwanHole : public al::LiveActor {
public:
    WanwanHole(const char* name);
    void init(const al::ActorInitInfo& initInfo) override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void exeWait();
    void exeIn();
};

static_assert(sizeof(WanwanHole) == 0x108);
