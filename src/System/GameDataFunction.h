#pragma once

#include "Library/LiveActor/LiveActor.h"

#include "System/GameDataHolder.h"
#include "System/GameDataHolderAccessor.h"
#include "System/GameDataHolderWriter.h"

struct BindInfo;
class ChangeStageInfo;
class ScenarioStartCamera;
struct ShineInfo;
class Shine;
class RiseMapPartsHolder;
class QuestInfo;
class WorldTravelingNpc;
class HackObjInfo;
class CollectBgm;

namespace nn::g3d {
class ResFile;
}

namespace al {
class CameraTicket;
class GameDataHolderBase;
class LayoutActor;
class PlacementId;
class PlacementInfo;
class Scene;
}  // namespace al

namespace ShopItem {
struct ItemInfo;
}

enum class SessionEventProgress {
    Entry = 0,
    Wait1stMusician,
    Wait2ndMusician,
    Wait3rdMusician,
    Wait4thMusician,
    RequestGoToThePowerPlant,
    WaitThePowerPlantWorks,
    TheCeremonyIsReady,
    GoForTheCeremony,
};

namespace GameDataFunction {
GameDataHolder* getGameDataHolder(const al::IUseSceneObjHolder* holder);
GameDataHolder* getGameDataHolder(const al::SceneObjHolder* holder);
GameDataHolder* getGameDataHolder(al::GameDataHolderBase* holder);
GameDataFile* getGameDataFile(GameDataHolderWriter writer);
void setPlayingFileId(GameDataHolderWriter writer, s32 fileId);
void initializeData(GameDataHolderWriter writer, s32 fileId);
bool isPlayDemoOpening(GameDataHolderAccessor accessor);
bool isGameClear(GameDataHolderAccessor accessor);
void setRequireSave(GameDataHolderWriter writer);
void setRequireSaveFrame(GameDataHolderWriter writer);
u64 getPlayTimeTotal(GameDataHolderAccessor accessor);
u64 getPlayTimeAcrossFile(GameDataHolderAccessor accessor);
s64 getSaveDataIdForPrepo(GameDataHolderAccessor accessor);
void startDemoStage(GameDataHolderWriter writer, const char* stageName);
bool tryChangeNextStage(GameDataHolderWriter writer, const ChangeStageInfo* stageInfo);
bool tryChangeNextStageWithStartRaceFlag(GameDataHolderWriter writer,
                                         const ChangeStageInfo* stageInfo);
bool tryChangeNextStageWithStartRaceYukimaru(GameDataHolderWriter writer,
                                             const ChangeStageInfo* stageInfo);
bool tryChangeNextStageWithDemoWorldWarp(GameDataHolderWriter writer, const char* stageName);
bool tryChangeNextStageWithWorldWarpHole(GameDataHolderWriter writer, const char* stageName);
void changeNextStageWithStartTimeBalloon(GameDataHolderWriter writer, s32 scenarioNo);
const char* getCurrentStageName(GameDataHolderAccessor accessor);
void changeNextStageWithEndTimeBalloon(GameDataHolderWriter writer);
void changeNextStageWithCloset(GameDataHolderWriter writer);
void findAreaAndChangeNextStage(GameDataHolderWriter writer, const al::LiveActor* actor,
                                const sead::Vector3f* actorTrans);
void returnPrevStage(GameDataHolderWriter writer);
bool isTimeBalloonSequence(GameDataHolderAccessor accessor);
const char* getNextStageName(GameDataHolderAccessor accessor);
const char* getCurrentStageName(GameDataHolderAccessor accessor, s32 fileId);
bool isSeaOfTreeStage(GameDataHolderAccessor accessor);
const char* getNextStageName(GameDataHolderAccessor accessor, s32 fileId);
s32 calcNextScenarioNo(GameDataHolderAccessor accessor);
void restartStage(GameDataHolderWriter writer);
void missAndRestartStage(GameDataHolderWriter writer);
bool isMissEndPrevStageForInit(GameDataHolderAccessor accessor);
bool isMissEndPrevStageForSceneDead(GameDataHolderAccessor accessor);
void reenterStage(GameDataHolderWriter writer);
s32 getNextWorldId(GameDataHolderAccessor accessor);
s32 getPrevWorldId(GameDataHolderAccessor accessor);
s32 getWorldNumForNewReleaseShop(GameDataHolderAccessor accessor);
bool isAlreadyGoWorld(GameDataHolderAccessor accessor, s32 worldId);
const char* getWorldDevelopName(GameDataHolderAccessor accessor, s32 worldId);
s32 getWorldIdForNewReleaseShop(GameDataHolderAccessor accessor, s32 worldId);
bool isForwardWorldWarpDemo(GameDataHolderAccessor accessor);
s32 getWorldNum(GameDataHolderAccessor accessor);
bool isFirstTimeNextWorld(GameDataHolderAccessor accessor);
bool checkIsNewWorldInAlreadyGoWorld(GameDataHolderAccessor accessor);
s32 getCurrentWorldIdNoDevelop(GameDataHolderAccessor accessor);
s32 getScenarioNo(const al::LiveActor* actor);
s32 getScenarioNo(const al::LayoutActor* layout);
s32 getScenarioNoPlacement(GameDataHolderAccessor accessor);
bool isEqualScenario(const RiseMapPartsHolder*, s32 scenarioNo);
s32 getMainQuestMin(const al::LiveActor* actor);
s32 getCurrentWorldId(GameDataHolderAccessor accessor);
void clearStartId(GameDataHolderWriter writer);
void setCheckpointId(GameDataHolderWriter writer, const al::PlacementId* checkpointFlag);
void setRestartPointId(GameDataHolderWriter writer, const al::PlacementId* playerRestartInfo);
const char* tryGetRestartPointIdString(GameDataHolderAccessor accessor);
const char* tryGetPlayerStartId(GameDataHolderAccessor accessor);
bool isPlayerStartObj(const al::LiveActor* actor, const al::ActorInitInfo& initInfo);
bool isPlayerStartObj(const al::LiveActor* actor, const al::PlacementInfo& placementInfo);
bool isPlayerStartObj(const al::LiveActor* actor, const char* playerStartId);
bool isPlayerStartLinkedObj(const al::LiveActor* actor, const al::ActorInitInfo& initInfo,
                            const char* linkName);
void setStartShine(const al::LiveActor* actor, const ShineInfo* shineInfo);
s32 getStartShineNextIndex(GameDataHolderAccessor accessor);
bool isAlreadyShowExplainCheckpointFlag(GameDataHolderAccessor accessor);
bool isEnableShowExplainCheckpointFlag(GameDataHolderAccessor accessor);
void showExplainCheckpointFlag(GameDataHolderAccessor accessor);
void getLifeMaxUpItem(const al::LiveActor* actor);
s32 getPlayerHitPoint(GameDataHolderAccessor accessor);
s32 getPlayerHitPointMaxCurrent(GameDataHolderAccessor accessor);
s32 getPlayerHitPointMaxNormal(GameDataHolderAccessor accessor);
bool isPlayerHitPointMax(GameDataHolderAccessor accessor);
bool isPlayerHitPointMaxWithItem(GameDataHolderAccessor accessor);
void recoveryPlayer(const al::LiveActor* actor);
void recoveryPlayerForDebug(const al::LiveActor* actor);
void recoveryPlayerMax(const al::LiveActor* actor);
void recoveryPlayerForSystem(const GameDataHolder* holder);
void recoveryPlayerMaxForSystem(const GameDataHolder* holder);
void initPlayerHitPointForSystem(const GameDataHolder* holder);
void damagePlayer(GameDataHolderWriter writer);
void killPlayer(GameDataHolderWriter writer);
bool isPlayerLifeZero(GameDataHolderAccessor accessor);
bool isGotShine(GameDataHolderAccessor accessor, const ShineInfo* shineInfo);
bool isGotShine(GameDataHolderAccessor accessor, const al::PlacementId* shine);
bool isGotShine(GameDataHolderAccessor accessor, const char* stageName, const char* objId);
bool isGotShine(GameDataHolderAccessor accessor, s32 shineIdx);
bool isGotShine(const Shine* shineActor);
void setGotShine(GameDataHolderWriter writer, const ShineInfo* shineInfo);
s32 getGotShineNum(GameDataHolderAccessor accessor, s32 fileId = -1);
ShineInfo* getLatestGetShineInfo(GameDataHolderAccessor accessor);
s32 getCurrentShineNum(GameDataHolderAccessor accessor);
s32 getTotalShineNum(GameDataHolderAccessor accessor, s32 fileId = -1);
s32 getTotalShopShineNum(GameDataHolderAccessor accessor, s32 fileId = -1);
bool tryGetNextMainScenarioLabel(sead::BufferedSafeStringBase<char>* scenarioName,
                                 sead::BufferedSafeStringBase<char>* stageName,
                                 const al::IUseSceneObjHolder* holder);
s32 getMainScenarioNumMax(GameDataHolderAccessor accessor);
void setMainScenarioNo(GameDataHolderWriter writer, s32 worldId);
bool tryGetNextMainScenarioPos(sead::Vector3f*, GameDataHolderAccessor accessor);
bool isPlayScenarioCamera(GameDataHolderAccessor accessor, const QuestInfo* questInfo);
bool isPlayAlreadyScenarioStartCamera(GameDataHolderAccessor accessor, s32);
bool isEnterStageFirst(GameDataHolderAccessor accessor);
bool isNextMainShine(GameDataHolderAccessor accessor, s32 shineIdx);
bool isMainShine(GameDataHolderAccessor accessor, s32 shineIdx);
bool isLatestGetMainShine(GameDataHolderAccessor accessor, const ShineInfo* shineInfo);
s32 tryFindLinkedShineIndex(const al::LiveActor* actor, const al::ActorInitInfo& initInfo);
s32 tryFindLinkedShineIndex(const al::LiveActor* actor, const al::ActorInitInfo& initInfo,
                            s32 linkIdx);
s32 tryFindLinkedShineIndexByLinkName(const al::LiveActor* actor, const al::ActorInitInfo& initInfo,
                                      const char* linkName);
s32 calcLinkedShineNum(const al::LiveActor* actor, const al::ActorInitInfo& initInfo);
s32 tryFindShineIndex(const al::LiveActor* actor, const al::ActorInitInfo& initInfo);
s32 tryFindShineIndex(const al::LiveActor* actor, const char* stageName, const char* objId);
s32 tryFindShineUniqueId(const al::Scene* scene, const ShineInfo* shineInfo);
void disableHintByShineIndex(const al::LiveActor* actor, s32 shineIdx);
void enableHintByShineIndex(const al::LiveActor* actor, s32 shineIdx);
bool calcIsGetMainShineAll(const al::IUseSceneObjHolder* holder);
bool calcIsGetShineAllInWorld(GameDataHolderAccessor accessor, s32 worldId);
bool calcIsGetShineAllInAllWorld(GameDataHolderAccessor accessor);
bool tryFindAndInitShineInfoByOptionalId(ShineInfo* shineInfo, GameDataHolderAccessor accessor,
                                         const char* optionalId);
bool isGotLinkedShineBeforeInitActor(const al::ActorInitInfo& actorInitInfo, const char* linkName);
bool checkIsComplete(const al::IUseSceneObjHolder* holder, s32 fileId);
bool isExistInHackDictionary(GameDataHolderAccessor accessor, const char* hackName, s32 fileId);
s32 getCollectedBgmNum(GameDataHolderAccessor accessor, s32 fileId);
s32 getCollectedBgmMaxNum(GameDataHolderWriter writer);
bool isEnableOpenMoonRock(const al::LiveActor* actor);
void openMoonRock(const al::LiveActor* actor, const al::PlacementId* moonRock);
bool isOpenMoonRock(GameDataHolderAccessor accessor);
bool isEnableShowDemoOpenMoonRockFirst(GameDataHolderAccessor accessor);
bool isEnableShowDemoOpenMoonRockWorld(GameDataHolderAccessor accessor);
void showDemoOpenMoonRockFirst(GameDataHolderAccessor accessor);
void showDemoOpenMoonRockWorld(GameDataHolderAccessor accessor);
s32 calcMoonRockTalkMessageIndex(GameDataHolderAccessor accessor);
void addMoonRockTalkMessageIndex(GameDataHolderWriter writer);
bool isAppearedMoonRockTalkMessage(GameDataHolderAccessor accessor);
void addPayShine(GameDataHolderWriter writer, s32 count);
void addPayShineCurrentAll(GameDataHolderWriter writer);
s32 getPayShineNum(GameDataHolderAccessor accessor);
s32 getPayShineNum(GameDataHolderAccessor accessor, s32 worldId);
s32 getTotalPayShineNum(GameDataHolderAccessor accessor);
s32 getTotalPayShineNumClamp(GameDataHolderAccessor accessor);
bool isPayShineAllInAllWorld(GameDataHolderAccessor accessor);
void addKey(GameDataHolderWriter writer, s32 count);
s32 getKeyNum(GameDataHolderAccessor accessor);
s32 getCurrentKeyNum(GameDataHolderAccessor accessor);
s32 getOpenDoorLockNum(GameDataHolderAccessor accessor);
void openDoorLock(GameDataHolderWriter writer, const al::PlacementId* doorLock);
bool isOpenDoorLock(GameDataHolderAccessor accessor, const al::PlacementId* doorLock);
void setObjStarted(GameDataHolder* holder, const al::PlacementId* obj);
bool isObjStarted(const GameDataHolder* holder, const al::PlacementId* obj);
bool isObjStarted(GameDataHolderAccessor accessor, const char* stageName, const char* objId);
void saveObjS32(GameDataHolderWriter writer, const al::PlacementId* obj, s32 value);
bool tryFindSaveObjS32Value(s32* outValue, GameDataHolderAccessor accessor,
                            const al::PlacementId* obj);
void onObjNoWriteSaveData(GameDataHolderWriter writer, const al::PlacementId* obj);
void offObjNoWriteSaveData(GameDataHolderWriter writer, const al::PlacementId* obj);
bool isOnObjNoWriteSaveData(GameDataHolderAccessor accessor, const al::PlacementId* obj);
void onObjNoWriteSaveDataResetMiniGame(GameDataHolderWriter writer, const al::PlacementId* obj);
void offObjNoWriteSaveDataResetMiniGame(GameDataHolderWriter writer, const al::PlacementId* obj);
bool isOnObjNoWriteSaveDataResetMiniGame(GameDataHolderAccessor accessor,
                                         const al::PlacementId* obj);
void onObjNoWriteSaveDataInSameScenario(GameDataHolder* holder, const al::PlacementId* obj);
bool isOnObjNoWriteSaveDataInSameScenario(const GameDataHolder* holder, const al::PlacementId* obj);
void setSessionEventProgress(GameDataHolderWriter writer, const SessionEventProgress& progress);
const SessionEventProgress& getSessionEventProgress(GameDataHolderAccessor accessor);
bool isPayCoinToSphinx(const al::LiveActor* actor);
void payCoinToSphinx(const al::LiveActor* actor);
bool isRemovedCapByJango(const al::LiveActor* actor);
bool isMainStage(GameDataHolderAccessor accessor);
bool isGetCapFromJango(const al::LiveActor* actor);
void removeCapByJango(const al::LiveActor* actor);
void getCapFromJango(const al::LiveActor* actor);
void addJangoCount(const al::LiveActor* actor);
s32 getJangoCount(const al::LiveActor* actor);
bool isFirstWorldTravelingStatus(const WorldTravelingNpc* actor);
void saveWorldTravelingStatus(const WorldTravelingNpc* actor, const char* status);
const char* getWorldTravelingStatus(const WorldTravelingNpc* actor);
bool isRaceStart(GameDataHolderAccessor accessor);
bool isRaceStartFlag(GameDataHolderAccessor accessor);
bool isRaceStartYukimaru(GameDataHolderAccessor accessor);
bool isRaceStartYukimaruTutorial(GameDataHolderAccessor accessor);
bool isRaceWin(GameDataHolderAccessor accessor);
bool isRaceWin(GameDataHolderAccessor accessor, s32);
bool isRaceResultSecond(GameDataHolderAccessor accessor);
bool isRaceResultThird(GameDataHolderAccessor accessor);
bool isRaceLose(GameDataHolderAccessor accessor);
bool isRaceCancel(GameDataHolderAccessor accessor);
void winRace(GameDataHolderWriter writer);
void endRaceResultSecond(GameDataHolderWriter writer);
void endRaceResultThird(GameDataHolderWriter writer);
void loseRace(GameDataHolderWriter writer);
s32 getRaceRivalLevel(const al::LiveActor* actor);
s32 getRaceRivalLevel(const al::Scene* scene);
void setRaceRivalLevel(const al::LiveActor* actor, s32 level);
void setLastRaceRanking(GameDataHolderAccessor accessor, s32 ranking);
s32 getLastRaceRanking(GameDataHolderWriter writer);
void incrementRaceLoseCount(const al::Scene* scene, s32 level);
s32 getRaceLoseCount(const al::LiveActor*, s32 level);
void addCoinCollect(GameDataHolderWriter writer, const al::PlacementId* coinCollect);
void useCoinCollect(GameDataHolderWriter writer, s32 count);
bool isGotCoinCollect(GameDataHolderAccessor accessor, const al::ActorInitInfo& coinCollect);
s32 getCoinCollectNum(GameDataHolderAccessor accessor);
s32 getCoinCollectGotNum(GameDataHolderAccessor accessor);
s32 getCoinCollectGotNum(GameDataHolderAccessor accessor, s32 worldId);
s32 getCoinCollectNumMax(GameDataHolderAccessor accessor);
s32 getCoinCollectNumMax(GameDataHolderAccessor accessor, s32 worldId);
bool tryFindExistCoinCollectStagePosExcludeHomeStageInCurrentWorld(sead::Vector3f* pos,
                                                                   const char**,
                                                                   GameDataHolderAccessor accessor);
s32 getWorldScenarioNo(GameDataHolderAccessor accessor, s32 worldId);
void addCoin(GameDataHolderWriter writer, s32 count);
void subCoin(GameDataHolderWriter writer, s32 count);
s32 getCoinNum(GameDataHolderAccessor accessor);
s32 getTotalCoinNum(GameDataHolderAccessor accessor);
bool isAppearCourseName(GameDataHolderAccessor accessor);
void setStageHakoniwa(GameDataHolderWriter writer);
bool isEnableCheckpointWarp(GameDataHolderAccessor accessor);
void validateCheckpointWarp(GameDataHolderWriter writer);
void invalidateCheckpointWarp(GameDataHolderWriter writer);
s32 getCheckpointNumMaxInWorld(GameDataHolderAccessor accessor);
const sead::Vector3f& getCheckpointTransInWorld(GameDataHolderAccessor accessor, s32 checkpointIdx);
const char* getCheckpointObjIdInWorld(GameDataHolderAccessor accessor, s32 checkpointIdx);
const sead::Vector3f& getCheckpointTransInWorld(GameDataHolderAccessor accessor, const char* objId);
bool isGotCheckpointInWorld(GameDataHolderAccessor accessor, s32 checkpointIdx);
bool isGotCheckpoint(GameDataHolderAccessor accessor, al::PlacementId* checkpoint);
s32 calcGotCheckpointNumInWorld(GameDataHolderAccessor accessor);
void changeNextSceneByGotCheckpoint(GameDataHolderWriter writer, s32 checkpointIdx);
void changeNextSceneByHome(GameDataHolderWriter writer);
bool isWarpCheckpoint(GameDataHolderAccessor accessor);
const char* getCheckpointWarpObjId(GameDataHolderAccessor accessor);
void registerCheckpointTrans(GameDataHolderWriter writer, const al::PlacementId* checkpoint,
                             const sead::Vector3f& trans);
bool isEnableUnlockHint(GameDataHolderAccessor accessor);
void unlockHint(GameDataHolderWriter writer);
void unlockHintAmiibo(GameDataHolderWriter writer);
s32 calcHintNum(GameDataHolderAccessor accessor);
s32 calcRestHintNum(GameDataHolderAccessor accessor);
bool checkExistHint(GameDataHolderAccessor accessor);
s32 getHintNumMax(GameDataHolderAccessor accessor);
const sead::Vector3f& calcHintTrans(GameDataHolderAccessor accessor, s32 hintIdx);
const sead::Vector3f& getLatestHintTrans(GameDataHolderAccessor accessor);
bool checkLatestHintSeaOfTree(GameDataHolderAccessor accessor);
s32 calcHintMoonRockNum(GameDataHolderAccessor accessor);
s32 getHintMoonRockNumMax(GameDataHolderAccessor accessor);
const sead::Vector3f& calcHintMoonRockTrans(GameDataHolderAccessor accessor, s32 hintIdx);
void initializeHintList(GameDataHolderWriter writer);
const sead::Vector3f& calcHintTransMostEasy(GameDataHolderAccessor accessor);
bool calcHintTransMostNear(sead::Vector3f* trans, GameDataHolderAccessor accessor,
                           const sead::Vector3f& pos);
bool checkHintSeaOfTree(GameDataHolderAccessor accessor, s32 hintIdx);
bool checkHintSeaOfTreeMoonRock(GameDataHolderAccessor accessor, s32 hintIdx);
s32 findUnlockShineNum(bool* isGameClear, GameDataHolderAccessor accessor);
s32 findUnlockShineNumByWorldId(bool* isGameClear, GameDataHolderAccessor accessor, s32 worldId);
bool isUnlockedWorld(GameDataHolderAccessor accessor, s32 worldId);
bool isUnlockedNextWorld(GameDataHolderAccessor accessor);
bool isUnlockedAllWorld(GameDataHolderAccessor accessor);
s32 getWorldIndexSpecial2();
bool isUnlockedCurrentWorld(GameDataHolderAccessor accessor);
bool isCollectShineForNextWorldAndNotUnlockNextWorld(const al::LiveActor* actor);
const char* getMainStageName(GameDataHolderAccessor accessor, s32 worldId);
const char* tryGetCurrentMainStageName(GameDataHolderAccessor accessor);
const char16* tryGetWorldName(const al::LayoutActor* layout, s32 worldId);
const char16* tryGetWorldNameCurrent(const al::LayoutActor* layout);
const char16* tryGetRegionNameCurrent(const al::LayoutActor* layout);
const char* getWorldDevelopNameCurrent(GameDataHolderAccessor accessor);
s32 getWorldScenarioNum(GameDataHolderAccessor accessor, s32 worldId);
const char* findMainStageNameByDevelopName(GameDataHolderAccessor accessor,
                                           const char* developName);
s32 findWorldIdByDevelopName(GameDataHolderAccessor accessor, const char* developName);
s32 tryFindWorldIdByMainStageName(const al::Scene* scene, const char* mainStageName);
s32 tryFindWorldIdByMainStageName(const al::IUseSceneObjHolder* holder, const char* mainStageName);
bool checkEnableUnlockWorldSpecial1(const al::LiveActor* actor);
s32 getWorldIndexSpecial1();
bool checkEnableUnlockWorldSpecial2(const al::LiveActor* actor);
const char16* tryGetWorldNameByFileId(const al::LayoutActor* layout, s32 fileId);
bool isNewSaveDataByFileId(const al::LayoutActor*, s32 fileId);
u64 getLastUpdateFileTime(const al::LayoutActor*, s32 fileId);
void makeTextureSaveDataFileName(sead::BufferedSafeStringBase<char>* fileName,
                                 const nn::g3d::ResFile* textureSaveData,
                                 const GameDataHolder* holder, s32 fileId);
s32 getWorldIndexPeach();
s32 getWorldIndexCity();
bool isCityWorldCeremonyAll(s32 worldId, s32 scenarioNo);
s32 getWorldIndexSea();
void unlockWorld(GameDataHolderWriter writer, s32 worldId);
s32 getUnlockWorldIdForWorldMap(const al::LayoutActor* layout, s32 worldId);
s32 getUnlockWorldIdForWorldMap(const al::LiveActor* actor, s32 worldId);
s32 getUnlockWorldIdForWorldMap(const al::Scene* scene, s32 worldId);
s32 getUnlockWorldIdForWorldMap(const GameDataHolder* holder, s32 worldId);
s32 getUnlockWorldNumForWorldMap(const al::Scene* scene);
s32 getUnlockWorldNumForWorldMap(const al::LiveActor* actor);
s32 getUnlockWorldNumForWorldMap(const al::LayoutActor* layout);
s32 calcNextLockedWorldIdForWorldMap(const al::LayoutActor* layout, s32 worldId);
s32 calcNextLockedWorldIdForWorldMap(const al::Scene* scene, s32 worldId);
s32 calcNextLockedWorldNumForWorldMap(const al::Scene* scene);
s32 calcNextLockedWorldNumForWorldMap(const al::LayoutActor* layout);
s32 getWorldIdForShineList(const al::LayoutActor* layout, s32 worldId);
s32 calcWorldNumForShineList(const al::LayoutActor* layout);
s32 getLatestUnlockWorldIdForShineTowerMeter(const al::LiveActor* actor);
bool isClearSandWorldScenario1(const al::Scene* scene);
bool isClearCityWorldScenario1(const al::Scene* scene);
bool isClearSkyWorldScenario1(const al::Scene* scene);
bool isCityWorldScenario2(const al::IUseSceneObjHolder* holder);
bool isWorldCity(GameDataHolderAccessor accessor);
s32 calcNextWorldId(GameDataHolderAccessor accessor);
bool isPlayDemoWorldWarp(GameDataHolderAccessor accessor);
bool isPlayDemoWorldWarpHole(GameDataHolderAccessor accessor);
void noPlayDemoWorldWarp(GameDataHolderWriter writer);
bool isPlayDemoReturnToHome(GameDataHolderAccessor accessor);
void requestPlayDemoReturnToHome(GameDataHolderWriter writer);
bool isPlayDemoAwardSpecial(GameDataHolderAccessor accessor);
void requestPlayDemoAwardSpecial(GameDataHolderWriter writer);
bool isPlayDemoLavaErupt(GameDataHolderAccessor accessor);
bool isPlayDemoPlayerDownForBattleKoopaAfter(GameDataHolderAccessor accessor);
void disablePlayDemoPlayerDownForBattleKoopaAfter(GameDataHolderWriter writer);
void enteredStage(GameDataHolderWriter writer);
const char* getCurrentCostumeTypeName(GameDataHolderAccessor accessor);
const char* getCurrentCapTypeName(GameDataHolderAccessor accessor);
void wearCostume(GameDataHolderWriter writer, const char* costumeName);
void wearCap(GameDataHolderWriter writer, const char* capName);
void wearCostumeRandom(al::IUseSceneObjHolder* holder);
void wearCapRandom(al::IUseSceneObjHolder* holder);
void tryWearCostumeRandomIfEnable(al::IUseSceneObjHolder* holder);
bool isCostumeRandomMode(al::IUseSceneObjHolder* holder);
void tryWearCapRandomIfEnable(al::IUseSceneObjHolder* holder);
bool isCapRandomMode(al::IUseSceneObjHolder* holder);
void setCostumeRandomMode(al::IUseSceneObjHolder* holder);
void setCapRandomMode(al::IUseSceneObjHolder* holder);
void resetCostumeRandomMode(al::IUseSceneObjHolder* holder);
void resetCapRandomMode(al::IUseSceneObjHolder* holder);
const sead::PtrArray<ShopItem::ItemInfo>& getShopItemInfoList(GameDataHolderAccessor accessor);
bool isExistHackObjInfo(GameDataHolderAccessor accessor, const char* hackName);
const HackObjInfo& getHackObjInfo(GameDataHolderAccessor accessor, const char* hackName);
void addHackDictionary(GameDataHolderWriter writer, const char* hackName);
bool isExistInHackDictionary(GameDataHolderAccessor accessor, const char* hackName);
bool isShowHackTutorial(GameDataHolderAccessor accessor, const char* hackName, const char* suffix);
void setShowHackTutorial(GameDataHolderWriter writer, const char* hackName, const char* suffix);
bool isShowBindTutorial(GameDataHolderAccessor accessor, const BindInfo& bindInfo);
void setShowBindTutorial(GameDataHolderWriter writer, const BindInfo& bindInfo);
bool isGotShine(GameDataHolderAccessor accessor, s32 worldId, s32 shineIdx);
bool isOpenShineName(const al::LayoutActor* layout, s32 worldId, s32 shineIdx);
bool checkAchievementShine(const al::LayoutActor* layout, s32 worldId, s32 shineIdx);
s32 calcShineNumInOneShine(const al::LayoutActor* layout, s32 worldId, s32 shineIdx);
const char16* tryFindShineMessage(const al::LayoutActor* layout, s32 worldId, s32 shineIdx);
const char16* tryFindShineMessage(const al::LiveActor* actor,
                                  const al::IUseMessageSystem* messageSystem, s32 worldId,
                                  s32 shineIdx);
u64 findShineGetTime(const al::LayoutActor* layout, s32 worldId, s32 shineIdx);
bool checkMoonRockShineForShineList(const al::LayoutActor* layout, s32 worldId, s32 shineIdx);
bool checkUnlockHintByHintNpcForShineList(const al::LayoutActor* layout, s32 worldId, s32 shineIdx);
bool checkUnlockHintByAmiiboForShineList(const al::LayoutActor* layout, s32 worldId, s32 shineIdx);
bool checkShineSeaOfTreeForShineList(const al::LayoutActor* layout, s32 worldId, s32 shineIdx);
s32 getWorldTotalShineNum(GameDataHolderAccessor accessor, s32 worldId);
s32 getWorldTotalShineNumMax(GameDataHolderAccessor accessor, s32 worldId);
void findShineTrans(sead::Vector3f* trans, GameDataHolderAccessor accessor, s32 worldId,
                    s32 shineIdx);
const sead::Vector3f& getHomeTrans(GameDataHolderAccessor accessor);
bool isExistHome(GameDataHolderAccessor accessor);
bool isActivateHome(GameDataHolderAccessor accessor);
s32 getMiniGameNum(GameDataHolderAccessor accessor);
s32 getMiniGameNumMax(GameDataHolderAccessor accessor);
const sead::Vector3f& getMiniGameTrans(GameDataHolderAccessor accessor, s32 miniGameIdx);
const char* getMiniGameName(GameDataHolderAccessor accessor, s32 miniGameIdx);
const sead::Vector3f& getRaceStartTrans(GameDataHolderAccessor accessor);
const sead::Vector3f& getRaceGoalTrans(GameDataHolderAccessor accessor);
const sead::Vector3f& getHintNpcTrans(GameDataHolderAccessor accessor);
const sead::Vector3f& getShopNpcTrans(GameDataHolderAccessor accessor, s32 shopNpcIdx);
bool isShopSellout(GameDataHolderAccessor accessor, s32 shopNpcIdx);
s32 calcShopNum(GameDataHolderAccessor accessor);
s32 getShopNpcIconNumMax(GameDataHolderAccessor accessor);
bool isExistRaceStartNpc(GameDataHolderAccessor accessor);
bool isExistHintNpc(GameDataHolderAccessor accessor);
const sead::Matrix34f& getCurrentMapView(GameDataHolderAccessor accessor);
bool isExistJango(GameDataHolderAccessor accessor);
const sead::Vector3f& getJangoTrans(GameDataHolderAccessor accessor);
bool isExistAmiiboNpc(GameDataHolderAccessor accessor);
const sead::Vector3f& getAmiiboNpcTrans(GameDataHolderAccessor accessor);
bool isInInvalidOpenMapStage(GameDataHolderAccessor accessor);
const sead::Vector3f& getStageMapPlayerPos(GameDataHolderAccessor accessor);
bool isExistTimeBalloonNpc(GameDataHolderAccessor accessor);
const sead::Vector3f& getTimeBalloonNpcTrans(GameDataHolderAccessor accessor);
bool isExistPoetter(GameDataHolderAccessor accessor);
const sead::Vector3f& getPoetterTrans(GameDataHolderAccessor accessor);
const sead::Vector3f& getMoonRockTrans(GameDataHolderAccessor accessor);
void setHomeTrans(const al::LiveActor* actor, const sead::Vector3f& trans);
void setRaceStartNpcTrans(const al::LiveActor* actor);
void setRaceStartTrans(const al::LiveActor* actor);
void setRaceGoalTrans(const al::LiveActor* actor);
void setHintNpcTrans(const al::LiveActor* actor);
void setJangoTrans(const al::LiveActor* actor);
void disableJangoTrans(const al::LiveActor* actor);
void setAmiiboNpcTrans(const al::LiveActor* actor);
void setShopNpcTrans(const al::LiveActor* actor, const char* storeName, s32 shopNpcIdx);
void setShopNpcTrans(GameDataHolderAccessor accessor, const al::PlacementInfo& placementInfo);
void setMiniGameInfo(GameDataHolderAccessor accessor, const al::PlacementInfo& placementInfo);
void setTimeBalloonTrans(GameDataHolderAccessor accessor, const sead::Vector3f& trans);
void setPoetterTrans(GameDataHolderAccessor accessor, const sead::Vector3f& trans);
void setStageMapPlayerPos(GameDataHolderWriter writer, const sead::Vector3f& trans);
void setMoonRockTrans(GameDataHolderWriter writer, const sead::Vector3f& trans);
s32 getHomeLevel(GameDataHolderAccessor accessor);
void upHomeLevel(GameDataHolderWriter writer);
void activateHome(GameDataHolderWriter writer);
bool isLaunchHome(GameDataHolderAccessor accessor);
void launchHome(GameDataHolderWriter writer);
bool isCrashHome(GameDataHolderAccessor accessor);
void crashHome(GameDataHolderWriter writer);
bool isRepairHome(GameDataHolderAccessor accessor);
void repairHome(GameDataHolderWriter writer);
bool isBossAttackedHome(GameDataHolderAccessor accessor);
void bossAttackHome(GameDataHolderWriter writer);
bool isRepairHomeByCrashedBoss(GameDataHolderAccessor accessor);
void repairHomeByCrashedBoss(GameDataHolderWriter writer);
bool isFindKoopaNext(GameDataHolderAccessor accessor, s32 worldId);
bool isBossAttackedHomeNext(GameDataHolderAccessor accessor, s32 worldId);
bool isFindKoopa(GameDataHolderAccessor accessor);
void findKoopa(GameDataHolderWriter writer);
bool isEnableCap(GameDataHolderAccessor accessor);
bool isMeetCap(GameDataHolderAccessor accessor);
void enableCap(GameDataHolderWriter writer);
void disableCapByPlacement(const al::LiveActor* actor);
bool isTalkedCapNearHomeInWaterfall(const al::LiveActor* actor);
void talkCapNearHomeInWaterfall(const al::LiveActor* actor);
bool isFlagOnTalkMessageInfo(const al::IUseSceneObjHolder* holder, s32 talkMessageIdx);
void onFlagTalkMessageInfo(const al::IUseSceneObjHolder* holder, s32 talkMessageIdx);
s32 getWorldIndexHat();
s32 getWorldIndexWaterfall();
s32 getWorldIndexSand();
s32 getWorldIndexForest();
s32 getWorldIndexLake();
s32 getWorldIndexCloud();
s32 getWorldIndexClash();
s32 getWorldIndexSnow();
s32 getWorldIndexLava();
s32 getWorldIndexBoss();
s32 getWorldIndexSky();
s32 getWorldIndexMoon();
bool isWorldCap(GameDataHolderAccessor accessor);
bool isWorldWaterfall(GameDataHolderAccessor accessor);
bool isWorldSand(GameDataHolderAccessor accessor);
bool isWorldForest(GameDataHolderAccessor accessor);
bool isWorldLake(GameDataHolderAccessor accessor);
bool isWorldCloud(GameDataHolderAccessor accessor);
bool isWorldClash(GameDataHolderAccessor accessor);
bool isWorldSnow(GameDataHolderAccessor accessor);
bool isWorldSea(GameDataHolderAccessor accessor);
bool isWorldBoss(GameDataHolderAccessor accessor);
bool isWorldSky(GameDataHolderAccessor accessor);
bool isWorldMoon(GameDataHolderAccessor accessor);
bool isWorldPeach(GameDataHolderAccessor accessor);
bool isWorldSpecial1(GameDataHolderAccessor accessor);
bool isWorldSpecial2(GameDataHolderAccessor accessor);
bool isWorldTypeMoon(GameDataHolderAccessor accessor, s32 worldId);
bool isSnowMainScenario1(const al::IUseSceneObjHolder* holder);
bool isSnowMainScenario2(const al::IUseSceneObjHolder* holder);
bool isHomeShipStage(const GameDataHolder* holder);
const char* getHomeShipStageName();
bool isCityWorldCeremonyAgain(s32 worldId, s32 scenarioNo);
bool isGoToCeremonyFromInsideHomeShip(GameDataHolderAccessor accessor);
void registerScenarioStartCameraInfo(const ScenarioStartCamera* cameraInfo, s32,
                                     al::CameraTicket* ticket);
bool isEnableExplainAmiibo(GameDataHolderAccessor accessor);
void endExplainAmiibo(GameDataHolderWriter writer);
void startSearchHintByAmiibo(GameDataHolderWriter writer);
void endSearchHintByAmiibo(GameDataHolderWriter writer);
void setKidsModeOn(const al::Scene* scene);
void setKidsModeOff(const al::Scene* scene);
bool isCollectedBgm(GameDataHolderAccessor accessor, const char* resourceName,
                    const char* situationName);
const CollectBgm& getCollectBgmByIndex(GameDataHolderAccessor accessor, s32 bgmIdx);
bool trySetCollectedBgm(GameDataHolderWriter writer, const char* resourceName,
                        const char* situationName);
s32 getCollectedBgmNum(GameDataHolderWriter writer);
void registerShineInfo(GameDataHolderAccessor accessor, const ShineInfo* shineInfo,
                       const sead::Vector3f& trans);
void setHintTrans(GameDataHolderAccessor accessor, s32 shineIdx, const sead::Vector3f& trans);
void resetHintTrans(GameDataHolderAccessor accessor, s32 shineIdx);
bool isKoopaLv3(GameDataHolderAccessor accessor);
}  // namespace GameDataFunction
