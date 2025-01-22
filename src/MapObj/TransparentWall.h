#pragma once

#include "Library/LiveActor/LiveActor.h"

class TransparentWall : public al::LiveActor {
public:
    TransparentWall(const char* name);

    void init(const al::ActorInitInfo& info) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
};
