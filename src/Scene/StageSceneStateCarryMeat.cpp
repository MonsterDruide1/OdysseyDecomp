#include "Scene/StageSceneStateCarryMeat.h"

#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Scene/SceneUtil.h"

#include "Scene/StageScene.h"
#include "Scene/StageSceneStateSkipDemo.h"
#include "Util/DemoUtil.h"

namespace {
NERVE_IMPL(StageSceneStateCarryMeat, FindMeat);
NERVE_IMPL(StageSceneStateCarryMeat, SkipDemoFindMeat);
NERVE_IMPL(StageSceneStateCarryMeat, SkipDemoCarryMeat);
NERVE_IMPL(StageSceneStateCarryMeat, CarryMeat);
NERVES_MAKE_NOSTRUCT(StageSceneStateCarryMeat, FindMeat, SkipDemoFindMeat, SkipDemoCarryMeat,
                     CarryMeat);
}  // namespace

StageSceneStateCarryMeat::StageSceneStateCarryMeat(const char* name, al::Scene* scene)
    : al::HostStateBase<al::Scene>(name, scene) {
    // NOTE: reserves space for 3 substates, but only assigns 2
    initNerve(&FindMeat, 3);
}

void StageSceneStateCarryMeat::setState(StageSceneStateSkipDemo* skipDemo) {
    mSkipDemo = skipDemo;
    al::addNerveState(this, skipDemo, &SkipDemoFindMeat, "デモスキップ[肉発見]");
    al::addNerveState(this, skipDemo, &SkipDemoCarryMeat, "デモスキップ[肉運び]");
}

void StageSceneStateCarryMeat::appear() {
    al::NerveStateBase::appear();
    al::setNerve(this, &FindMeat);
}

void StageSceneStateCarryMeat::kill() {
    al::NerveStateBase::kill();
}

void StageSceneStateCarryMeat::exeFindMeat() {
    al::updateKitListPrev(getHost());
    rs::updateKitListDemoNormalWithPauseEffect(getHost());
    al::updateKitListPostDemoWithPauseNormalEffect(getHost());

    if (rs::isActiveDemoBirdCarryMeat(getHost())) {
        al::setNerve(this, &CarryMeat);
        return;
    }

    mSkipDemo->tryAppearSkipDemoLayout();
    if (mSkipDemo->tryStartSkipDemo())
        al::setNerve(this, &SkipDemoFindMeat);
}

void StageSceneStateCarryMeat::exeCarryMeat() {
    rs::updateBirdCarryMeat(getHost());
    rs::updateDemoSystemOnly(getHost());

    if (!rs::isActiveDemo(getHost()))
        kill();

    mSkipDemo->tryAppearSkipDemoLayout();
    if (mSkipDemo->tryStartSkipDemo())
        al::setNerve(this, &SkipDemoCarryMeat);
}

void StageSceneStateCarryMeat::exeSkipDemoFindMeat() {
    if (al::updateNerveState(this)) {
        if (mSkipDemo->isCancelSkip())
            al::setNerve(this, &FindMeat);
        else
            al::setNerve(this, &CarryMeat);
    }
}

void StageSceneStateCarryMeat::exeSkipDemoCarryMeat() {
    if (al::updateNerveState(this)) {
        mSkipDemo->isCancelSkip();
        al::setNerve(this, &CarryMeat);
    }
}
