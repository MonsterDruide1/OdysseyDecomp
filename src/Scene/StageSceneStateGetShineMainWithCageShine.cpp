#include "Scene/StageSceneStateGetShineMainWithCageShine.h"

#include "Library/Nerve/Nerve.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Scene/StageScene.h"
#include "Scene/StageSceneStateBreakCageShine.h"
#include "Scene/StageSceneStateGetShineMain.h"
#include "Scene/StageSceneStateGetShineMainLast.h"
#include "Scene/StageSceneStateRecoverLife.h"
#include "Scene/StageSceneStateScenarioCamera.h"
#include "Util/PlayerDemoUtil.h"

namespace {
NERVE_IMPL(StageSceneStateGetShineMainWithCageShine, DemoGetShine);
NERVE_IMPL(StageSceneStateGetShineMainWithCageShine, DemoBreakCageShine);
NERVE_IMPL(StageSceneStateGetShineMainWithCageShine, DemoScenarioCamera);
NERVE_IMPL(StageSceneStateGetShineMainWithCageShine, DemoRecoverLife);
NERVE_IMPL(StageSceneStateGetShineMainWithCageShine, DemoEnd);
NERVES_MAKE_NOSTRUCT(StageSceneStateGetShineMainWithCageShine, DemoGetShine, DemoBreakCageShine,
                     DemoScenarioCamera, DemoRecoverLife, DemoEnd);
}  // namespace

StageSceneStateGetShineMainWithCageShine*
StageSceneStateGetShineMainWithCageShine::tryCreate(al::Scene* scene,
                                                    const al::ActorInitInfo& initInfo) {
    StageSceneStateBreakCageShine* breakCageShine =
        StageSceneStateBreakCageShine::tryCreate(scene, initInfo);
    if (!breakCageShine)
        return nullptr;

    return new StageSceneStateGetShineMainWithCageShine("メインムーンゲットデモ[ケージシャイン]",
                                                        scene, breakCageShine);
}

StageSceneStateGetShineMainWithCageShine::StageSceneStateGetShineMainWithCageShine(
    const char* name, al::Scene* scene, StageSceneStateBreakCageShine* breakCageShine)
    : al::HostStateBase<al::Scene>(name, scene), mStateBreakCageShine(breakCageShine) {
    initNerve(&DemoGetShine, 5);
    al::initNerveState(this, mStateBreakCageShine, &DemoBreakCageShine, "ケージ壊れる");
}

void StageSceneStateGetShineMainWithCageShine::init() {}

void StageSceneStateGetShineMainWithCageShine::appear() {
    al::NerveStateBase::appear();
    al::setNerve(this, &DemoGetShine);
}

void StageSceneStateGetShineMainWithCageShine::kill() {
    al::NerveStateBase::kill();
}

void StageSceneStateGetShineMainWithCageShine::setState(
    StageSceneStateGetShineMain* getShineMain, StageSceneStateScenarioCamera* scenarioCamera,
    StageSceneStateRecoverLife* recoverLife, StageSceneStateGetShineMainLast* getShineMainLast) {
    mStateGetShineMain = getShineMain;
    getShineMain->setSkipDemoAfterGet(true);
    al::addNerveState(this, getShineMain, &DemoGetShine, "ムーンゲット");

    mStateScenarioCamera = scenarioCamera;
    al::addNerveState(this, mStateScenarioCamera, &DemoScenarioCamera, "シナリオ紹介カメラ");

    mStateRecoverLife = recoverLife;
    al::addNerveState(this, mStateRecoverLife, &DemoRecoverLife, "ライフ回復");

    mStateGetShineMainLast = getShineMainLast;
    al::addNerveState(this, mStateGetShineMainLast, &DemoEnd, "ムーンゲット最後");
}

void StageSceneStateGetShineMainWithCageShine::exeDemoGetShine() {
    if (al::updateNerveState(this))
        al::setNerve(this, &DemoBreakCageShine);
}

void StageSceneStateGetShineMainWithCageShine::exeDemoBreakCageShine() {
    if (!al::updateNerveState(this))
        return;

    if (mStateScenarioCamera->tryStart()) {
        rs::forcePutOffMarioHeadCap(rs::getPlayerActor(getHost()));
        al::setNerve(this, &DemoScenarioCamera);
    } else {
        al::setNerve(this, &DemoRecoverLife);
    }
}

void StageSceneStateGetShineMainWithCageShine::exeDemoScenarioCamera() {
    if (al::updateNerveState(this))
        al::setNerve(this, &DemoRecoverLife);
}

void StageSceneStateGetShineMainWithCageShine::exeDemoRecoverLife() {
    if (al::updateNerveState(this))
        al::setNerve(this, &DemoEnd);
}

void StageSceneStateGetShineMainWithCageShine::exeDemoEnd() {
    if (al::updateNerveState(this))
        kill();
}
