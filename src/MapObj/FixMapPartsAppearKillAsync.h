#pragma once

#include "Library/LiveActor/LiveActor.h"

class FixMapPartsAppearKillAsync : public al::LiveActor {
public:
    FixMapPartsAppearKillAsync(const char* actorName);

    void init(const al::ActorInitInfo& info) override;
    void appear() override;
    void kill() override;
    void movement() override;
    void calcAnim() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

private:
    bool mIsStatic = false;
};

static_assert(sizeof(FixMapPartsAppearKillAsync) == 0x110);
