#pragma once

namespace al {
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

struct ActorSceneInfo {
    ActorSceneInfo();

    AreaObjDirector* mAreaObjDirector = nullptr;
    CameraDirector* mCameraDirector = nullptr;
    ClippingDirector* mClippingDirector = nullptr;
    CollisionDirector* mCollisionDirector = nullptr;
    DemoDirector* mDemoDirector = nullptr;
    GameDataHolderBase* mGameDataHolder = nullptr;
    GravityHolder* mGravityHolder = nullptr;
    ItemDirectorBase* mItemDirector = nullptr;
    NatureDirector* mNatureDirector = nullptr;
    const GamePadSystem* mGamePadSystem = nullptr;
    PadRumbleDirector* mPadRumbleDirector = nullptr;
    PlayerHolder* mPlayerHolder = nullptr;
    SceneObjHolder* mSceneObjHolder = nullptr;
    SceneStopCtrl* mSceneStopCtrl = nullptr;
    SceneMsgCtrl* mSceneMsgCtrl = nullptr;
    ScreenCoverCtrl* mScreenCoverCtrl = nullptr;
    ShadowDirector* mShadowDirector = nullptr;
    ModelGroup* mModelGroup = nullptr;
    GraphicsSystemInfo* mGraphicsSystemInfo = nullptr;
    ModelDrawBufferCounter* mModelDrawBufferCounter = nullptr;
};
}  // namespace al
