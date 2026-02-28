#pragma once

#include "Library/LiveActor/LiveActor.h"

class GrowerBug : public al::LiveActor {
public:
    GrowerBug(const char* name,
              bool createdFromFactory =
                  true);  // Defaults to true so that the default createActorFunction template works

    void init(const al::ActorInitInfo&) override;
    void onKill();
    void appear() override;
    void updateCollider() override;

    void exeAppear();
    void exeWander();
    void exeFind();
    void exeAttackSign();
    void exeAttack();
    void exeBurst();
    void exeBlowDown();
    void exePressDown();
    void exeDead();

    void attackSensor(al::HitSensor*, al::HitSensor*) override;
    bool receiveMsg(const al::SensorMsg*, al::HitSensor*, al::HitSensor*) override;

private:
    char _108[0x140 - sizeof(al::LiveActor)];
};
