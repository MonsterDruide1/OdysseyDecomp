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
    AreaObjDirector* mAreaObjDirector;
    CameraDirector* mCameraDirector;
    ClippingDirector* mClippingDirector;
    CollisionDirector* mCollisionDirector;
    DemoDirector* mDemoDirector;
    GameDataHolderBase* mGameDataHolder;
    GravityHolder* mGravityHolder;
    ItemDirectorBase* mItemDirector;
    NatureDirector* mNatureDirector;
    const GamePadSystem* mGamePadSystem;
    PadRumbleDirector* mPadRumbleDirector;
    PlayerHolder* mPlayerHolder;
    SceneObjHolder* mSceneObjHolder;
    SceneStopCtrl* mSceneStopCtrl;
    SceneMsgCtrl* mSceneMsgCtrl;
    ScreenCoverCtrl* mScreenCoverCtrl;
    ShadowDirector* mShadowDirector;
    ModelGroup* mModelGroup;
    GraphicsSystemInfo* mGraphicsSystemInfo;
    ModelDrawBufferCounter* mModelDrawBufferCounter;
};
}  // namespace al
