#pragma once

#include "Library/LiveActor/LiveActor.h"

class CapAppearMapParts;

class CapAppearTarget : public al::LiveActor {
public:
    CapAppearTarget(const char* name, CapAppearMapParts* mapParts);

    void init(const al::ActorInitInfo& info) override;
    void appear() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void switchAppear();

    void exeWait();
    void exeReaction();
    void exeAppear();
    void exeDisappear();
    void exeDead();

private:
    CapAppearMapParts* mMapParts = nullptr;
    bool mIsValidTouchReaction = true;
};

static_assert(sizeof(CapAppearTarget) == 0x118);
