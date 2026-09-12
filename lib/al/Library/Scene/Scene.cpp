#include "Library/Scene/Scene.h"

#include "Library/Audio/AudioDirector.h"
#include "Library/Draw/GraphicsSystemInfo.h"
#include "Library/Layout/LayoutSystem.h"
#include "Library/LiveActor/LiveActorKit.h"
#include "Library/Model/ModelDisplayListController.h"
#include "Library/Model/ModelOcclusionCullingDirector.h"
#include "Library/Model/SkyDirector.h"
#include "Library/Scene/DemoDirector.h"
#include "Library/Scene/SceneMsgCtrl.h"
#include "Library/Scene/SceneObjHolder.h"
#include "Library/Scene/SceneStopCtrl.h"
#include "Library/Scene/SceneUtil.h"
#include "Library/Screen/ScreenCoverCtrl.h"
#include "Library/Stage/StageResourceKeeper.h"
#include "Library/System/GameSystemInfo.h"
#include "Project/LiveActor/ActorExecuteFunction.h"

namespace al {

Scene::Scene(const char* name) : NerveExecutor(name), mName(name) {}

Scene::~Scene() {
    if (mAudioDirector)
        mAudioDirector->finalize();
    if (mStageResourceKeeper)
        delete mStageResourceKeeper;
    mStageResourceKeeper = nullptr;
    if (mLiveActorKit)
        delete mLiveActorKit;
    mLiveActorKit = nullptr;
    if (mLayoutKit)
        delete mLayoutKit;
    mLayoutKit = nullptr;
}

void Scene::appear() {
    mIsAlive = true;
    if (mLiveActorKit)
        executeUpdateDrawBuffer(mLiveActorKit);
}

void Scene::kill() {
    mIsAlive = false;
}

void Scene::movement() {
    if (mLiveActorKit)
        incrementDrawBufferCounter(mLiveActorKit);
    if (mSceneStopCtrl)
        mSceneStopCtrl->update();
    if (mScreenCoverCtrl)
        mScreenCoverCtrl->update();
    if (mLiveActorKit)
        waitUpdateDrawBuffer(mLiveActorKit);
    if (mAudioDirector)
        mAudioDirector->updatePre();

    updateNerve();
    control();

    if (mAudioDirector)
        mAudioDirector->updatePost();
    if (mLiveActorKit) {
        waitUpdateCalcView(mLiveActorKit);
        if (mLiveActorKit->getModelDisplayListController())
            mLiveActorKit->getModelDisplayListController()->update();
        executeUpdateDrawBuffer(mLiveActorKit);
        ModelOcclusionCullingDirector* modelOcclusionCullingDirector =
            mLiveActorKit->getGraphicsSystemInfo()->getModelOcclusionCullingDirector();
        if (modelOcclusionCullingDirector)
            modelOcclusionCullingDirector->calc();
    }
}

CameraDirector* Scene::getCameraDirector() const {
    return mLiveActorKit->getCameraDirector();
}

void Scene::initializeAsync(const SceneInitInfo& info) {
    init(info);
}

void Scene::initDrawSystemInfo(const SceneInitInfo& info) {
    mDrawSystemInfo = info.gameSystemInfo->drawSystemInfo;
}

void Scene::initSceneObjHolder(SceneObjHolder* sceneObjHolder) {
    mSceneObjHolder = sceneObjHolder;
}

void Scene::initAndLoadStageResource(const char* stageName, s32 scenarioNo) {
    mStageResourceKeeper = new StageResourceKeeper();
    mStageResourceKeeper->initAndLoadResource(stageName, scenarioNo);
}

void Scene::initLiveActorKit(const SceneInitInfo& info, s32 maxActors, s32 maxPlayers,
                             s32 maxCameras) {
    initLiveActorKitImpl(info, maxActors, maxPlayers, maxCameras);

    DrawSystemInfo* drawSystemInfo = info.gameSystemInfo->drawSystemInfo;
    GraphicsInitArg graphicsInitArg = {getSceneDrawContext(this),
                                       drawSystemInfo->dockedRenderBuffer};
    graphicsInitArg._3c = maxCameras;
    graphicsInitArg._10 = maxCameras << graphicsInitArg.atmosScatterViewNum;
    mLiveActorKit->initGraphics(graphicsInitArg);

    mLiveActorKit->getGraphicsSystemInfo()->setApplicationMessageReceiver(
        info.gameSystemInfo->applicationMessageReceiver);
}

void Scene::initLiveActorKitImpl(const SceneInitInfo& info, s32 maxActors, s32 maxPlayers,
                                 s32 maxCameras) {
    mLiveActorKit = new LiveActorKit(maxActors, maxPlayers);
    mLiveActorKit->setSceneDrawContext(getSceneDrawContext(this));
    mLiveActorKit->setGamePadSystem(info.gameSystemInfo->gamePadSystem);
    mLiveActorKit->setEffectSystem(info.gameSystemInfo->effectSystem);
    mLiveActorKit->init(maxCameras);
}

void Scene::initLiveActorKitWithGraphics(const GraphicsInitArg& graphicsInitArg,
                                         const SceneInitInfo& info, s32 maxActors, s32 maxPlayers,
                                         s32 maxCameras) {
    initLiveActorKitImpl(info, maxActors, maxPlayers, maxCameras);
    mLiveActorKit->initGraphics(graphicsInitArg);
    mLiveActorKit->getGraphicsSystemInfo()->setApplicationMessageReceiver(
        info.gameSystemInfo->applicationMessageReceiver);
}

void Scene::initLayoutKit(const SceneInitInfo& info) {
    mLayoutKit = new LayoutKit(info.gameSystemInfo->fontHolder);
    mLayoutKit->setEffectSystem(info.gameSystemInfo->effectSystem);
    mLayoutKit->setLayoutSystem(info.gameSystemInfo->layoutSystem);
    mLayoutKit->setDrawContext(getSceneDrawContext(this));
}

void Scene::initSceneStopCtrl() {
    mSceneStopCtrl = new SceneStopCtrl();
}

void Scene::initSceneMsgCtrl() {
    mSceneMsgCtrl = new SceneMsgCtrl();
}

void Scene::initScreenCoverCtrl() {
    mScreenCoverCtrl = new ScreenCoverCtrl();
}

void Scene::endInit(const ActorInitInfo& info) {
    if (mAudioDirector)
        mAudioDirector->initAfterInitPlacement();
    if (mSceneObjHolder)
        mSceneObjHolder->initAfterPlacementSceneObj(info);
    if (mLiveActorKit) {
        GraphicsSystemInfo* graphicsSystemInfo = mLiveActorKit->getGraphicsSystemInfo();
        if (graphicsSystemInfo && graphicsSystemInfo->getSkyDirector())
            graphicsSystemInfo->getSkyDirector()->init(info);
        if (mLiveActorKit) {
            if (mLiveActorKit->getDemoDirector())
                mLiveActorKit->getDemoDirector()->endInit(info);
            mLiveActorKit->endInit();
        }
    }
    if (mLayoutKit)
        mLayoutKit->endInit();
}

}  // namespace al
