#pragma once

#include "Library/LiveActor/LiveActor.h"

class Senobi : public al::LiveActor {
public:
    using LiveActor::LiveActor;

    void resetCounter();

    void setBossGenerated(bool val) { mIsBossGenerated = val; }

private:
    u8 _108[0x3D];
    bool mIsBossGenerated = false;
};

static_assert(sizeof(Senobi) == 0x148);
