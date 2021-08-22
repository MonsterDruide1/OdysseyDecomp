#pragma once

#include "al/LiveActor/LiveActor.h"

namespace al {

    bool isClipped(LiveActor*);
    void tryInitFixedModelGpuBuffer(LiveActor*);
    void offUpdateMovementEffectAudioCollisionSensor(LiveActor*);

}