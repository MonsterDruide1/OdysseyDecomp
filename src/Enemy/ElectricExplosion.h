#pragma once

#include "Library/LiveActor/LiveActor.h"

// Unused class
class ElectricExplosion : public al::LiveActor {
public:
    ElectricExplosion(const char* name, al::LiveActor*);

    void init(const al::ActorInitInfo&) override;
    void attack(s32);
    void attackSensor(al::HitSensor*, al::HitSensor*) override;

    void exeWait();
    void exeAttack();
};
