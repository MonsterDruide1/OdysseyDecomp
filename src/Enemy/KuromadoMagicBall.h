#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"

class KuromadoMagicBall : public al::LiveActor {
public:
    KuromadoMagicBall(const char* actorName);

    void init(const al::ActorInitInfo& info) override;
    void appear() override;
    void control() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;

private:
    s32 mAliveTimer = 0;
};
