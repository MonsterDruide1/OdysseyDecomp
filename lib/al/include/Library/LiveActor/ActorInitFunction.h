#pragma once

#include <basis/seadTypes.h>
#include <gfx/seadDrawContext.h>
#include <math/seadBoundBox.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>
#include <prim/seadSafeString.h>

#include "Library/Nerve/NerveUtil.h"

namespace al {
class ActorInitInfo;
class ActorResource;
class AudioDirector;
class ByamlIter;
class CollisionDirector;
class GraphicsSystemInfo;
class LayoutInitInfo;
class LiveActor;
class LiveActorGroup;
class Resource;
class PlacementInfo;
class SceneCameraInfo;

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
void initActorPoseTRSV(LiveActor*);
void initActorPoseTRMSV(LiveActor*);
void initActorPoseTRGMSV(LiveActor*);
void initActorPoseTFSV(LiveActor*);
void initActorPoseTFUSV(LiveActor*);
void initActorPoseTFGSV(LiveActor*);
void initActorPoseTQSV(LiveActor*);
void initActorPoseTQGSV(LiveActor*);
void initActorPoseTQGMSV(LiveActor*);
void initActorSRT(LiveActor*, const ActorInitInfo&);
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
LiveActor* createLinksActorFromFactory(const ActorInitInfo&, const char*, s32);
void createLinksActorGroupFromFactory(const ActorInitInfo&, const char*, const char*);
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
