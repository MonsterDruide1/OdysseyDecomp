#include "Library/LiveActor/ActorInitInfo.h"

#include "Library/Clipping/ClippingActorHolder.h"
#include "Library/Clipping/ClippingActorInfo.h"
#include "Library/Clipping/ViewIdHolder.h"
#include "Library/LiveActor/LiveActor.h"
#include "Project/Clipping/ClippingDirector.h"

namespace al {
ActorInitInfo::ActorInitInfo() = default;

void ActorInitInfo::initNew(
    const PlacementInfo* pPlacementInfo, const LayoutInitInfo* pLayoutInitInfo,
    LiveActorGroup* pAllActorsGroup, const ActorFactory* pActorFactory,
    ActorResourceHolder* pActorResourceHolder, AreaObjDirector* pAreaObjDirector,
    AudioDirector* pAudioDirector, CameraDirector* pCameraDirector,
    ClippingDirector* pClippingDirector, CollisionDirector* pCollisionDirector,
    DemoDirector* pDemoDirector, EffectSystemInfo* pEffectSystemInfo,
    ExecuteDirector* pExecuteDirector, GameDataHolderBase* pGameDataHolder,
    GravityHolder* pGravityHolder, HitSensorDirector* pHitSensorDirector,
    ItemDirectorBase* pItemDirector, NatureDirector* pNatureDirector,
    const GamePadSystem* pGamePadSystem, PadRumbleDirector* pPadRumbleDirector,
    PlayerHolder* pPlayerHolder, SceneObjHolder* pSceneObjHolder, SceneMsgCtrl* pSceneMsgCtrl,
    SceneStopCtrl* pSceneStopCtrl, ScreenCoverCtrl* pScreenCoverCtrl,
    ScreenPointDirector* pScreenPointDirector, ShadowDirector* pShadowDirector,
    StageSwitchDirector* pStageSwitchDirector, ModelGroup* pModelGroup,
    GraphicsSystemInfo* pGraphicsSystemInfo, ModelDrawBufferCounter* pModelDrawBufferCounter,
    LiveActorGroup* pKitDrawingGroup) {
    actorSceneInfo.modelDrawBufferCounter = pModelDrawBufferCounter;
    allActorsGroup = pAllActorsGroup;
    actorFactory = pActorFactory;
    actorResourceHolder = pActorResourceHolder;
    effectSystemInfo = pEffectSystemInfo;
    executeDirector = pExecuteDirector;
    layoutInitInfo = pLayoutInitInfo;
    actorSceneInfo.areaObjDirector = pAreaObjDirector;
    hitSensorDirector = pHitSensorDirector;
    stageSwitchDirector = pStageSwitchDirector;
    screenPointDirector = pScreenPointDirector;
    actorSceneInfo.cameraDirector = pCameraDirector;
    actorSceneInfo.clippingDirector = pClippingDirector;
    actorSceneInfo.gameDataHolder = pGameDataHolder;
    actorSceneInfo.gravityHolder = pGravityHolder;
    actorSceneInfo.collisionDirector = pCollisionDirector;
    actorSceneInfo.demoDirector = pDemoDirector;
    actorSceneInfo.itemDirector = pItemDirector;
    actorSceneInfo.natureDirector = pNatureDirector;
    actorSceneInfo.gamePadSystem = pGamePadSystem;
    actorSceneInfo.padRumbleDirector = pPadRumbleDirector;
    actorSceneInfo.playerHolder = pPlayerHolder;
    actorSceneInfo.sceneObjHolder = pSceneObjHolder;
    actorSceneInfo.sceneStopCtrl = pSceneStopCtrl;
    actorSceneInfo.sceneMsgCtrl = pSceneMsgCtrl;
    actorSceneInfo.screenCoverCtrl = pScreenCoverCtrl;
    actorSceneInfo.shadowDirector = pShadowDirector;
    actorSceneInfo.modelGroup = pModelGroup;
    actorSceneInfo.graphicsSystemInfo = pGraphicsSystemInfo;
    kitDrawingGroup = pKitDrawingGroup;
    audioDirector = pAudioDirector;
    placementInfo = pPlacementInfo;
    viewIdHolder = ViewIdHolder::tryCreate(*pPlacementInfo);
}

void ActorInitInfo::initViewIdSelf(const PlacementInfo* pInfo, const ActorInitInfo& actorInfo) {
    viewIdHolder = ViewIdHolder::tryCreate(*pInfo);
    copyHostInfo(actorInfo, pInfo);
}

void ActorInitInfo::copyHostInfo(const ActorInitInfo& actorInfo, const PlacementInfo* pInfo) {
    placementInfo = pInfo;
    layoutInitInfo = actorInfo.layoutInitInfo;
    allActorsGroup = actorInfo.allActorsGroup;
    actorFactory = actorInfo.actorFactory;
    actorSceneInfo = actorInfo.actorSceneInfo;
    actorResourceHolder = actorInfo.actorResourceHolder;
    audioDirector = actorInfo.audioDirector;
    executeDirector = actorInfo.executeDirector;
    effectSystemInfo = actorInfo.effectSystemInfo;
    hitSensorDirector = actorInfo.hitSensorDirector;
    screenPointDirector = actorInfo.screenPointDirector;
    stageSwitchDirector = actorInfo.stageSwitchDirector;
    kitDrawingGroup = actorInfo.kitDrawingGroup;
}

void ActorInitInfo::initViewIdHost(const PlacementInfo* pInfo, const ActorInitInfo& actorInfo) {
    viewIdHolder = actorInfo.viewIdHolder;
    copyHostInfo(actorInfo, pInfo);
}

void ActorInitInfo::initViewIdHostActor(const ActorInitInfo& actorInfo, const LiveActor* actor) {
    viewIdHolder = actor->getSceneInfo()
                       ->clippingDirector->getClippingActorHolder()
                       ->find(actor)
                       ->viewIdHolder;

    copyHostInfo(actorInfo, actorInfo.placementInfo);
}

void ActorInitInfo::initNoViewId(const PlacementInfo* pInfo, const ActorInitInfo& actorInfo) {
    copyHostInfo(actorInfo, pInfo);
}
}  // namespace al
