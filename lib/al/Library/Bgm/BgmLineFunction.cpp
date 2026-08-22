#include "Library/Bgm/BgmLineFunction.h"

#include "Library/Audio/AudioEventController.h"
#include "Library/Audio/AudioInfo.h"
#include "Library/Audio/System/AudioKeeper.h"
#include "Library/Audio/System/AudioKeeperFunction.h"
#include "Library/Base/StringUtil.h"
#include "Library/Bgm/Bgm3DParamsController.h"
#include "Library/Bgm/BgmDirector.h"
#include "Library/Bgm/BgmFunction.h"
#include "Library/Bgm/BgmKeeper.h"
#include "Library/Bgm/BgmLine.h"
#include "Library/Bgm/BgmLineKeeper.h"
#include "Library/Bgm/BgmPlayEventController.h"
#include "Library/Bgm/BgmPlayingRequest.h"
#include "Library/Bgm/BgmRhythmDetector.h"
#include "Library/Bgm/BgmRhythmSyncDirector.h"
#include "Library/Bgm/BgmSituationDirector.h"
#include "Library/File/FileUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Resource/Resource.h"
#include "Library/Yaml/ByamlIter.h"
#include "Project/Bgm/Bgm.h"
#include "Project/Bgm/BgmInfo.h"

namespace al {
namespace {

AudioKeeper* tryGetAudioKeeper(const IUseAudioKeeper* audioUser) {
    return audioUser->getAudioKeeper();
}

BgmKeeper* tryGetBgmKeeper(const IUseAudioKeeper* audioUser) {
    AudioKeeper* audioKeeper = tryGetAudioKeeper(audioUser);
    if (!audioKeeper)
        return nullptr;
    return audioKeeper->getBgmKeeper();
}

BgmDirector* tryGetDirector(const IUseAudioKeeper* audioUser) {
    BgmKeeper* bgmKeeper = tryGetBgmKeeper(audioUser);
    if (!bgmKeeper)
        return nullptr;
    return bgmKeeper->getBgmDirector();
}

AudioEventController* tryGetAudioEventController(const IUseAudioKeeper* audioUser) {
    AudioKeeper* audioKeeper = tryGetAudioKeeper(audioUser);
    if (!audioKeeper)
        return nullptr;
    return audioKeeper->getAudioEventController();
}

BgmSituationDirector* tryGetSituationDirector(const IUseAudioKeeper* audioUser) {
    BgmDirector* director = tryGetDirector(audioUser);
    if (!director)
        return nullptr;
    return director->getBgmSituationDirector();
}

Bgm3DParamsController* tryGet3DParamsController(const IUseAudioKeeper* audioUser) {
    BgmDirector* director = tryGetDirector(audioUser);
    if (!director)
        return nullptr;
    return director->getBgm3DParamsController();
}

BgmRhythmSyncDirector* tryGetRhythmSyncDirector(const IUseAudioKeeper* audioUser) {
    BgmDirector* director = tryGetDirector(audioUser);
    if (!director)
        return nullptr;
    return director->getBgmRhythmSyncDirector();
}

BgmRhythmDetector* tryGetRhythmDetector(const IUseAudioKeeper* audioUser) {
    BgmRhythmSyncDirector* rhythmSyncDirector = tryGetRhythmSyncDirector(audioUser);
    if (!rhythmSyncDirector)
        return nullptr;
    return rhythmSyncDirector->getActiveRhythmDetector();
}

BgmLine* tryGetActiveLine(const IUseAudioKeeper* audioUser) {
    BgmDirector* director = tryGetDirector(audioUser);
    if (!director)
        return nullptr;
    return director->getActiveBgmLine();
}

bool canStartBgm(const IUseAudioKeeper* audioUser, const char* playName, bool isForceStart) {
    if (isForceStart)
        return true;

    AudioEventController* eventController = tryGetAudioEventController(audioUser);
    if (!eventController)
        return true;

    BgmPlayEventController* bgmEventController = eventController->getBgmPlayEventController();
    if (!bgmEventController)
        return true;

    return bgmEventController->checkWhetherItCanStartBgm(playName);
}

bool canStartBgm(const IUseAudioKeeper* audioUser, const BgmPlayingRequest& request,
                 bool isForceStart) {
    if (isForceStart)
        return true;

    AudioEventController* eventController = tryGetAudioEventController(audioUser);
    if (!eventController)
        return true;

    BgmPlayEventController* bgmEventController = eventController->getBgmPlayEventController();
    if (!bgmEventController)
        return true;

    return bgmEventController->checkWhetherItCanStartBgm(request.bgmName);
}

}  // namespace

BgmDirector* getBgmDirector(const IUseAudioKeeper* audioUser) {
    return tryGetDirector(audioUser);
}

BgmDirector* tryGetBgmDirector(const IUseAudioKeeper* audioUser) {
    return tryGetDirector(audioUser);
}

BgmSituationDirector* getBgmSituationDirector(const IUseAudioKeeper* audioUser) {
    return tryGetSituationDirector(audioUser);
}

Bgm3DParamsController* getBgm3DParamsController(const IUseAudioKeeper* audioUser) {
    return tryGet3DParamsController(audioUser);
}

BgmLine* getActiveBgmLine(const IUseAudioKeeper* audioUser) {
    return tryGetActiveLine(audioUser);
}

BgmLine* getPlayingBgmLine(const IUseAudioKeeper* audioUser, const char* playName) {
    BgmDirector* director = tryGetDirector(audioUser);
    if (!director)
        return nullptr;
    return director->getPlayingBgmLine(playName);
}

const char* getCurPlayingBgmPlayName(const IUseAudioKeeper* audioUser) {
    BgmLine* line = tryGetActiveLine(audioUser);
    if (!line)
        return nullptr;
    return line->getCurPlayName();
}

const char* getCurPlayingBgmResourceName(const IUseAudioKeeper* audioUser) {
    BgmLine* line = tryGetActiveLine(audioUser);
    if (!line)
        return nullptr;

    Bgm* bgm = line->getActiveBgmPlayer();
    if (!bgm)
        return nullptr;

    return bgm->getRunningResourceName();
}

const char* getBgmPlayNameInCurPosition(const IUseAudioKeeper* audioUser, bool isStopArea) {
    AudioEventController* eventController = tryGetAudioEventController(audioUser);
    if (!eventController)
        return nullptr;
    return alAudioEventControllerFunction::getBgmPlayNameInCurPosition(eventController, isStopArea);
}

const char* getBgmResourceNameInCurPosition(const IUseAudioKeeper* audioUser, bool isStopArea) {
    const char* playName = getBgmPlayNameInCurPosition(audioUser, isStopArea);
    if (!playName)
        return nullptr;

    BgmDirector* director = tryGetDirector(audioUser);
    if (!director)
        return nullptr;

    BgmPlayInfo* playInfo = alBgmFunction::tryFindBgmPlayInfo(director->getBgmDataBase(), playName);
    if (!playInfo)
        return nullptr;

    BgmLine* line = director->getBgmLineByLineName(playInfo->lineName);
    if (!line)
        return nullptr;

    return line->getResourceName(playName);
}

s64 getActiveBgmCurSamplePosition(const IUseAudioKeeper* audioUser) {
    BgmLine* line = tryGetActiveLine(audioUser);
    if (!line)
        return -1;

    Bgm* bgm = line->getActiveBgmPlayer();
    if (!bgm)
        return -1;

    return bgm->getCurSamplePosition();
}

s64 getBgmLoopEndSamplePosition(const IUseAudioKeeper* audioUser, const char* playName) {
    BgmDirector* director = tryGetDirector(audioUser);
    if (!director)
        return 0;
    return director->getLoopEndSamplePosition(playName);
}

void startBgm(const IUseAudioKeeper* audioUser, const char* playName, s32 fadeFrame,
              s32 startFrame) {
    BgmPlayingRequest request(playName, nullptr, nullptr, fadeFrame, startFrame, -1, 0, 0, false,
                              false, false, false);

    if (!canStartBgm(audioUser, playName, false))
        return;

    BgmDirector* director = tryGetDirector(audioUser);
    if (director)
        director->startBgm(request, false);
}

void startBgm(const IUseAudioKeeper* audioUser, const BgmPlayingRequest& request, bool isForceStart,
              bool isImmediate) {
    if (!canStartBgm(audioUser, request, isForceStart))
        return;

    BgmDirector* director = tryGetDirector(audioUser);
    if (director)
        director->startBgm(request, isImmediate);
}

void startBgmWithSuffix(const IUseAudioKeeper* audioUser, const char* playName, const char* suffix,
                        s32 fadeFrame, s32 startFrame) {
    BgmPlayingRequest request(playName, suffix, nullptr, fadeFrame, startFrame, -1, 0, 0, false,
                              false, false, false);

    if (!canStartBgm(audioUser, playName, false))
        return;

    BgmDirector* director = tryGetDirector(audioUser);
    if (director)
        director->startBgm(request, false);
}

void startAndStopBgmInCurPosition(const IUseAudioKeeper* audioUser, bool isStopArea) {
    AudioEventController* eventController = tryGetAudioEventController(audioUser);
    if (!eventController || !eventController->isEnableBgmChangeEvent())
        return;

    alAudioEventControllerFunction::tryStartAndStopBgmInCurPosition(eventController, isStopArea,
                                                                    true);

    const char* situationName = eventController->getBgmSituationNameByAreaChecker();
    if (!situationName)
        return;

    BgmDirector* director = tryGetDirector(audioUser);
    if (director)
        director->startSituation(situationName, false, false, false);
}

void startBgmSituation(const IUseAudioKeeper* audioUser, const char* situationName, bool isApply) {
    BgmDirector* director = tryGetDirector(audioUser);
    if (director)
        director->startSituation(situationName, isApply, false, false);
}

void prepareBgm(const IUseAudioKeeper* audioUser, const BgmPlayingRequest& request) {
    BgmDirector* director = tryGetDirector(audioUser);
    if (director)
        director->prepareBgm(request);
}

void prepareBgmWithAreaCheck(const IUseAudioKeeper* audioUser) {
    AudioEventController* eventController = tryGetAudioEventController(audioUser);
    if (!eventController)
        return;

    const char* playName =
        alAudioEventControllerFunction::getBgmPlayNameInCurPosition(eventController, false);
    if (eventController->isInBgmStopArea(playName))
        return;

    BgmPlayingRequest request(playName, nullptr, nullptr, -1, 0, -1, 0, 0, false, false, false,
                              false);
    prepareBgm(audioUser, request);
}

void stopBgm(const IUseAudioKeeper* audioUser, const char* playName, s32 fadeFrame) {
    BgmDirector* director = tryGetDirector(audioUser);
    if (director)
        director->stopBgm(playName, fadeFrame);
}

void stopBgm(const IUseAudioKeeper* audioUser, const BgmPlayingRequest& request) {
    BgmDirector* director = tryGetDirector(audioUser);
    if (director)
        director->stopBgm(request.bgmName, request._18);
}

void pauseBgm(const IUseAudioKeeper* audioUser, const char* playName, s32 fadeFrame) {
    BgmDirector* director = tryGetDirector(audioUser);
    if (director)
        director->pauseBgm(playName, fadeFrame);
}

void resumeBgm(const IUseAudioKeeper* audioUser, const char* playName, s32 fadeFrame) {
    BgmDirector* director = tryGetDirector(audioUser);
    if (director)
        director->resumeBgm(playName, fadeFrame);
}

bool isPlayingBgm(const IUseAudioKeeper* audioUser) {
    BgmDirector* director = tryGetDirector(audioUser);
    return director && director->isPlayingBgm();
}

bool isPlayingBgm(const IUseAudioKeeper* audioUser, const char* playName) {
    BgmDirector* director = tryGetDirector(audioUser);
    return director && director->isPlayingBgm(playName);
}

bool isRunningBgm(const IUseAudioKeeper* audioUser, const char* playName) {
    BgmDirector* director = tryGetDirector(audioUser);
    return director && director->isRunningBgm(playName);
}

bool isPlayingResourceCategoryBgm(const IUseAudioKeeper* audioUser, const char* categoryName) {
    const char* resourceName = getCurPlayingBgmResourceName(audioUser);
    if (!resourceName)
        return false;
    return isEqualString(resourceName, categoryName);
}

void stopActiveBgm(const IUseAudioKeeper* audioUser, s32 fadeFrame) {
    BgmLine* line = tryGetActiveLine(audioUser);
    if (line)
        line->stopBgm(fadeFrame);
}

void pauseActiveBgm(const IUseAudioKeeper* audioUser, s32 fadeFrame) {
    BgmLine* line = tryGetActiveLine(audioUser);
    if (line)
        line->pauseBgm(fadeFrame);
}

void resumeActiveBgm(const IUseAudioKeeper* audioUser, s32 fadeFrame) {
    BgmLine* line = tryGetActiveLine(audioUser);
    if (line)
        line->resumeBgm(fadeFrame, false);
}

bool isPlayingActiveBgm(const IUseAudioKeeper* audioUser, const char* playName) {
    BgmLine* line = tryGetActiveLine(audioUser);
    if (!line || !line->isRunning())
        return false;

    if (!playName)
        return true;

    const char* currentPlayName = line->getCurPlayName();
    return currentPlayName && isEqualString(playName, currentPlayName);
}

bool isPauseActiveBgm(const IUseAudioKeeper* audioUser) {
    BgmLine* line = tryGetActiveLine(audioUser);
    return line && line->isPause();
}

void stopAllBgm(const IUseAudioKeeper* audioUser, s32 fadeFrame) {
    BgmDirector* director = tryGetDirector(audioUser);
    if (director)
        director->stopAllBgm(fadeFrame, false);
}

void tryStopAllBgm(const IUseAudioKeeper* audioUser, s32 fadeFrame) {
    BgmDirector* director = tryGetDirector(audioUser);
    if (director)
        director->tryStopAllBgm(fadeFrame);
}

void setTriggerEventForStopAllBgm(const IUseAudioKeeper* audioUser, const FunctorBase& functor) {
    BgmDirector* director = tryGetDirector(audioUser);
    if (director)
        director->setTriggerEventForStopAllBgm(functor);
}

void tryPauseBgmIfDifferBgmArea(const LiveActor* actor, const sead::Vector3f& position,
                                s32 fadeFrame) {
    const IUseAudioKeeper* audioUser = actor;
    AudioEventController* eventController = tryGetAudioEventController(audioUser);

    const char* currentPlayName = alAudioEventControllerFunction::getBgmPlayNameInTargetPosition(
        eventController, getTrans(actor));
    const char* targetPlayName =
        alAudioEventControllerFunction::getBgmPlayNameInTargetPosition(eventController, position);

    if (!currentPlayName)
        return;

    if (!targetPlayName) {
        getBgmDirector(audioUser)->tryPauseBgmIfNotPlaying(currentPlayName, fadeFrame);
        return;
    }

    if (!isEqualString(currentPlayName, targetPlayName))
        getBgmDirector(audioUser)->tryPauseBgmIfLowPriority(targetPlayName, fadeFrame);
}

void tryPauseBgmIfLowPriority(const IUseAudioKeeper* audioUser, const char* playName,
                              s32 fadeFrame) {
    BgmDirector* director = tryGetDirector(audioUser);
    if (director)
        director->tryPauseBgmIfLowPriority(playName, fadeFrame);
}

void startBgmSituation(const IUseAudioKeeper* audioUser, const char* situationName, bool isApply,
                       bool isForce) {
    BgmDirector* director = tryGetDirector(audioUser);
    if (director)
        director->startSituation(situationName, isApply, isForce, false);
}

void startBgmSituation(const IUseAudioKeeper* audioUser,
                       sead::PtrArray<IBgmParamsChanger>* paramsChangers, const char* situationName,
                       bool isApply, bool isForce) {
    BgmDirector* director = tryGetDirector(audioUser);
    if (director)
        director->startSituation(paramsChangers, situationName, isApply, isForce, false);
}

void forceStartBgmSituation(const IUseAudioKeeper* audioUser, const char* situationName,
                            bool isApply, bool isForce) {
    BgmDirector* director = tryGetDirector(audioUser);
    if (director)
        director->startSituation(situationName, isApply, isForce, true);
}

void endBgmSituation(const IUseAudioKeeper* audioUser, const char* situationName, bool isApply) {
    BgmDirector* director = tryGetDirector(audioUser);
    if (director)
        director->endSituation(situationName, isApply, false);
}

void endBgmSituation(const IUseAudioKeeper* audioUser,
                     sead::PtrArray<IBgmParamsChanger>* paramsChangers, const char* situationName,
                     bool isApply) {
    BgmDirector* director = tryGetDirector(audioUser);
    if (director)
        director->endSituation(paramsChangers, situationName, isApply, false);
}

void forceEndBgmSituation(const IUseAudioKeeper* audioUser, const char* situationName,
                          bool isApply) {
    BgmDirector* director = tryGetDirector(audioUser);
    if (director)
        director->endSituation(situationName, isApply, true);
}

void startBgm3D(const IUseAudioKeeper* audioUser) {
    Bgm3DParamsController* controller = tryGet3DParamsController(audioUser);
    if (controller)
        controller->start3D();
}

void endBgm3D(const IUseAudioKeeper* audioUser) {
    Bgm3DParamsController* controller = tryGet3DParamsController(audioUser);
    if (controller)
        controller->end3D();
}

void changeBgm3DParams(const IUseAudioKeeper* audioUser, f32 volume, f32 pitch, f32 lpfFreq,
                       f32 auxBusSend, bool isForce) {
    Bgm3DParamsController* controller = tryGet3DParamsController(audioUser);
    if (controller)
        controller->change3DParams(volume, pitch, lpfFreq, auxBusSend, isForce);
}

void activateBgm3DController(const IUseAudioKeeper* audioUser) {
    Bgm3DParamsController* controller = tryGet3DParamsController(audioUser);
    if (controller)
        controller->activate();
}

void deactivateBgm3DController(const IUseAudioKeeper* audioUser) {
    Bgm3DParamsController* controller = tryGet3DParamsController(audioUser);
    if (controller)
        controller->deactivate();
}

bool isActivateBgm3DController(const IUseAudioKeeper* audioUser) {
    Bgm3DParamsController* controller = tryGet3DParamsController(audioUser);
    return controller && controller->isActive();
}

s32 getActiveBgmSituationNum(const IUseAudioKeeper* audioUser) {
    BgmSituationDirector* director = tryGetSituationDirector(audioUser);
    if (!director)
        return 0;
    return director->getActiveSituationNum();
}

const char* getActiveBgmSituationName(const IUseAudioKeeper* audioUser, s32 index) {
    BgmSituationDirector* director = tryGetSituationDirector(audioUser);
    if (!director)
        return nullptr;
    return director->getActiveSituationName(index);
}

s32 getDeactiveBgmSituationNum(const IUseAudioKeeper* audioUser) {
    BgmSituationDirector* director = tryGetSituationDirector(audioUser);
    if (!director)
        return 0;
    return director->getDeactiveSituationNum();
}

const char* getDeactiveBgmSituationName(const IUseAudioKeeper* audioUser, s32 index) {
    BgmSituationDirector* director = tryGetSituationDirector(audioUser);
    if (!director)
        return nullptr;
    return director->getDeactiveSituationName(index);
}

void takeBgmSituationSnapShot(const IUseAudioKeeper* audioUser) {
    BgmSituationDirector* director = tryGetSituationDirector(audioUser);
    if (director)
        director->takeBgmSituationSnapShot();
}

void applyBgmSituationSnapShot(const IUseAudioKeeper* audioUser) {
    BgmSituationDirector* director = tryGetSituationDirector(audioUser);
    if (director)
        director->applyBgmSituationSnapShot();
}

void applyNoAppliedSituationRecord(const IUseAudioKeeper* audioUser) {
    BgmSituationDirector* director = tryGetSituationDirector(audioUser);
    if (director)
        director->applyNoAppliedSituationRecord();
}

bool isStartedBgmSituation(const IUseAudioKeeper* audioUser, const char* situationName) {
    BgmSituationDirector* director = tryGetSituationDirector(audioUser);
    if (!director || director->getActiveSituationNum() < 1)
        return false;

    for (s32 i = 0; i < director->getActiveSituationNum(); i++)
        if (isEqualString(situationName, director->getActiveSituationName(i)))
            return true;

    return false;
}

void endAllBgmSituation(const IUseAudioKeeper* audioUser) {
    BgmSituationDirector* director = tryGetSituationDirector(audioUser);
    if (director)
        director->clear();
}

void clearBgmDataForStepOverScene(const IUseAudioKeeper* audioUser) {
    stopAllBgm(audioUser, 0);

    BgmDirector* director = tryGetDirector(audioUser);
    if (director)
        director->clearPlayData();
}

void clearBgmPlayPointRecord(const IUseAudioKeeper* audioUser) {
    BgmDirector* director = tryGetDirector(audioUser);
    if (!director)
        return;

    BgmLineKeeper* lineKeeper = director->getBgmLineKeeper();
    if (lineKeeper)
        lineKeeper->clearPlayPointRecord();
}

void startBgmSyncedCurBgmBeat(const IUseAudioKeeper* audioUser, const BgmPlayingRequest& request,
                              s32 syncBeat) {
    AudioKeeper* audioKeeper = tryGetAudioKeeper(audioUser);
    AudioRequestKeeperSyncedBgm* requestKeeper = nullptr;
    if (audioKeeper)
        requestKeeper = audioKeeper->getAudioRequestKeeperSyncedBgm();

    requestKeeper->requestBgm(BgmPlayingType_Start, request, syncBeat);
}

void stopBgmSyncedCurBgmBeat(const IUseAudioKeeper* audioUser, const BgmPlayingRequest& request,
                             s32 syncBeat) {
    AudioKeeper* audioKeeper = tryGetAudioKeeper(audioUser);
    AudioRequestKeeperSyncedBgm* requestKeeper = nullptr;
    if (audioKeeper)
        requestKeeper = audioKeeper->getAudioRequestKeeperSyncedBgm();

    requestKeeper->requestBgm(BgmPlayingType_Stop, request, syncBeat);
}

void disableBgmLineChange(const IUseAudioKeeper* audioUser) {
    BgmDirector* director = tryGetDirector(audioUser);
    if (director)
        director->disableLineChange();
}

void enableBgmLineChange(const IUseAudioKeeper* audioUser) {
    BgmDirector* director = tryGetDirector(audioUser);
    if (director)
        director->enableLineChange();
}

bool isDisableBgmLineChange(const IUseAudioKeeper* audioUser) {
    BgmDirector* director = tryGetDirector(audioUser);
    return director && director->isDisableLineChange();
}

void disableBgmSituationChange(const IUseAudioKeeper* audioUser) {
    BgmSituationDirector* director = tryGetSituationDirector(audioUser);
    if (director)
        director->disableSituationChange();
}

void enableBgmSituationChange(const IUseAudioKeeper* audioUser, bool isForce) {
    BgmSituationDirector* director = tryGetSituationDirector(audioUser);
    if (director)
        director->enableSituationChange(isForce);
}

bool isDisableBgmSituationChange(const IUseAudioKeeper* audioUser) {
    BgmSituationDirector* director = tryGetSituationDirector(audioUser);
    return director && director->isDisableSituationChange();
}

BgmMultiPlayingController* tryAllocBgmMultiPlayingController(const IUseAudioKeeper* audioUser) {
    BgmDirector* director = tryGetDirector(audioUser);
    if (!director)
        return nullptr;
    return director->tryAllocMultiPlayingController();
}

void tryReleaseBgmMultiPlayingController(const IUseAudioKeeper* audioUser,
                                         BgmMultiPlayingController* controller) {
    BgmDirector* director = tryGetDirector(audioUser);
    if (director)
        director->tryReleaseMultiPlayingController(controller);
}

s32 getBgmLineNum(const IUseAudioKeeper* audioUser, bool isWithoutUpperLayer) {
    BgmDirector* director = tryGetDirector(audioUser);
    if (!director)
        return -1;
    return director->getBgmLineNum(isWithoutUpperLayer);
}

BgmLine* getBgmLineAccessor(const IUseAudioKeeper* audioUser, s32 index, bool isWithoutUpperLayer) {
    BgmDirector* director = tryGetDirector(audioUser);
    if (!director)
        return nullptr;
    return director->getBgmLineAccessor(index, isWithoutUpperLayer);
}

s32 getgBgmLineIndex(const IUseAudioKeeper* audioUser, const char* lineName,
                     bool isWithoutUpperLayer) {
    BgmDirector* director = tryGetDirector(audioUser);
    if (!director)
        return 0;
    return director->getBgmLineIndex(lineName, isWithoutUpperLayer);
}

void replaceBgmPlayInfoResourceName(const IUseAudioKeeper* audioUser, const char* playName,
                                    const char* resourceName) {
    BgmDirector* director = tryGetDirector(audioUser);
    if (!director)
        return;

    BgmPlayInfo* playInfo = alBgmFunction::tryFindBgmPlayInfo(director->getBgmDataBase(), playName);
    if (!playInfo)
        return;

    BgmLine* line = director->getBgmLineByLineName(playInfo->lineName);
    if (line)
        line->replacePlayInfoResourceName(playName, resourceName);
}

bool isEnableRhythmAnim(const IUseAudioKeeper* audioUser, const char*) {
    BgmRhythmSyncDirector* director = tryGetRhythmSyncDirector(audioUser);
    return director && director->isEnableRhythmAnim();
}

bool isTriggerBeat(const IUseAudioKeeper* audioUser) {
    BgmRhythmDetector* detector = tryGetRhythmDetector(audioUser);
    return detector && detector->isTriggerBeat();
}

bool isTriggerBackBeat(const IUseAudioKeeper* audioUser) {
    BgmRhythmDetector* detector = tryGetRhythmDetector(audioUser);
    return detector && detector->isTriggerBackBeat();
}

bool isTriggerMeasureTop(const IUseAudioKeeper* audioUser) {
    BgmRhythmDetector* detector = tryGetRhythmDetector(audioUser);
    return detector && detector->isTriggerMeasureTop();
}

f32 getBpm(const IUseAudioKeeper* audioUser) {
    BgmRhythmDetector* detector = tryGetRhythmDetector(audioUser);
    if (!detector)
        return -1.0f;
    return detector->getBpm();
}

f32 getBeatRate(const IUseAudioKeeper* audioUser) {
    BgmRhythmDetector* detector = tryGetRhythmDetector(audioUser);
    if (!detector)
        return -1.0f;
    return detector->getBeatRate();
}

f32 getCurBeat(const IUseAudioKeeper* audioUser) {
    BgmRhythmDetector* detector = tryGetRhythmDetector(audioUser);
    if (!detector)
        return -1.0f;
    return detector->getCurBeat();
}

f32 getCurBeatOnMeasure(const IUseAudioKeeper* audioUser) {
    BgmRhythmDetector* detector = tryGetRhythmDetector(audioUser);
    if (!detector)
        return -1.0f;
    return detector->getCurBeatOnMeasure();
}

f32 getLoopStartBeat(const IUseAudioKeeper* audioUser) {
    BgmRhythmDetector* detector = tryGetRhythmDetector(audioUser);
    if (!detector)
        return -1.0f;
    return detector->getLoopStartBeat();
}

f32 getLoopEndBeat(const IUseAudioKeeper* audioUser) {
    BgmRhythmDetector* detector = tryGetRhythmDetector(audioUser);
    if (!detector)
        return -1.0f;
    return detector->getLoopEndBeat();
}

f32 getFramePerMeasure(const IUseAudioKeeper* audioUser) {
    BgmRhythmDetector* detector = tryGetRhythmDetector(audioUser);
    if (!detector)
        return -1.0f;
    return detector->getFramePerMeasure();
}

s32 getBeatPerMeasure(const IUseAudioKeeper* audioUser) {
    BgmRhythmDetector* detector = tryGetRhythmDetector(audioUser);
    if (!detector)
        return -1;
    return detector->getBeatPerMeasure();
}

bool isChangeActiveBgmResourceName(const IUseAudioKeeper* audioUser) {
    BgmRhythmSyncDirector* director = tryGetRhythmSyncDirector(audioUser);
    return director && director->isChangeActiveResourceName();
}

bool tryRegistTargetBgmResourceName(const IUseAudioKeeper* audioUser, const char* resourceName) {
    BgmRhythmSyncDirector* director = tryGetRhythmSyncDirector(audioUser);
    return director && director->tryRegistTargetResourceName(resourceName);
}

void muteOnRunningLineTrack(IUseAudioKeeper*, u32, bool) {}

void muteOffRunningLineTrack(IUseAudioKeeper*, u32, bool) {}

BgmDataBase* BgmDataBase::create(const char* archiveName, const char*) {
    if (!isExistArchive(archiveName))
        return nullptr;

    Resource* resource = alAudioSystemFunction::tryGetAudioStationedResource(archiveName, nullptr);
    return new BgmDataBase(resource, nullptr);
}

// NONMATCHING: https://decomp.me/scratch/KWqbv
BgmDataBase::BgmDataBase(Resource* resource, Resource* localizedResource) {
    Resource* mainResource = resource;
    BgmDataBase* dataBase = this;
    Resource* localResource = localizedResource;
    dataBase->mBgmCombinedLineInfoList = nullptr;
    dataBase->mBgmPlayInfoList = nullptr;
    dataBase->mBgmResourceInfoList = nullptr;
    dataBase->mBgmResourceCategoryInfoList = nullptr;
    dataBase->mBgmStageInfoList = nullptr;
    dataBase->mBgmSituationInfoList = nullptr;
    dataBase->mBgmUserInfoList = nullptr;
    dataBase->mBgmDemoSyncedProcInfoList = nullptr;

    AudioInfoListWithParts<BgmCombinedLineInfo>* combinedLineInfoList = nullptr;
    if (mainResource->isExistFile("BgmLineInfoList.byml")) {
        ByamlIter rootIter(mainResource->getByml("BgmLineInfoList"));
        ByamlIter listIter;
        if (rootIter.tryGetIterByKey(&listIter, "CombinedLineInfoList"))
            combinedLineInfoList = createAudioInfoList<BgmCombinedLineInfo>(listIter, 0);
    }
    dataBase->mBgmCombinedLineInfoList = combinedLineInfoList;

    AudioInfoListWithParts<BgmPlayInfo>* playInfoList = nullptr;
    if (mainResource->isExistFile("BgmPlayInfoList.byml")) {
        ByamlIter rootIter(mainResource->getByml("BgmPlayInfoList"));
        ByamlIter listIter;
        if (rootIter.tryGetIterByKey(&listIter, "PlayInfoList"))
            playInfoList = createAudioInfoList<BgmPlayInfo>(listIter, 0);
    }
    dataBase->mBgmPlayInfoList = playInfoList;

    AudioInfoListWithParts<BgmResourceInfo>* resourceInfoList = nullptr;
    if (mainResource->isExistFile("BgmResourceInfoList.byml")) {
        ByamlIter rootIter(mainResource->getByml("BgmResourceInfoList"));
        ByamlIter listIter;
        if (rootIter.tryGetIterByKey(&listIter, "ResourceInfoList")) {
            u32 size = listIter.getSize();
            resourceInfoList = new AudioInfoListWithParts<BgmResourceInfo>;
            resourceInfoList->init(size, 0);
            resourceInfoList->setLinearSearch(true);

            u32 resourceId = 0;
            if (size != 0) {
                for (u32 i = 0; i < size; i++) {
                    ByamlIter infoIter;
                    listIter.tryGetIterByIndex(&infoIter, i);
                    BgmResourceInfo* info = BgmResourceInfo::createInfo(infoIter);
                    if (info) {
                        info->resourceId = resourceId;
                        resourceId++;
                        resourceInfoList->setInfo(info);
                    }
                }
            }

            resourceInfoList->sort();
        }
    }
    dataBase->mBgmResourceInfoList = resourceInfoList;

    AudioInfoListWithParts<BgmResourceCategoryInfo>* resourceCategoryInfoList = nullptr;
    if (mainResource->isExistFile("BgmResourceCategoryInfoList.byml")) {
        ByamlIter rootIter(mainResource->getByml("BgmResourceCategoryInfoList"));
        ByamlIter listIter;
        if (rootIter.tryGetIterByKey(&listIter, "ResourceCategoryInfoList"))
            resourceCategoryInfoList = createAudioInfoList<BgmResourceCategoryInfo>(listIter, 0);
    }
    dataBase->mBgmResourceCategoryInfoList = resourceCategoryInfoList;

    AudioInfoListWithParts<BgmStageInfo>* stageInfoList = nullptr;
    if (mainResource->isExistFile("BgmStageInfoList.byml")) {
        ByamlIter rootIter(mainResource->getByml("BgmStageInfoList"));
        ByamlIter listIter;
        if (rootIter.tryGetIterByKey(&listIter, "StageInfoList")) {
            if (localResource && localResource->isExistFile("BgmStageInfoList.byml")) {
                ByamlIter localizedRootIter(localResource->getByml("BgmStageInfoList"));
                ByamlIter localizedListIter;
                if (localizedRootIter.tryGetIterByKey(&localizedListIter, "StageInfoList"))
                    stageInfoList =
                        createAudioInfoList<BgmStageInfo>(listIter, localizedListIter, 0, 0);
                else
                    stageInfoList = createAudioInfoList<BgmStageInfo>(listIter, 0);
            } else {
                stageInfoList = createAudioInfoList<BgmStageInfo>(listIter, 0);
            }
        }
    }
    dataBase->mBgmStageInfoList = stageInfoList;

    AudioInfoListWithParts<BgmSituationInfo>* situationInfoList = nullptr;
    if (mainResource->isExistFile("BgmSituationInfoList.byml")) {
        ByamlIter rootIter(mainResource->getByml("BgmSituationInfoList"));
        ByamlIter listIter;
        if (rootIter.tryGetIterByKey(&listIter, "SituationInfoList"))
            situationInfoList = createAudioInfoList<BgmSituationInfo>(listIter, 0);
    }
    dataBase->mBgmSituationInfoList = situationInfoList;

    s32 entryNum = mainResource->getEntryNum("/");
    AudioInfoListWithParts<BgmUserInfo>* userInfoList = new AudioInfoListWithParts<BgmUserInfo>;
    userInfoList->setLinearSearch(false);
    userInfoList->init(entryNum, 0);

    for (s32 i = 0; i < entryNum; i++) {
        sead::FixedSafeString<128> fileName;
        mainResource->getEntryName(&fileName, "/", i);

        const char* name = fileName.cstr();
        if (isEqualString(name, "BgmLineInfoList.byml") ||
            isEqualString(name, "BgmPlayInfoList.byml") ||
            isEqualString(name, "BgmResourceInfoList.byml") ||
            isEqualString(name, "BgmResourceCategoryInfoList.byml") ||
            isEqualString(name, "BgmSituationInfoList.byml") ||
            isEqualString(name, "BgmStageInfoList.byml"))
            continue;

        ByamlIter iter(static_cast<const u8*>(mainResource->getOtherFile(fileName)));
        fileName.removeSuffix(".byml");
        BgmUserInfo* info = BgmUserInfo::createInfo(iter, fileName);
        userInfoList->setInfo(info);
    }

    sead::PtrArray<const BgmUserInfo>* userList = userInfoList->getListForSort();
    if (userList->size() < 10)
        userList->sort(BgmUserInfo::compareInfo);
    else
        userList->heapSort(BgmUserInfo::compareInfo);

    sead::PtrArray<AudioInfoList<BgmUserInfo>>* userParts = userInfoList->getParts();
    if (userParts)
        for (s32 i = 0; i < userParts->size(); i++)
            userParts->at(i)->sort();
    dataBase->mBgmUserInfoList = userInfoList;

    AudioInfoListWithParts<BgmDemoSyncedProcInfo>* demoSyncedProcInfoList = nullptr;
    if (mainResource->isExistFile("BgmDemoSyncedProcInfoList.byml")) {
        ByamlIter rootIter(mainResource->getByml("BgmDemoSyncedProcInfoList"));
        ByamlIter listIter;
        if (rootIter.tryGetIterByKey(&listIter, "DemoSyncedProcInfoList"))
            demoSyncedProcInfoList = createAudioInfoList<BgmDemoSyncedProcInfo>(listIter, 0);
    }
    if (demoSyncedProcInfoList) {
        for (s32 i = 0; i < demoSyncedProcInfoList->getSize(); i++) {
            const BgmDemoSyncedProcInfo* info = demoSyncedProcInfoList->getInfoAt(i);
            if (info->isPartsProc || !info->partsProcNameList)
                continue;

            for (s32 j = 0; j < info->partsProcNameList->size(); j++) {
                const char* partsName = info->partsProcNameList->at(j);
                const BgmDemoSyncedProcInfo* partsInfo = nullptr;
                const sead::PtrArray<const BgmDemoSyncedProcInfo>* list =
                    demoSyncedProcInfoList->getList();

                if (demoSyncedProcInfoList->isLinearSearch()) {
                    s32 size = list->size();
                    for (s32 k = 0; k < size; k++) {
                        const BgmDemoSyncedProcInfo* findInfo = list->at(k);
                        if (strcmp(findInfo->name, partsName) == 0) {
                            partsInfo = findInfo;
                            break;
                        }
                    }
                } else {
                    s32 index = -1;
                    s32 size = list->size();
                    if (size > 0) {
                        s32 start = 0;
                        s32 end = size - 1;
                        while (start < end) {
                            s32 mid = (start + end) / 2;
                            s32 compare = AudioInfoList<BgmDemoSyncedProcInfo>::compareInfoAndKey(
                                list->at(mid), partsName);
                            if (compare == 0) {
                                index = mid;
                                break;
                            }
                            if (compare < 0)
                                start = mid + 1;
                            else
                                end = mid;
                        }
                        if (index < 0 && AudioInfoList<BgmDemoSyncedProcInfo>::compareInfoAndKey(
                                             list->at(start), partsName) == 0)
                            index = start;
                    }
                    if (index >= 0)
                        partsInfo = list->at(index);
                }

                if (!partsInfo) {
                    s32 partsSize = demoSyncedProcInfoList->getPartsSize();
                    for (s32 k = 0; k < partsSize; k++) {
                        const AudioInfoList<BgmDemoSyncedProcInfo>* partsList =
                            demoSyncedProcInfoList->getPartsListAt(k);
                        list = partsList->getList();

                        if (partsList->isLinearSearch()) {
                            s32 size = list->size();
                            for (s32 l = 0; l < size; l++) {
                                const BgmDemoSyncedProcInfo* findInfo = list->at(l);
                                if (strcmp(findInfo->name, partsName) == 0) {
                                    partsInfo = findInfo;
                                    break;
                                }
                            }
                        } else {
                            s32 index = -1;
                            s32 size = list->size();
                            if (size > 0) {
                                s32 start = 0;
                                s32 end = size - 1;
                                while (start < end) {
                                    s32 mid = (start + end) / 2;
                                    s32 compare =
                                        AudioInfoList<BgmDemoSyncedProcInfo>::compareInfoAndKey(
                                            list->at(mid), partsName);
                                    if (compare == 0) {
                                        index = mid;
                                        break;
                                    }
                                    if (compare < 0)
                                        start = mid + 1;
                                    else
                                        end = mid;
                                }
                                if (index < 0 &&
                                    AudioInfoList<BgmDemoSyncedProcInfo>::compareInfoAndKey(
                                        list->at(start), partsName) == 0)
                                    index = start;
                            }
                            if (index >= 0)
                                partsInfo = list->at(index);
                        }

                        if (partsInfo)
                            break;
                    }
                }

                if (!partsInfo)
                    continue;

                if (info->demoProcInfoList && partsInfo->demoProcInfoList)
                    info->demoProcInfoList->addParts(partsInfo->demoProcInfoList);
                if (info->demoEndProcInfoList && partsInfo->demoEndProcInfoList)
                    info->demoEndProcInfoList->addParts(partsInfo->demoEndProcInfoList);
            }
        }
    }
    dataBase->mBgmDemoSyncedProcInfoList = demoSyncedProcInfoList;
}

template s32 AudioInfoList<BgmDemoSyncedProcInfo>::compareInfoAndKey(const BgmDemoSyncedProcInfo*,
                                                                     const char*);

}  // namespace al
