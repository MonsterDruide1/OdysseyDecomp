#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;
class HitSensor;
}

class PlayerColliderHackCap {
public:
    PlayerColliderHackCap(al::LiveActor* actor);

    void update();

private:
    void* filler[0x30 / 8];
};

static_assert(sizeof(PlayerColliderHackCap) == 0x30);
