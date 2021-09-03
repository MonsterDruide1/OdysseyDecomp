#pragma once

#include "al/LiveActor/LiveActor.h"

namespace al {

    bool isClipped(const LiveActor*);
    void tryInitFixedModelGpuBuffer(const LiveActor*);
    void offUpdateMovementEffectAudioCollisionSensor(const LiveActor*);
    bool isDead(const LiveActor*);
    bool isAlive(const LiveActor*);
    bool isHideModel(const LiveActor*);
    void hideModelIfShow(const LiveActor*);
    void showModelIfHide(const LiveActor*);
    void setModelAlphaMask(const LiveActor*, float);
    void resetPosition(const LiveActor*);

}