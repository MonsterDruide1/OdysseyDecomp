#pragma once

#include "Library/LiveActor/LiveActor.h"

class ReactionMapParts : public al::LiveActor {
public:
    ReactionMapParts(const char* actorName);

    void init(const al::ActorInitInfo& info) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void exeWait();
    void exeReaction();
};

static_assert(sizeof(ReactionMapParts) == 0x108);
