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
void initExecutorUpdate(LiveActor* actor, const ActorInitInfo& info, const char*);
void initExecutorDraw(LiveActor* actor, const ActorInitInfo& info, const char*);
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
void initActorModelKeeper(LiveActor*, const ActorInitInfo&, const ActorResource*, s32);
void initActorModelKeeper(LiveActor*, const ActorInitInfo&, const char*, s32, const char*);
void initActorModelKeeperByHost(LiveActor*, const LiveActor*);
void initActorModelForceCubeMap(LiveActor*, const ActorInitInfo&);
void initActorActionKeeper(LiveActor*, const ActorInitInfo&, const char*, const char*);
void initActorActionKeeper(LiveActor*, const ActorResource*, const char*, const char*);
void initActorEffectKeeper(LiveActor*, const ActorInitInfo&, const char*);
void initActorSeKeeper(LiveActor*, const ActorInitInfo&, const char*, const sead::Vector3f*,
                       const sead::Matrix34f*);
void initActorSeKeeper(LiveActor*, const ActorInitInfo&, const char*);
void initActorSeKeeperWithout3D(LiveActor*, const ActorInitInfo&, const char*);
void initActorBgmKeeper(LiveActor*, const ActorInitInfo&, const char*);
bool isInitializedBgmKeeper(LiveActor*);
void initHitReactionKeeper(LiveActor*, const char*);
void initHitReactionKeeper(LiveActor*, const Resource*, const char*);
void initActorParamHolder(LiveActor*, const char*);
void initActorParamHolder(LiveActor*, const Resource*, const char*);
void initDepthShadowMapCtrl(LiveActor*, const Resource*, const ActorInitInfo&, const char*);
void initDepthShadowMapCtrlWithoutIter(LiveActor*, s32, bool);
void addDepthShadowMapInfo(const LiveActor*, const char*, s32, s32, s32, f32, bool,
                           const sead::Vector3f&, bool, const sead::Vector3f&,
                           const sead::Vector3f&, bool, const char*, s32, bool, f32, f32, f32, bool,
                           bool, f32, s32, bool);
void declareUseDepthShadowMap(const LiveActor*, s32);
void createDepthShadowMap(const LiveActor*, const char*, s32, s32, s32);
void initShadowMaskCtrl(LiveActor*, const ActorInitInfo&, const ByamlIter&, const char*);
void initShadowMaskCtrlWithoutInitFile(LiveActor*, const ActorInitInfo&, s32);
void createShadowMaskSphere(LiveActor*, const char*, const char*, const char*);
void createShadowMaskCube(LiveActor*, const char*, const char*, const char*, const sead::Color4f&,
                          const sead::Vector3f&, f32, f32, f32, const sead::Vector3f&, f32);
void createShadowMaskCylinder(LiveActor*, const char*, const char*, const char*,
                              const sead::Color4f&, const sead::Vector3f&, f32, f32, f32, f32, f32);
void createShadowMaskCastOvalCylinder(LiveActor*, const char*, const char*, const char*,
                                      const sead::Color4f&, const sead::Vector3f&,
                                      const sead::Vector3f&, f32, f32, f32, f32);
void initActorCollision(LiveActor*, const sead::SafeString&, HitSensor*, const sead::Matrix34f*);
void initActorCollisionWithResource(LiveActor*, const Resource*, const sead::SafeString&,
                                    HitSensor*, const sead::Matrix34f*, const char*);
void initActorCollisionWithArchiveName(LiveActor*, const sead::SafeString&, const sead::SafeString&,
                                       HitSensor*, const sead::Matrix34f*);
void initActorCollisionWithFilePtr(LiveActor*, void*, const void*, HitSensor*,
                                   const sead::Matrix34f*, const char*, const char*, s32);
void initStageSwitch(LiveActor*, const ActorInitInfo&);
void initActorItemKeeper(LiveActor*, const ActorInitInfo&, const ByamlIter&);
bool initActorPrePassLightKeeper(LiveActor*, const Resource*, const ActorInitInfo&, const char*);
void initActorOcclusionKeeper(LiveActor*, const Resource*, const ActorInitInfo&, const char*);
void initSubActorKeeper(LiveActor*, const ActorInitInfo&, const char*, s32);
void initSubActorKeeperNoFile(LiveActor*, const ActorInitInfo&, s32);
void registerSubActor(LiveActor*, LiveActor*);
void registerSubActorSyncClipping(LiveActor*, LiveActor*);
void registerSubActorSyncClippingAndHide(LiveActor*, LiveActor*);
void registerSubActorSyncAll(LiveActor*, LiveActor*);
void setSubActorOffSyncClipping(LiveActor*);
void initScreenPointKeeper(LiveActor*, const Resource*, const ActorInitInfo&, const char*);
void initScreenPointKeeperNoYaml(LiveActor*, s32);
void initActorMaterialCategory(LiveActor*, const ActorInitInfo&, const char*);
}  // namespace al
