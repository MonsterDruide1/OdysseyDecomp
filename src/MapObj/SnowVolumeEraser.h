#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al

class SnowVolumeEraser : public al::LiveActor {
public:
    SnowVolumeEraser(const char* actorName);

    void init(const al::ActorInitInfo& info) override;
    void start();
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void exeAppear();
};

static_assert(sizeof(SnowVolumeEraser) == 0x108);
