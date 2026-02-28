#pragma once

#include "Library/LiveActor/LiveActor.h"

class Gunetter : public al::LiveActor {
public:
    Gunetter(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void initAfterPlacement() override;
    void control() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void exeWait();
    void exeChase();
    void exeKeyMove();
    void exeKeyMoveSide();

private:
    char _108[0x148 - sizeof(al::LiveActor)];
};
