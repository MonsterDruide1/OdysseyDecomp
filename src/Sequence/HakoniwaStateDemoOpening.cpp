#include "Sequence/HakoniwaStateDemoOpening.h"

#include <heap/seadHeap.h>
#include <thread/seadThread.h>

#include "Library/Audio/System/AudioKeeperFunction.h"
#include "Library/Bgm/BgmLineFunction.h"
#include "Library/Layout/LayoutActorUtil.h"
#include "Library/Memory/HeapUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Play/Layout/SimpleLayoutAppearWaitEnd.h"
#include "Library/Play/Layout/WipeHolder.h"
#include "Library/Screen/ScreenFunction.h"
#include "Library/Se/SeFunction.h"
#include "Library/Thread/AsyncFunctorThread.h"
#include "Project/Memory/SceneHeapSetter.h"
#include "Project/Scene/SceneCreator.h"

#include "Layout/BootLayout.h"
#include "Layout/LoadLayoutCtrl.h"
#include "Scene/DemoSceneWithCinemaCaption.h"
#include "Scene/FirstSequenceScene.h"
#include "Sequence/HakoniwaSequence.h"
#include "Sequence/HakoniwaStateDeleteScene.h"
#include "Sequence/WorldResourceLoader.h"

namespace {
struct SceneThreadInfo {
    s32 priority;
    s32 defaultPriority;
    f32 unused;
    s32 coreId;
};

const SceneThreadInfo sSceneThreadInfo = {
    sead::Thread::cDefaultPriority + 3,
    sead::Thread::cDefaultPriority,
    -100.0f,
    0,
};

NERVE_IMPL(HakoniwaStateDemoOpening, Boot);
NERVE_IMPL(HakoniwaStateDemoOpening, DestroyFirstSequence);
NERVE_IMPL(HakoniwaStateDemoOpening, End);
NERVE_IMPL(HakoniwaStateDemoOpening, LoadFirstSequence);
NERVE_IMPL(HakoniwaStateDemoOpening, FirstSequence);
NERVE_IMPL(HakoniwaStateDemoOpening, Load);
NERVE_IMPL(HakoniwaStateDemoOpening, FadeToText);
NERVE_IMPL(HakoniwaStateDemoOpening, TextAppear);
NERVE_IMPL(HakoniwaStateDemoOpening, Text);
NERVE_IMPL(HakoniwaStateDemoOpening, FadeToDemo);
NERVE_IMPL(HakoniwaStateDemoOpening, DemoOpening);

NERVES_MAKE_NOSTRUCT(HakoniwaStateDemoOpening, LoadFirstSequence, FirstSequence, TextAppear, Text,
                     FadeToDemo, DemoOpening);
NERVES_MAKE_STRUCT(HakoniwaStateDemoOpening, Boot, DestroyFirstSequence, End, Load, FadeToText);
}  // namespace

HakoniwaStateDemoOpening::HakoniwaStateDemoOpening(
    HakoniwaSequence* sequence, al::WipeHolder* wipeHolder,
    al::ScreenCaptureExecutor* screenCaptureExecutor, WorldResourceLoader* resourceLoader,
    BootLayout* bootLayout, const al::LayoutInitInfo& layoutInitInfo,
    HakoniwaStateDeleteScene* stateDeleteScene, al::AsyncFunctorThread* initThread,
    LoadLayoutCtrl* loadLayoutCtrl)
    : al::HostStateBase<HakoniwaSequence>("オープニングデモ", sequence), mWipeHolder(wipeHolder),
      mBootLayout(bootLayout), mScreenCaptureExecutor(screenCaptureExecutor),
      mResourceLoader(resourceLoader), mStateDeleteScene(stateDeleteScene), mInitThread(initThread),
      mLoadLayoutCtrl(loadLayoutCtrl) {
    mStartOpeningDemo = new al::SimpleLayoutAppearWaitEnd(
        "オープニングテキスト", "StartOpeningDemo", layoutInitInfo, nullptr, false);
    al::killLayoutIfActive(mStartOpeningDemo);
}

HakoniwaStateDemoOpening::~HakoniwaStateDemoOpening() {
    delete mDemoScene;
    mDemoScene = nullptr;
}

void HakoniwaStateDemoOpening::init() {
    initNerve(&NrvHakoniwaStateDemoOpening.Boot, 2);
    al::addNerveState(this, mStateDeleteScene, &NrvHakoniwaStateDemoOpening.DestroyFirstSequence,
                      "シーン破棄[初回シーケンス]");
    al::addNerveState(this, mStateDeleteScene, &NrvHakoniwaStateDemoOpening.End,
                      "シーン破棄[オープニング]");
}

void HakoniwaStateDemoOpening::appear() {
    al::NerveStateBase::appear();
    al::setNerve(this, &NrvHakoniwaStateDemoOpening.Boot);
}

void HakoniwaStateDemoOpening::startSecond() {
    mIsSecond = true;
}

void HakoniwaStateDemoOpening::exeBoot() {
    if (al::isFirstStep(this)) {
        al::createSceneHeap(nullptr, true);
        al::SceneHeapSetter heapSetter;
        mFirstSequenceScene = new FirstSequenceScene();
        alAudioSystemFunction::resetDataDependedStage(getHost()->getAudioDirector(), nullptr, 1);
        al::setSceneAndUseInitThread(getHost(), mFirstSequenceScene, sSceneThreadInfo.priority,
                                     nullptr, 1, "Sequence=ProductSequence", nullptr);
        mInitThread->start();
    }

    if (mIsSecond || al::isGreaterEqualStep(this, 60))
        al::setNerve(this, &LoadFirstSequence);
}

void HakoniwaStateDemoOpening::exeLoadFirstSequence() {
    getHost()->updatePadSystem();

    if (al::isFirstStep(this) && !mIsSecond)
        mBootLayout->appear();

    if (!al::tryEndSceneInitThread(getHost()) || !mInitThread->isDone() ||
        (!mIsSecond && al::isLessEqualStep(this, 540)))
        return;

    al::setSequenceAudioKeeperToSceneSeDirector(getHost(), mFirstSequenceScene);
    mFirstSequenceScene->appear();
    getHost()->setNextScene(mFirstSequenceScene);
    al::setSequenceNameForActorPickTool(getHost(), mFirstSequenceScene);
    al::setNerve(this, &FirstSequence);
}

void HakoniwaStateDemoOpening::exeFirstSequence() {
    getHost()->updatePadSystem();

    if (al::isFirstStep(this)) {
        mBootLayout->kill();
        mScreenCaptureExecutor->offDraw();
        if (mWipeHolder->isClose()) {
            mFirstSequenceScene->setNoWipe();
            mWipeHolder->startOpen(-1);
        }
    }

    if (mFirstSequenceScene->isAlive())
        return;

    mScreenCaptureExecutor->requestCapture(true, 0);
    al::setNerve(this, &NrvHakoniwaStateDemoOpening.DestroyFirstSequence);
}

void HakoniwaStateDemoOpening::exeDestroyFirstSequence() {
    if (al::isFirstStep(this)) {
        mStateDeleteScene->start(mFirstSequenceScene, true, false, 0);
        mWipeHolder->startClose("FadeBlack", 150);
        al::stopBgm(getHost(), "Stage", 300);
        return;
    }

    if (al::updateNerveState(this)) {
        mFirstSequenceScene = nullptr;
        al::setNerve(this, &NrvHakoniwaStateDemoOpening.Load);
    }
}

void HakoniwaStateDemoOpening::exeLoad() {
    if (!mInitThread->isDone()) {
        al::setNerve(this, &NrvHakoniwaStateDemoOpening.Load);
        return;
    }

    if (al::isFirstStep(this)) {
        const char* stageName = "DemoOpeningStage";
        al::createSceneHeap(stageName, true);
        al::SceneHeapSetter heapSetter;
        mDemoScene = new DemoSceneWithCinemaCaption(mWipeHolder);
        alAudioSystemFunction::resetDataDependedStage(getHost()->getAudioDirector(), stageName, 1);
        al::setSceneAndUseInitThread(getHost(), mDemoScene, sSceneThreadInfo.priority, stageName, 1,
                                     "Sequence=ProductSequence", nullptr);
    }

    al::setNerve(this, &NrvHakoniwaStateDemoOpening.FadeToText);
}

void HakoniwaStateDemoOpening::exeFadeToText() {
    if (!mWipeHolder->isCloseEnd())
        return;

    mStartOpeningDemo->appear();
    mBootLayout->kill();
    al::setNerve(this, &TextAppear);
    al::startSe(getHost(), "AmbBeforeOP");
}

void HakoniwaStateDemoOpening::exeTextAppear() {
    if (mStartOpeningDemo->isWait())
        al::setNerve(this, &Text);
}

void HakoniwaStateDemoOpening::exeText() {
    if (al::isGreaterEqualStep(this, 100)) {
        mStartOpeningDemo->end();
        al::setNerve(this, &FadeToDemo);
    }
}

void HakoniwaStateDemoOpening::exeFadeToDemo() {
    if (!al::isDead(mStartOpeningDemo) || !al::tryEndSceneInitThread(getHost()))
        return;

    al::setSequenceAudioKeeperToSceneSeDirector(getHost(), mDemoScene);
    al::getSceneHeap()->adjust();
    al::setSequenceNameForActorPickTool(getHost(), mDemoScene);
    mScreenCaptureExecutor->offDraw();
    mDemoScene->setLoadEnd();
    mDemoScene->appear();
    getHost()->setNextScene(mDemoScene);
    mWipeHolder->startOpen(150);
    al::setNerve(this, &DemoOpening);
}

void HakoniwaStateDemoOpening::exeDemoOpening() {
    getHost()->updatePadSystem();

    if (al::isFirstStep(this)) {
        mResourceLoader->requestLoadWorldHomeStageResource(0, 1);
        al::tryStopSe(getHost(), "AmbBeforeOP", -1, nullptr);
    }

    if (mDemoScene->isAlive())
        return;

    mLoadLayoutCtrl->startWaitLoad();
    mScreenCaptureExecutor->requestCapture(true, 0);
    if (!mWipeHolder->isClose())
        mWipeHolder->startClose("FadeBlack", -1);
    al::setNerve(this, &NrvHakoniwaStateDemoOpening.End);
}

void HakoniwaStateDemoOpening::exeEnd() {
    if (al::isFirstStep(this)) {
        if (mResourceLoader->isEndLoadWorldResource()) {
            mStateDeleteScene->start(mDemoScene, true, false, 0);
            al::stopBgm(getHost(), "Stage", 300);
            return;
        }
        al::setNerve(this, &NrvHakoniwaStateDemoOpening.End);
        return;
    }

    if (al::updateNerveState(this)) {
        mDemoScene = nullptr;
        kill();
    }
}
