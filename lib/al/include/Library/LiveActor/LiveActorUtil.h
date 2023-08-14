#pragma once

#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>
#include <prim/seadSafeString.h>
#include "Library/LiveActor/LiveActor.h"
#include "Library/Nerve/Nerve.h"

namespace al {

template <typename T>
al::LiveActor* createActorFunction(const char* actorName);

const sead::Vector3f& getCameraUp(const al::LiveActor*, s32);

const sead::Vector3f& getVelocity(const al::LiveActor* actor);
sead::Vector3f* getVelocityPtr(al::LiveActor* actor);
sead::Vector3f* getGravityPtr(al::LiveActor* actor);

bool isClipped(const LiveActor*);
void tryInitFixedModelGpuBuffer(const LiveActor*);
bool isDead(const LiveActor*);
bool isAlive(const LiveActor*);
bool isHideModel(const LiveActor*);
void hideModelIfShow(al::LiveActor*);
void showModelIfHide(al::LiveActor*);
void setModelAlphaMask(const LiveActor*, f32);
void onSyncClippingSubActor(LiveActor*, const LiveActor*);
void onSyncHideSubActor(LiveActor*, const LiveActor*);
void onSyncAlphaMaskSubActor(LiveActor*, const LiveActor*);
void setMaterialProgrammable(al::LiveActor*);
bool isExistModel(const al::LiveActor*);
bool isViewDependentModel(const al::LiveActor*);
void calcViewModel(const al::LiveActor*);
void calcQuat(sead::Quatf*, const al::LiveActor*);
void calcJointFrontDir(sead::Vector3f*, const al::LiveActor*, const char*);

f32* findActorParamF32(const al::LiveActor*, const char*);
s32* findActorParamS32(const al::LiveActor*, const char*);

void setNerveAtActionEnd(al::LiveActor*, const al::Nerve*);
void initActorWithArchiveName(al::LiveActor* actor, const al::ActorInitInfo& info,
                              const sead::SafeString&, const char* suffix);
void initMapPartsActor(al::LiveActor* actor, const al::ActorInitInfo& info, const char* suffix);

void initJointControllerKeeper(const al::LiveActor*, s32);
void initJointGlobalQuatController(const al::LiveActor*, const sead::Quatf*, const char*);

bool isEffectEmitting(const IUseEffectKeeper*, const char*);

void registActorToDemoInfo(al::LiveActor* actor, const al::ActorInitInfo& info);

}  // namespace al

namespace rs {

sead::Vector3f* getPlayerPos(const al::LiveActor*);

}
