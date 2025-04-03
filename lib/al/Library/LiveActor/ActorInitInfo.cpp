#include "Library/LiveActor/ActorInitInfo.h"

#include "Library/Clipping/ViewIdHolder.h"

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
}  // namespace al
