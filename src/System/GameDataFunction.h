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
class SessionEventProgress;
class RiseMapPartsHolder;
class QuestInfo;
class WorldTravelingNpc;

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

class GameDataFunction {
public:
    // gets prev save file's current world id
    static s32 getPrevWorldId(GameDataHolderAccessor);
    // gets current save file's current world id
    static s32 getCurrentWorldId(GameDataHolderAccessor);
    // gets next save file's current world id
    static s32 getNextWorldId(GameDataHolderAccessor);

    static s32 getCurrentShineNum(GameDataHolderAccessor);

    static s32 calcNextScenarioNo(GameDataHolderAccessor);
    // gets total moons collected on a specified save file (-1 for current save)
    static s32 getTotalShineNum(GameDataHolderAccessor, s32);
    // gets the total amount of moons available in a kingdom
    static s32 getWorldTotalShineNum(GameDataHolderAccessor, s32);
    // gets the current scenario No of the specified kingdom
    static s32 getWorldScenarioNo(GameDataHolderAccessor, s32);

    static char* getCurrentStageName(GameDataHolderAccessor);

    static char* getMainStageName(GameDataHolderAccessor, s32);

    static char* getNextStageName(GameDataHolderAccessor);

    // checks save file if shine is collected in kingdom index
    static bool isGotShine(GameDataHolderAccessor, s32, s32);

    // checks save file if shine is collected by shine index only (0 through 725)
    static bool isGotShine(GameDataHolderAccessor, s32);

    // checks if current stage is a main stage
    static bool isMainStage(GameDataHolderAccessor);

    // Gets Index for X Kingdom
    static s32 getWorldIndexWaterfall();
    static s32 getWorldIndexMoon();

    // gets the current level of the Odyssey
    static s32 getHomeLevel(GameDataHolderAccessor);

    // enables cappy if not enabled already
    static void enableCap(GameDataHolderWriter);

    // kills the player
    static void killPlayer(GameDataHolderWriter);

    // upgrades the odyssey
    static void upHomeLevel(GameDataHolderWriter);

    // unlocks a kingdom based off index
    static void unlockWorld(GameDataHolderWriter, s32);
    // sets the scenario of the specified kingdom
    static void setMainScenarioNo(GameDataHolderWriter, s32 scenarioNo);

    // checks if odyssey is/needs a repair
    static bool isRepairHome(GameDataHolderAccessor);
    static void repairHome(GameDataHolderWriter);

    // checks if odyssey is crashed
    static bool isCrashHome(GameDataHolderAccessor);
    static void crashHome(GameDataHolderWriter);

    // checks if odyssey is activated
    static bool isActivateHome(GameDataHolderAccessor);
    static void activateHome(GameDataHolderWriter);

    // checks if the odyssey has launched for the first time.
    static bool isLaunchHome(GameDataHolderAccessor);
    static void launchHome(GameDataHolderWriter);

    static bool isHomeShipStage(const GameDataHolder*);

    // used during the event that enables the odyssey to be used (enables the globe for the odyssey)
    static void talkCapNearHomeInWaterfall(const al::LiveActor*);

    static void getGameDataHolder(const al::IUseSceneObjHolder*);
    static void getGameDataHolder(const al::SceneObjHolder*);
    static void getGameDataHolder(al::GameDataHolderBase*);
    static void getGameDataFile(GameDataHolderWriter);
    static void setPlayingFileId(GameDataHolderWriter, s32);
    static void initializeData(GameDataHolderWriter, s32);
    static bool isPlayDemoOpening(GameDataHolderAccessor);
    static bool isGameClear(GameDataHolderAccessor);
    static void setRequireSave(GameDataHolderWriter);
    static void setRequireSaveFrame(GameDataHolderWriter);
    static void getPlayTimeTotal(GameDataHolderAccessor);
    static void getPlayTimeAcrossFile(GameDataHolderAccessor);
    static void getSaveDataIdForPrepo(GameDataHolderAccessor);
    static void startDemoStage(GameDataHolderWriter, const char*);
    static void tryChangeNextStage(GameDataHolderWriter, const ChangeStageInfo*);
    static void tryChangeNextStageWithStartRaceFlag(GameDataHolderWriter, const ChangeStageInfo*);
    static void tryChangeNextStageWithStartRaceYukimaru(GameDataHolderWriter,
                                                        const ChangeStageInfo*);
    static void tryChangeNextStageWithDemoWorldWarp(GameDataHolderWriter, const char*);
    static void tryChangeNextStageWithWorldWarpHole(GameDataHolderWriter, const char*);
    static void changeNextStageWithStartTimeBalloon(GameDataHolderWriter, s32);
    static void changeNextStageWithEndTimeBalloon(GameDataHolderWriter);
    static void changeNextStageWithCloset(GameDataHolderWriter);
    static void findAreaAndChangeNextStage(GameDataHolderWriter, const al::LiveActor*,
                                           const sead::Vector3<f32>*);
    static void returnPrevStage(GameDataHolderWriter);
    static bool isTimeBalloonSequence(GameDataHolderAccessor);
    static void getCurrentStageName(GameDataHolderAccessor, s32);
    static bool isSeaOfTreeStage(GameDataHolderAccessor);
    static void getNextStageName(GameDataHolderAccessor, s32);
    static void restartStage(GameDataHolderWriter);
    static void missAndRestartStage(GameDataHolderWriter);
    static bool isMissEndPrevStageForInit(GameDataHolderAccessor);
    static bool isMissEndPrevStageForSceneDead(GameDataHolderAccessor);
    static void reenterStage(GameDataHolderWriter);
    static void getWorldNumForNewReleaseShop(GameDataHolderAccessor);
    static bool isAlreadyGoWorld(GameDataHolderAccessor, s32);
    static void getWorldDevelopName(GameDataHolderAccessor, s32);
    static void getWorldIdForNewReleaseShop(GameDataHolderAccessor, s32);
    static bool isForwardWorldWarpDemo(GameDataHolderAccessor);
    static void getWorldNum(GameDataHolderAccessor);
    static bool isFirstTimeNextWorld(GameDataHolderAccessor);
    static void checkIsNewWorldInAlreadyGoWorld(GameDataHolderAccessor);
    static void getCurrentWorldIdNoDevelop(GameDataHolderAccessor);
    static s32 getScenarioNo(const al::LiveActor*);
    static void getScenarioNo(const al::LayoutActor*);
    static void getScenarioNoPlacement(GameDataHolderAccessor);
    static bool isEqualScenario(const RiseMapPartsHolder*, s32);
    static void getMainQuestMin(const al::LiveActor*);
    static void clearStartId(GameDataHolderWriter);
    static void setCheckpointId(GameDataHolderWriter, const al::PlacementId*);
    static void setRestartPointId(GameDataHolderWriter, const al::PlacementId*);
    static void tryGetRestartPointIdString(GameDataHolderAccessor);
    static void tryGetPlayerStartId(GameDataHolderAccessor);
    static bool isPlayerStartObj(const al::LiveActor*, const al::ActorInitInfo&);
    static bool isPlayerStartObj(const al::LiveActor*, const al::PlacementInfo&);
    static bool isPlayerStartObj(const al::LiveActor*, const char*);
    static bool isPlayerStartLinkedObj(const al::LiveActor*, const al::ActorInitInfo&, const char*);
    static void setStartShine(const al::LiveActor*, const ShineInfo*);
    static void getStartShineNextIndex(GameDataHolderAccessor);
    static bool isAlreadyShowExplainCheckpointFlag(GameDataHolderAccessor);
    static bool isEnableShowExplainCheckpointFlag(GameDataHolderAccessor);
    static void showExplainCheckpointFlag(GameDataHolderAccessor);
    static void getLifeMaxUpItem(const al::LiveActor*);
    static s32 getPlayerHitPoint(GameDataHolderAccessor);
    static s32 getPlayerHitPointMaxCurrent(GameDataHolderAccessor);
    static s32 getPlayerHitPointMaxNormal(GameDataHolderAccessor);
    static bool isPlayerHitPointMax(GameDataHolderAccessor);
    static bool isPlayerHitPointMaxWithItem(GameDataHolderAccessor);
    static void recoveryPlayer(const al::LiveActor*);
    static void recoveryPlayerForDebug(const al::LiveActor*);
    static void recoveryPlayerMax(const al::LiveActor*);
    static void recoveryPlayerForSystem(const GameDataHolder*);
    static void recoveryPlayerMaxForSystem(const GameDataHolder*);
    static void initPlayerHitPointForSystem(const GameDataHolder*);
    static void damagePlayer(GameDataHolderWriter);
    static bool isPlayerLifeZero(GameDataHolderAccessor);
    static bool isGotShine(GameDataHolderAccessor, const ShineInfo*);
    static bool isGotShine(GameDataHolderAccessor, const al::PlacementId*);
    static bool isGotShine(GameDataHolderAccessor, const char*, const char*);
    static bool isGotShine(const Shine*);
    static void setGotShine(GameDataHolderWriter, const ShineInfo*);
    static void getGotShineNum(GameDataHolderAccessor, s32);
    static void getLatestGetShineInfo(GameDataHolderAccessor);
    static void getTotalShopShineNum(GameDataHolderAccessor, s32);
    static void tryGetNextMainScenarioLabel(sead::BufferedSafeStringBase<char>*,
                                            sead::BufferedSafeStringBase<char>*,
                                            const al::IUseSceneObjHolder*);
    static void getMainScenarioNumMax(GameDataHolderAccessor);
    static void tryGetNextMainScenarioPos(sead::Vector3<f32>*, GameDataHolderAccessor);
    static bool isPlayScenarioCamera(GameDataHolderAccessor, const QuestInfo*);
    static bool isPlayAlreadyScenarioStartCamera(GameDataHolderAccessor, s32);
    static bool isEnterStageFirst(GameDataHolderAccessor);
    static bool isNextMainShine(GameDataHolderAccessor, s32);
    static bool isMainShine(GameDataHolderAccessor, s32);
    static bool isLatestGetMainShine(GameDataHolderAccessor, const ShineInfo*);
    static void tryFindLinkedShineIndex(const al::LiveActor*, const al::ActorInitInfo&);
    static void tryFindLinkedShineIndex(const al::LiveActor*, const al::ActorInitInfo&, s32);
    static void tryFindLinkedShineIndexByLinkName(const al::LiveActor*, const al::ActorInitInfo&,
                                                  const char*);
    static void calcLinkedShineNum(const al::LiveActor*, const al::ActorInitInfo&);
    static void tryFindShineIndex(const al::LiveActor*, const al::ActorInitInfo&);
    static void tryFindShineIndex(const al::LiveActor*, const char*, const char*);
    static void tryFindShineUniqueId(const al::Scene*, const ShineInfo*);
    static void disableHintByShineIndex(const al::LiveActor*, s32);
    static void enableHintByShineIndex(const al::LiveActor*, s32);
    static void calcIsGetMainShineAll(const al::IUseSceneObjHolder*);
    static void calcIsGetShineAllInWorld(GameDataHolderAccessor, s32);
    static void calcIsGetShineAllInAllWorld(GameDataHolderAccessor);
    static void tryFindAndInitShineInfoByOptionalId(ShineInfo*, GameDataHolderAccessor,
                                                    const char*);
    static bool isGotLinkedShineBeforeInitActor(const al::ActorInitInfo&, const char*);
    static void checkIsComplete(const al::IUseSceneObjHolder*, s32);
    static bool isExistInHackDictionary(GameDataHolderAccessor, const char*, s32);
    static void getCollectedBgmNum(GameDataHolderAccessor, s32);
    static void getCollectedBgmMaxNum(GameDataHolderWriter);
    static bool isEnableOpenMoonRock(const al::LiveActor*);
    static void openMoonRock(const al::LiveActor*, const al::PlacementId*);
    static bool isOpenMoonRock(GameDataHolderAccessor);
    static bool isEnableShowDemoOpenMoonRockFirst(GameDataHolderAccessor);
    static bool isEnableShowDemoOpenMoonRockWorld(GameDataHolderAccessor);
    static void showDemoOpenMoonRockFirst(GameDataHolderAccessor);
    static void showDemoOpenMoonRockWorld(GameDataHolderAccessor);
    static void calcMoonRockTalkMessageIndex(GameDataHolderAccessor);
    static void addMoonRockTalkMessageIndex(GameDataHolderWriter);
    static bool isAppearedMoonRockTalkMessage(GameDataHolderAccessor);
    static void addPayShine(GameDataHolderWriter, s32);
    static void addPayShineCurrentAll(GameDataHolderWriter);
    static void getPayShineNum(GameDataHolderAccessor);
    static void getPayShineNum(GameDataHolderAccessor, s32);
    static void getTotalPayShineNum(GameDataHolderAccessor);
    static void getTotalPayShineNumClamp(GameDataHolderAccessor);
    static bool isPayShineAllInAllWorld(GameDataHolderAccessor);
    static void addKey(GameDataHolderWriter, s32);
    static void getKeyNum(GameDataHolderAccessor);
    static void getCurrentKeyNum(GameDataHolderAccessor);
    static void getOpenDoorLockNum(GameDataHolderAccessor);
    static void openDoorLock(GameDataHolderWriter, const al::PlacementId*);
    static bool isOpenDoorLock(GameDataHolderAccessor, const al::PlacementId*);
    static void setObjStarted(GameDataHolder*, const al::PlacementId*);
    static bool isObjStarted(const GameDataHolder*, const al::PlacementId*);
    static bool isObjStarted(GameDataHolderAccessor, const char*, const char*);
    static void saveObjS32(GameDataHolderWriter, const al::PlacementId*, s32);
    static void tryFindSaveObjS32Value(s32*, GameDataHolderAccessor, const al::PlacementId*);
    static void onObjNoWriteSaveData(GameDataHolderWriter, const al::PlacementId*);
    static void offObjNoWriteSaveData(GameDataHolderWriter, const al::PlacementId*);
    static bool isOnObjNoWriteSaveData(GameDataHolderAccessor, const al::PlacementId*);
    static void onObjNoWriteSaveDataResetMiniGame(GameDataHolderWriter, const al::PlacementId*);
    static void offObjNoWriteSaveDataResetMiniGame(GameDataHolderWriter, const al::PlacementId*);
    static bool isOnObjNoWriteSaveDataResetMiniGame(GameDataHolderAccessor, const al::PlacementId*);
    static void onObjNoWriteSaveDataInSameScenario(GameDataHolder*, const al::PlacementId*);
    static bool isOnObjNoWriteSaveDataInSameScenario(const GameDataHolder*, const al::PlacementId*);
    static void setSessionEventProgress(GameDataHolderWriter, const SessionEventProgress&);
    static void getSessionEventProgress(GameDataHolderAccessor);
    static bool isPayCoinToSphinx(const al::LiveActor*);
    static void payCoinToSphinx(const al::LiveActor*);
    static bool isRemovedCapByJango(const al::LiveActor*);
    static bool isGetCapFromJango(const al::LiveActor*);
    static void removeCapByJango(const al::LiveActor*);
    static void getCapFromJango(const al::LiveActor*);
    static void addJangoCount(const al::LiveActor*);
    static void getJangoCount(const al::LiveActor*);
    static bool isFirstWorldTravelingStatus(const WorldTravelingNpc*);
    static void saveWorldTravelingStatus(const WorldTravelingNpc*, const char*);
    static void getWorldTravelingStatus(const WorldTravelingNpc*);
    static bool isRaceStart(GameDataHolderAccessor);
    static bool isRaceStartFlag(GameDataHolderAccessor);
    static bool isRaceStartYukimaru(GameDataHolderAccessor);
    static bool isRaceStartYukimaruTutorial(GameDataHolderAccessor);
    static bool isRaceWin(GameDataHolderAccessor);
    static bool isRaceWin(GameDataHolderAccessor, s32);
    static bool isRaceResultSecond(GameDataHolderAccessor);
    static bool isRaceResultThird(GameDataHolderAccessor);
    static bool isRaceLose(GameDataHolderAccessor);
    static bool isRaceCancel(GameDataHolderAccessor);
    static void winRace(GameDataHolderWriter);
    static void endRaceResultSecond(GameDataHolderWriter);
    static void endRaceResultThird(GameDataHolderWriter);
    static void loseRace(GameDataHolderWriter);
    static void getRaceRivalLevel(const al::LiveActor*);
    static void getRaceRivalLevel(const al::Scene*);
    static void setRaceRivalLevel(const al::LiveActor*, s32);
    static void setLastRaceRanking(GameDataHolderAccessor, s32);
    static void getLastRaceRanking(GameDataHolderWriter);
    static void incrementRaceLoseCount(const al::Scene*, s32);
    static void getRaceLoseCount(const al::LiveActor*, s32);
    static void addCoinCollect(GameDataHolderWriter, const al::PlacementId*);
    static void useCoinCollect(GameDataHolderWriter, s32);
    static bool isGotCoinCollect(GameDataHolderAccessor, const al::ActorInitInfo&);
    static void getCoinCollectNum(GameDataHolderAccessor);
    static void getCoinCollectGotNum(GameDataHolderAccessor);
    static void getCoinCollectGotNum(GameDataHolderAccessor, s32);
    static void getCoinCollectNumMax(GameDataHolderAccessor);
    static void getCoinCollectNumMax(GameDataHolderAccessor, s32);
    static void
    tryFindExistCoinCollectStagePosExcludeHomeStageInCurrentWorld(sead::Vector3<f32>*, const char**,
                                                                  GameDataHolderAccessor);
    static void addCoin(GameDataHolderWriter, s32);
    static void subCoin(GameDataHolderWriter, s32);
    static void getCoinNum(GameDataHolderAccessor);
    static void getTotalCoinNum(GameDataHolderAccessor);
    static bool isAppearCourseName(GameDataHolderAccessor);
    static void setStageHakoniwa(GameDataHolderWriter);
    static bool isEnableCheckpointWarp(GameDataHolderAccessor);
    static void validateCheckpointWarp(GameDataHolderWriter);
    static void invalidateCheckpointWarp(GameDataHolderWriter);
    static void getCheckpointNumMaxInWorld(GameDataHolderAccessor);
    static void getCheckpointTransInWorld(GameDataHolderAccessor, s32);
    static void getCheckpointObjIdInWorld(GameDataHolderAccessor, s32);
    static void getCheckpointTransInWorld(GameDataHolderAccessor, const char*);
    static bool isGotCheckpointInWorld(GameDataHolderAccessor, s32);
    static bool isGotCheckpoint(GameDataHolderAccessor, al::PlacementId*);
    static void calcGotCheckpointNumInWorld(GameDataHolderAccessor);
    static void changeNextSceneByGotCheckpoint(GameDataHolderWriter, s32);
    static void changeNextSceneByHome(GameDataHolderWriter);
    static bool isWarpCheckpoint(GameDataHolderAccessor);
    static void getCheckpointWarpObjId(GameDataHolderAccessor);
    static void registerCheckpointTrans(GameDataHolderWriter, const al::PlacementId*,
                                        const sead::Vector3<f32>&);
    static bool isEnableUnlockHint(GameDataHolderAccessor);
    static void unlockHint(GameDataHolderWriter);
    static void unlockHintAmiibo(GameDataHolderWriter);
    static void calcHintNum(GameDataHolderAccessor);
    static void calcRestHintNum(GameDataHolderAccessor);
    static void checkExistHint(GameDataHolderAccessor);
    static void getHintNumMax(GameDataHolderAccessor);
    static void calcHintTrans(GameDataHolderAccessor, s32);
    static void getLatestHintTrans(GameDataHolderAccessor);
    static void checkLatestHintSeaOfTree(GameDataHolderAccessor);
    static void calcHintMoonRockNum(GameDataHolderAccessor);
    static void getHintMoonRockNumMax(GameDataHolderAccessor);
    static void calcHintMoonRockTrans(GameDataHolderAccessor, s32);
    static void initializeHintList(GameDataHolderWriter);
    static void calcHintTransMostEasy(GameDataHolderAccessor);
    static void calcHintTransMostNear(sead::Vector3<f32>*, GameDataHolderAccessor,
                                      const sead::Vector3<f32>&);
    static void checkHintSeaOfTree(GameDataHolderAccessor, s32);
    static void checkHintSeaOfTreeMoonRock(GameDataHolderAccessor, s32);
    static void findUnlockShineNum(bool*, GameDataHolderAccessor);
    static void findUnlockShineNumByWorldId(bool*, GameDataHolderAccessor, s32);
    static bool isUnlockedWorld(GameDataHolderAccessor, s32);
    static bool isUnlockedNextWorld(GameDataHolderAccessor);
    static bool isUnlockedAllWorld(GameDataHolderAccessor);
    static s32 getWorldIndexSpecial2();
    static bool isUnlockedCurrentWorld(GameDataHolderAccessor);
    static bool isCollectShineForNextWorldAndNotUnlockNextWorld(const al::LiveActor*);
    static void tryGetCurrentMainStageName(GameDataHolderAccessor);
    static void tryGetWorldName(const al::LayoutActor*, s32);
    static void tryGetWorldNameCurrent(const al::LayoutActor*);
    static void tryGetRegionNameCurrent(const al::LayoutActor*);
    static void getWorldDevelopNameCurrent(GameDataHolderAccessor);
    static void getWorldScenarioNum(GameDataHolderAccessor, s32);
    static void findMainStageNameByDevelopName(GameDataHolderAccessor, const char*);
    static void findWorldIdByDevelopName(GameDataHolderAccessor, const char*);
    static void tryFindWorldIdByMainStageName(const al::Scene*, const char*);
    static void tryFindWorldIdByMainStageName(const al::IUseSceneObjHolder*, const char*);
    static void checkEnableUnlockWorldSpecial1(const al::LiveActor*);
    static s32 getWorldIndexSpecial1();
    static void checkEnableUnlockWorldSpecial2(const al::LiveActor*);
    static void tryGetWorldNameByFileId(const al::LayoutActor*, s32);
    static bool isNewSaveDataByFileId(const al::LayoutActor*, s32);
    static void getLastUpdateFileTime(const al::LayoutActor*, s32);
    static void makeTextureSaveDataFileName(sead::BufferedSafeStringBase<char>*,
                                            const nn::g3d::ResFile*, const GameDataHolder*, s32);
    static s32 getWorldIndexPeach();
    static s32 getWorldIndexCity();
    static bool isCityWorldCeremonyAll(s32, s32);
    static s32 getWorldIndexSea();
    static void getUnlockWorldIdForWorldMap(const al::LayoutActor*, s32);
    static void getUnlockWorldIdForWorldMap(const al::LiveActor*, s32);
    static void getUnlockWorldIdForWorldMap(const al::Scene*, s32);
    static void getUnlockWorldIdForWorldMap(const GameDataHolder*, s32);
    static void getUnlockWorldNumForWorldMap(const al::Scene*);
    static void getUnlockWorldNumForWorldMap(const al::LiveActor*);
    static void getUnlockWorldNumForWorldMap(const al::LayoutActor*);
    static void calcNextLockedWorldIdForWorldMap(const al::LayoutActor*, s32);
    static void calcNextLockedWorldIdForWorldMap(const al::Scene*, s32);
    static void calcNextLockedWorldNumForWorldMap(const al::Scene*);
    static void calcNextLockedWorldNumForWorldMap(const al::LayoutActor*);
    static void getWorldIdForShineList(const al::LayoutActor*, s32);
    static void calcWorldNumForShineList(const al::LayoutActor*);
    static void getLatestUnlockWorldIdForShineTowerMeter(const al::LiveActor*);
    static bool isClearSandWorldScenario1(const al::Scene*);
    static bool isClearCityWorldScenario1(const al::Scene*);
    static bool isClearSkyWorldScenario1(const al::Scene*);
    static bool isCityWorldScenario2(const al::IUseSceneObjHolder*);
    static bool isWorldCity(GameDataHolderAccessor);
    static void calcNextWorldId(GameDataHolderAccessor);
    static bool isPlayDemoWorldWarp(GameDataHolderAccessor);
    static bool isPlayDemoWorldWarpHole(GameDataHolderAccessor);
    static void noPlayDemoWorldWarp(GameDataHolderWriter);
    static bool isPlayDemoReturnToHome(GameDataHolderAccessor);
    static void requestPlayDemoReturnToHome(GameDataHolderWriter);
    static bool isPlayDemoAwardSpecial(GameDataHolderAccessor);
    static void requestPlayDemoAwardSpecial(GameDataHolderWriter);
    static bool isPlayDemoLavaErupt(GameDataHolderAccessor);
    static bool isPlayDemoPlayerDownForBattleKoopaAfter(GameDataHolderAccessor);
    static void disablePlayDemoPlayerDownForBattleKoopaAfter(GameDataHolderWriter);
    static void enteredStage(GameDataHolderWriter);
    static void getCurrentCostumeTypeName(GameDataHolderAccessor);
    static void getCurrentCapTypeName(GameDataHolderAccessor);
    static void wearCostume(GameDataHolderWriter, const char*);
    static void wearCap(GameDataHolderWriter, const char*);
    static void wearCostumeRandom(al::IUseSceneObjHolder*);
    static void wearCapRandom(al::IUseSceneObjHolder*);
    static void tryWearCostumeRandomIfEnable(al::IUseSceneObjHolder*);
    static bool isCostumeRandomMode(al::IUseSceneObjHolder*);
    static void tryWearCapRandomIfEnable(al::IUseSceneObjHolder*);
    static bool isCapRandomMode(al::IUseSceneObjHolder*);
    static void setCostumeRandomMode(al::IUseSceneObjHolder*);
    static void setCapRandomMode(al::IUseSceneObjHolder*);
    static void resetCostumeRandomMode(al::IUseSceneObjHolder*);
    static void resetCapRandomMode(al::IUseSceneObjHolder*);
    static void getShopItemInfoList(GameDataHolderAccessor);
    static bool isExistHackObjInfo(GameDataHolderAccessor, const char*);
    static void getHackObjInfo(GameDataHolderAccessor, const char*);
    static void addHackDictionary(GameDataHolderWriter, const char*);
    static bool isExistInHackDictionary(GameDataHolderAccessor, const char*);
    static bool isShowHackTutorial(GameDataHolderAccessor, const char*, const char*);
    static void setShowHackTutorial(GameDataHolderWriter, const char*, const char*);
    static bool isShowBindTutorial(GameDataHolderAccessor, const BindInfo&);
    static void setShowBindTutorial(GameDataHolderWriter, const BindInfo&);
    static bool isOpenShineName(const al::LayoutActor*, s32, s32);
    static void checkAchievementShine(const al::LayoutActor*, s32, s32);
    static void calcShineNumInOneShine(const al::LayoutActor*, s32, s32);
    static void tryFindShineMessage(const al::LayoutActor*, s32, s32);
    static void tryFindShineMessage(const al::LiveActor*, const al::IUseMessageSystem*, s32, s32);
    static void findShineGetTime(const al::LayoutActor*, s32, s32);
    static void checkMoonRockShineForShineList(const al::LayoutActor*, s32, s32);
    static void checkUnlockHintByHintNpcForShineList(const al::LayoutActor*, s32, s32);
    static void checkUnlockHintByAmiiboForShineList(const al::LayoutActor*, s32, s32);
    static void checkShineSeaOfTreeForShineList(const al::LayoutActor*, s32, s32);
    static void getWorldTotalShineNumMax(GameDataHolderAccessor, s32);
    static void findShineTrans(sead::Vector3<f32>*, GameDataHolderAccessor, s32, s32);
    static void getHomeTrans(GameDataHolderAccessor);
    static bool isExistHome(GameDataHolderAccessor);
    static void getMiniGameNum(GameDataHolderAccessor);
    static void getMiniGameNumMax(GameDataHolderAccessor);
    static void getMiniGameTrans(GameDataHolderAccessor, s32);
    static void getMiniGameName(GameDataHolderAccessor, s32);
    static void getRaceStartTrans(GameDataHolderAccessor);
    static void getRaceGoalTrans(GameDataHolderAccessor);
    static void getHintNpcTrans(GameDataHolderAccessor);
    static void getShopNpcTrans(GameDataHolderAccessor, s32);
    static bool isShopSellout(GameDataHolderAccessor, s32);
    static void calcShopNum(GameDataHolderAccessor);
    static void getShopNpcIconNumMax(GameDataHolderAccessor);
    static bool isExistRaceStartNpc(GameDataHolderAccessor);
    static bool isExistHintNpc(GameDataHolderAccessor);
    static void getCurrentMapView(GameDataHolderAccessor);
    static bool isExistJango(GameDataHolderAccessor);
    static void getJangoTrans(GameDataHolderAccessor);
    static bool isExistAmiiboNpc(GameDataHolderAccessor);
    static void getAmiiboNpcTrans(GameDataHolderAccessor);
    static bool isInInvalidOpenMapStage(GameDataHolderAccessor);
    static void getStageMapPlayerPos(GameDataHolderAccessor);
    static bool isExistTimeBalloonNpc(GameDataHolderAccessor);
    static void getTimeBalloonNpcTrans(GameDataHolderAccessor);
    static bool isExistPoetter(GameDataHolderAccessor);
    static void getPoetterTrans(GameDataHolderAccessor);
    static void getMoonRockTrans(GameDataHolderAccessor);
    static void setHomeTrans(const al::LiveActor*, const sead::Vector3<f32>&);
    static void setRaceStartNpcTrans(const al::LiveActor*);
    static void setRaceStartTrans(const al::LiveActor*);
    static void setRaceGoalTrans(const al::LiveActor*);
    static void setHintNpcTrans(const al::LiveActor*);
    static void setJangoTrans(const al::LiveActor*);
    static void disableJangoTrans(const al::LiveActor*);
    static void setAmiiboNpcTrans(const al::LiveActor*);
    static void setShopNpcTrans(const al::LiveActor*, const char*, s32);
    static void setShopNpcTrans(GameDataHolderAccessor, const al::PlacementInfo&);
    static void setMiniGameInfo(GameDataHolderAccessor, const al::PlacementInfo&);
    static void setTimeBalloonTrans(GameDataHolderAccessor, const sead::Vector3f&);
    static void setPoetterTrans(GameDataHolderAccessor, const sead::Vector3<f32>&);
    static void setStageMapPlayerPos(GameDataHolderWriter, const sead::Vector3<f32>&);
    static void setMoonRockTrans(GameDataHolderWriter, const sead::Vector3<f32>&);
    static bool isBossAttackedHome(GameDataHolderAccessor);
    static void bossAttackHome(GameDataHolderWriter);
    static bool isRepairHomeByCrashedBoss(GameDataHolderAccessor);
    static void repairHomeByCrashedBoss(GameDataHolderWriter);
    static bool isFindKoopaNext(GameDataHolderAccessor, s32);
    static bool isBossAttackedHomeNext(GameDataHolderAccessor, s32);
    static bool isFindKoopa(GameDataHolderAccessor);
    static void findKoopa(GameDataHolderWriter);
    static bool isEnableCap(GameDataHolderAccessor);
    static bool isMeetCap(GameDataHolderAccessor);
    static void disableCapByPlacement(const al::LiveActor*);
    static bool isTalkedCapNearHomeInWaterfall(const al::LiveActor*);
    static bool isFlagOnTalkMessageInfo(const al::IUseSceneObjHolder*, s32);
    static void onFlagTalkMessageInfo(const al::IUseSceneObjHolder*, s32);
    static s32 getWorldIndexHat();
    static s32 getWorldIndexSand();
    static s32 getWorldIndexForest();
    static s32 getWorldIndexLake();
    static s32 getWorldIndexCloud();
    static s32 getWorldIndexClash();
    static s32 getWorldIndexSnow();
    static s32 getWorldIndexLava();
    static s32 getWorldIndexBoss();
    static s32 getWorldIndexSky();
    static bool isWorldCap(GameDataHolderAccessor);
    static bool isWorldWaterfall(GameDataHolderAccessor);
    static bool isWorldSand(GameDataHolderAccessor);
    static bool isWorldForest(GameDataHolderAccessor);
    static bool isWorldLake(GameDataHolderAccessor);
    static bool isWorldCloud(GameDataHolderAccessor);
    static bool isWorldClash(GameDataHolderAccessor);
    static bool isWorldSnow(GameDataHolderAccessor);
    static bool isWorldSea(GameDataHolderAccessor);
    static bool isWorldBoss(GameDataHolderAccessor);
    static bool isWorldSky(GameDataHolderAccessor);
    static bool isWorldMoon(GameDataHolderAccessor);
    static bool isWorldPeach(GameDataHolderAccessor);
    static bool isWorldSpecial1(GameDataHolderAccessor);
    static bool isWorldSpecial2(GameDataHolderAccessor);
    static bool isWorldTypeMoon(GameDataHolderAccessor, s32);
    static bool isSnowMainScenario1(const al::IUseSceneObjHolder*);
    static bool isSnowMainScenario2(const al::IUseSceneObjHolder*);
    static void getHomeShipStageName();
    static bool isCityWorldCeremonyAgain(s32, s32);
    static bool isGoToCeremonyFromInsideHomeShip(GameDataHolderAccessor);
    static void registerScenarioStartCameraInfo(const ScenarioStartCamera*, s32, al::CameraTicket*);
    static bool isEnableExplainAmiibo(GameDataHolderAccessor);
    static void endExplainAmiibo(GameDataHolderWriter);
    static void startSearchHintByAmiibo(GameDataHolderWriter);
    static void endSearchHintByAmiibo(GameDataHolderWriter);
    static void setKidsModeOn(const al::Scene*);
    static void setKidsModeOff(const al::Scene*);
    static bool isCollectedBgm(GameDataHolderAccessor, const char*, const char*);
    static void getCollectBgmByIndex(GameDataHolderAccessor, s32);
    static void trySetCollectedBgm(GameDataHolderWriter, const char*, const char*);
    static void getCollectedBgmNum(GameDataHolderWriter);
    static void registerShineInfo(GameDataHolderAccessor, const ShineInfo*, const sead::Vector3f&);
    static void setHintTrans(GameDataHolderAccessor, s32, const sead::Vector3f&);
    static void resetHintTrans(GameDataHolderAccessor, s32);
    static bool isKoopaLv3(GameDataHolderAccessor);
};
