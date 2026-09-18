#pragma once

#include "Library/LiveActor/LiveActor.h"

class FixMapPartsDitherAppear : public al::LiveActor {
public:
    FixMapPartsDitherAppear(const char* actorName);

    void init(const al::ActorInitInfo& info) override;
    void calcAnim() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void ditherAppear();
    void waitAppear();
    void exeAppear();
    void exeWait();

private:
    s32 mAnimFrame = -1;
};

static_assert(sizeof(FixMapPartsDitherAppear) == 0x110);
