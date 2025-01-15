#pragma once

#include "Library/LiveActor/LiveActor.h"

class VocalMike : public al::LiveActor {
public:
    VocalMike(const char* actorName);

    void init(const al::ActorInitInfo& info) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* source,
                    al::HitSensor* target) override;
};
