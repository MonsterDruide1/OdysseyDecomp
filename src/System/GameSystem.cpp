#include "System/GameSystem.h"

#include <gfx/nin/seadGraphicsNvn.h>
#include <heap/seadExpHeap.h>
#include <nn/friends.h>
#include <nn/oe.h>

#include "Library/Application/ApplicationMessageReceiver.h"
#include "Library/Base/Macros.h"
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

ALWAYS_INLINE void
addAudioInfo(al::AudioInfoListWithParts<al::AudioResourceLoadInfo>* info, const char* name,
             bool isBgm) {
    al::AudioResourceLoadInfo* resourceLoadInfo = new al::AudioResourceLoadInfo;
    resourceLoadInfo->initialize(name, isBgm);
    if (info)
        al::trySetAudioInfo(info, resourceLoadInfo, false);
}

void GameSystem::init() {
    mSystemInfo = new al::GameSystemInfo;
    mSystemInfo->drawSystemInfo = Application::instance()->getDrawSystemInfo();

    mGameConfigData = new GameConfigData();
    initNerve(&NrvGameSystem.Play);

    mAccountHolder = Application::instance()->getAccountHolder();
    nn::friends::Initialize();

    mNetworkSystem = new al::NetworkSystem(mAccountHolder->getUserHandle(), true);
    mNetworkSystem->requestSystemInitialize();
    mSystemInfo->setNetworkSystem(mNetworkSystem);

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
    audioSystemInitInfo._11 = true;
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

    al::AudioInfoListWithParts<al::AudioResourceLoadGroupInfo>* audioLoadInfo =
        new al::AudioInfoListWithParts<al::AudioResourceLoadGroupInfo>(2);
    mAudioLoadInfo = audioLoadInfo;

    al::AudioResourceLoadGroupInfo* residentGroupInfo = new al::AudioResourceLoadGroupInfo();
    residentGroupInfo->name = "システム常駐";

    residentGroupInfo->userManagementGroupList =
        new al::AudioInfoListWithParts<al::AudioResourceLoadInfo>(1);
    residentGroupInfo->addonSoundArchiveList =
        new al::AudioInfoListWithParts<al::AudioResourceLoadInfo>(1);

    addAudioInfo(residentGroupInfo->userManagementGroupList, "SeResourceStdSystem", false);
    addAudioInfo(residentGroupInfo->addonSoundArchiveList, "TestSE", false);

    al::trySetAudioInfo(audioLoadInfo, residentGroupInfo, false);

    al::AudioResourceLoadGroupInfo* nonResidentGroupInfo = new al::AudioResourceLoadGroupInfo;
    nonResidentGroupInfo->name = "システム常駐以外の常駐";

    nonResidentGroupInfo->userManagementGroupList =
        new al::AudioInfoListWithParts<al::AudioResourceLoadInfo>(5);
    nonResidentGroupInfo->addonSoundArchiveList = nullptr;

    addAudioInfo(nonResidentGroupInfo->userManagementGroupList, "SeResourceStd1st", false);
    addAudioInfo(nonResidentGroupInfo->userManagementGroupList, "SeResourceStd2nd", false);
    addAudioInfo(nonResidentGroupInfo->userManagementGroupList, "BgmResourceStd1st", true);
    addAudioInfo(nonResidentGroupInfo->userManagementGroupList, "BgmResourceStd2nd", true);
    addAudioInfo(nonResidentGroupInfo->userManagementGroupList, "BgmResourceStdPrefetch", true);

    al::trySetAudioInfo(audioLoadInfo, nonResidentGroupInfo, false);

    alAudioSystemFunction::loadAudioResource(
        "システム常駐", mAudioLoadInfo,
        alAudioSystemFunction::getSeadAudioPlayerForSe(mAudioSystem),
        alAudioSystemFunction::getSeadAudioPlayerForBgm(mAudioSystem));
    mSystemInfo->setAudioSystem(mAudioSystem);
    mGamePadSystem = new al::GamePadSystem();

    setPadName();

    mSystemInfo->setGamePadSystem(mGamePadSystem);
    mGamePadSystem->setAudioSystem(mAudioSystem);
    mWaveVibrationHolder = new al::WaveVibrationHolder(mGamePadSystem);
    mSystemInfo->setWaveVibrationHolder(mWaveVibrationHolder);
    mAudioSystem->addAudiioFrameProccess(mWaveVibrationHolder);

    mApplicationMessageReceiver = new al::ApplicationMessageReceiver();
    mApplicationMessageReceiver->init();
    mSystemInfo->setApplicationMessageReceiver(mApplicationMessageReceiver);

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
        if (al::isEqualString("HakoniwaSequence", mSequence->getName())) {
            GameDataHolder* gameDataHolder =
                static_cast<HakoniwaSequence*>(mSequence)->getGameDataHolder();
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
        0, al::getSystemMessageString(&messageSystemUser, "ControllerApplet", "SeparatePlayer1"));
    mGamePadSystem->setPadName(
        1, al::getSystemMessageString(&messageSystemUser, "ControllerApplet", "SeparatePlayer2"));
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
        GameDataHolder* gameDataHolder =
            static_cast<HakoniwaSequence*>(mSequence)->getGameDataHolder();
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
