#include "System/GameDataUtil.h"

#include <prim/seadSafeString.h>

#include "Library/Base/StringUtil.h"
#include "Library/Layout/LayoutActor.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorSceneInfo.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Scene/GameDataHolderBase.h"
#include "Library/Scene/Scene.h"

#include "MapObj/KoopaShip.h"
#include "MapObj/ShineTowerRocket.h"
#include "MapObj/WorldWarpHole.h"
#include "ModeBalloon/RankingLocalFunction.h"
#include "Npc/NpcEventDirector.h"
#include "Scene/QuestInfo.h"
#include "System/CollectBgm.h"
#include "System/GameDataFile.h"
#include "System/GameDataFunction.h"
#include "System/GameDataHolder.h"
#include "System/GameDataHolderAccessor.h"
#include "System/HintPhotoData.h"
#include "System/NetworkUploadFlag.h"
#include "System/RaceRecord.h"
#include "System/SaveObjInfo.h"
#include "System/ShopTalkData.h"
#include "System/TimeBalloonSaveData.h"
#include "System/VisitStageData.h"
#include "System/WorldList.h"
#include "System/WorldWarpTalkData.h"

namespace rs {

inline GameDataHolder* getGameDataHolderFromActor(const al::LiveActor* actor) {
    return (GameDataHolder*)(actor->getSceneInfo()->gameDataHolder);
}

inline GameDataHolderAccessor getGameDataHolderAccessor(const al::IUseSceneObjHolder* user) {
    return GameDataHolderAccessor(user);
}

SaveObjInfo* createSaveObjInfoWriteSaveData(const al::ActorInitInfo& actorInitInfo) {
    return createSaveObjInfoWriteSaveData(actorInitInfo, al::getPlacementInfo(actorInitInfo));
}

SaveObjInfo* createSaveObjInfoWriteSaveData(const al::ActorInitInfo& actorInitInfo,
                                            const al::PlacementInfo& placementInfo) {
    return createSaveObjInfoWriteSaveData(actorInitInfo, al::createPlacementId(placementInfo));
}

SaveObjInfo* createSaveObjInfoNoWriteSaveDataInSameWorld(const al::ActorInitInfo& actorInitInfo) {
    return createSaveObjInfoNoWriteSaveDataInSameWorld(actorInitInfo,
                                                       al::getPlacementInfo(actorInitInfo));
}

SaveObjInfo* createSaveObjInfoNoWriteSaveDataInSameWorld(const al::ActorInitInfo& actorInitInfo,
                                                         const al::PlacementInfo& placementInfo) {
    return createSaveObjInfoNoWriteSaveDataInSameWorld(actorInitInfo,
                                                       al::createPlacementId(placementInfo));
}

SaveObjInfo*
createSaveObjInfoNoWriteSaveDataInSameWorldResetMiniGame(const al::ActorInitInfo& actorInitInfo) {
    return createSaveObjInfoNoWriteSaveDataInSameWorldResetMiniGame(
        actorInitInfo, al::getPlacementInfo(actorInitInfo));
}

SaveObjInfo*
createSaveObjInfoNoWriteSaveDataInSameWorldResetMiniGame(const al::ActorInitInfo& actorInitInfo,
                                                         const al::PlacementInfo& placementInfo) {
    return createSaveObjInfoNoWriteSaveDataInSameWorldResetMiniGame(
        actorInitInfo, al::createPlacementId(placementInfo));
}

SaveObjInfo*
createSaveObjInfoNoWriteSaveDataInSameScenario(const al::ActorInitInfo& actorInitInfo) {
    return createSaveObjInfoNoWriteSaveDataInSameScenario(actorInitInfo,
                                                          al::getPlacementInfo(actorInitInfo));
}

SaveObjInfo*
createSaveObjInfoNoWriteSaveDataInSameScenario(const al::ActorInitInfo& actorInitInfo,
                                               const al::PlacementInfo& placementInfo) {
    return createSaveObjInfoNoWriteSaveDataInSameScenario(actorInitInfo,
                                                          al::createPlacementId(placementInfo));
}

SaveObjInfo* createSaveObjInfoWriteSaveData(const al::ActorInitInfo& actorInitInfo,
                                            const al::PlacementId* placementId) {
    return SaveObjInfo::createSaveObjInfoWriteSaveData(actorInitInfo, placementId);
}

SaveObjInfo* createSaveObjInfoNoWriteSaveDataInSameWorld(const al::ActorInitInfo& actorInitInfo,
                                                         const al::PlacementId* placementId) {
    return SaveObjInfo::createSaveObjInfoNoWriteSaveDataInSameWorld(actorInitInfo, placementId);
}

SaveObjInfo*
createSaveObjInfoNoWriteSaveDataInSameWorldResetMiniGame(const al::ActorInitInfo& actorInitInfo,
                                                         const al::PlacementId* placementId) {
    return SaveObjInfo::createSaveObjInfoNoWriteSaveDataInSameWorldResetMiniGame(actorInitInfo,
                                                                                 placementId);
}

SaveObjInfo* createSaveObjInfoNoWriteSaveDataInSameScenario(const al::ActorInitInfo& actorInitInfo,
                                                            const al::PlacementId* placementId) {
    return SaveObjInfo::createSaveObjInfoNoWriteSaveDataInSameScenario(actorInitInfo, placementId);
}

void onSaveObjInfo(SaveObjInfo* saveObjInfo) {
    saveObjInfo->on();
}

void offSaveObjInfo(SaveObjInfo* saveObjInfo) {
    saveObjInfo->off();
}

bool isOnSaveObjInfo(const SaveObjInfo* saveObjInfo) {
    return saveObjInfo->isOn();
}

// TODO: rename parameters `placement1`, `placement2`
void setGrowFlowerTime(const al::LiveActor* actor, const al::PlacementId* placement1,
                       const al::PlacementId* placement2, u64 time) {
    getGameDataHolderFromActor(actor)->getGameDataFile()->setGrowFlowerTime(placement1, placement2,
                                                                            time);
}

void setGrowFlowerTime(const al::LiveActor* actor, const al::PlacementId* placement, u64 time) {
    getGameDataHolderFromActor(actor)->getGameDataFile()->setGrowFlowerTime(placement, time);
}

u64 getGrowFlowerTime(const al::LiveActor* actor, const al::PlacementId* placement) {
    return getGameDataHolderFromActor(actor)->getGameDataFile()->getGrowFlowerTime(placement);
}

bool isUsedGrowFlowerSeed(const al::LiveActor* actor, const al::PlacementId* placement) {
    return getGameDataHolderFromActor(actor)->getGameDataFile()->isUsedGrowFlowerSeed(placement);
}

void addGrowFlowerGrowLevel(const al::LiveActor* actor, const al::PlacementId* placement,
                            u32 growLevel) {
    getGameDataHolderFromActor(actor)->getGameDataFile()->addGrowFlowerGrowLevel(placement,
                                                                                 growLevel);
}

u32 getGrowFlowerGrowLevel(const al::LiveActor* actor, const al::PlacementId* placement) {
    return getGameDataHolderFromActor(actor)->getGameDataFile()->getGrowFlowerGrowLevel(placement);
}

const char* findGrowFlowerPotIdFromSeedId(const al::LiveActor* actor,
                                          const al::PlacementId* placement) {
    return getGameDataHolderFromActor(actor)->getGameDataFile()->findGrowFlowerPotIdFromSeedId(
        placement);
}

bool isPlayFirstTimeBalloon(const al::LiveActor* actor) {
    return !getGameDataHolderFromActor(actor)
                ->getGameDataFile()
                ->getTimeBalloonSaveData()
                ->isExplainedTutorial();
}

void playTimeBalloonFirst(const al::LiveActor* actor) {
    getGameDataHolderFromActor(actor)
        ->getGameDataFile()
        ->getTimeBalloonSaveData()
        ->setExplainedTutorial();
}

void onShortExplainGet(const al::LiveActor* actor) {
    getGameDataHolderFromActor(actor)
        ->getGameDataFile()
        ->getTimeBalloonSaveData()
        ->setShortExplainGet();
}

void onShortExplainSet(const al::LiveActor* actor) {
    getGameDataHolderFromActor(actor)
        ->getGameDataFile()
        ->getTimeBalloonSaveData()
        ->setShortExplainSet();
}

bool isShortExplainGet(const al::LiveActor* actor) {
    return getGameDataHolderFromActor(actor)
        ->getGameDataFile()
        ->getTimeBalloonSaveData()
        ->isShortExplainGet();
}

bool isShortExplainSet(const al::LiveActor* actor) {
    return getGameDataHolderFromActor(actor)
        ->getGameDataFile()
        ->getTimeBalloonSaveData()
        ->isShortExplainSet();
}

void onReactedLuigiCostume(const al::LiveActor* actor) {
    getGameDataHolderFromActor(actor)
        ->getGameDataFile()
        ->getTimeBalloonSaveData()
        ->setReactedLuigiCostume();
}

bool isReactedLuigiCostume(const al::LiveActor* actor) {
    return getGameDataHolderFromActor(actor)
        ->getGameDataFile()
        ->getTimeBalloonSaveData()
        ->isReactedLuigiCostume();
}

s32 getPlayerRank(const al::LiveActor* actor) {
    return getPlayerRank(getGameDataHolderFromActor(actor));
}

s32 getPlayerRank(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getTimeBalloonSaveData()->getPlayerRank();
}

void setPlayerRank(const al::LiveActor* actor, s32 rank) {
    setPlayerRank(getGameDataHolderFromActor(actor), rank);
}

void setPlayerRank(GameDataHolderAccessor accessor, s32 rank) {
    accessor->getGameDataFile()->getTimeBalloonSaveData()->setPlayerRank(rank);
}

u64 getPrincipalId(const al::LiveActor* actor) {
    return getGameDataHolderFromActor(actor)
        ->getGameDataFile()
        ->getTimeBalloonSaveData()
        ->getPrincipalId();
}

void setPrincipalId(const al::LiveActor* actor, u64 principalId) {
    getGameDataHolderFromActor(actor)->getGameDataFile()->getTimeBalloonSaveData()->setPrincipalId(
        principalId);
}

bool isAnswerCorrectSphinxQuiz(GameDataHolderAccessor accessor, const char* worldName) {
    return accessor->getGameDataFile()->isAnswerCorrectSphinxQuiz(
        accessor->getWorldList()->tryFindWorldIndexByDevelopName(worldName));
}

void answerCorrectSphinxQuiz(const al::LiveActor* actor) {
    getGameDataHolderFromActor(actor)->getGameDataFile()->answerCorrectSphinxQuiz();
}

bool isAnswerCorrectSphinxQuizAll(GameDataHolderAccessor accessor, const char* worldName) {
    return accessor->getGameDataFile()->isAnswerCorrectSphinxQuizAll(
        accessor->getWorldList()->tryFindWorldIndexByDevelopName(worldName));
}

void answerCorrectSphinxQuizAll(const al::LiveActor* actor) {
    getGameDataHolderFromActor(actor)->getGameDataFile()->answerCorrectSphinxQuizAll();
}

bool isTalkAlreadyLocalLanguage(const NpcEventDirector* npcEventDirector) {
    return getGameDataHolderAccessor(npcEventDirector)
        ->getGameDataFile()
        ->isTalkAlreadyLocalLanguage();
}

void talkLocalLanguage(const al::LiveActor* actor) {
    getGameDataHolderFromActor(actor)->getGameDataFile()->talkLocalLanguage();
}

const char* getLocalLanguageName(const al::LiveActor* actor) {
    return getGameDataHolderFromActor(actor)->getWorldList()->getWorldDevelopName(
        getGameDataHolderFromActor(actor)->getGameDataFile()->getCurrentWorldId());
}

const char* getLocalLanguageName(const NpcEventDirector* npcEventDirector) {
    return getGameDataHolderAccessor(npcEventDirector)
        ->getWorldList()
        ->getWorldDevelopName(
            getGameDataHolderAccessor(npcEventDirector)->getGameDataFile()->getCurrentWorldId());
}

bool isOnFlagKinopioBrigadeNpcFirstTalk(const al::LiveActor* actor) {
    return getGameDataHolderAccessor(actor)->getGameDataFile()->isKinopioBrigadeNpcFirstTalk();
}

void onFlagKinopioBrigadeNpcFirstTalk(const al::LiveActor* actor) {
    return getGameDataHolderAccessor(actor)->getGameDataFile()->kinopioBrigadeNpcFirstTalk();
}

bool isOnFlagHintNpcFirstTalk(const al::LiveActor* actor) {
    return getGameDataHolderAccessor(actor)->getGameDataFile()->isHintNpcFirstTalk();
}

void onFlagHintNpcFirstTalk(const al::LiveActor* actor) {
    getGameDataHolderAccessor(actor)->getGameDataFile()->hintNpcFirstTalk();
}

void playScenarioStartCamera(const al::LiveActor* actor, const QuestInfo* questInfo) {
    getGameDataHolderFromActor(actor)->playScenarioStartCamera(questInfo->getQuestNo());
}

void playScenarioStartCamera(const al::LiveActor* actor, s32 idx) {
    getGameDataHolderFromActor(actor)->playScenarioStartCamera(idx);
}

void setExistKoopaShip(const KoopaShip* koopaShip) {
    getGameDataHolderFromActor(koopaShip)->setExistKoopaShip(true);
}

void resetExistKoopaShip(const KoopaShip* koopaShip) {
    getGameDataHolderFromActor(koopaShip)->setExistKoopaShip(false);
}

bool isExistKoopaShip(const ShineTowerRocket* shineTowerRocket) {
    return getGameDataHolderFromActor(shineTowerRocket)->isExistKoopaShip() ||
           isExistKoopaShipInSky(shineTowerRocket);
}

bool isExistKoopaShipInSky(const ShineTowerRocket* shineTowerRocket) {
    return getGameDataHolderFromActor(shineTowerRocket)->getGameDataFile()->getScenarioNo() == 1 &&
           getGameDataHolderFromActor(shineTowerRocket)->getGameDataFile()->getCurrentWorldId() ==
               GameDataFunction::getWorldIndexSky();
}

bool tryUnlockShineName(const al::LiveActor* actor, s32 shineIdx) {
    return getGameDataHolderFromActor(actor)->getGameDataFile()->tryUnlockShineName(
        GameDataFunction::getCurrentWorldId(actor), shineIdx);
}

// TODO: rename parameters unk, out
void calcShineIndexTableNameAvailable(s32* unk, s32* out, const al::LiveActor* actor) {
    getGameDataHolderFromActor(actor)->getGameDataFile()->calcShineIndexTableNameAvailable(
        unk, out, GameDataFunction::getCurrentWorldId(actor));
}

// TODO: rename parameters unk, out
void calcShineIndexTableNameUnlockable(s32* unk, s32* out, const al::LiveActor* actor) {
    getGameDataHolderFromActor(actor)->getGameDataFile()->calcShineIndexTableNameUnlockable(
        unk, out, GameDataFunction::getCurrentWorldId(actor));
}

s32 calcWorldIdByWorldWarpHole(const WorldWarpHole* worldWarpHole, const char* changeStageId) {
    sead::FixedSafeString<0x80> stageName;
    sead::FixedSafeString<0x80> label;
    getGameDataHolderFromActor(worldWarpHole)
        ->calcWorldWarpHoleLabelAndStageName(&label, &stageName, changeStageId,
                                             GameDataFunction::getCurrentWorldId(worldWarpHole));
    if (label.isEmpty())
        return -1;

    return getGameDataHolderFromActor(worldWarpHole)
        ->getWorldList()
        ->tryFindWorldIndexByMainStageName(stageName.cstr());
}

bool checkIsOpenWorldWarpHoleCurrentScenario(const WorldWarpHole* worldWarpHole) {
    s32 scenarioNo = GameDataFunction::getScenarioNo(worldWarpHole);
    s32 worldId = GameDataFunction::getCurrentWorldId(worldWarpHole);
    return getGameDataHolderFromActor(worldWarpHole)
        ->checkIsOpenWorldWarpHoleInScenario(worldId, scenarioNo);
}

bool isKidsMode(const al::LiveActor* actor) {
    return isKidsMode(getGameDataHolderFromActor(actor));
}

bool isKidsMode(const al::LayoutActor* layout) {
    return isKidsMode(GameDataHolderAccessor(layout));
}

bool isKidsMode(const al::Scene* scene) {
    return isKidsMode(GameDataHolderAccessor(scene));
}

bool isKidsMode(const al::IUseSceneObjHolder* user) {
    return isKidsMode(GameDataHolderAccessor(user));
}

bool isKidsMode(const GameDataHolder* holder) {
    // TODO: GameDataHolderAccessor should be `const GameDataHolder*`
    if (GameDataFunction::isRaceStartFlag(const_cast<GameDataHolder*>(holder)) ||
        GameDataFunction::isTimeBalloonSequence(const_cast<GameDataHolder*>(holder))) {
        return false;
    }

    return holder->getGameDataFile()->isKidsMode();
}

bool isEnableCapMessageLifeOneKidsMode(const al::LiveActor* actor) {
    return getGameDataHolderFromActor(actor)
        ->getGameDataFile()
        ->isEnableCapMessageLifeOneKidsMode();
}

void disableCapMessageLifeOneKidsMode(const al::LiveActor* actor) {
    return getGameDataHolderFromActor(actor)->getGameDataFile()->disableCapMessageLifeOneKidsMode();
}

bool isInvalidChangeStage(const al::LiveActor* actor) {
    return isSequenceTimeBalloonOrRace(actor);
}

bool isSequenceTimeBalloonOrRace(const al::LiveActor* actor) {
    return isSequenceTimeBalloonOrRace(getGameDataHolderAccessor(actor));
}

bool isSequenceTimeBalloonOrRace(const GameDataHolder* holder) {
    return GameDataFunction::isRaceStart((GameDataHolder*)holder) ||
           GameDataFunction::isTimeBalloonSequence((GameDataHolder*)holder);
}

bool isTreasureBoxDeadStage(const al::LiveActor* actor) {
    return isSequenceTimeBalloonOrRace(actor);
}

void findRaceRecord(bool* isExistRecord, bool* isRecordSet, s32* record, s32* bestRecord,
                    s32* lapRecord, const al::LiveActor* actor, const char* raceName) {
    RaceRecord* raceRecord =
        getGameDataHolderFromActor(actor)->getGameDataFile()->findRaceRecord(raceName);

    if (record)
        *record = raceRecord->getRecord();

    if (bestRecord)
        *bestRecord = raceRecord->getBestRecord();

    if (lapRecord)
        *lapRecord = raceRecord->getLapRecord();

    if (isExistRecord)
        *isExistRecord = raceRecord->isExistRecord();

    if (isRecordSet)
        *isRecordSet = raceRecord->isRecordSet();
}

void findRaceRecord(bool* isExistRecord, bool* isRecordSet, s32* record, s32* bestRecord,
                    s32* lapRecord, const al::Scene* scene, const char* raceName) {
    RaceRecord* raceRecord =
        getGameDataHolderAccessor(scene)->getGameDataFile()->findRaceRecord(raceName);

    if (record)
        *record = raceRecord->getRecord();

    if (bestRecord)
        *bestRecord = raceRecord->getBestRecord();

    if (lapRecord)
        *lapRecord = raceRecord->getLapRecord();

    if (isExistRecord)
        *isExistRecord = raceRecord->isExistRecord();

    if (isRecordSet)
        *isRecordSet = raceRecord->isRecordSet();
}

void findRaceRecordRaceManRace(bool* isExistRecord, bool* isRecordSet, s32* record, s32* bestRecord,
                               s32* lapRecord, const al::Scene* scene) {
    findRaceRecordRaceManRaceWithWorldId(isExistRecord, isRecordSet, record, bestRecord, lapRecord,
                                         scene,
                                         GameDataFunction::getCurrentWorldIdNoDevelop(scene));
}

void findRaceRecordRaceManRace(bool* isExistRecord, bool* isRecordSet, s32* record, s32* bestRecord,
                               s32* lapRecord, const al::LiveActor* actor) {
    findRaceRecordRaceManRaceWithWorldId(isExistRecord, isRecordSet, record, bestRecord, lapRecord,
                                         actor,
                                         GameDataFunction::getCurrentWorldIdNoDevelop(actor));
}

void findRaceRecordRaceManRaceWithWorldId(bool* isExistRecord, bool* isRecordSet, s32* record,
                                          s32* bestRecord, s32* lapRecord, const al::Scene* scene,
                                          s32 worldId) {
    al::StringTmp<32> raceName("RaceManRace_%d", worldId);
    findRaceRecord(isExistRecord, isRecordSet, record, bestRecord, lapRecord, scene,
                   raceName.cstr());
}

void findRaceRecordRaceManRaceWithWorldId(bool* isExistRecord, bool* isRecordSet, s32* record,
                                          s32* bestRecord, s32* lapRecord,
                                          const al::LiveActor* actor, s32 worldId) {
    al::StringTmp<32> raceName("RaceManRace_%d", worldId);
    findRaceRecord(isExistRecord, isRecordSet, record, bestRecord, lapRecord, actor,
                   raceName.cstr());
}

void findRaceRecordRadicon(bool* isExistRecord, bool* isRecordSet, s32* record, s32* bestRecord,
                           s32* lapRecord, const al::Scene* scene) {
    al::StringTmp<32> raceName("Radicon");
    findRaceRecord(isExistRecord, isRecordSet, record, bestRecord, lapRecord, scene,
                   raceName.cstr());
}

void findRaceRecordRadicon(bool* isExistRecord, bool* isRecordSet, s32* record, s32* bestRecord,
                           s32* lapRecord, const al::LiveActor* actor) {
    al::StringTmp<32> raceName("Radicon");
    findRaceRecord(isExistRecord, isRecordSet, record, bestRecord, lapRecord, actor,
                   raceName.cstr());
}

void findRaceRecordYukimaru(bool* isExistRecord, bool* isRecordSet, s32* record, s32* bestRecord,
                            s32* lapRecord, const al::Scene* scene, s32 level) {
    al::StringTmp<32> raceName;
    switch (level) {
    case 0:
        raceName.format("Yukimaru_1");
        break;
    case 1:
        raceName.format("Yukimaru_2");
        break;
    }

    findRaceRecord(isExistRecord, isRecordSet, record, bestRecord, lapRecord, scene,
                   raceName.cstr());
}

void findRaceRecordYukimaru(bool* isExistRecord, bool* isRecordSet, s32* record, s32* bestRecord,
                            s32* lapRecord, const al::LiveActor* actor, s32 level) {
    al::StringTmp<32> raceName;
    switch (level) {
    case 0:
        raceName.format("Yukimaru_1");
        break;
    case 1:
        raceName.format("Yukimaru_2");
        break;
    }

    findRaceRecord(isExistRecord, isRecordSet, record, bestRecord, lapRecord, actor,
                   raceName.cstr());
}

inline void writeRaceRecord(const GameDataHolder* dataHolder, const char* raceName, s32 record,
                            s32 bestRecord, s32 lapRecord) {
    RaceRecord* raceRecord = dataHolder->getGameDataFile()->findRaceRecord(raceName);
    raceRecord->setRecord(record, bestRecord, lapRecord, false);

    dataHolder->getGameDataFile()->getNetworkUploadFlag()->updateScore(
        RankingLocalFunction::findRankingCategoryByRaceRecordName(raceName));
}

void writeRaceRecord(const al::LiveActor* actor, const char* raceName, s32 record, s32 bestRecord,
                     s32 lapRecord) {
    writeRaceRecord(getGameDataHolderFromActor(actor), raceName, record, bestRecord, lapRecord);
}

void writeRaceRecord(const al::Scene* scene, const char* raceName, s32 record, s32 bestRecord,
                     s32 lapRecord) {
    writeRaceRecord(getGameDataHolderAccessor(scene), raceName, record, bestRecord, lapRecord);
}

s32 getJumpingRopeBestCount(const al::LiveActor* actor) {
    return getGameDataHolderAccessor(actor)->getGameDataFile()->getJumpingRopeBestCount();
}

s32 getJumpingRopeBestCount(const al::Scene* scene) {
    return getGameDataHolderAccessor(scene)->getGameDataFile()->getJumpingRopeBestCount();
}

void setJumpingRopeBestCount(const al::LiveActor* actor, s32 count) {
    getGameDataHolderAccessor(actor)->getGameDataFile()->setJumpingRopeBestCount(count);
}

void setJumpingRopeBestCount(const al::Scene* scene, s32 count) {
    getGameDataHolderAccessor(scene)->getGameDataFile()->setJumpingRopeBestCount(count);
}

s32 getJumpingRopeDayCount(const al::LiveActor* actor) {
    return getGameDataHolderAccessor(actor)->getGameDataFile()->getJumpingRopeDayCount();
}

void setJumpingRopeDayCount(const al::LiveActor* actor, s32 count) {
    GameDataFile* dataFile = getGameDataHolderAccessor(actor)->getGameDataFile();
    dataFile->existRecordJumpingRope();
    dataFile->existRecordInDayJumpingRope();
    dataFile->setJumpingRopeDayCount(count);
}

void setJumpingRopeUpdateScoreFlag(const al::LiveActor* actor) {
    getGameDataHolderAccessor(actor)->getGameDataFile()->setUpdateJumpingRopeScoreFlag();
}

bool isExistRecordJumpingRope(const al::LiveActor* actor) {
    return getGameDataHolderAccessor(actor)->getGameDataFile()->isExistRecordJumpingRope();
}

bool isExistRecordJumpingRope(const al::Scene* scene) {
    return getGameDataHolderAccessor(scene)->getGameDataFile()->isExistRecordJumpingRope();
}

bool isExistRecordInDayJumpingRope(const al::LayoutActor* layout) {
    return getGameDataHolderAccessor(layout)->getGameDataFile()->isExistRecordInDayJumpingRope();
}

s32 getVolleyballBestCount(const al::LiveActor* actor) {
    return getGameDataHolderAccessor(actor)->getGameDataFile()->getVolleyballBestCount();
}

s32 getVolleyballBestCount(const al::Scene* scene) {
    return getGameDataHolderAccessor(scene)->getGameDataFile()->getVolleyballBestCount();
}

void setVolleyballBestCount(const al::LiveActor* actor, s32 count) {
    getGameDataHolderAccessor(actor)->getGameDataFile()->setVolleyballBestCount(count);
}

void setVolleyballBestCount(const al::Scene* scene, s32 count) {
    getGameDataHolderAccessor(scene)->getGameDataFile()->setVolleyballBestCount(count);
}

s32 getVolleyballDayCount(const al::LiveActor* actor) {
    return getGameDataHolderAccessor(actor)->getGameDataFile()->getVolleyballDayCount();
}

void setVolleyballDayCount(const al::LiveActor* actor, s32 count) {
    GameDataFile* dataFile = getGameDataHolderAccessor(actor)->getGameDataFile();
    dataFile->existRecordVolleyball();
    dataFile->existRecordInDayVolleyball();
    dataFile->setVolleyballDayCount(count);
}

void setVolleyballUpdateScoreFlag(const al::LiveActor* actor) {
    getGameDataHolderAccessor(actor)->getGameDataFile()->setUpdateVolleyballScoreFlag();
}

bool isExistRecordVolleyball(const al::LiveActor* actor) {
    return getGameDataHolderAccessor(actor)->getGameDataFile()->isExistRecordVolleyball();
}

bool isExistRecordVolleyball(const al::Scene* scene) {
    return getGameDataHolderAccessor(scene)->getGameDataFile()->isExistRecordVolleyball();
}

bool isExistRecordInDayVolleyball(const al::LayoutActor* layout) {
    return getGameDataHolderAccessor(layout)->getGameDataFile()->isExistRecordInDayVolleyball();
}

void writeRaceRecordRaceManRace(const al::LiveActor* actor, s32 record) {
    al::StringTmp<32> raceName("RaceManRace_%d",
                               GameDataFunction::getCurrentWorldIdNoDevelop(actor));
    RaceRecord* raceRecord =
        getGameDataHolderFromActor(actor)->getGameDataFile()->findRaceRecord(raceName.cstr());
    raceRecord->setRecord(record, record, record, false);

    getGameDataHolderFromActor(actor)->getGameDataFile()->getNetworkUploadFlag()->updateScore(
        RankingLocalFunction::findRankingCategoryByRaceRecordName(raceName.cstr()));
}

bool isTalkFirstAmiiboNpc(const al::LiveActor* actor) {
    return getGameDataHolderFromActor(actor)->getGameDataFile()->isTalkFirstAmiiboNpc();
}

void offFlagTalkFirstAmiiboNpc(const al::LiveActor* actor) {
    getGameDataHolderFromActor(actor)->getGameDataFile()->offTalkFirstAmiiboNpc();
}

bool checkGetShineForWorldTravelingPeach(GameDataHolderAccessor accessor, const char* worldName) {
    return accessor->getGameDataFile()->calcGetShineNumByObjectNameWithWorldId(
               "世界旅行ピーチ", GameDataFunction::findWorldIdByDevelopName(accessor, worldName)) >
           0;
}

bool checkEnableAppearMoonWorldTravelingPeach(const al::LiveActor* actor) {
    const char* worldNames[13] = {"Cap",  "Waterfall", "Sand", "Forest", "Lake",   "Cloud", "Clash",
                                  "City", "Sea",       "Snow", "Lava",   "Attack", "Sky"};

    for (s32 i = 0; i < 13; i++)
        if (!checkGetShineForWorldTravelingPeach(actor, worldNames[i]))
            return false;
    return true;
}

bool isStartWorldTravelingPeach(const al::LiveActor* actor) {
    return getGameDataHolderFromActor(actor)->getGameDataFile()->isStartWorldTravelingPeach();
}

void startWorldTravelingPeach(const al::LiveActor* actor) {
    getGameDataHolderFromActor(actor)->getGameDataFile()->startWorldTravelingPeach();
}

bool checkGetEnoughShine(const al::LiveActor* actor, s32 worldId) {
    bool isCountTotal = false;
    return getGameDataHolderFromActor(actor)->getGameDataFile()->getShineNum(worldId) >=
           getGameDataHolderFromActor(actor)->findUnlockShineNum(&isCountTotal, worldId);
}

bool checkGetEnoughShineInCurrentWorld(const al::LiveActor* actor) {
    bool isCountTotal = false;
    s32 worldId = GameDataFunction::getCurrentWorldIdNoDevelop(actor);
    return getGameDataHolderFromActor(actor)->getGameDataFile()->getShineNum(worldId) >=
           getGameDataHolderFromActor(actor)->findUnlockShineNum(&isCountTotal, worldId);
}

void setFlagOnStartKoopaCapture(const al::LiveActor* actor) {
    getGameDataHolderFromActor(actor)->getGameDataFile()->startKoopaCapture();
}

bool isStartKoopaCapture(const al::LiveActor* actor) {
    return getGameDataHolderFromActor(actor)->getGameDataFile()->isStartKoopaCapture();
}

bool isAlreadyTalkPrevWorldInWorldWarp(const al::LiveActor* actor, s32 worldId) {
    return getGameDataHolderFromActor(actor)
        ->getGameDataFile()
        ->getWorldWarpTalkData()
        ->isAlreadyTalkPrevWorld(worldId);
}

bool isAlreadyTalkNextWorldInWorldWarp(const al::LiveActor* actor, s32 worldId) {
    return getGameDataHolderFromActor(actor)
        ->getGameDataFile()
        ->getWorldWarpTalkData()
        ->isAlreadyTalkNextWorld(worldId);
}

void setAlreadyTalkPrevWorldInWorldWarp(const al::LiveActor* actor, s32 worldId) {
    return getGameDataHolderFromActor(actor)
        ->getGameDataFile()
        ->getWorldWarpTalkData()
        ->setAlreadyTalkPrevWorld(worldId);
}

void setAlreadyTalkNextWorldInWorldWarp(const al::LiveActor* actor, s32 worldId) {
    return getGameDataHolderFromActor(actor)
        ->getGameDataFile()
        ->getWorldWarpTalkData()
        ->setAlreadyTalkNextWorld(worldId);
}

bool checkNeedTalkPrevWorld(const al::LiveActor* actor) {
    s32 worldId = GameDataFunction::getPrevWorldId(actor);
    if (isAlreadyTalkPrevWorldInWorldWarp(actor, worldId))
        return false;
    return checkGetEnoughShine(actor, worldId);
}

s32 getTipsIdInWorldWarp(const al::LiveActor* actor) {
    return getGameDataHolderFromActor(actor)
        ->getGameDataFile()
        ->getWorldWarpTalkData()
        ->getTipsId();
}

void addTipsIdInWorldWarp(const al::LiveActor* actor) {
    getGameDataHolderFromActor(actor)->getGameDataFile()->getWorldWarpTalkData()->addTipsId();
}

void setTalkSpecialInWorldWarp(const al::LiveActor* actor, bool isTalkFindKoopa,
                               bool isTalkBossRaid) {
    getGameDataHolderFromActor(actor)->getGameDataFile()->getWorldWarpTalkData()->setTalkSpecial(
        isTalkFindKoopa, isTalkBossRaid);
}

bool isTalkFindKoopaInWorldWarp(const al::LiveActor* actor) {
    return getGameDataHolderFromActor(actor)
        ->getGameDataFile()
        ->getWorldWarpTalkData()
        ->isTalkFindKoopa();
}

bool isTalkBossRaidInWorldWarp(const al::LiveActor* actor) {
    return getGameDataHolderFromActor(actor)
        ->getGameDataFile()
        ->getWorldWarpTalkData()
        ->isTalkBossRaid();
}

bool isClearMainScenario(const al::LiveActor* actor, s32 worldId) {
    s32 scenarioNo = getGameDataHolderFromActor(actor)->getGameDataFile()->getScenarioNo(worldId);
    WorldList* worldList = getGameDataHolderFromActor(actor)->getWorldList();
    return worldList->isEqualClearMainScenarioNo(worldId, scenarioNo) ||
           worldList->isEqualAfterEndingScenarioNo(worldId, scenarioNo) ||
           worldList->isEqualMoonRockScenarioNo(worldId, scenarioNo);
}

bool checkAlreadyVisitStage(const GameDataHolder* holder, const char* stageName) {
    return holder->getGameDataFile()->getVisitStageData()->checkAlreadyVisit(stageName);
}

void visitStage(const GameDataHolder* holder, const char* stageName) {
    holder->getGameDataFile()->getVisitStageData()->visit(stageName);
}

bool isSequenceGoToNextWorld(const al::IUseSceneObjHolder* user) {
    if (isSequenceCollectShineBeforeGameClear(user))
        return false;
    if (isSequenceCollectShineForRepairHome(user))
        return false;
    if (getGameDataHolderAccessor(user)->getGameDataFile()->tryGetNextMainScenarioLabel(nullptr,
                                                                                        nullptr))
        return false;
    if (!GameDataFunction::isGameClear(user) && !GameDataFunction::isExistJango(user))
        return true;
    return false;
}

bool isSequenceCollectShineBeforeGameClear(const al::IUseSceneObjHolder* user) {
    if (GameDataFunction::isGameClear(user))
        return false;
    s32 worldId = GameDataFunction::getCurrentWorldIdNoDevelop(user);
    if (getGameDataHolderAccessor(user)->getGameDataFile()->tryGetNextMainScenarioLabel(nullptr,
                                                                                        nullptr)) {
        return false;
    }

    if (!getGameDataHolderAccessor(user)->getGameDataFile()->isClearWorldMainScenario(worldId))
        return false;

    bool isCountTotal = false;
    return GameDataFunction::getGotShineNum(user, -1) <
           getGameDataHolderAccessor(user)->findUnlockShineNum(&isCountTotal, worldId);
}

bool isSequenceCollectShineForRepairHome(const al::IUseSceneObjHolder* user) {
    if (!GameDataFunction::isWorldClash(user))
        return false;

    bool isCountTotal = false;
    return GameDataFunction::getGotShineNum(user, -1) <
           getGameDataHolderAccessor(user)->findUnlockShineNum(
               &isCountTotal, GameDataFunction::getWorldIndexClash());
}

bool isSequenceCollectShine(const al::IUseSceneObjHolder* user) {
    if (isSequenceCollectShineBeforeGameClear(user))
        return true;

    s32 worldId = GameDataFunction::getCurrentWorldIdNoDevelop(user);
    if (!getGameDataHolderAccessor(user)->getGameDataFile()->isClearWorldMainScenario(worldId))
        return false;

    if (getGameDataHolderAccessor(user)->getGameDataFile()->getMainScenarioNo(
            GameDataFunction::getWorldIndexSpecial2()) == 1)
        return false;
    if (getGameDataHolderAccessor(user)->getGameDataFile()->tryGetNextMainScenarioLabel(nullptr,
                                                                                        nullptr))
        return false;

    return GameDataFunction::isGameClear(user);
}

bool isSequenceGoToMoonRock(const al::IUseSceneObjHolder* user) {
    if (getGameDataHolderAccessor(user)->getGameDataFile()->tryGetNextMainScenarioLabel(nullptr,
                                                                                        nullptr)) {
        return false;
    }

    return getGameDataHolderAccessor(user)->getGameDataFile()->isEnableOpenMoonRock(
        getGameDataHolderAccessor(user)->getGameDataFile()->getCurrentWorldId());
}

bool isEnableExplainShopCoin(const al::LiveActor* actor) {
    s32 worldId = GameDataFunction::getCurrentWorldIdNoDevelop(actor);
    return getGameDataHolderFromActor(actor)
        ->getGameDataFile()
        ->getShopTalkData()
        ->isEnableExplainShopCoin(worldId);
}

void explainShopCoin(const al::LiveActor* actor) {
    getGameDataHolderFromActor(actor)
        ->getGameDataFile()
        ->getShopTalkData()
        ->resetIsEnableExplainShopCoin(GameDataFunction::getCurrentWorldIdNoDevelop(actor));
}

bool checkShowNewItemAndPrepareSave(const al::LiveActor* actor) {
    return getGameDataHolderFromActor(actor)
        ->getGameDataFile()
        ->getShopTalkData()
        ->checkShowNewItemAndPrepareSave(actor, GameDataFunction::isGameClear(actor),
                                         GameDataFunction::getTotalPayShineNum(actor));
}

bool isInNormalCoinShopFirst(const al::LiveActor* actor) {
    return getGameDataHolderFromActor(actor)
        ->getGameDataFile()
        ->getShopTalkData()
        ->isInNormalCoinShopFirst();
}

void offInNormalCoinShopFirst(const al::LiveActor* actor) {
    getGameDataHolderFromActor(actor)
        ->getGameDataFile()
        ->getShopTalkData()
        ->resetInNormalCoinShopFirst();
}

bool checkExistNewShopItem(const al::LiveActor* actor, s32 unk1, s32 unk2, bool unk3) {
    return getGameDataHolderFromActor(actor)
        ->getGameDataFile()
        ->getShopTalkData()
        ->checkExistNewItem(actor, unk1, unk2, unk3);
}

CapMessageBossData* getCapMessageBossData(const al::LiveActor* actor) {
    return getGameDataHolderAccessor(actor)->getCapMessageBossData();
}

void getYoshiFruit(SaveObjInfo* yoshiFruit) {
    yoshiFruit->on();
}

bool isGetYoshiFruit(const SaveObjInfo* yoshiFruit) {
    return yoshiFruit->isOn();
}

bool isTalkKakku(const al::LiveActor* actor) {
    return getGameDataHolderFromActor(actor)->getGameDataFile()->isTalkKakku();
}

void talkKakku(const al::LiveActor* actor) {
    getGameDataHolderFromActor(actor)->getGameDataFile()->talkKakku();
}

bool isTalkWorldTravelingPeach(const al::LiveActor* actor) {
    return getGameDataHolderFromActor(actor)->getGameDataFile()->isTalkWorldTravelingPeach();
}

void talkWorldTravelingPeach(const al::LiveActor* actor) {
    getGameDataHolderFromActor(actor)->getGameDataFile()->talkWorldTravelingPeach();
}

bool isTalkCollectBgmNpc(const al::LiveActor* actor) {
    return getGameDataHolderFromActor(actor)->getGameDataFile()->isTalkCollectBgmNpc();
}

void talkCollectBgmNpc(const al::LiveActor* actor) {
    getGameDataHolderFromActor(actor)->getGameDataFile()->talkCollectBgmNpc();
}

s32 getTokimekiMayorNpcFavorabilityRating(const al::LiveActor* actor) {
    return getGameDataHolderFromActor(actor)
        ->getGameDataFile()
        ->getTokimekiMayorNpcFavorabilityRating();
}

void setTokimekiMayorNpcFavorabilityRating(const al::LiveActor* actor, s32 rating) {
    getGameDataHolderFromActor(actor)->getGameDataFile()->setTokimekiMayorNpcFavorabilityRating(
        rating);
}

// TODO: rename parameter `out`
bool tryFindCoinStackSave(s32* out, const al::LiveActor* actor, const al::PlacementId* placement) {
    return GameDataFunction::tryFindSaveObjS32Value(out, actor, placement);
}

// TODO: rename parameter `value`
void saveCoinStack(const al::LiveActor* actor, const al::PlacementId* placementId, s32 value) {
    GameDataFunction::saveObjS32(actor, placementId, value);
}

bool isCollectedBgmCityWorldCelemony(const al::LiveActor* actor) {
    return GameDataFunction::isCollectedBgm(actor, CollectBgm::cCeremonyCollectBgmName, nullptr);
}

bool isCollectedBgmCityWorldCelemony2D(const al::LiveActor* actor) {
    return GameDataFunction::isCollectedBgm(actor, CollectBgm::cBonusCollectBgmName02,
                                            CollectBgm::cBonusCollectBgmSituationName02);
}

s32 calcGetShineNumCollectedBgm(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->calcGetShineNumByObjectNameOrOptionalId(
        "CollectBgmNpcShine", GameDataFile::CountType::Value_1);
}

bool checkGetShineCollectedBgmAll(const al::LiveActor* actor) {
    return calcGetShineNumCollectedBgm(actor) ==
           getGameDataHolderFromActor(actor)
               ->getGameDataFile()
               ->calcAllShineNumByObjectNameOrOptionalId("CollectBgmNpcShine");
}

bool checkSavedHintPhoto(const al::LiveActor* actor, const char* name) {
    return getGameDataHolderFromActor(actor)
        ->getGameDataFile()
        ->getHintPhotoData()
        ->checkSavedHintPhoto(name);
}

void saveHintPhoto(const al::LiveActor* actor, const char* name) {
    getGameDataHolderFromActor(actor)->getGameDataFile()->getHintPhotoData()->saveHintPhoto(name);
}

bool isRideSphinx(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->isRideSphinx();
}

void rideSphinx(const al::LiveActor* actor) {
    getGameDataHolderFromActor(actor)->getGameDataFile()->rideSphinx();
}

bool isRideMotorcycle(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->isRideMotorcycle();
}

void rideMotorcycle(const al::LiveActor* actor) {
    getGameDataHolderFromActor(actor)->getGameDataFile()->rideMotorcycle();
}

void setShowInformationPoleGrab(const al::IUseSceneObjHolder* user) {
    getGameDataHolderAccessor(user)->writeTempSaveDataToHash("InformationPoleGrab", true);
}

bool isNeedShowInformationPoleGrab(const al::IUseSceneObjHolder* user) {
    return !getGameDataHolderAccessor(user)->findValueFromTempSaveDataHash("InformationPoleGrab");
}

void setShowInformationPoleClimb(const al::IUseSceneObjHolder* user) {
    getGameDataHolderAccessor(user)->writeTempSaveDataToHash("InformationPoleClimb", true);
}

bool isNeedShowInformationPoleClimb(const al::IUseSceneObjHolder* user) {
    return !getGameDataHolderAccessor(user)->findValueFromTempSaveDataHash("InformationPoleClimb");
}

void setShowInformationPoleClimbTop(const al::IUseSceneObjHolder* user) {
    getGameDataHolderAccessor(user)->writeTempSaveDataToHash("InformationPoleClimbTop", true);
}

bool isNeedShowInformationPoleClimbTop(const al::IUseSceneObjHolder* user) {
    return !getGameDataHolderAccessor(user)->findValueFromTempSaveDataHash(
        "InformationPoleClimbTop");
}

bool isNeedConfirmNetworkFirst(const al::IUseSceneObjHolder* user) {
    return getGameDataHolderAccessor(user)->getGameDataFile()->isFirstNetwork();
}

void saveConfirmNetworkFirst(const al::IUseSceneObjHolder* user) {
    getGameDataHolderAccessor(user)->getGameDataFile()->noFirstNetwork();
}
}  // namespace rs
