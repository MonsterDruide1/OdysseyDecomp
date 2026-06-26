#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;
}

class HackCapJointControlKeeper {
public:
    HackCapJointControlKeeper(al::LiveActor* actor);

    void update();

private:
    void* filler[0x50 / 8];
};

static_assert(sizeof(HackCapJointControlKeeper) == 0x50);
