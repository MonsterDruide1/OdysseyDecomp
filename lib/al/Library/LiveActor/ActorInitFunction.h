#pragma once

#include <math/seadMatrix.h>
#include <prim/seadSafeString.h>

namespace sead {
class Color4f;
}

namespace alNerveFunction {
class NerveActionCollector;
}

namespace al {
struct ActorInitInfo;
class ActorResource;
class ByamlIter;
class HitSensor;
class LiveActor;
class Nerve;
class PlacementInfo;
class Resource;
class ShadowMaskBase;

void initActorSceneInfo(LiveActor* actor, const ActorInitInfo& info);
void initExecutorUpdate(LiveActor* actor, const ActorInitInfo& info, const char* listName);
void initExecutorDraw(LiveActor* actor, const ActorInitInfo& info, const char* listName);
void initExecutorPlayer(LiveActor* actor, const ActorInitInfo& info);
void initExecutorPlayerPreMovement(LiveActor* actor, const ActorInitInfo& info);
void initExecutorPlayerMovement(LiveActor* actor, const ActorInitInfo& info);
void initExecutorPlayerModel(LiveActor* actor, const ActorInitInfo& info);
void initExecutorPlayerDecoration(LiveActor* actor, const ActorInitInfo& info);
void initExecutorEnemy(LiveActor* actor, const ActorInitInfo& info);
void initExecutorEnemyMovement(LiveActor* actor, const ActorInitInfo& info);
void initExecutorEnemyDecoration(LiveActor* actor, const ActorInitInfo& info);
void initExecutorEnemyDecorationMovement(LiveActor* actor, const ActorInitInfo& info);
void initExecutorMapObj(LiveActor* actor, const ActorInitInfo& info);
void initExecutorMapObjMovement(LiveActor* actor, const ActorInitInfo& info);
void initExecutorMapObjDecoration(LiveActor* actor, const ActorInitInfo& info);
void initExecutorNpcDecoration(LiveActor* actor, const ActorInitInfo& info);
void initExecutorShadowVolume(LiveActor* actor, const ActorInitInfo& info);
void initExecutorShadowVolumeFillStencil(LiveActor* actor, const ActorInitInfo& info);
void initExecutorCollisionMapObjDecorationMovement(LiveActor* actor, const ActorInitInfo& info);
void initExecutorWatchObj(LiveActor* actor, const ActorInitInfo& info);
void initExecutorDebugMovement(LiveActor* actor, const ActorInitInfo& info);
void initExecutorModelUpdate(LiveActor* actor, const ActorInitInfo& info);
void initExecutorDrcAssistMovement(LiveActor* actor, const ActorInitInfo& info);
void initActorPoseTRSV(LiveActor* actor);
void initActorPoseTRMSV(LiveActor* actor);
void initActorPoseTRGMSV(LiveActor* actor);
void initActorPoseTFSV(LiveActor* actor);
void initActorPoseTFUSV(LiveActor* actor);
void initActorPoseTFGSV(LiveActor* actor);
void initActorPoseTQSV(LiveActor* actor);
void initActorPoseTQGSV(LiveActor* actor);
void initActorPoseTQGMSV(LiveActor* actor);
void initActorSRT(LiveActor* actor, const ActorInitInfo& info);
void initActorModelKeeper(LiveActor* actor, const ActorInitInfo& info,
                          const ActorResource* resource, s32 blendAnimMax);
void initActorModelKeeper(LiveActor* actor, const ActorInitInfo& info, const char* actorResource,
                          s32 blendAnimMax, const char* animResource);
void initActorModelKeeperByHost(LiveActor* actor, const LiveActor* host);
void initActorModelForceCubeMap(LiveActor* actor, const ActorInitInfo& info);
void initActorActionKeeper(LiveActor* actor, const ActorInitInfo& info,
                           const char* modelArchiveName, const char* suffix);
void initActorActionKeeper(LiveActor* actor, const ActorResource* resource,
                           const char* modelArchiveName, const char* suffix);
void initActorEffectKeeper(LiveActor* actor, const ActorInitInfo& info, const char* name);
void initActorSeKeeper(LiveActor* actor, const ActorInitInfo& info, const char* seName,
                       const sead::Vector3f* transPtr, const sead::Matrix34f* baseMtx);
void initActorSeKeeper(LiveActor* actor, const ActorInitInfo& info, const char* seName);
void initActorSeKeeperWithout3D(LiveActor* actor, const ActorInitInfo& info, const char* seName);
void initActorBgmKeeper(LiveActor* actor, const ActorInitInfo& info, const char* bgmName);
bool isInitializedBgmKeeper(LiveActor* actor);
void initHitReactionKeeper(LiveActor* actor, const char* suffix);
void initHitReactionKeeper(LiveActor* actor, const Resource* resource, const char* suffix);
void initActorParamHolder(LiveActor* actor, const char* suffix);
void initActorParamHolder(LiveActor* actor, const Resource* resource, const char* suffix);
void initDepthShadowMapCtrl(LiveActor* actor, const Resource* resource, const ActorInitInfo& info,
                            const char* suffix);
void initDepthShadowMapCtrlWithoutIter(LiveActor* actor, s32 size, bool isAppendSubActor);
void addDepthShadowMapInfo(const LiveActor* actor, const char* a1, s32 a2, s32 a3, s32 a4, f32 a5,
                           bool a6, const sead::Vector3f& a7, bool a8, const sead::Vector3f& a9,
                           const sead::Vector3f& a10, bool a11, const char* a12, s32 a13, bool a14,
                           f32 a15, f32 a16, f32 a17, bool a18, bool a19, f32 a20, s32 a21,
                           bool a22);
void declareUseDepthShadowMap(const LiveActor* actor, s32 num);
void createDepthShadowMap(const LiveActor* actor, const char* a1, s32 a2, s32 a3, s32 a4);
void initShadowMaskCtrl(LiveActor* actor, const ActorInitInfo& info, const ByamlIter& iter,
                        const char* unused);
void initShadowMaskCtrlWithoutInitFile(LiveActor* actor, const ActorInitInfo& info, s32 numMasks);
void createShadowMaskSphere(LiveActor* actor, const char* name, const char* jointName,
                            const char* drawCategory);
void createShadowMaskCube(LiveActor* actor, const char* name, const char* jointName,
                          const char* drawCategory, const sead::Color4f& color,
                          const sead::Vector3f& offset, f32 a2, f32 a3, f32 dropLength,
                          const sead::Vector3f& a5, f32 a6);
void createShadowMaskCylinder(LiveActor* actor, const char* name, const char* jointName,
                              const char* drawCategory, const sead::Color4f& color,
                              const sead::Vector3f& offset, f32 a2, f32 dropLength, f32 a3, f32 a5,
                              f32 a6);
void createShadowMaskCastOvalCylinder(LiveActor* actor, const char* name, const char* jointName,
                                      const char* drawCategory, const sead::Color4f& color,
                                      const sead::Vector3f& offset, const sead::Vector3f& scale,
                                      f32 dropLength, f32 expXZ, f32 expY, f32 distYBase);
void initActorCollision(LiveActor* actor, const sead::SafeString& filePath,
                        HitSensor* connectedSensor, const sead::Matrix34f* jointMtx);
void initActorCollisionWithResource(LiveActor* actor, const Resource* resource,
                                    const sead::SafeString& filePath, HitSensor* connectedSensor,
                                    const sead::Matrix34f* jointMtx, const char* suffix);
void initActorCollisionWithArchiveName(LiveActor* actor, const sead::SafeString& resourceName,
                                       const sead::SafeString& filePath, HitSensor* connectedSensor,
                                       const sead::Matrix34f* jointMtx);
void initActorCollisionWithFilePtr(LiveActor* actor, void* kcl, const void* byml,
                                   HitSensor* connectedSensor, const sead::Matrix34f* jointMtx,
                                   const char* specialPurpose, const char* optionalPurpose,
                                   s32 priority);
void initStageSwitch(LiveActor* actor, const ActorInitInfo& info);
void initActorItemKeeper(LiveActor* actor, const ActorInitInfo& info, const ByamlIter& iter);
bool initActorPrePassLightKeeper(LiveActor* actor, const Resource* resource,
                                 const ActorInitInfo& info, const char* suffix);
void initActorOcclusionKeeper(LiveActor* actor, const Resource* resource, const ActorInitInfo& info,
                              const char* fileSuffix);
void initSubActorKeeper(LiveActor* actor, const ActorInitInfo& info, const char* suffix,
                        s32 maxSubActors);
void initSubActorKeeperNoFile(LiveActor* actor, const ActorInitInfo& info, s32 maxSubActors);
void registerSubActor(LiveActor* actor, LiveActor* subActor);
void registerSubActorSyncClipping(LiveActor* actor, LiveActor* subActor);
void registerSubActorSyncClippingAndHide(LiveActor* actor, LiveActor* subActor);
void registerSubActorSyncAll(LiveActor* actor, LiveActor* subActor);
void setSubActorOffSyncClipping(LiveActor* actor);
void initScreenPointKeeper(LiveActor* actor, const Resource* resource, const ActorInitInfo& info,
                           const char* fileName);
void initScreenPointKeeperNoYaml(LiveActor* actor, s32 size);
void initActorMaterialCategory(LiveActor* actor, const ActorInitInfo& info, const char* a1);
}  // namespace al
