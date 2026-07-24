#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
}  // namespace al

class ElectricExplosion : public al::LiveActor {
public:
    ElectricExplosion(const char* name, al::LiveActor* hostActor);

    void init(const al::ActorInitInfo& info) override;
    void attack(s32 attackLevel);
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;

    void exeWait();
    void exeAttack();

private:
    al::LiveActor* mHostActor = nullptr;
    s32 mAttackLevel = 0;
};

static_assert(sizeof(ElectricExplosion) == 0x118);
