#include "Library/LiveActor/LiveActorKit.h"

#include "Library/Area/AreaObjDirector.h"
#include "Library/Area/SwitchAreaDirector.h"
#include "Library/Camera/CameraDirector.h"
#include "Library/Collision/CollisionDirector.h"
#include "Library/Controller/PadRumbleDirector.h"
#include "Library/Draw/GraphicsSystemInfo.h"
#include "Library/Effect/EffectSystem.h"
#include "Library/Execute/ExecuteDirector.h"
#include "Library/Execute/ExecuteSystemInitInfo.h"
#include "Library/Execute/ExecuteTableHolderUpdate.h"
#include "Library/LiveActor/LiveActorGroup.h"
#include "Library/Model/ModelDisplayListController.h"
#include "Library/Model/ModelDrawBufferUpdater.h"
#include "Library/Model/ModelGroup.h"
#include "Library/Nature/NatureDirector.h"
#include "Library/Obj/DynamicDrawActor.h"
#include "Library/Player/PlayerHolder.h"
#include "Library/Resource/ActorResourceHolder.h"
#include "Library/Scene/ItemDirectorBase.h"
#include "Library/Screen/ScreenPointDirector.h"
#include "Library/Shader/ForwardRendering/ShaderHolder.h"
#include "Library/Shadow/ShadowDirector.h"
#include "Library/Shadow/ShadowKeeper.h"
#include "Library/Stage/StageSwitchDirector.h"
#include "Project/Clipping/ClippingDirector.h"
#include "Project/Execute/ExecuteAsyncExecutor.h"
#include "Project/Gravity/GravityHolder.h"
#include "Project/HitSensor/HitSensorDirector.h"

namespace al {

LiveActorKit::LiveActorKit(s32 maxActors, s32 maxPlayers) : mMaxActors(maxActors) {
    mLiveActorGroupAllActors = new LiveActorGroup("全てのアクター", maxActors);
    mPlayerHolder = new PlayerHolder(maxPlayers);
    mDynamicDrawActorGroup = new LiveActorGroup("独自描画グループ", 256);
}

LiveActorKit::~LiveActorKit() {
    if (mExecuteDirector) {
        delete mExecuteDirector;
        mExecuteDirector = nullptr;
    }
    if (mCameraDirector) {
        delete mCameraDirector;
        mCameraDirector = nullptr;
    }
    if (mClippingDirector) {
        delete mClippingDirector;
        mClippingDirector = nullptr;
    }
    if (mModelGroup) {
        delete mModelGroup;
        mModelGroup = nullptr;
    }
    if (mGraphicsSystemInfo)
        delete mGraphicsSystemInfo;
    if (mModelDrawBufferUpdater)
        delete mModelDrawBufferUpdater;

    if (mExecutorCore1)
        delete mExecutorCore1;
    if (mExecutorCore2)
        delete mExecutorCore2;

    if (mDynamicDrawActorGroup)
        for (s32 i = 0; i < mDynamicDrawActorGroup->getActorCount(); i++)
            ((DynamicDrawActor*)mDynamicDrawActorGroup->getActor(i))->finalize();

    if (mEffectSystem) {
        mEffectSystem->endScene();
        mEffectSystem->setCameraDirector(nullptr);
        if (mEffectSystem)
            mEffectSystem->setGraphicsSystemInfo(nullptr);
    }
}

void LiveActorKit::init(s32 maxCameras) {
    mDrawBufferDirector = new DrawBufferDirector();

    ExecuteSystemInitInfo info{};
    mExecuteDirector = new ExecuteDirector(mMaxActors);
    info.drawCtx = mSceneDrawContext;
    mExecuteDirector->init(info);

    mModelDrawBufferUpdater = new ModelDrawBufferUpdater(mExecuteDirector);
    mExecutorCore1 = new ExecuteAsyncExecutorUpdate(mExecuteDirector, "ビュー更新(コア1)", 1);
    mExecutorCore2 = new ExecuteAsyncExecutorUpdate(mExecuteDirector, "ビュー更新(コア2)", 2);
    mAreaObjDirector = new AreaObjDirector();
    mCollisionDirector = new CollisionDirector(mExecuteDirector);
    mCameraDirector = new CameraDirector(maxCameras);

    if (mEffectSystem) {
        mEffectSystem->setCameraDirector(mCameraDirector);
        mEffectSystem->initScene();
    }

    mPadRumbleDirector = new PadRumbleDirector(mPlayerHolder, mCameraDirector);
    mClippingDirector = new ClippingDirector(mMaxActors, mAreaObjDirector, mPlayerHolder,
                                             mCameraDirector->getSceneCameraInfo());
    registerExecutorUser(mClippingDirector, mExecuteDirector, "クリッピング");
    mStageSwitchDirector = new StageSwitchDirector(mExecuteDirector);
    mScreenPointDirector = new ScreenPointDirector();
    mModelGroup = new ModelGroup(mMaxActors);
    mModelDisplayListController = new ModelDisplayListController(mModelGroup);
    mActorResourceHolder = new ActorResourceHolder(mMaxActors);
}

void LiveActorKit::initGraphics(const GraphicsInitArg& graphics) {
    mGraphicsSystemInfo = new (0x10) GraphicsSystemInfo();
    mGraphicsSystemInfo->init(graphics, mAreaObjDirector, mExecuteDirector, mEffectSystem,
                              mPlayerHolder, mCameraDirector->getSceneCameraInfo(),
                              ShaderHolder::instance());

    if (mModelDisplayListController)
        mModelDisplayListController->setGraphicsSystemInfo(mGraphicsSystemInfo);
}

void LiveActorKit::initHitSensorDirector() {
    mHitSensorDirector = new HitSensorDirector(mExecuteDirector);
}

void LiveActorKit::initGravityHolder() {
    mGravityHolder = new GravityHolder();
    mGravityHolder->init();
}

void LiveActorKit::initShadowDirector() {}

void LiveActorKit::initEffectSystem() {
    mEffectSystem = EffectSystem::initializeSystem(mGraphicsSystemInfo->getDrawContext(), nullptr);
}

void LiveActorKit::initSwitchAreaDirector(s32 val1, s32 val2) {
    mSwitchAreaDirector =
        SwitchAreaDirector::tryCreate(mAreaObjDirector, mPlayerHolder, mCameraDirector, val1, val2);
}

void LiveActorKit::initNatureDirector() {
    mNatureDirector = new NatureDirector();
    mNatureDirector->init();
}

void LiveActorKit::endInit() {
    mCollisionDirector->endInit();
    mClippingDirector->endInit(mAreaObjDirector);
    mAreaObjDirector->endInit();
    mCameraDirector->endInit(mPlayerHolder);

    if (mEffectSystem)
        mEffectSystem->startScene(mExecuteDirector);

    if (mSwitchAreaDirector)
        mSwitchAreaDirector->endInit();

    if (mItemDirector)
        mItemDirector->endInit();

    if (mShadowDirector)
        mShadowDirector->endInit();

    if (mNatureDirector && mAreaObjDirector) {
        AreaObjGroup* waterArea = mAreaObjDirector->getAreaObjGroup("WaterArea");
        mNatureDirector->setWaterAreaGroup(waterArea);
    }

    if (mEffectSystem)
        mEffectSystem->setGraphicsSystemInfo(mGraphicsSystemInfo);

    mGraphicsSystemInfo->endInit();

    if (mEffectSystem)
        mEffectSystem->endInit();

    for (s32 i = 0; i < mLiveActorGroupAllActors->getActorCount(); i++) {
        LiveActor* actor = mLiveActorGroupAllActors->getActor(i);
        actor->initAfterPlacement();
        if (actor->getShadowKeeper())
            actor->getShadowKeeper()->initAfterPlacement(mGraphicsSystemInfo);
    }

    mGraphicsSystemInfo->initAfterPlacement();
    mExecuteDirector->createExecutorListTable();
}

void LiveActorKit::update(const char* unk) {
    clearGraphicsRequest();

    if (mPadRumbleDirector)
        mPadRumbleDirector->update();

    if (mExecuteDirector)
        mExecuteDirector->execute(unk);

    updateGraphics();

    if (mAreaObjDirector)
        mAreaObjDirector->update();

    if (mSwitchAreaDirector)
        mSwitchAreaDirector->update();
}

void LiveActorKit::clearGraphicsRequest() {
    if (mGraphicsSystemInfo)
        mGraphicsSystemInfo->clearGraphicsRequest();
}

void LiveActorKit::updateGraphics() {
    mExecutorCore1->executeAsync();
    mExecutorCore2->executeAsync();

    if (mGraphicsSystemInfo)
        mGraphicsSystemInfo->updateGraphics();
}

void LiveActorKit::preDrawGraphics() {
    if (mGraphicsSystemInfo)
        mGraphicsSystemInfo->preDrawGraphics(mCameraDirector->getSceneCameraInfo());
}

}  // namespace al
