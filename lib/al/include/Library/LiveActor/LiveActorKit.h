#pragma once

#include "Library/HostIO/HioNode.h"

namespace agl {
class DrawContext;
}

namespace al {
class GraphicsInitArg;
class ActorResourceHolder;
class AreaObjDirector;
class ExecuteDirector;
class GravityHolder;
class EffectSystem;
class GraphicsSystemInfo;
class ModelDrawBufferUpdater;
class ExecuteAsyncExecutor;
class ExecuteAsyncExecutor;
class ModelDisplayListController;
class LiveActorGroup;
class CameraDirector;
class ClippingDirector;
class CollisionDirector;
class ItemDirectorBase;
class PlayerHolder;
class HitSensorDirector;
class ScreenPointDirector;
class ShadowDirector;
class StageSwitchDirector;
class SwitchAreaDirector;
class LiveActorGroup;
class DemoDirector;
class GamePadSystem;
class PadRumbleDirector;
class NatureDirector;
class ModelGroup;

class LiveActorKit : public HioNode {
public:
    class DrawBufferDirector {
    public:
        DrawBufferDirector() = default;

    private:
        int mMaxDrawBuffer = 3;
        int mCurrentDrawBuffer = 0;
    };

    LiveActorKit(int maxActors, int maxPlayers);
    virtual ~LiveActorKit();

    void init(int maxCameras);
    void initGraphics(const GraphicsInitArg& graphics);
    void initHitSensorDirector();
    void initGravityHolder();
    void initShadowDirector();
    void initEffectSystem();
    void initSwitchAreaDirector(int, int);
    void initNatureDirector();
    void endInit();
    void update(const char*);
    void clearGraphicsRequest();
    void updateGraphics();
    void preDrawGraphics();

private:
    int mMaxActors;
    ActorResourceHolder* mActorResourceHolder = nullptr;
    AreaObjDirector* mAreaObjDirector = nullptr;
    ExecuteDirector* mExecuteDirector = nullptr;
    GravityHolder* mGravityHolder = nullptr;
    EffectSystem* mEffectSystem = nullptr;
    GraphicsSystemInfo* mGraphicsSystemInfo = nullptr;
    DrawBufferDirector* mDrawBufferDirector = nullptr;
    ModelDrawBufferUpdater* mModelDrawBufferUpdater = nullptr;
    ExecuteAsyncExecutor* mExecutor1 = nullptr;
    ExecuteAsyncExecutor* mExecutor2 = nullptr;
    ModelDisplayListController* mModelDisplayListController = nullptr;
    agl::DrawContext* mSceneDrawContext = nullptr;
    LiveActorGroup* mDynamicDrawActorGroup = nullptr;
    CameraDirector* mCameraDirector = nullptr;
    ClippingDirector* mClippingDirector = nullptr;
    CollisionDirector* mCollisionDirector = nullptr;
    ItemDirectorBase* mItemDirector = nullptr;
    PlayerHolder* mPlayerHolder = nullptr;
    HitSensorDirector* mHitSensorDirector = nullptr;
    ScreenPointDirector* mScreenPointDirector = nullptr;
    ShadowDirector* mShadowDirector = nullptr;
    StageSwitchDirector* mStageSwitchDirector = nullptr;
    SwitchAreaDirector* mSwitchAreaDirector = nullptr;
    LiveActorGroup* mLiveActorGroupAllActors = nullptr;
    DemoDirector* mDemoDirector = nullptr;
    GamePadSystem* mGamePadSystem = nullptr;
    PadRumbleDirector* mPadRumbleDirector = nullptr;
    NatureDirector* mNatureDirector = nullptr;
    ModelGroup* mModelGroup = nullptr;
};

}  // namespace al
