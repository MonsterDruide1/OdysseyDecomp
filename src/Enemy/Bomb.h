#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al

class Bomb : public al::LiveActor {
public:
    Bomb(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void appearWithMsg(const al::LiveActor* actor, const al::SensorMsg* message);
    void exeWait();
    void exeTrampled();
    void exeWhipHold();
    void exeThrow();
    void exeExplosion();
};

static_assert(sizeof(Bomb) == 0x108);
