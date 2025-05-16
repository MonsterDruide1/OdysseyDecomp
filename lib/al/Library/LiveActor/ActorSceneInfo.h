#pragma once

namespace al {
class AreaObjDirector;
class CameraDirector;
class ClippingDirector;
class CollisionDirector;
class DemoDirector;
class GameDataHolderBase;
class GamePadSystem;
class GraphicsSystemInfo;
class GravityHolder;
class ItemDirectorBase;
struct ModelDrawBufferCounter;
class ModelGroup;
class NatureDirector;
class PadRumbleDirector;
class PlayerHolder;
class SceneMsgCtrl;
class ScreenCoverCtrl;
class SceneObjHolder;
class SceneStopCtrl;
class ShadowDirector;

struct ActorSceneInfo {
    ActorSceneInfo();

    AreaObjDirector* areaObjDirector = nullptr;
    CameraDirector* cameraDirector = nullptr;
    ClippingDirector* clippingDirector = nullptr;
    CollisionDirector* collisionDirector = nullptr;
    DemoDirector* demoDirector = nullptr;
    GameDataHolderBase* gameDataHolder = nullptr;
    GravityHolder* gravityHolder = nullptr;
    ItemDirectorBase* itemDirector = nullptr;
    NatureDirector* natureDirector = nullptr;
    const GamePadSystem* gamePadSystem = nullptr;
    PadRumbleDirector* padRumbleDirector = nullptr;
    PlayerHolder* playerHolder = nullptr;
    SceneObjHolder* sceneObjHolder = nullptr;
    SceneStopCtrl* sceneStopCtrl = nullptr;
    SceneMsgCtrl* sceneMsgCtrl = nullptr;
    ScreenCoverCtrl* screenCoverCtrl = nullptr;
    ShadowDirector* shadowDirector = nullptr;
    ModelGroup* modelGroup = nullptr;
    GraphicsSystemInfo* graphicsSystemInfo = nullptr;
    ModelDrawBufferCounter* modelDrawBufferCounter = nullptr;
};
}  // namespace al
