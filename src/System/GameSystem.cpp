#include "System/GameSystem.h"

#include <gfx/nin/seadGraphicsNvn.h>
#include <heap/seadExpHeap.h>
#include <nn/friends.h>
#include <nn/oe.h>

#include "Library/Application/ApplicationMessageReceiver.h"
#include "Library/Audio/AudioInfo.h"
#include "Library/Audio/AudioLoadGroup.h"
#include "Library/Audio/System/AudioKeeperFunction.h"
#include "Library/Audio/System/AudioSystemInfo.h"
#include "Library/Base/StringUtil.h"
#include "Library/Collision/CollisionCodeFunction.h"
#include "Library/Controller/GamePadSystem.h"
#include "Library/Controller/GamePadWaveVibrationData.h"
#include "Library/Effect/EffectSystem.h"
#include "Library/File/FileUtil.h"
#include "Library/Framework/GameFrameworkNx.h"
#include "Library/Layout/LayoutSystem.h"
#include "Library/Memory/HeapUtil.h"
#include "Library/Message/IUseMessageSystem.h"
#include "Library/Message/MessageHolder.h"
#include "Library/Message/MessageSystem.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Network/AccountHolder.h"
#include "Library/Network/HtmlViewer.h"
#include "Library/Network/NetworkSystem.h"
#include "Library/Resource/ResourceFunction.h"
#include "Library/Sequence/Sequence.h"
#include "Library/System/GameSystemInfo.h"

#include "Sequence/HakoniwaSequence.h"
#include "Sequence/SequenceFactory.h"
#include "System/Application.h"
#include "System/GameConfigData.h"
#include "System/GameDataHolder.h"
#include "System/ProjectNfpDirector.h"

namespace {
NERVE_IMPL(GameSystem, Play);

NERVES_MAKE_STRUCT(GameSystem, Play);

class MessageSystemUser : public al::IUseMessageSystem {
public:
    MessageSystemUser(const al::MessageSystem* messageSystem) : mMessageSystem(messageSystem) {}

    const al::MessageSystem* getMessageSystem() const override { return mMessageSystem; }

private:
    const al::MessageSystem* mMessageSystem;
};
}  // namespace

GameSystem::GameSystem() : NerveExecutor("ゲームシステム") {}

void GameSystem::init() {
    mSystemInfo = new al::GameSystemInfo;
    mSystemInfo->drawSystemInfo = Application::instance()->getDrawSystemInfo();

    mGameConfigData = new GameConfigData();
    initNerve(&NrvGameSystem.Play);

    mAccountHolder = Application::instance()->getAccountHolder();
    nn::friends::Initialize();

    mNetworkSystem = new al::NetworkSystem(mAccountHolder->getUserHandle(), true);
    mNetworkSystem->requestSystemInitialize();
    mSystemInfo->networkSystem = mNetworkSystem;

    mHtmlViewer = new al::HtmlViewer();
    mSystemInfo->htmlViewer = mHtmlViewer;

    mNfpDirector = new ProjectNfpDirector();
    mSystemInfo->nfpDirector = mNfpDirector;
    mNfpDirector->initialize();

    sead::ExpHeap* heap =
        sead::ExpHeap::create(0x4600000, "EffectSystemHeap", nullptr, 8,
                              sead::Heap::HeapDirection::cHeapDirection_Forward, false);
    al::addNamedHeap(heap, "EffectSystemHeap");

    al::EffectSystem* effectSystem =
        al::EffectSystem::createSystem(mSystemInfo->drawSystemInfo->drawContext, heap);
    mSystemInfo->effectSystem = effectSystem;

    al::LayoutSystem* layoutSystem = new al::LayoutSystem;
    layoutSystem->init();
    mSystemInfo->layoutSystem = layoutSystem;
    mSystemInfo->messageSystem = new al::MessageSystem;

    al::AudioSystemInitInfo audioSystemInitInfo;
    al::CollisionCodeList* materialCode =
        alCollisionCodeFunction::tyrCreateCollisionCodeList("MaterialCode");
    al::CollisionCodeList* materialCodePrefix =
        alCollisionCodeFunction::tyrCreateCollisionCodeList("MaterialCodePrefix");

    mSystemInfo->effectSystem->setMaterialCodeList(materialCode);
    mSystemInfo->effectSystem->setMaterialCodePrefix(materialCodePrefix);
    audioSystemInitInfo.setMaterialCode(materialCode, materialCodePrefix);

    audioSystemInitInfo.seDataName = "SeData";
    audioSystemInitInfo.seBgmName = "BgmData";
    audioSystemInitInfo.dockedVolume = 0.401f;
    audioSystemInitInfo.undockedVolume = 1.0f;
    audioSystemInitInfo.unk2 = true;
    audioSystemInitInfo.isBgmOnSameMixIndex = true;
    audioSystemInitInfo.useAudioMaximizer = true;
    audioSystemInitInfo.changeInputBgmChannelVolume = true;
    audioSystemInitInfo.monoVolume = 0.3f;
    audioSystemInitInfo.stereoVolume = 1.061f;
    audioSystemInitInfo.cacheSizePerSound = 0x40000;

    audioSystemInitInfo.setHeapSize(al::tryFindNamedHeap("AudioHeap"));

    mAudioSystem = new al::AudioSystem();
    mAudioSystem->init(audioSystemInitInfo);

    al::setAudioPlayerToResourceSystem(
        alAudioSystemFunction::getSeadAudioPlayerForSe(mAudioSystem),
        alAudioSystemFunction::getSeadAudioPlayerForBgm(mAudioSystem));

    al::AudioInfoListWithParts<al::AudioResourceLoadGroupInfo>* groupList =
        new al::AudioInfoListWithParts<al::AudioResourceLoadGroupInfo>;
    groupList->init(2, 0);
    mAudioInfoList = groupList;

    al::AudioResourceLoadGroupInfo* loadInfo = new al::AudioResourceLoadGroupInfo();
    loadInfo->name = "システム常駐";

    al::AudioInfoListWithParts<al::AudioResourceLoadInfo>* userInfoList =
        new al::AudioInfoListWithParts<al::AudioResourceLoadInfo>;
    userInfoList->init(1, 0);
    loadInfo->userManagementGroupLoadInfoList = userInfoList;

    al::AudioInfoListWithParts<al::AudioResourceLoadInfo>* addonInfoList =
        new al::AudioInfoListWithParts<al::AudioResourceLoadInfo>;
    addonInfoList->init(1, 0);
    auto* copy = loadInfo->userManagementGroupLoadInfoList;
    loadInfo->addonSoundArchiveLoadInfoList = addonInfoList;

    al::AudioResourceLoadInfo* strSystemInfo = new al::AudioResourceLoadInfo;
    strSystemInfo->setName("SeResourceStdSystem", false);
    if (copy)
        al::trySetAudioInfo2(copy, strSystemInfo, false);

    auto* copy2 = loadInfo->addonSoundArchiveLoadInfoList;
    al::AudioResourceLoadInfo* testSEInfo = new al::AudioResourceLoadInfo;
    testSEInfo->setName("TestSE", false);
    if (copy2)
        al::trySetAudioInfo2(copy2, testSEInfo, false);

    al::trySetAudioInfo(groupList, loadInfo, false);

    al::AudioResourceLoadGroupInfo* nextGroupList = new al::AudioResourceLoadGroupInfo;
    nextGroupList->name = "システム常駐以外の常駐";

    al::AudioInfoListWithParts<al::AudioResourceLoadInfo>* findout =
        new al::AudioInfoListWithParts<al::AudioResourceLoadInfo>;
    findout->init(5, 0);
    nextGroupList->userManagementGroupLoadInfoList = findout;
    nextGroupList->addonSoundArchiveLoadInfoList = nullptr;

    al::AudioResourceLoadInfo* Std1stInfo = new al::AudioResourceLoadInfo;
    Std1stInfo->setName("SeResourceStd1st", false);
    al::trySetAudioInfo2(findout, Std1stInfo, false);

    auto* copy3 = nextGroupList->userManagementGroupLoadInfoList;
    al::AudioResourceLoadInfo* Std2stInfo = new al::AudioResourceLoadInfo;
    Std2stInfo->setName("SeResourceStd2nd", false);
    if (copy3)
        al::trySetAudioInfo2(copy3, Std2stInfo, false);

    auto* copy4 = nextGroupList->userManagementGroupLoadInfoList;
    al::AudioResourceLoadInfo* bgmStd1stInfo = new al::AudioResourceLoadInfo;
    bgmStd1stInfo->setName("BgmResourceStd1st", true);
    if (copy4)
        al::trySetAudioInfo2(copy4, bgmStd1stInfo, false);

    auto* copy5 = nextGroupList->userManagementGroupLoadInfoList;
    al::AudioResourceLoadInfo* bgmStd2stInfo = new al::AudioResourceLoadInfo();
    bgmStd2stInfo->setName("BgmResourceStd2nd", true);
    if (copy5)
        al::trySetAudioInfo2(copy5, bgmStd2stInfo, false);

    auto* copy6 = nextGroupList->userManagementGroupLoadInfoList;
    al::AudioResourceLoadInfo* prefetch = new al::AudioResourceLoadInfo();
    prefetch->setName("BgmResourceStdPrefetch", true);
    if (copy6)
        al::trySetAudioInfo2(copy6, prefetch, false);

    al::trySetAudioInfo(groupList, nextGroupList, false);

    alAudioSystemFunction::loadAudioResource(
        "システム常駐", mAudioInfoList,
        alAudioSystemFunction::getSeadAudioPlayerForSe(mAudioSystem),
        alAudioSystemFunction::getSeadAudioPlayerForBgm(mAudioSystem));
    mSystemInfo->audioSystem = mAudioSystem;
    mGamePadSystem = new al::GamePadSystem();

    setPadName();

    mSystemInfo->gamePadSystem = mGamePadSystem;
    mGamePadSystem->setAudioSystem(mAudioSystem);
    mWaveVibrationHolder = new al::WaveVibrationHolder(mGamePadSystem);
    mSystemInfo->waveVibrationHolder = mWaveVibrationHolder;
    mAudioSystem->addAudiioFrameProccess(mWaveVibrationHolder);

    mApplicationMessageReceiver = new al::ApplicationMessageReceiver();
    mApplicationMessageReceiver->init();
    mSystemInfo->applicationMessageReceiver = mApplicationMessageReceiver;
    al::setGpuPerformance(al::GpuPerformance::unk3, nn::oe::PerformanceMode_Normal);
    al::createSequenceHeap();
    tryChangeSequence("HakoniwaSequence");
    nn::oe::FinishStartupLogo();
    Application::instance()->getGameFramework()->requestChangeUseGPU(true);
}

void GameSystem::movement() {
    mApplicationMessageReceiver->update();

    if (mApplicationMessageReceiver->isUpdatedOperationMode())
        mGamePadSystem->setInvalidateDisconnectFrame(600);
    mGamePadSystem->update();

    if (mNetworkSystem)
        mNetworkSystem->updateBeforeScene();

    if (mApplicationMessageReceiver->getLocalPerformanceMode() == nn::oe::PerformanceMode_Boost) {
        ((sead::GraphicsNvn*)sead::Graphics::instance())
            ->setDisplayBufferWindowCrop(0, 0, 1600, 900);
        Application::instance()->getGameFramework()->setDocked(true);
        mSystemInfo->drawSystemInfo->isDocked = true;
    } else {
        ((sead::GraphicsNvn*)sead::Graphics::instance())
            ->setDisplayBufferWindowCrop(0, 0, 1280, 720);
        Application::instance()->getGameFramework()->setDocked(false);
        mSystemInfo->drawSystemInfo->isDocked = false;
    }

    updateNerve();
    mAudioSystem->update();
    if (mNetworkSystem)
        mNetworkSystem->updateAfterScene();

    if (!mSequence->isAlive()) {
        if (al::isEqualString("HakoniwaSequence", mSequence->getName().cstr())) {
            GameDataHolder* gameDataHolder = ((HakoniwaSequence*)(mSequence))->getGameDataHolder();
            mIsSinglePlay = gameDataHolder->isSeparatePlay();
            mIsSequenceSetupIncomplete = true;
            *mGameConfigData = *gameDataHolder->getGameConfigData();
        }

        mSystemInfo->layoutSystem->prepareInitFontForChangeLanguage();
        al::removeResourceCategory("常駐[ローカライズ]");

        al::findNamedHeap("LocalizeResourceHeap")->freeAll();
        al::addResourceCategory("常駐[ローカライズ]", 80,
                                al::findNamedHeap("LocalizeResourceHeap"));
        al::clearFileLoaderEntry();
        al::createCategoryResourceAll("常駐[ローカライズ]");
        mSystemInfo->layoutSystem->initFontForChangeLanguage();
        mSystemInfo->messageSystem->initMessageForChangeLanguage();
        tryChangeSequence("HakoniwaSequence");
    }
}

void GameSystem::setPadName() {
    MessageSystemUser messageSystemUser = MessageSystemUser{mSystemInfo->messageSystem};
    mGamePadSystem->setPadName(
        0, al::getSystemMessageString(&messageSystemUser, "ControllerApplet", "SeperatePlayer1"));
    mGamePadSystem->setPadName(
        1, al::getSystemMessageString(&messageSystemUser, "ControllerApplet", "SeperatePlayer2"));
}

bool GameSystem::tryChangeSequence(const char* name) {
    if (mSequence) {
        if (!mSequence->isDisposable())
            return false;
        if (mSequence)
            delete (mSequence);
        mSequence = nullptr;
        al::freeAllSequenceHeap();
    }
    setPadName();

    sead::ScopedCurrentHeapSetter heap(al::getSequenceHeap());

    al::Sequence* sequence = SequenceFactory::createSequence(name);
    if (!sequence)
        return false;

    al::SequenceInitInfo initInfo(mSystemInfo);
    sequence->init(initInfo);
    mSequence = sequence;

    if (al::isEqualString(name, "HakoniwaSequence")) {
        GameDataHolder* gameDataHolder = ((HakoniwaSequence*)(mSequence))->getGameDataHolder();
        gameDataHolder->setSeparatePlay(mIsSinglePlay);
        if (mIsSequenceSetupIncomplete) {
            *gameDataHolder->getGameConfigData() = *mGameConfigData;
            mIsSequenceSetupIncomplete = false;
        }
    }

    return true;
}

void GameSystem::drawMain() {
    Application::instance()->getGameFramework()->clearFrameBuffer();
    mSystemInfo->layoutSystem->beginDraw();

    if (mSequence)
        mSequence->drawMain();
    mSystemInfo->layoutSystem->endDraw();
}

void GameSystem::exePlay() {
    mNfpDirector->update();
    if (mSequence)
        mSequence->update();
}
