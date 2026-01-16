#pragma once

#include "Library/LiveActor/ActorSceneInfo.h"

namespace al {
class ActorFactory;
class ActorResourceHolder;
class AreaObjDirector;
class AudioDirector;
class CameraDirector;
class ClippingDirector;
class CollisionDirector;
class DemoDirector;
class EffectSystemInfo;
class ExecuteDirector;
class GameDataHolderBase;
class GamePadSystem;
class GraphicsSystemInfo;
class GravityHolder;
class HitSensorDirector;
class ItemDirectorBase;
class LayoutInitInfo;
class LiveActor;
class LiveActorGroup;
struct ModelDrawBufferCounter;
class ModelGroup;
class NatureDirector;
class PadRumbleDirector;
class PlacementInfo;
class PlayerHolder;
class SceneMsgCtrl;
class SceneObjHolder;
class SceneStopCtrl;
class ScreenCoverCtrl;
class ScreenPointDirector;
class ShadowDirector;
class StageSwitchDirector;
class ViewIdHolder;

struct ActorInitInfo {
    ActorInitInfo();

    void initNew(const PlacementInfo* pPlacementInfo, const LayoutInitInfo* pLayoutInitInfo,
                 LiveActorGroup* pAllActorsGroup, const ActorFactory* pActorFactory,
                 ActorResourceHolder* pActorResourceHolder, AreaObjDirector* pAreaObjDirector,
                 AudioDirector* pAudioDirector, CameraDirector* pCameraDirector,
                 ClippingDirector* pClippingDirector, CollisionDirector* pCollisionDirector,
                 DemoDirector* pDemoDirector, EffectSystemInfo* pEffectSystemInfo,
                 ExecuteDirector* pExecuteDirector, GameDataHolderBase* pGameDataHolder,
                 GravityHolder* pGravityHolder, HitSensorDirector* pHitSensorDirector,
                 ItemDirectorBase* pItemDirector, NatureDirector* pNatureDirector,
                 const GamePadSystem* pGamePadSystem, PadRumbleDirector* pPadRumbleDirector,
                 PlayerHolder* pPlayerHolder, SceneObjHolder* pSceneObjHolder,
                 SceneMsgCtrl* pSceneMsgCtrl, SceneStopCtrl* pSceneStopCtrl,
                 ScreenCoverCtrl* pScreenCoverCtrl, ScreenPointDirector* pScreenPointDirector,
                 ShadowDirector* pShadowDirector, StageSwitchDirector* pStageSwitchDirector,
                 ModelGroup* pModelGroup, GraphicsSystemInfo* pGraphicsSystemInfo,
                 ModelDrawBufferCounter* pModelDrawBufferCounter, LiveActorGroup* pKitDrawingGroup);
    void initViewIdSelf(const PlacementInfo* pInfo, const ActorInitInfo& actorInfo);
    void copyHostInfo(const ActorInitInfo& actorInfo, const PlacementInfo* pInfo);
    void initViewIdHost(const PlacementInfo* pInfo, const ActorInitInfo& actorInfo);
    void initViewIdHostActor(const ActorInitInfo& actorInfo, const LiveActor* actor);
    void initNoViewId(const PlacementInfo* pInfo, const ActorInitInfo& actorInfo);

    LiveActorGroup* kitDrawingGroup = nullptr;
    const PlacementInfo* placementInfo = nullptr;
    const LayoutInitInfo* layoutInitInfo = nullptr;
    ActorSceneInfo actorSceneInfo = {};
    LiveActorGroup* allActorsGroup = nullptr;
    const ActorFactory* actorFactory = nullptr;
    ActorResourceHolder* actorResourceHolder = nullptr;
    AudioDirector* audioDirector = nullptr;
    EffectSystemInfo* effectSystemInfo = nullptr;
    ExecuteDirector* executeDirector = nullptr;
    HitSensorDirector* hitSensorDirector = nullptr;
    ScreenPointDirector* screenPointDirector = nullptr;
    StageSwitchDirector* stageSwitchDirector = nullptr;
    const ViewIdHolder* viewIdHolder = nullptr;
};
}  // namespace al
