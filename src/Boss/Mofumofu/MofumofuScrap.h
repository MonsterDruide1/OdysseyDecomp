#pragma once

#include "Library/LiveActor/LiveActor.h"

class MofumofuScrap : public al::LiveActor {
public:
    MofumofuScrap(const char* name);

    void init(const al::ActorInitInfo& info) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void exeWait();
    void exeReaction();
    void exeReactionCap();
};
