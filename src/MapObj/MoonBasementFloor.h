#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al

class MoonBasementFloor : public al::LiveActor {
public:
    MoonBasementFloor(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void movement() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void startFallBySwitch();

    void exeWait();
    void exeFallSignStart();
    void exeFallSign();
    void exeFallSignBySwitch();
    void exeFall();

private:
    s32 mFallSignStepBySwitch = -1;
};

static_assert(sizeof(MoonBasementFloor) == 0x110);
