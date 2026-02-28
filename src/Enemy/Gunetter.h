#pragma once

#include "Library/LiveActor/LiveActor.h"

class Gunetter : public al::LiveActor {
public:
    Gunetter(const char* name);

    void init(const al::ActorInitInfo&) override;
    void initAfterPlacement() override;
    void control() override;
    void attackSensor(al::HitSensor*, al::HitSensor*) override;
    bool receiveMsg(const al::SensorMsg*, al::HitSensor*, al::HitSensor*) override;

    void exeWait();
    void exeChase();
    void exeKeyMove();
    void exeKeyMoveSide();

private:
    char _108[0x148 - sizeof(al::LiveActor)];
};
