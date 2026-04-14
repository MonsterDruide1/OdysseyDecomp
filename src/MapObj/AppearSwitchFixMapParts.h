#pragma once

#include "Library/LiveActor/LiveActor.h"

class AppearSwitchFixMapParts : public al::LiveActor {
public:
    AppearSwitchFixMapParts(const char* actorName);

    void init(const al::ActorInitInfo& info) override;
    void appear() override;
    void movement() override;
    void calcAnim() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

private:
    bool mIsStatic = false;
};

static_assert(sizeof(AppearSwitchFixMapParts) == 0x110);
