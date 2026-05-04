#include "Scene/StageSceneStateBreakCageShine.h"

#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Scene/SceneUtil.h"

#include "MapObj/CageShineWatcher.h"
#include "Scene/StageScene.h"
#include "Util/DemoUtil.h"

namespace {
NERVE_IMPL(StageSceneStateBreakCageShine, Wait);
NERVES_MAKE_NOSTRUCT(StageSceneStateBreakCageShine, Wait);
}  // namespace

StageSceneStateBreakCageShine*
StageSceneStateBreakCageShine::tryCreate(al::Scene* scene, const al::ActorInitInfo& actorInitInfo) {
    al::DeriveActorGroup<CageShineWatcher>* cageShineWatcherGroup =
        new al::DeriveActorGroup<CageShineWatcher>("ケージシャイン監視", 4);
    if (!al::tryInitPlacementActorGroup(cageShineWatcherGroup, scene, actorInitInfo, 0,
                                        "SceneWatchObjList", "CageShine"))
        return nullptr;

    return new StageSceneStateBreakCageShine("ケージ壊れる", scene, cageShineWatcherGroup);
}

StageSceneStateBreakCageShine::StageSceneStateBreakCageShine(
    const char* name, al::Scene* scene,
    al::DeriveActorGroup<CageShineWatcher>* cageShineWatcherGroup)
    : al::HostStateBase<al::Scene>(name, scene), mCageShineWatcherGroup(cageShineWatcherGroup) {
    initNerve(&Wait, 0);
}

void StageSceneStateBreakCageShine::appear() {
    al::NerveStateBase::appear();
    al::setNerve(this, &Wait);
}

void StageSceneStateBreakCageShine::kill() {
    al::NerveStateBase::kill();
}

void StageSceneStateBreakCageShine::exeWait() {
    if (al::isFirstStep(this)) {
        Shine* demoShineActor = rs::getDemoShineActor(getHost());
        al::DeriveActorGroup<CageShineWatcher>* cageShineWatcherGroup = mCageShineWatcherGroup;
        for (s32 i = 0; i < cageShineWatcherGroup->getActorCount(); i++) {
            bool isSameShine =
                cageShineWatcherGroup->getDeriveActor(i)->isSameShine(demoShineActor);
            cageShineWatcherGroup = mCageShineWatcherGroup;
            if (isSameShine) {
                cageShineWatcherGroup->getDeriveActor(i)->startBreakDemo();
                break;
            }
        }
    }

    al::updateKitListPrev(getHost());
    rs::updateKitListDemoNormalWithPauseEffect(getHost());
    al::updateKitListPostDemoWithPauseNormalEffect(getHost());

    if (!rs::isActiveDemo(getHost()))
        kill();
}
