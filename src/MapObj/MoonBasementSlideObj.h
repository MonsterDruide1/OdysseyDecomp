#pragma once

#include "Library/LiveActor/LiveActor.h"

class MoonBasementSlideObj : public al::LiveActor {
public:
    MoonBasementSlideObj(const char* name);

    void init(const al::ActorInitInfo& info) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void exeWait();
    void exeSlide();

private:
    s32 field_108 = 0;
};
