#pragma once

#include <basis/seadTypes.h>
#include <gfx/seadColor.h>
#include <gfx/seadDrawContext.h>
#include <math/seadBoundBox.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>
#include <prim/seadSafeString.h>
#include "Library/LiveActor/ActorSceneInfo.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementInfo.h"

namespace al {
class LiveActor;
class LiveActorGroup;
class PlacementInfo;
class LayoutInitInfo;
class AreaObjDirector;
class CameraDirector;
class ClippingDirector;
class CollisionDirector;
class DemoDirector;
class GameDataHolderBase;
class GravityHolder;
class ItemDirectorBase;
class NatureDirector;
class GamePadSystem;
class PadRumbleDirector;
class PlayerHolder;
class SceneObjHolder;
class SceneStopCtrl;
class SceneMsgCtrl;
class ScreenCoverCtrl;
class ShadowDirector;
class ModelGroup;
class GraphicsSystemInfo;
class ModelDrawBufferCounter;
class ActorFactory;
class ActorResource;
class ActorResourceHolder;
class AudioDirector;
class EffectSystemInfo;
class ExecuteDirector;
class HitSensorDirector;
class StageSwitchDirector;
class ScreenPointDirector;
class ViewIdHolder;
class SceneCameraInfo;
class Nerve;
class PlacementInfo;
class Resource;
class LayoutInitInfo;
class AudioDirector;
class CollisionDirector;
class GraphicsSystemInfo;
class ShadowMaskBase;
class HitSensor;

class ActorInitInfo {
public:
    ActorInitInfo();
    void initNew(const PlacementInfo*, const LayoutInitInfo*, LiveActorGroup*, const ActorFactory*,
                 ActorResourceHolder*, AreaObjDirector*, AudioDirector*, CameraDirector*,
                 ClippingDirector*, CollisionDirector*, DemoDirector*, EffectSystemInfo*,
                 ExecuteDirector*, GameDataHolderBase*, GravityHolder*, HitSensorDirector*,
                 ItemDirectorBase*, NatureDirector*, const GamePadSystem*, PadRumbleDirector*,
                 PlayerHolder*, SceneObjHolder*, SceneMsgCtrl*, SceneStopCtrl*, ScreenCoverCtrl*,
                 ScreenPointDirector*, ShadowDirector*, StageSwitchDirector*, ModelGroup*,
                 GraphicsSystemInfo*, ModelDrawBufferCounter*, LiveActorGroup*);
    void initViewIdSelf(const PlacementInfo*, const ActorInitInfo&);
    void copyHostInfo(const ActorInitInfo&, const PlacementInfo*);
    void initViewIdHost(const PlacementInfo*, const ActorInitInfo&);
    void initViewIdHostActor(const ActorInitInfo&, const LiveActor*);
    void initNoViewId(const PlacementInfo*, const ActorInitInfo&);

    const PlacementInfo& getPlacementInfo() const { return *mPlacementInfo; }

private:
    LiveActorGroup* mKitDrawingGroup;
    const PlacementInfo* mPlacementInfo;
    const LayoutInitInfo* mLayoutInitInfo;
    ActorSceneInfo mActorSceneInfo;
    LiveActorGroup* mAllActorsGroup;
    const ActorFactory* mActorFactory;
    ActorResourceHolder* mActorResourceHolder;
    AudioDirector* mAudioDirector;
    EffectSystemInfo* mEffectSystemInfo;
    ExecuteDirector* mExecuteDirector;
    HitSensorDirector* mHitSensorDirector;
    ScreenPointDirector* mScreenPointDirector;
    StageSwitchDirector* mStageSwitchDirector;
    ViewIdHolder* mViewIdHolder;
};

void initActor(LiveActor* actor, const ActorInitInfo& initInfo);
void initActorSuffix(LiveActor* actor, const ActorInitInfo& initInfo, const char* suffix);
void initActorChangeModel(al::LiveActor* actor, const al::ActorInitInfo& initInfo);
void initActorChangeModelSuffix(al::LiveActor* actor, const al::ActorInitInfo& initInfo,
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

void initShadowMaskCtrl(LiveActor*, const ActorInitInfo&, ByamlIter const&, const char*);
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
void initActorCollisionWithFilePtr(LiveActor*, void*, void const*, HitSensor*,
                                   const sead::Matrix34f*, const char*, const char*, s32);
void initStageSwitch(LiveActor*, const ActorInitInfo&);
void initActorItemKeeper(LiveActor*, const ActorInitInfo&, ByamlIter const&);
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

// TODO: move these
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
void createLinksActorFromFactory(const ActorInitInfo&, const char*, s32);
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
