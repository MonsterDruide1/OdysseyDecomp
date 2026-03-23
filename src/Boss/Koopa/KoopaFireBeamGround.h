#pragma once

#include "Library/LiveActor/LiveActor.h"

class KoopaFireBeamGround : public al::LiveActor {
public:
    KoopaFireBeamGround(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void appear() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void appearSign();

    void exeAppearSign();
    void exeWait();
};
