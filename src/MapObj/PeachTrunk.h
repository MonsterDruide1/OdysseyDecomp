#pragma once

#include "Library/LiveActor/LiveActor.h"

class PeachTrunk : public al::LiveActor {
public:
    PeachTrunk(const char* actorName);

    void init(const al::ActorInitInfo& info) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void exeWait();
    void exeReactionCap();
};

static_assert(sizeof(PeachTrunk) == 0x108);
