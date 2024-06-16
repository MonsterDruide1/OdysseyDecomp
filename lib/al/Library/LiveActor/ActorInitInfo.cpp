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
    mActorSceneInfo.mModelDrawBufferCounter = modelDrawBufferCounter;
    mAllActorsGroup = allActorsGroup;
    mActorFactory = actorFactory;
    mActorResourceHolder = actorResourceHolder;
    mEffectSystemInfo = effectSystemInfo;
    mExecuteDirector = executeDirector;
    mLayoutInitInfo = layoutInitInfo;
    mActorSceneInfo.mAreaObjDirector = areaObjDirector;
    mHitSensorDirector = hitSensorDirector;
    mStageSwitchDirector = stageSwitchDirector;
    mScreenPointDirector = screenPointDirector;
    mActorSceneInfo.mCameraDirector = cameraDirector;
    mActorSceneInfo.mClippingDirector = clippingDirector;
    mActorSceneInfo.mGameDataHolder = gameDataHolder;
    mActorSceneInfo.mGravityHolder = gravityHolder;
    mActorSceneInfo.mCollisionDirector = collisionDirector;
    mActorSceneInfo.mDemoDirector = demoDirector;
    mActorSceneInfo.mItemDirector = itemDirector;
    mActorSceneInfo.mNatureDirector = natureDirector;
    mActorSceneInfo.mGamePadSystem = gamePadSystem;
    mActorSceneInfo.mPadRumbleDirector = padRumbleDirector;
    mActorSceneInfo.mPlayerHolder = playerHolder;
    mActorSceneInfo.mSceneObjHolder = sceneObjHolder;
    mActorSceneInfo.mSceneStopCtrl = sceneStopCtrl;
    mActorSceneInfo.mSceneMsgCtrl = sceneMsgCtrl;
    mActorSceneInfo.mScreenCoverCtrl = screenCoverCtrl;
    mActorSceneInfo.mShadowDirector = shadowDirector;
    mActorSceneInfo.mModelGroup = modelGroup;
    mActorSceneInfo.mGraphicsSystemInfo = graphicsSystemInfo;
    mKitDrawingGroup = kitDrawingGroup;
    mAudioDirector = audioDirector;
    mPlacementInfo = placementInfo;
    mViewIdHolder = ViewIdHolder::tryCreate(*placementInfo);
}
}  // namespace al
