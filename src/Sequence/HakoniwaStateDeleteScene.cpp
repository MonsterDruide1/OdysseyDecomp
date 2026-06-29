#include "Sequence/HakoniwaStateDeleteScene.h"

#include <thread/seadThread.h>

#include "Library/Audio/AudioDirector.h"
#include "Library/Memory/HeapUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Scene/Scene.h"
#include "Library/Scene/SceneUtil.h"
#include "Library/Sequence/Sequence.h"
#include "Library/Thread/AsyncFunctorThread.h"
#include "Library/Thread/FunctorV0M.h"

#include "Sequence/WorldResourceLoader.h"

namespace {
using namespace al;

NERVE_IMPL(HakoniwaStateDeleteScene, Prepare)
NERVE_IMPL(HakoniwaStateDeleteScene, DeleteScene)
NERVE_IMPL(HakoniwaStateDeleteScene, FinalizeAudio)

NERVES_MAKE_NOSTRUCT(HakoniwaStateDeleteScene, Prepare)
NERVES_MAKE_STRUCT(HakoniwaStateDeleteScene, DeleteScene, FinalizeAudio)
}  // namespace

const s32 cDefaultPriority = sead::Thread::cDefaultPriority;
const s32 cPriority = cDefaultPriority + 1;

HakoniwaStateDeleteScene::HakoniwaStateDeleteScene(al::Sequence* sequence,
                                                   WorldResourceLoader* resourceLoader)
    : al::HostStateBase<al::Sequence>("シーン破棄", sequence), mResourceLoader(resourceLoader) {
    using DeleteSceneFunctor =
        al::FunctorV0M<HakoniwaStateDeleteScene*, void (HakoniwaStateDeleteScene::*)()>;

    initNerve(&Prepare, 0);
    mDeleteSceneThread = new al::AsyncFunctorThread(
        "DeleteScene", DeleteSceneFunctor{this, &HakoniwaStateDeleteScene::deleteScene}, cPriority,
        0x80000, sead::CoreId::cMain);
}

HakoniwaStateDeleteScene::~HakoniwaStateDeleteScene() {
    if (mDeleteSceneThread)
        delete mDeleteSceneThread;
}

void HakoniwaStateDeleteScene::deleteScene() {
    if (mScene)
        delete mScene;
    mScene = nullptr;
    al::destroySceneHeap(mIsDestroySceneHeap);
}

void HakoniwaStateDeleteScene::appear() {
    al::NerveStateBase::appear();
    al::setNerve(this, &Prepare);
}

void HakoniwaStateDeleteScene::kill() {
    al::NerveStateBase::kill();
}

void HakoniwaStateDeleteScene::start(al::Scene* scene, bool isDestroySceneHeap,
                                     bool isFinalizeAudio, s32 stopSeIndex) {
    mIsDestroySceneHeap = isDestroySceneHeap;
    mScene = scene;
    mStopSeIndex = stopSeIndex;
    mIsFinalizeAudio = isFinalizeAudio;
}

void HakoniwaStateDeleteScene::exePrepare() {
    getHost()->setCurrentScene(nullptr);
    if (mIsFinalizeAudio)
        al::setNerve(this, &NrvHakoniwaStateDeleteScene.FinalizeAudio);
    else
        al::setNerve(this, &NrvHakoniwaStateDeleteScene.DeleteScene);
}

void HakoniwaStateDeleteScene::exeFinalizeAudio() {
    al::AudioDirector* audioDirector = mScene->getAudioDirector();
    if (al::isFirstStep(this)) {
        if (!audioDirector) {
            al::setNerve(this, &NrvHakoniwaStateDeleteScene.DeleteScene);
            return;
        }
        audioDirector->startFinalizeUnsafeModuleInParallelThread();
    }

    audioDirector->updateFinalizeUnsafeModuleInParallelThread();
    if (!audioDirector->isFinalizedUnsafeModuleInParallelThread())
        return;

    al::stopAllSe(mScene, mStopSeIndex);
    al::setNerve(this, &NrvHakoniwaStateDeleteScene.DeleteScene);
}

void HakoniwaStateDeleteScene::exeDeleteScene() {
    if (al::isFirstStep(this))
        mDeleteSceneThread->start();

    if (mDeleteSceneThread->isDone() && mResourceLoader->isEndLoadWorldResource())
        kill();
}
