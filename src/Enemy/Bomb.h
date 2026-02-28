#pragma once

#include "Library/LiveActor/LiveActor.h"

// Unused class
class Bomb : public al::LiveActor {
public:
    Bomb(const char* name);

    void init(const al::ActorInitInfo&) override;
    void appearWithMsg(const al::LiveActor*, const al::SensorMsg*);
    void exeWait();
    void exeTrampled();
    void exeWhipHold();
    void exeThrow();
    void exeExplosion();

    void attackSensor(al::HitSensor*, al::HitSensor*) override;
    bool receiveMsg(const al::SensorMsg*, al::HitSensor*, al::HitSensor*) override;
};
