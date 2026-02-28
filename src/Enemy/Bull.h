#pragma once

#include "Library/LiveActor/LiveActor.h"

class Bull : public al::LiveActor {
public:
    Bull(const char* name) : al::LiveActor(name) { /* TODO: Implement inlined ctor */ }

    const char* getSensorNameSendAttack();
    void init(const al::ActorInitInfo& info) override;
    void switchOn();
    void control() override;
    void deathAndReset();
    void updateCollider() override;

    void exeNoStart();
    void exeWait();
    void exeTurn();
    void exeFind();
    void exeChase();
    void endChase();
    void exeBrake();
    void exeClash();
    void exeAttack();
    void exeFall();
    void exeDamageCap();
    void exeTrampled();
    void exeTrampledEnd();
    void exeAngry();
    void exePressDown();
    void exeBlowDown();
    void exeSwoon();
    void exeReset();
    void exeRevive();
    void exeReviveAppear();
    void exeHack();

    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void startHack(const al::SensorMsg*, al::HitSensor*, al::HitSensor*);
    bool tryReceiveMsgTrample(const al::SensorMsg*, const al::HitSensor*, const al::HitSensor*);

private:
    char _108[0x148 - sizeof(al::LiveActor)];
};
