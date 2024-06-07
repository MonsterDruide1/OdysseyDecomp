#pragma once

#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>
#include <prim/seadSafeString.h>

#include "Library/LiveActor/LiveActor.h"
#include "Library/Nerve/Nerve.h"

namespace al {

template <typename T>
LiveActor* createActorFunction(const char* actorName) {
    return new T(actorName);
}

const sead::Vector3f& getCameraUp(const LiveActor*, s32);

const sead::Vector3f& getVelocity(const LiveActor* actor);
sead::Vector3f* getVelocityPtr(LiveActor* actor);
sead::Vector3f* getGravityPtr(LiveActor* actor);

bool isClipped(const LiveActor*);
void tryInitFixedModelGpuBuffer(const LiveActor*);
bool isDead(const LiveActor*);
bool isAlive(const LiveActor*);
bool isHideModel(const LiveActor*);
void hideModelIfShow(LiveActor*);
void showModelIfHide(LiveActor*);
void setModelAlphaMask(const LiveActor*, f32);
void onSyncClippingSubActor(LiveActor*, const LiveActor*);
void onSyncHideSubActor(LiveActor*, const LiveActor*);
void onSyncAlphaMaskSubActor(LiveActor*, const LiveActor*);
void setMaterialProgrammable(LiveActor*);
bool isExistModel(const LiveActor*);
bool isViewDependentModel(const LiveActor*);
void calcViewModel(const LiveActor*);
void calcQuat(sead::Quatf*, const LiveActor*);
void calcJointFrontDir(sead::Vector3f*, const LiveActor*, const char*);

f32* findActorParamF32(const LiveActor*, const char*);
s32* findActorParamS32(const LiveActor*, const char*);

void setNerveAtActionEnd(LiveActor*, const Nerve*);
void initActorWithArchiveName(LiveActor* actor, const ActorInitInfo& info, const sead::SafeString&,
                              const char* suffix);
void initMapPartsActor(LiveActor* actor, const ActorInitInfo& info, const char* suffix);

void initJointControllerKeeper(const LiveActor*, s32);
void initJointGlobalQuatController(const LiveActor*, const sead::Quatf*, const char*);

bool isEffectEmitting(const IUseEffectKeeper*, const char*);

void registActorToDemoInfo(LiveActor* actor, const ActorInitInfo& info);

}  // namespace al

namespace rs {

const sead::Vector3f& getPlayerPos(const al::LiveActor*);

}
