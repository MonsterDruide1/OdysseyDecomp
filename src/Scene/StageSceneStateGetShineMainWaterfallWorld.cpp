#include "Scene/StageSceneStateGetShineMainWaterfallWorld.h"

#include "Library/Bgm/BgmLineFunction.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Scene/SceneUtil.h"

#include "MapObj/WaterfallWorldFallDownBridge.h"
#include "Scene/QuestInfoHolder.h"
#include "Scene/StageScene.h"
#include "Scene/StageSceneStateGetShineMain.h"
#include "Scene/StageSceneStateGetShineMainLast.h"
#include "Scene/StageSceneStateRecoverLife.h"
#include "Scene/StageSceneStateScenarioCamera.h"
#include "Util/CapManHeroDemoUtil.h"
#include "Util/PlayerDemoUtil.h"

namespace {
NERVE_IMPL(StageSceneStateGetShineMainWaterfallWorld, DemoGetShine);
NERVE_IMPL(StageSceneStateGetShineMainWaterfallWorld, DemoEnd);
NERVE_IMPL(StageSceneStateGetShineMainWaterfallWorld, DemoDownBridge);
NERVE_IMPL(StageSceneStateGetShineMainWaterfallWorld, DemoCapManHeroTalkFirstMoonGet);
NERVE_IMPL(StageSceneStateGetShineMainWaterfallWorld, DemoScenarioCamera);
NERVE_IMPL(StageSceneStateGetShineMainWaterfallWorld, DemoRecoverLife);
NERVES_MAKE_NOSTRUCT(StageSceneStateGetShineMainWaterfallWorld, DemoGetShine, DemoEnd,
                     DemoDownBridge, DemoCapManHeroTalkFirstMoonGet, DemoScenarioCamera,
                     DemoRecoverLife);
}  // namespace

StageSceneStateGetShineMainWaterfallWorld*
StageSceneStateGetShineMainWaterfallWorld::tryCreate(al::Scene* scene,
                                                     const al::ActorInitInfo& info) {
    auto* bridge = al::tryInitPlacementSingleObject(scene, info, 0, "SceneWatchObjList",
                                                    "WaterfallWorldHomeStoneBridge");

    if (bridge == nullptr)
        return nullptr;

    return new StageSceneStateGetShineMainWaterfallWorld(
        "メインムーンゲットデモ[滝]", scene, static_cast<WaterfallWorldFallDownBridge*>(bridge));
}

StageSceneStateGetShineMainWaterfallWorld::StageSceneStateGetShineMainWaterfallWorld(
    const char* name, al::Scene* scene, WaterfallWorldFallDownBridge* bridge)
    : al::HostStateBase<al::Scene>(name, scene), mBridge(bridge) {
    initNerve(&DemoGetShine, 5);
}

void StageSceneStateGetShineMainWaterfallWorld::init() {
    if (rs::getActiveQuestNum(getHost()) == 0) {
        mBridge->fallDown();
        return;
    }

    if (rs::getActiveQuestNo(getHost()) != 0)
        mBridge->fallDown();
}

void StageSceneStateGetShineMainWaterfallWorld::appear() {
    al::NerveStateBase::appear();
    al::setNerve(this, &DemoGetShine);
}

void StageSceneStateGetShineMainWaterfallWorld::kill() {
    al::NerveStateBase::kill();
}

void StageSceneStateGetShineMainWaterfallWorld::setStateGetShine(
    StageSceneStateGetShineMain* stateGetShine) {
    mStateGetShine = stateGetShine;
    mStateGetShine->setIsEndAfterDemoLand(true);
    al::addNerveState(this, stateGetShine, &DemoGetShine, "ムーンゲット");
}

void StageSceneStateGetShineMainWaterfallWorld::setStateScenarioCamera(
    StageSceneStateScenarioCamera* stateScenarioCamera) {
    mStateScenarioCamera = stateScenarioCamera;
    al::addNerveState(this, stateScenarioCamera, &DemoScenarioCamera, "シナリオ紹介カメラ");
}

void StageSceneStateGetShineMainWaterfallWorld::setStateRecoverLife(
    StageSceneStateRecoverLife* stateRecoverLife) {
    mStateRecoverLife = stateRecoverLife;
    al::addNerveState(this, stateRecoverLife, &DemoRecoverLife, "ライフ回復");
}

void StageSceneStateGetShineMainWaterfallWorld::setStateGetShineLast(
    StageSceneStateGetShineMainLast* stateGetShineLast) {
    mStateGetShineLast = stateGetShineLast;
    al::addNerveState(this, stateGetShineLast, &DemoEnd, "ムーンゲット最後");
}

void StageSceneStateGetShineMainWaterfallWorld::exeDemoGetShine() {
    if (al::updateNerveState(this)) {
        CapManHeroDemoUtil::getCapManHero(getHost())->appear();
        al::setNerve(this, &DemoDownBridge);
    }
}

void StageSceneStateGetShineMainWaterfallWorld::exeDemoDownBridge() {
    if (al::isFirstStep(this)) {
        mBridge->startDemo();
        rs::forcePutOffMarioHeadCap(rs::getPlayerActor(getHost()));
        rs::keepMarioCapVisibilityEndDemo(mBridge);
    }

    al::updateKitListPrev(getHost());
    rs::updateKitListDemoNormalWithPauseEffect(getHost());
    al::updateKitListPostDemoWithPauseNormalEffect(getHost());

    if (mBridge->isEndDemo()) {
        if (mStateScenarioCamera->tryStart()) {
            rs::forcePutOffMarioHeadCap(rs::getPlayerActor(getHost()));
            al::setNerve(this, &DemoScenarioCamera);
            return;
        }

        al::setNerve(this, &DemoRecoverLife);
    }
}

void StageSceneStateGetShineMainWaterfallWorld::exeDemoScenarioCamera() {
    if (al::isFirstStep(this))
        rs::keepMarioCapVisibilityEndDemo(mBridge);

    if (al::updateNerveState(this)) {
        CapManHeroDemoUtil::preEventFromSceneFirstMoonGet(getHost(), "PrepareShow");
        al::setNerve(this, &DemoRecoverLife);
    }
}

void StageSceneStateGetShineMainWaterfallWorld::exeDemoRecoverLife() {
    if (al::isFirstStep(this)) {
        mStateGetShine->getCapManHero()->kill();
        CapManHeroDemoUtil::preEventFromSceneFirstMoonGet(getHost(), "ShowModel");
    }

    if (al::updateNerveState(this)) {
        CapManHeroDemoUtil::startTalkDemoFirstMoonGet(getHost());
        al::setNerve(this, &DemoCapManHeroTalkFirstMoonGet);
    }
}

void StageSceneStateGetShineMainWaterfallWorld::exeDemoCapManHeroTalkFirstMoonGet() {
    al::updateKitListPrev(getHost());
    rs::updateKitListDemoPlayerWithPauseEffect(getHost());
    al::updateKitListPostDemoWithPauseNormalEffect(getHost());

    if (CapManHeroDemoUtil::isEndDemo(getHost())) {
        al::updateKitList(getHost(), "Npc");
        al::enableBgmLineChange(getHost());
        kill();
    }
}

void StageSceneStateGetShineMainWaterfallWorld::exeDemoEnd() {
    if (al::updateNerveState(this))
        kill();
}
