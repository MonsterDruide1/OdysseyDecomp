#pragma once

#include <math/seadMatrix.h>
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
    const LiveActor* getSubActor(const LiveActor*, const char*); //NOTE: unknown return type
    sead::Matrix34f* getJointMtxPtr(const LiveActor*, const char*); //return type might be const
    void onSyncClippingSubActor(LiveActor*, const LiveActor*);
    void onSyncHideSubActor(LiveActor*, const LiveActor*);
    void onSyncAlphaMaskSubActor(LiveActor*, const LiveActor*);
    void setMaterialProgrammable(al::LiveActor*);
    void startAction(al::LiveActor*, char const*);

    float* findActorParamF32(const al::LiveActor*, const char*);
    int* findActorParamS32(const al::LiveActor*, const char*);

}