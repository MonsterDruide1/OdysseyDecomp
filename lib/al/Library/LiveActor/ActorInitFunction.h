#pragma once

#include <basis/seadTypes.h>
#include <gfx/seadColor.h>
#include <gfx/seadDrawContext.h>
#include <math/seadBoundBox.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>
#include <prim/seadSafeString.h>

namespace alNerveFunction {
class NerveActionCollector;
}

namespace al {
class LiveActor;
class Resource;
class HitSensor;
class ActorInitInfo;
class ByamlIter;
class ActorResource;
class PlacementInfo;
class LiveActorGroup;
class Nerve;
class ShadowMaskBase;
class SceneCameraInfo;
class GraphicsSystemInfo;
class CollisionDirector;
class AudioDirector;
class LayoutInitInfo;

void initActorPoseTRSV(LiveActor* actor);
void initActorPoseTRMSV(LiveActor* actor);
void initActorPoseTRGMSV(LiveActor* actor);
void initActorPoseTFSV(LiveActor* actor);
void initActorPoseTFUSV(LiveActor* actor);
void initActorPoseTFGSV(LiveActor* actor);
void initActorPoseTQSV(LiveActor* actor);
void initActorPoseTQGSV(LiveActor* actor);
void initActorPoseTQGMSV(LiveActor* actor);
void initActor(LiveActor* actor, const ActorInitInfo& initInfo);

// WARNING: This function doesn't have a symbol and shouldn't be called from mods
void initActorWithArchivePath(LiveActor* actor, const ActorInitInfo& info,
                              const sead::SafeString& archiveFolder,
                              const sead::SafeString& archiveName, const char* unk = nullptr);

void initActorSuffix(LiveActor* actor, const ActorInitInfo& initInfo, const char* suffix);
void initActorChangeModel(LiveActor* actor, const ActorInitInfo& initInfo);
void initActorChangeModelSuffix(LiveActor* actor, const ActorInitInfo& initInfo,
                                const char* suffix);
void initActorWithArchiveName(LiveActor* actor, const ActorInitInfo& initInfo,
                              const sead::SafeString& archiveName, const char* suffix);
void initChildActorWithArchiveNameWithPlacementInfo(LiveActor* actor, const ActorInitInfo& initInfo,
                                                    const sead::SafeString& archiveName,
                                                    const char* suffix);
void initChildActorWithArchiveNameNoPlacementInfo(LiveActor* actor, const ActorInitInfo& initInfo,
                                                  const sead::SafeString& archiveName,
                                                  const char* suffix);
void initMapPartsActor(LiveActor* actor, const ActorInitInfo& initInfo, const char* suffix);
void initLinksActor(LiveActor* actor, const ActorInitInfo& initInfo, const char* suffix,
                    s32 linkIndex);

void createChildLinkSimpleActor(const char* actorName, const char* archiveName,
                                const ActorInitInfo& initInfo, bool alive);
void createChildLinkMapPartsActor(const char* actorName, const char* archiveName,
                                  const ActorInitInfo& initInfo, s32 linkIndex, bool alive);

ActorInitInfo* createLinksPlayerActorInfo(LiveActor* actor, const ActorInitInfo& initInfo);

void initShadowMaskCtrl(LiveActor*, const ActorInitInfo&, const ByamlIter&, const char*);
void initShadowMaskCtrlWithoutInitFile(LiveActor*, const ActorInitInfo&, s32);
ShadowMaskBase* createShadowMaskSphere(LiveActor*, const char*, const char*, const char*);
ShadowMaskBase* createShadowMaskCube(LiveActor*, const char*, const char*, const char*,
                                     const sead::Color4f&, const sead::Vector3f&, f32, f32, f32,
                                     const sead::Vector3f&, f32);
ShadowMaskBase* createShadowMaskCylinder(LiveActor*, const char*, const char*, const char*,
                                         const sead::Color4f&, const sead::Vector3f&, f32, f32, f32,
                                         f32, f32);
ShadowMaskBase* createShadowMaskCastOvalCylinder(LiveActor*, const char*, const char*, const char*,
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
void initActorPrePassLightKeeper(LiveActor*, const Resource*, const ActorInitInfo&, const char*);
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

void initActorSceneInfo(LiveActor*, const ActorInitInfo&);
void initExecutorUpdate(LiveActor*, const ActorInitInfo&, const char*);
void initExecutorDraw(LiveActor*, const ActorInitInfo&, const char*);
void initExecutorPlayer(LiveActor*, const ActorInitInfo&);
void initExecutorPlayerPreMovement(LiveActor*, const ActorInitInfo&);
void initExecutorPlayerMovement(LiveActor*, const ActorInitInfo&);
void initExecutorPlayerModel(LiveActor*, const ActorInitInfo&);
void initExecutorPlayerDecoration(LiveActor*, const ActorInitInfo&);
void initExecutorEnemy(LiveActor*, const ActorInitInfo&);
void initExecutorEnemyMovement(LiveActor*, const ActorInitInfo&);
void initExecutorEnemyDecoration(LiveActor*, const ActorInitInfo&);
void initExecutorEnemyDecorationMovement(LiveActor*, const ActorInitInfo&);
void initExecutorMapObj(LiveActor*, const ActorInitInfo&);
void initExecutorMapObjMovement(LiveActor*, const ActorInitInfo&);
void initExecutorMapObjDecoration(LiveActor*, const ActorInitInfo&);
void initExecutorNpcDecoration(LiveActor*, const ActorInitInfo&);
void initExecutorShadowVolume(LiveActor*, const ActorInitInfo&);
void initExecutorShadowVolumeFillStencil(LiveActor*, const ActorInitInfo&);
void initExecutorCollisionMapObjDecorationMovement(LiveActor*, const ActorInitInfo&);
void initExecutorWatchObj(LiveActor*, const ActorInitInfo&);
void initExecutorDebugMovement(LiveActor*, const ActorInitInfo&);
void initExecutorModelUpdate(LiveActor*, const ActorInitInfo&);
void initExecutorDrcAssistMovement(LiveActor*, const ActorInitInfo&);
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
void isInitializedBgmKeeper(LiveActor*);
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

void initCreateActorWithPlacementInfo(LiveActor*, const ActorInitInfo&);
void initCreateActorWithPlacementInfo(LiveActor*, const ActorInitInfo&, const PlacementInfo&);
void initCreateActorNoPlacementInfo(LiveActor*, const ActorInitInfo&);
void initCreateActorNoPlacementInfoNoViewId(LiveActor*, const ActorInitInfo&);
void createPlacementActorFromFactory(const ActorInitInfo&, const PlacementInfo*);
LiveActor* createLinksActorFromFactory(const ActorInitInfo& info, const char* linkName,
                                       s32 linkNum);
LiveActorGroup* createLinksActorGroupFromFactory(const ActorInitInfo&, const char*, const char*);
void tryCreateLinksActorGroupFromFactory(const ActorInitInfo&, const char*, const char*);
void tryCreateLinksActorFromFactorySingle(const ActorInitInfo&, const char*);
void createAndRegisterLinksActorFromFactory(LiveActorGroup*, const ActorInitInfo&, const char*);
void makeMapPartsModelName(sead::BufferedSafeString*, sead::BufferedSafeString*,
                           const PlacementInfo&);
void makeMapPartsModelName(sead::BufferedSafeString*, sead::BufferedSafeString*,
                           const ActorInitInfo&);

void initNerve(LiveActor* actor, const Nerve* nerve, s32 maxStates);
void initNerveAction(LiveActor* actor, const char* actionName,
                     alNerveFunction::NerveActionCollector* collector, s32 maxStates);

bool trySyncStageSwitchAppear(LiveActor* actor);
void initActorSRT(LiveActor* actor, const ActorInitInfo& info);
bool trySyncStageSwitchAppearAndKill(LiveActor* actor);
void initActorCollisionWithResource(LiveActor* actor, const Resource* res,
                                    const sead::SafeString& collisionFileName, HitSensor* hitSensor,
                                    const sead::Matrix34f* joinMtx, const char* suffix);
void initExecutorCollisionMapObjDecorationMovement(LiveActor* actor, const ActorInitInfo& info);
void initActorModelKeeperByHost(LiveActor* actor, const LiveActor* parent);
void initExecutorDraw(LiveActor* actor, const ActorInitInfo& info, const char* executorDrawName);
void initChildActorWithArchiveNameWithPlacementInfo(LiveActor* actor, const ActorInitInfo& info,
                                                    const sead::SafeString& archiveName,
                                                    const char* suffix);
bool tryListenStageSwitchKill(LiveActor* actor);
void initActorWithArchiveName(LiveActor* actor, const ActorInitInfo& info,
                              const sead::SafeString& archiveName, const char* suffix);
void initActorEffectKeeper(LiveActor* actor, const ActorInitInfo& info, const char* name);
void initExecutorUpdate(LiveActor* actor, const ActorInitInfo& info, const char* name);
void initNerveAction(LiveActor* actor, const char* actionName,
                     alNerveFunction::NerveActionCollector* collector, s32 step);
void initMapPartsActor(LiveActor* actor, const ActorInitInfo& info, const char* name);
bool trySyncStageSwitchKill(LiveActor* actor);
void initLinksActor(LiveActor* actor, const ActorInitInfo& info, const char* linkName, s32 linkNum);
bool trySyncStageSwitchAppear(LiveActor* actor);
bool trySyncStageSwitchKill(LiveActor* actor);
bool trySyncStageSwitchAppearAndKill(LiveActor* actor);
bool tryListenStageSwitchAppear(LiveActor* actor);
bool tryListenStageSwitchKill(LiveActor* actor);

void syncSensorScaleY(LiveActor* actor);
void syncSensorAndColliderScaleY(LiveActor* actor);

void setMaterialCode(LiveActor* actor, const char*);
void initMaterialCode(LiveActor* actor, const ActorInitInfo& initInfo);

bool tryAddDisplayRotate(LiveActor* actor, const ActorInitInfo& initInfo);
bool tryAddDisplayOffset(LiveActor* actor, const ActorInitInfo& initInfo);
bool tryAddDisplayScale(LiveActor* actor, const ActorInitInfo& initInfo);

PlacementInfo* getPlacementInfo(const ActorInitInfo& initInfo);
const LayoutInitInfo* getLayoutInitInfo(const ActorInitInfo& initInfo);
AudioDirector* getAudioDirector(const ActorInitInfo& initInfo);
CollisionDirector* getCollisionDirectorFromInfo(const ActorInitInfo& initInfo);
const SceneCameraInfo* getSceneCameraInfoFromInfo(const ActorInitInfo& initInfo);
GraphicsSystemInfo* getGraphicsSystemInfo(const ActorInitInfo& initInfo);
sead::DrawContext* getDrawContext(const ActorInitInfo& initInfo);

void getActorRecourseDataF32(f32*, LiveActor*, const char*, const char*);
void getActorRecourseDataString(const char**, LiveActor*, const char*, const char*);
void getActorRecourseDataV3f(sead::Vector3f*, LiveActor* actor, const char*, const char*);
void getActorRecourseDataBox3f(sead::BoundBox3f* box, LiveActor* actor, const char*, const char*);

void createPartsEffectGroup(LiveActor* actor, const ActorInitInfo& initInfo, s32);
}  // namespace al
