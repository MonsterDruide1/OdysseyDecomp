#pragma once

#include <basis/seadTypes.h>
#include <gfx/seadDrawContext.h>
#include <math/seadBoundBox.h>
#include <math/seadVector.h>
#include <prim/seadSafeString.h>
#include "al/Library/LiveActor/ActorSceneInfo.h"
#include "al/Library/Nerve/NerveUtil.h"
#include "al/Library/Placement/PlacementInfo.h"

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
class LayoutInitInfo;
class AudioDirector;
class CollisionDirector;
class GraphicsSystemInfo;

class ActorInitInfo {
public:
    ActorInitInfo();
    void initNew(const PlacementInfo*, const LayoutInitInfo*, LiveActorGroup*, const ActorFactory*, ActorResourceHolder*, AreaObjDirector*,
                 AudioDirector*, CameraDirector*, ClippingDirector*, CollisionDirector*, DemoDirector*, EffectSystemInfo*, ExecuteDirector*,
                 GameDataHolderBase*, GravityHolder*, HitSensorDirector*, ItemDirectorBase*, NatureDirector*, const GamePadSystem*,
                 PadRumbleDirector*, PlayerHolder*, SceneObjHolder*, SceneMsgCtrl*, SceneStopCtrl*, ScreenCoverCtrl*, ScreenPointDirector*,
                 ShadowDirector*, StageSwitchDirector*, ModelGroup*, GraphicsSystemInfo*, ModelDrawBufferCounter*, LiveActorGroup*);
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
void initActorWithArchiveName(LiveActor* actor, const ActorInitInfo& initInfo, const sead::SafeString& archiveName, const char* suffix);
void initChildActorWithArchiveNameWithPlacementInfo(LiveActor* actor, const ActorInitInfo& initInfo, const sead::SafeString& archiveName,
                                                    const char* suffix);
void initChildActorWithArchiveNameNoPlacementInfo(LiveActor* actor, const ActorInitInfo& initInfo, const sead::SafeString& archiveName,
                                                  const char* suffix);
void initMapPartsActor(LiveActor* actor, const ActorInitInfo& initInfo, const char* suffix);
void initLinksActor(LiveActor* actor, const ActorInitInfo& initInfo, const char* suffix, s32 linkIndex);

void createChildLinkSimpleActor(const char* actorName, const char* archiveName, const ActorInitInfo& initInfo, bool alive);
void createChildLinkMapPartsActor(const char* actorName, const char* archiveName, const ActorInitInfo& initInfo, s32 linkIndex, bool alive);

ActorInitInfo* createLinksPlayerActorInfo(LiveActor* actor, const ActorInitInfo& initInfo);

void initNerve(LiveActor* actor, const Nerve* nerve, s32 maxStates);
void initNerveAction(LiveActor* actor, const char* actionName, alNerveFunction::NerveActionCollector* collector, s32 maxStates);

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
