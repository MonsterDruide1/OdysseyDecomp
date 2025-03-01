#include "Library/LiveActor/ActorInitInfo.h"

#include "Library/Clipping/ViewIdHolder.h"

namespace al {
ActorInitInfo::ActorInitInfo() = default;

void ActorInitInfo::initNew(
    const PlacementInfo* placementInfo, const LayoutInitInfo* layoutInitInfo,
    LiveActorGroup* allActorsGroup, const ActorFactory* actorFactory,
    ActorResourceHolder* actorResourceHolder, AreaObjDirector* areaObjDirector,
    AudioDirector* audioDirector, CameraDirector* cameraDirector,
    ClippingDirector* clippingDirector, CollisionDirector* collisionDirector,
    DemoDirector* demoDirector, EffectSystemInfo* effectSystemInfo,
    ExecuteDirector* executeDirector, GameDataHolderBase* gameDataHolder,
    GravityHolder* gravityHolder, HitSensorDirector* hitSensorDirector,
    ItemDirectorBase* itemDirector, NatureDirector* natureDirector,
    const GamePadSystem* gamePadSystem, PadRumbleDirector* padRumbleDirector,
    PlayerHolder* playerHolder, SceneObjHolder* sceneObjHolder, SceneMsgCtrl* sceneMsgCtrl,
    SceneStopCtrl* sceneStopCtrl, ScreenCoverCtrl* screenCoverCtrl,
    ScreenPointDirector* screenPointDirector, ShadowDirector* shadowDirector,
    StageSwitchDirector* stageSwitchDirector, ModelGroup* modelGroup,
    GraphicsSystemInfo* graphicsSystemInfo, ModelDrawBufferCounter* modelDrawBufferCounter,
    LiveActorGroup* kitDrawingGroup) {
    mActorSceneInfo.modelDrawBufferCounter = modelDrawBufferCounter;
    mAllActorsGroup = allActorsGroup;
    mActorFactory = actorFactory;
    mActorResourceHolder = actorResourceHolder;
    mEffectSystemInfo = effectSystemInfo;
    mExecuteDirector = executeDirector;
    mLayoutInitInfo = layoutInitInfo;
    mActorSceneInfo.areaObjDirector = areaObjDirector;
    mHitSensorDirector = hitSensorDirector;
    mStageSwitchDirector = stageSwitchDirector;
    mScreenPointDirector = screenPointDirector;
    mActorSceneInfo.cameraDirector = cameraDirector;
    mActorSceneInfo.clippingDirector = clippingDirector;
    mActorSceneInfo.gameDataHolder = gameDataHolder;
    mActorSceneInfo.gravityHolder = gravityHolder;
    mActorSceneInfo.collisionDirector = collisionDirector;
    mActorSceneInfo.demoDirector = demoDirector;
    mActorSceneInfo.itemDirector = itemDirector;
    mActorSceneInfo.natureDirector = natureDirector;
    mActorSceneInfo.gamePadSystem = gamePadSystem;
    mActorSceneInfo.padRumbleDirector = padRumbleDirector;
    mActorSceneInfo.playerHolder = playerHolder;
    mActorSceneInfo.sceneObjHolder = sceneObjHolder;
    mActorSceneInfo.sceneStopCtrl = sceneStopCtrl;
    mActorSceneInfo.sceneMsgCtrl = sceneMsgCtrl;
    mActorSceneInfo.screenCoverCtrl = screenCoverCtrl;
    mActorSceneInfo.shadowDirector = shadowDirector;
    mActorSceneInfo.modelGroup = modelGroup;
    mActorSceneInfo.graphicsSystemInfo = graphicsSystemInfo;
    mKitDrawingGroup = kitDrawingGroup;
    mAudioDirector = audioDirector;
    mPlacementInfo = placementInfo;
    mViewIdHolder = ViewIdHolder::tryCreate(*placementInfo);
}
}  // namespace al
