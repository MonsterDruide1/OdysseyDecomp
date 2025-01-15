#pragma once

#include "Library/LiveActor/LiveActor.h"

class TransparentWall : public al::LiveActor {
public:
    TransparentWall(const char* name);

    virtual void init(const al::ActorInitInfo& info) override;
    virtual bool receiveMsg(const al::SensorMsg* message, al::HitSensor* source,
                            al::HitSensor* target) override;
};
