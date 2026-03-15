#pragma once

#include "Library/LiveActor/LiveActor.h"

class GrowerWorm : public al::LiveActor {
public:
    GrowerWorm(const char* nane);

    void init(const al::ActorInitInfo& info) override;
    void onKill();
    void appear() override;
    bool isEnableReAppear() const;

    void exeWait();
    void exeAppear();
    void exeMove();
    void exeTurn();
    void exeBornSign();
    void exeBorn();
    void exeBornDead();
    void exeDead();
    void exePressDown();
    void exeBlowDown();

    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

private:
    char _108[0x130 - sizeof(al::LiveActor)];
};
