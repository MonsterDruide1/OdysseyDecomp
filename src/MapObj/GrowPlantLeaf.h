#pragma once

#include "Library/LiveActor/LiveActor.h"

class GrowPlantLeaf : public al::LiveActor {
public:
    GrowPlantLeaf();

    void init(const al::ActorInitInfo& info) override;
    void appear() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void control() override;

    void startGrow();
    void endGrow();

private:
    f32* mJointRotators = nullptr;
    f32* mJointRotatorVelocities = nullptr;
    bool mIsEndGrow = false;
};

static_assert(sizeof(GrowPlantLeaf) == 0x120);
