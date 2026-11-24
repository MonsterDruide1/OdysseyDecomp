#pragma once

#include "Library/LiveActor/LiveActor.h"

class GotogotonMark;

class GotogotonGoal : public al::LiveActor {
public:
    GotogotonGoal(const char* name);

    void init(const al::ActorInitInfo& info) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

private:
    GotogotonMark* mMark = nullptr;
};

static_assert(sizeof(GotogotonGoal) == 0x110);
