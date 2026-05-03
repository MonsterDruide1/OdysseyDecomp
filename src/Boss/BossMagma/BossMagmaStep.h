#pragma once

#include "MapObj/WaveSurfMapParts.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al

class BossMagmaStepAnimControl;

class BossMagmaStep : public WaveSurfMapParts {
public:
    BossMagmaStep();

    void init(const al::ActorInitInfo& info) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void rebirth();
    void down();
    void control() override;

private:
    BossMagmaStepAnimControl* mAnimControl = nullptr;
};

static_assert(sizeof(BossMagmaStep) == 0x160);
