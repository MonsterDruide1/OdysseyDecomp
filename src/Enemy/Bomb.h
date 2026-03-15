#pragma once

#include "Library/LiveActor/LiveActor.h"

// Unused class
class Bomb : public al::LiveActor {
public:
    Bomb(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void appearWithMsg(const al::LiveActor*, const al::SensorMsg*);
    void exeWait();
    void exeTrampled();
    void exeWhipHold();
    void exeThrow();
    void exeExplosion();

    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
};
