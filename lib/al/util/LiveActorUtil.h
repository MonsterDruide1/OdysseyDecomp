#pragma once

#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>
#include <prim/seadSafeString.h>
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
    bool isActionEnd(const al::LiveActor*);
    void calcQuat(sead::Quatf*, const al::LiveActor*);
    sead::Vector3f* getTrans(const al::LiveActor*);
    sead::Vector3f* getGravity(const al::LiveActor*);
    sead::Vector3f* getFront(const al::LiveActor*);
    void calcJointFrontDir(sead::Vector3f*, const al::LiveActor*, const char*);

    float* findActorParamF32(const al::LiveActor*, const char*);
    int* findActorParamS32(const al::LiveActor*, const char*);

    void setNerveAtActionEnd(al::LiveActor*, const al::Nerve*);
    void initActorWithArchiveName(al::LiveActor*, const al::ActorInitInfo&, const sead::SafeString&, const char*);
    void initJointControllerKeeper(const al::LiveActor*, int);
    void initJointGlobalQuatController(const al::LiveActor*, const sead::Quatf*, const char*);

    bool isEffectEmitting(const IUseEffectKeeper*, const char*);

}

namespace rs {

    sead::Vector3f* getPlayerPos(const al::LiveActor*);

}
