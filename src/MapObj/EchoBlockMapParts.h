#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al

class EchoBlockMapParts : public al::LiveActor {
public:
    EchoBlockMapParts(const char* name);

    void init(const al::ActorInitInfo& info) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void begin();
};

static_assert(sizeof(EchoBlockMapParts) == 0x108);
