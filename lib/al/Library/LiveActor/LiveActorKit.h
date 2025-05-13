#pragma once

#include <basis/seadTypes.h>

#include "Library/HostIO/HioNode.h"

namespace agl {
class DrawContext;
}

namespace al {
class ActorResourceHolder;
class AreaObjDirector;
class CameraDirector;
class ClippingDirector;
class CollisionDirector;
class DemoDirector;
class EffectSystem;
class ExecuteAsyncExecutorUpdate;
class ExecuteDirector;
class GamePadSystem;
struct GraphicsInitArg;
class GraphicsSystemInfo;
class GravityHolder;
class HitSensorDirector;
class ItemDirectorBase;
class LiveActorGroup;
class ModelDisplayListController;
class ModelDrawBufferUpdater;
class ModelGroup;
class NatureDirector;
class PadRumbleDirector;
class PlayerHolder;
class ScreenPointDirector;
class ShadowDirector;
class StageSwitchDirector;
class SwitchAreaDirector;

struct ModelDrawBufferCounter {
    s32 maxDrawBuffer = 3;
    s32 currentDrawBuffer = 0;
};

class LiveActorKit : public HioNode {
public:
    LiveActorKit(s32 maxActors, s32 maxPlayers);
    virtual ~LiveActorKit();

    void init(s32 maxCameras);
    void initGraphics(const GraphicsInitArg& graphics);
    void initHitSensorDirector();
    void initGravityHolder();
    void initShadowDirector();
    void initEffectSystem();
    void initSwitchAreaDirector(s32, s32);
    void initNatureDirector();
    void endInit();
    void update(const char*);
    void clearGraphicsRequest();
    void updateGraphics();
    void preDrawGraphics();

    ActorResourceHolder* getActorResourceHolder() const { return mActorResourceHolder; }

    AreaObjDirector* getAreaObjDirector() const { return mAreaObjDirector; }

    ExecuteDirector* getExecuteDirector() const { return mExecuteDirector; }

    GravityHolder* getGravityHolder() const { return mGravityHolder; }

    EffectSystem* getEffectSystem() const { return mEffectSystem; }

    GraphicsSystemInfo* getGraphicsSystemInfo() const { return mGraphicsSystemInfo; }

    ModelDrawBufferCounter* getModelDrawBufferCounter() const { return mModelDrawBufferCounter; }

    ModelDrawBufferUpdater* getModelDrawBufferUpdater() const { return mModelDrawBufferUpdater; }

    ExecuteAsyncExecutorUpdate* getExecutorCore1() const { return mExecutorCore1; }

    ExecuteAsyncExecutorUpdate* getExecutorCore2() const { return mExecutorCore2; }

    ModelDisplayListController* getModelDisplayListController() const {
        return mModelDisplayListController;
    }

    agl::DrawContext* getSceneDrawContext() const { return mSceneDrawContext; }

    LiveActorGroup* getDynamicDrawActorGroup() const { return mDynamicDrawActorGroup; }

    CameraDirector* getCameraDirector() const { return mCameraDirector; }

    ClippingDirector* getClippingDirector() const { return mClippingDirector; }

    CollisionDirector* getCollisionDirector() const { return mCollisionDirector; }

    ItemDirectorBase* getItemDirector() const { return mItemDirector; }

    PlayerHolder* getPlayerHolder() const { return mPlayerHolder; }

    HitSensorDirector* getHitSensorDirector() const { return mHitSensorDirector; }

    ScreenPointDirector* getScreenPointDirector() const { return mScreenPointDirector; }

    ShadowDirector* getShadowDirector() const { return mShadowDirector; }

    StageSwitchDirector* getStageSwitchDirector() const { return mStageSwitchDirector; }

    SwitchAreaDirector* getSwitchAreaDirector() const { return mSwitchAreaDirector; }

    LiveActorGroup* getLiveActorGroupAllActors() const { return mLiveActorGroupAllActors; }

    DemoDirector* getDemoDirector() const { return mDemoDirector; }

    GamePadSystem* getGamePadSystem() const { return mGamePadSystem; }

    PadRumbleDirector* getPadRumbleDirector() const { return mPadRumbleDirector; }

    NatureDirector* getNatureDirector() const { return mNatureDirector; }

    ModelGroup* getModelGroup() const { return mModelGroup; }

    void setItemDirector(ItemDirectorBase* itemDirector) { mItemDirector = itemDirector; }

    void setDemoDirector(DemoDirector* demoDirector) { mDemoDirector = demoDirector; }

    void setSceneDrawContext(agl::DrawContext* drawContext) { mSceneDrawContext = drawContext; }

    void setGamePadSystem(GamePadSystem* gamePadSystem) { mGamePadSystem = gamePadSystem; }

    void setEffectSystem(EffectSystem* effectSystem) { mEffectSystem = effectSystem; }

private:
    s32 mMaxActors;
    ActorResourceHolder* mActorResourceHolder = nullptr;
    AreaObjDirector* mAreaObjDirector = nullptr;
    ExecuteDirector* mExecuteDirector = nullptr;
    GravityHolder* mGravityHolder = nullptr;
    EffectSystem* mEffectSystem = nullptr;
    GraphicsSystemInfo* mGraphicsSystemInfo = nullptr;
    ModelDrawBufferCounter* mModelDrawBufferCounter = nullptr;
    ModelDrawBufferUpdater* mModelDrawBufferUpdater = nullptr;
    ExecuteAsyncExecutorUpdate* mExecutorCore1 = nullptr;
    ExecuteAsyncExecutorUpdate* mExecutorCore2 = nullptr;
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
