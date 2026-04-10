#pragma once

#include "Library/LiveActor/LiveActor.h"

class FixMapPartsBgmChangeAction : public al::LiveActor {
public:
    FixMapPartsBgmChangeAction(const char* actorName);

    void init(const al::ActorInitInfo& info) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void exeWait();
    void exePlayingBgm();

private:
    const char* mChangeBgm = nullptr;
};

static_assert(sizeof(FixMapPartsBgmChangeAction) == 0x110);
