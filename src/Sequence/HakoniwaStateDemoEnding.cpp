#include "Sequence/HakoniwaStateDemoEnding.h"

#include <heap/seadHeap.h>
#include <nn/oe.h>
#include <thread/seadThread.h>

#include "Library/Audio/System/AudioKeeperFunction.h"
#include "Library/Bgm/BgmLineFunction.h"
#include "Library/Memory/HeapUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Play/Layout/WipeHolder.h"
#include "Library/Resource/ResourceFunction.h"
#include "Library/Scene/Scene.h"
#include "Library/Screen/ScreenFunction.h"
#include "Library/Sequence/Sequence.h"
#include "Project/Memory/SceneHeapSetter.h"

#include "Scene/EndingScene.h"
#include "Scene/StaffRollScene.h"
#include "Sequence/HakoniwaSequence.h"
#include "Sequence/HakoniwaStateDeleteScene.h"
#include "Sequence/WorldResourceLoader.h"
#include "System/GameDataFunction.h"

const s32 cDefaultPriority = sead::Thread::cDefaultPriority;
const s32 cPriority = cDefaultPriority + 3;

namespace {
NERVE_IMPL(HakoniwaStateDemoEnding, Load);
NERVE_IMPL(HakoniwaStateDemoEnding, EndEnding);
NERVE_IMPL(HakoniwaStateDemoEnding, End);
NERVE_IMPL(HakoniwaStateDemoEnding, LoadWorldResource);
NERVE_IMPL(HakoniwaStateDemoEnding, LoadWorldResourceSecond);
NERVE_IMPL(HakoniwaStateDemoEnding, DemoEnding);
NERVE_IMPL(HakoniwaStateDemoEnding, LoadStaffRoll);
NERVE_IMPL(HakoniwaStateDemoEnding, DemoStaffRoll);

NERVES_MAKE_NOSTRUCT(HakoniwaStateDemoEnding, Load, DemoEnding, LoadStaffRoll, DemoStaffRoll);
NERVES_MAKE_STRUCT(HakoniwaStateDemoEnding, LoadWorldResource, EndEnding, End,
                   LoadWorldResourceSecond);
}  // namespace

HakoniwaStateDemoEnding::HakoniwaStateDemoEnding(HakoniwaSequence* sequence,
                                                 al::WipeHolder* wipeHolder,
                                                 al::ScreenCaptureExecutor* screenCaptureExecutor,
                                                 HakoniwaStateDeleteScene* stateDeleteScene,
                                                 WorldResourceLoader* resourceLoader,
                                                 GameDataHolder* gameDataHolder)
    : al::HostStateBase<HakoniwaSequence>("エンディングデモ", sequence), mWipeHolder(wipeHolder),
      mScreenCaptureExecutor(screenCaptureExecutor), mStateDeleteScene(stateDeleteScene),
      mResourceLoader(resourceLoader), mGameDataHolder(gameDataHolder) {}

HakoniwaStateDemoEnding::~HakoniwaStateDemoEnding() {
    if (nn::oe::IsUserInactivityDetectionTimeExtended()) {
        nn::oe::SetUserInactivityDetectionTimeExtended(false);
        nn::oe::IsUserInactivityDetectionTimeExtended();
    }

    if (mCurrentScene)
        delete mCurrentScene;
    mCurrentScene = nullptr;
}

void HakoniwaStateDemoEnding::init() {
    initNerve(&NrvHakoniwaStateDemoEnding.LoadWorldResource, 2);
    al::addNerveState(this, mStateDeleteScene, &NrvHakoniwaStateDemoEnding.EndEnding,
                      "シーン破棄[エンディング]");
    al::addNerveState(this, mStateDeleteScene, &NrvHakoniwaStateDemoEnding.End,
                      "シーン破棄[スタッフロール]");
}

void HakoniwaStateDemoEnding::appear() {
    al::NerveStateBase::appear();
    nn::oe::SetUserInactivityDetectionTimeExtended(true);

    if (mResourceLoader->getLoadWorldId() == GameDataFunction::getWorldIndexMoon())
        al::setNerve(this, &NrvHakoniwaStateDemoEnding.LoadWorldResource);
    else
        al::setNerve(this, &NrvHakoniwaStateDemoEnding.LoadWorldResourceSecond);
}

void HakoniwaStateDemoEnding::kill() {
    nn::oe::SetUserInactivityDetectionTimeExtended(false);
    al::NerveStateBase::kill();
}

void HakoniwaStateDemoEnding::exeLoadWorldResource() {
    if (al::isFirstStep(this))
        mResourceLoader->requestLoadWorldHomeStageResource(GameDataFunction::getWorldIndexMoon(),
                                                           1);

    if (mResourceLoader->isEndLoadWorldResource())
        al::setNerve(this, &Load);
}

void HakoniwaStateDemoEnding::exeLoadWorldResourceSecond() {
    if (al::isFirstStep(this))
        mResourceLoader->requestLoadWorldResource(GameDataFunction::getWorldIndexMoon());

    if (mResourceLoader->isEndLoadWorldResource())
        al::setNerve(this, &NrvHakoniwaStateDemoEnding.LoadWorldResource);
}

void HakoniwaStateDemoEnding::exeLoad() {
    HakoniwaSequence** sequence;

    if (al::isFirstStep(this)) {
        const char* stageName = "DemoEndingStage";
        al::createSceneHeap(stageName, true);
        al::setCurrentCategoryName("シーン");

        {
            al::SceneHeapSetter heapSetter;

            EndingScene* scene = new EndingScene(mWipeHolder);
            mCurrentScene = scene;
            scene->setLoadEnd(true);

            sequence = getHostPtr();
            alAudioSystemFunction::resetDataDependedStage((*sequence)->getAudioDirector(),
                                                          stageName, 1);
            al::setSceneAndUseInitThread(*sequence, mCurrentScene, cPriority, stageName, 1,
                                         "Sequence=ProductSequence", nullptr);
        }
    } else {
        sequence = getHostPtr();
    }

    if (al::tryEndSceneInitThread(*sequence)) {
        al::setSequenceAudioKeeperToSceneSeDirector(*sequence, mCurrentScene);
        mScreenCaptureExecutor->offDraw();
        mCurrentScene->appear();
        (*sequence)->setNextScene(mCurrentScene);

        if (!mWipeHolder->isOpenEnd())
            mWipeHolder->startOpen(-1);

        al::getSceneHeap()->adjust();
        al::setSequenceNameForActorPickTool(*sequence, mCurrentScene);
        al::setNerve(this, &DemoEnding);
    }
}

void HakoniwaStateDemoEnding::exeDemoEnding() {
    getHost()->updatePadSystem();

    if (!mCurrentScene->isAlive()) {
        mScreenCaptureExecutor->requestCapture(true, 0);
        al::setNerve(this, &NrvHakoniwaStateDemoEnding.EndEnding);
    }
}

void HakoniwaStateDemoEnding::exeEndEnding() {
    if (al::isFirstStep(this)) {
        mStateDeleteScene->start(mCurrentScene, true, false, 0);
        al::stopBgm(getHost(), "Ending", -1);
    }

    if (al::updateNerveState(this)) {
        mCurrentScene = nullptr;
        if (!GameDataFunction::isGameClear(mGameDataHolder))
            mResourceLoader->tryDestroyWorldResource();
        al::setNerve(this, &LoadStaffRoll);
    }
}

void HakoniwaStateDemoEnding::exeLoadStaffRoll() {
    HakoniwaSequence** sequence;

    if (al::isFirstStep(this)) {
        const char* stageName = "StaffRollStage";
        al::createSceneHeap(stageName, true);
        al::setCurrentCategoryName("シーン");

        {
            al::SceneHeapSetter heapSetter;

            mCurrentScene = new StaffRollScene(mWipeHolder);

            sequence = getHostPtr();
            alAudioSystemFunction::resetDataDependedStage((*sequence)->getAudioDirector(),
                                                          stageName, 1);
            al::setSceneAndUseInitThread(*sequence, mCurrentScene, cPriority, stageName, 1,
                                         "Sequence=ProductSequence", nullptr);
        }
    } else {
        sequence = getHostPtr();
    }

    if (al::tryEndSceneInitThread(*sequence)) {
        al::setSequenceAudioKeeperToSceneSeDirector(*sequence, mCurrentScene);
        mScreenCaptureExecutor->offDraw();
        mCurrentScene->appear();
        (*sequence)->setNextScene(mCurrentScene);
        al::getSceneHeap()->adjust();
        al::setSequenceNameForActorPickTool(*sequence, mCurrentScene);

        if (!GameDataFunction::isGameClear(mGameDataHolder)) {
            mResourceLoader->tryDestroyWorldResource();
            mResourceLoader->requestLoadWorldHomeStageResource(
                GameDataFunction::getWorldIndexPeach(), 2);
        }

        if (!mWipeHolder->isOpenEnd())
            mWipeHolder->startOpen(-1);

        al::setNerve(this, &DemoStaffRoll);
    }
}

void HakoniwaStateDemoEnding::exeDemoStaffRoll() {
    getHost()->updatePadSystem();

    if (!mCurrentScene->isAlive()) {
        mScreenCaptureExecutor->requestCapture(true, 0);

        if (!mWipeHolder->isClose())
            mWipeHolder->startClose("FadeBlack", -1);

        al::setNerve(this, &NrvHakoniwaStateDemoEnding.End);
    }
}

void HakoniwaStateDemoEnding::exeEnd() {
    if (al::isFirstStep(this)) {
        mStateDeleteScene->start(mCurrentScene, true, false, 0);
        return;
    }

    if (al::updateNerveState(this)) {
        mCurrentScene = nullptr;
        kill();
    }
}
