#pragma once

#include "Library/Nerve/NerveExecutor.h"

namespace al {
class HitSensor;
class LiveActor;
class SensorMsg;
}  // namespace al

class ItemAppearanceController : public al::NerveExecutor {
public:
    ItemAppearanceController(al::LiveActor* actor, s32 appearTime);

    bool requestAppear(const al::SensorMsg* message, const al::HitSensor* hitSensor);

    void exeWait();
    void exeCount();
    void exeAppear();
    void exeDone();

private:
    al::LiveActor* mActor;
    s32 mAppearTime;
};

static_assert(sizeof(ItemAppearanceController) == 0x20);
