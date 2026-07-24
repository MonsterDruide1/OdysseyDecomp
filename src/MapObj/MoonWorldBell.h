#pragma once

#include "Library/LiveActor/LiveActor.h"

class MoonWorldBell : public al::LiveActor {
public:
    MoonWorldBell(const char* name);

    void init(const al::ActorInitInfo& info) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void exeWait();
    void exeCapReaction();
    void exeHipDropReaction();
};

static_assert(sizeof(MoonWorldBell) == 0x108);
