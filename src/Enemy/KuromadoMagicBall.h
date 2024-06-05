#pragma once

#include "Library/LiveActor/LiveActor.h"

class KuromadoMagicBall : public al::LiveActor {
public:
    KuromadoMagicBall(const char* actorName);

    void init(const al::ActorInitInfo& info) override;
    void appear() override;
    void control() override;
    void attackSensor(al::HitSensor* target, al::HitSensor* source) override;

private:
    s32 mAliveTimer = 0;
};
