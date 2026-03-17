#include "Scene/StageSceneStateCarryMeat.h"

#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Scene/SceneUtil.h"

#include "Scene/StageSceneStateSkipDemo.h"
#include "Util/DemoUtil.h"

namespace rs {
bool updateBirdCarryMeat(al::Scene*);
void updateKitListDemoNormalWithPauseEffect(al::Scene*);
}  // namespace rs

namespace {
NERVE_IMPL(StageSceneStateCarryMeat, FindMeat);
NERVE_IMPL(StageSceneStateCarryMeat, SkipDemoFindMeat);
NERVE_IMPL(StageSceneStateCarryMeat, SkipDemoCarryMeat);
NERVE_IMPL(StageSceneStateCarryMeat, CarryMeat);
NERVES_MAKE_NOSTRUCT(StageSceneStateCarryMeat, FindMeat, SkipDemoFindMeat, SkipDemoCarryMeat,
                     CarryMeat);
}  // namespace

StageSceneStateCarryMeat::StageSceneStateCarryMeat(const char* name, al::Scene* scene)
    : al::NerveStateBase(name), mScene(scene) {
    initNerve(&FindMeat, 3);
}

void StageSceneStateCarryMeat::setState(StageSceneStateSkipDemo* skipDemo) {
    mSkipDemo = skipDemo;
    al::addNerveState(this, skipDemo, &SkipDemoFindMeat, u8"デモスキップ[肉発見]");
    al::addNerveState(this, skipDemo, &SkipDemoCarryMeat, u8"デモスキップ[肉運び]");
}

void StageSceneStateCarryMeat::appear() {
    al::NerveStateBase::appear();
    al::setNerve(this, &FindMeat);
}

void StageSceneStateCarryMeat::kill() {
    al::NerveStateBase::kill();
}

void StageSceneStateCarryMeat::exeFindMeat() {
    al::updateKitListPrev(mScene);
    rs::updateKitListDemoNormalWithPauseEffect(mScene);
    al::updateKitListPostDemoWithPauseNormalEffect(mScene);

    if (rs::isActiveDemoBirdCarryMeat(mScene)) {
        al::setNerve(this, &CarryMeat);
    } else {
        mSkipDemo->tryAppearSkipDemoLayout();
        if (mSkipDemo->tryStartSkipDemo())
            al::setNerve(this, &SkipDemoFindMeat);
    }
}

void StageSceneStateCarryMeat::exeCarryMeat() {
    rs::updateBirdCarryMeat(mScene);
    rs::updateDemoSystemOnly(mScene);

    if (!rs::isActiveDemo(mScene))
        kill();

    mSkipDemo->tryAppearSkipDemoLayout();
    if (mSkipDemo->tryStartSkipDemo())
        al::setNerve(this, &SkipDemoCarryMeat);
}

void StageSceneStateCarryMeat::exeSkipDemoFindMeat() {
    if (!al::updateNerveState(this))
        return;

    if (mSkipDemo->isCancelSkip())
        al::setNerve(this, &FindMeat);
    else
        al::setNerve(this, &CarryMeat);
}

void StageSceneStateCarryMeat::exeSkipDemoCarryMeat() {
    if (!al::updateNerveState(this))
        return;

    mSkipDemo->isCancelSkip();
    al::setNerve(this, &CarryMeat);
}
