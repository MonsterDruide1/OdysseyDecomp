#include "Npc/SessionMusicianBgmController.h"

#include "Library/Audio/System/AudioKeeper.h"
#include "Library/Base/StringUtil.h"
#include "Library/Bgm/BgmBeatCounter.h"
#include "Library/Bgm/BgmLineFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorSceneInfo.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Nerve/NerveExecutor.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Scene/SceneObjUtil.h"
#include "Library/Se/SeFunction.h"

#include "Audio/CollectBgmPlayer.h"
#include "Npc/SessionMusicianLocalFunction.h"

namespace {
NERVE_HOST_TYPE_IMPL(SessionMusicianBgmController, Wait);
NERVE_HOST_TYPE_IMPL(SessionMusicianBgmController, InArea);
NERVE_HOST_TYPE_IMPL(SessionMusicianBgmController, OutArea);

NERVES_MAKE_NOSTRUCT(HostType, Wait, InArea, OutArea);
}  // namespace

SessionMusicianBgmController::SessionMusicianBgmController(al::LiveActor* actor,
                                                           const al::ActorInitInfo& initInfo,
                                                           bool forceFullBand)
    : al::NerveExecutor("セッションBGM制御"), mActor(actor) {
    initNerve(&Wait, 0);
    al::initActorBgmKeeper(mActor, initInfo, nullptr);
    mAudioChecker = new al::AudioGeneralPurposeAreaChecker("SessionBgmChangeArea");
    mAudioChecker->init(mActor->getAreaObjDirector());
    mAudioChecker->setPlayerHolder(mActor->getSceneInfo()->playerHolder);

    bool isSessionFullMember = SessionMusicianLocalFunction::isSessionFullMember(mActor);
    if (forceFullBand)
        mIsFullBandPerformance = forceFullBand;
    else
        mIsFullBandPerformance = isSessionFullMember;

    if (mIsFullBandPerformance)
        al::replaceBgmPlayInfoResourceName(mActor, "WorldMain", "StmRsBgmCityScenario04");

    mBeatCounter = new al::BgmBeatCounter(mActor, -0.17f);

    mCollectBgmPlayer = al::getSceneObj<CollectBgmPlayer>(mActor);
}

static void endInstrumentPlayback(al::LiveActor* actor, bool playAllInstrumentsFlag,
                                  bool stopSituationFlag) {
    if (playAllInstrumentsFlag) {
        al::endBgmSituation(actor, "PlayEtcInst", false);
        SessionMusicianLocalFunction::endPlayingTheDs(actor);
        SessionMusicianLocalFunction::endPlayingTheBa(actor);
        SessionMusicianLocalFunction::endPlayingTheGt(actor);
        SessionMusicianLocalFunction::endPlayingTheTp(actor);
        return;
    }

    if (SessionMusicianLocalFunction::isSubscribed(actor, 0))
        SessionMusicianLocalFunction::endPlayingTheDs(actor);
    if (SessionMusicianLocalFunction::isSubscribed(actor, 1))
        SessionMusicianLocalFunction::endPlayingTheBa(actor);
    if (SessionMusicianLocalFunction::isSubscribed(actor, 2))
        SessionMusicianLocalFunction::endPlayingTheGt(actor);
    if (SessionMusicianLocalFunction::isSubscribed(actor, 3))
        SessionMusicianLocalFunction::endPlayingTheTp(actor);

    if (stopSituationFlag)
        al::endBgmSituation(actor, "StopPlayingEtcInst", false);
}

static void startInstrumentPlayback(al::LiveActor* actor, bool playAllInstrumentsFlag,
                                    bool stopSituationFlag) {
    if (playAllInstrumentsFlag) {
        al::startBgmSituation(actor, "PlayEtcInst", false, true);
        SessionMusicianLocalFunction::startPlayingTheDs(actor);
        SessionMusicianLocalFunction::startPlayingTheBa(actor);
        SessionMusicianLocalFunction::startPlayingTheGt(actor);
        SessionMusicianLocalFunction::startPlayingTheTp(actor);
        return;
    }

    if (SessionMusicianLocalFunction::isSubscribed(actor, 0))
        SessionMusicianLocalFunction::startPlayingTheDs(actor);
    if (SessionMusicianLocalFunction::isSubscribed(actor, 1))
        SessionMusicianLocalFunction::startPlayingTheBa(actor);
    if (SessionMusicianLocalFunction::isSubscribed(actor, 2))
        SessionMusicianLocalFunction::startPlayingTheGt(actor);
    if (SessionMusicianLocalFunction::isSubscribed(actor, 3))
        SessionMusicianLocalFunction::startPlayingTheTp(actor);

    if (stopSituationFlag)
        al::startBgmSituation(actor, "StopPlayingEtcInst", false, true);
}

void SessionMusicianBgmController::exeWait() {
    tryStartBgmSituation();
    mAudioChecker->update();
    mBeatCounter->update();
    if (mAudioChecker->isEnteredArea())
        al::setNerve(this, &InArea);
}

bool SessionMusicianBgmController::tryStartBgmSituation() {
    if (mIsFullBandPerformance)
        return false;

    const char* bgmName = al::getCurPlayingBgmPlayName(mActor);
    if (!(bgmName != nullptr && al::isEqualString("WorldMain", bgmName)) ||
        (mPrevBgmName != nullptr && al::isEqualString("WorldMain", mPrevBgmName))) {
        mPrevBgmName = bgmName;
        return false;
    }
    startInstrumentPlayback(mActor, false, false);
    mPrevBgmName = bgmName;
    return true;
}

void SessionMusicianBgmController::exeInArea() {
    mAudioChecker->update();
    mBeatCounter->update();

    if (al::isFirstStep(this)) {
        al::stopAllBgm(mActor, -1);
        mPrevBgmName = nullptr;

        bool isFullBand = mIsFullBandPerformance;
        if (!mIsFullBandPerformance) {
            if (SessionMusicianLocalFunction::isSessionFullMember(mActor)) {
                al::replaceBgmPlayInfoResourceName(mActor, "WorldMain", "StmRsBgmCityScenario04");
                isFullBand = mIsFullBandPerformance = true;
            } else {
                isFullBand = mIsFullBandPerformance;
            }
        }

        al::LiveActor* actor = mActor;
        al::startBgm(actor, "LiveTest", -1, 0);
        startInstrumentPlayback(actor, isFullBand, true);
    }

    s32 musicianCount =
        mIsFullBandPerformance ? 4 : SessionMusicianLocalFunction::getMemberMusicianNum(mActor);

    switch (musicianCount) {
    case 0:
        al::holdSe(mActor, "Empty");
        break;
    case 1:
        al::holdSe(mActor, "MusicianOne");
        break;
    case 2:
        al::holdSe(mActor, "MusiciansTwo");
        break;
    case 3:
        al::holdSe(mActor, "MusiciansThree");
        break;
    case 4:
        al::holdSe(mActor, "MusiciansFull");
        break;
    }

    if (mBeatCounter->isTriggerBeat(2, 1)) {
        switch (musicianCount) {
        case 1:
            al::startSe(mActor, "ClapS");
            break;
        case 2:
            al::startSe(mActor, "ClapM");
            break;
        case 3:
            al::startSe(mActor, "ClapL");
            break;
        case 4:
            al::startSe(mActor, "ClapL");
            break;
        }
    } else if (mBeatCounter->isTriggerBeat(2, 0)) {
        switch (musicianCount) {
        case 1:
            al::startSe(mActor, "ClapSOffBeat");
            break;
        case 2:
            al::startSe(mActor, "ClapMOffBeat");
            break;
        case 3:
            al::startSe(mActor, "ClapLOffBeat");
            break;
        case 4:
            al::startSe(mActor, "ClapLOffBeat");
            break;
        }
    }

    if (mAudioChecker->isExitedArea())
        al::setNerve(this, &OutArea);
}

void SessionMusicianBgmController::exeOutArea() {
    mAudioChecker->update();
    mBeatCounter->update();
    if (al::isFirstStep(this)) {
        auto* actor = mActor;
        bool isFullBandPerformance = mIsFullBandPerformance;
        al::stopBgm(mActor, "LiveTest", -1);
        endInstrumentPlayback(actor, isFullBandPerformance, true);
    }

    tryStartBgmSituation();

    if (mAudioChecker->isEnteredArea()) {
        endInstrumentPlayback(mActor, mIsFullBandPerformance, false);
        al::setNerve(this, &InArea);
    }
}
