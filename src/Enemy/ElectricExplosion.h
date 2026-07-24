#pragma once

#include "Library/LiveActor/LiveActor.h"

// Unused class
class ElectricExplosion : public al::LiveActor {
public:
    ElectricExplosion(const char* name, al::LiveActor*);

    void init(const al::ActorInitInfo& info) override;
    void attack(s32);
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;

    void exeWait();
    void exeAttack();

private:
    al::LiveActor* _108;
    s32 _110;
};

static_assert(sizeof(ElectricExplosion) == 0x118);
