#pragma once

#include "Library/LiveActor/LiveActor.h"

class GrowerWorm : public al::LiveActor {
public:
    GrowerWorm(const char* nane);

    void init(const al::ActorInitInfo&) override;
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

    void attackSensor(al::HitSensor*, al::HitSensor*) override;
    bool receiveMsg(const al::SensorMsg*, al::HitSensor*, al::HitSensor*) override;

private:
    char _108[0x130 - sizeof(al::LiveActor)];
};
