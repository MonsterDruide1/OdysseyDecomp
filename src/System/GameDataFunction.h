#pragma once

#include "Library/LiveActor/LiveActor.h"
#include "System/GameDataHolder.h"
#include "System/GameDataHolderAccessor.h"
#include "System/GameDataHolderWriter.h"

struct BindInfo;
struct ChangeStageInfo;
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

    static bool isHomeShipStage(GameDataHolder const*);

    // used during the event that enables the odyssey to be used (enables the globe for the odyssey)
    static void talkCapNearHomeInWaterfall(const al::LiveActor*);

    static void getGameDataHolder(al::IUseSceneObjHolder const*);
    static void getGameDataHolder(al::SceneObjHolder const*);
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
    static void tryChangeNextStage(GameDataHolderWriter, ChangeStageInfo const*);
    static void tryChangeNextStageWithStartRaceFlag(GameDataHolderWriter, ChangeStageInfo const*);
    static void tryChangeNextStageWithStartRaceYukimaru(GameDataHolderWriter,
                                                        ChangeStageInfo const*);
    static void tryChangeNextStageWithDemoWorldWarp(GameDataHolderWriter, const char*);
    static void tryChangeNextStageWithWorldWarpHole(GameDataHolderWriter, const char*);
    static void changeNextStageWithStartTimeBalloon(GameDataHolderWriter, s32);
    static void changeNextStageWithEndTimeBalloon(GameDataHolderWriter);
    static void changeNextStageWithCloset(GameDataHolderWriter);
    static void findAreaAndChangeNextStage(GameDataHolderWriter, al::LiveActor const*,
                                           sead::Vector3<f32> const*);
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
    static void getScenarioNo(al::LiveActor const*);
    static void getScenarioNo(al::LayoutActor const*);
    static void getScenarioNoPlacement(GameDataHolderAccessor);
    static bool isEqualScenario(RiseMapPartsHolder const*, s32);
    static void getMainQuestMin(al::LiveActor const*);
    static void clearStartId(GameDataHolderWriter);
    static void setCheckpointId(GameDataHolderWriter, al::PlacementId const*);
    static void setRestartPointId(GameDataHolderWriter, al::PlacementId const*);
    static void tryGetRestartPointIdString(GameDataHolderAccessor);
    static void tryGetPlayerStartId(GameDataHolderAccessor);
    static bool isPlayerStartObj(al::LiveActor const*, al::ActorInitInfo const&);
    static bool isPlayerStartObj(al::LiveActor const*, al::PlacementInfo const&);
    static bool isPlayerStartObj(al::LiveActor const*, const char*);
    static bool isPlayerStartLinkedObj(al::LiveActor const*, al::ActorInitInfo const&, const char*);
    static void setStartShine(al::LiveActor const*, ShineInfo const*);
    static void getStartShineNextIndex(GameDataHolderAccessor);
    static bool isAlreadyShowExplainCheckpointFlag(GameDataHolderAccessor);
    static bool isEnableShowExplainCheckpointFlag(GameDataHolderAccessor);
    static void showExplainCheckpointFlag(GameDataHolderAccessor);
    static void getLifeMaxUpItem(al::LiveActor const*);
    static void getPlayerHitPoint(GameDataHolderAccessor);
    static void getPlayerHitPointMaxCurrent(GameDataHolderAccessor);
    static void getPlayerHitPointMaxNormal(GameDataHolderAccessor);
    static bool isPlayerHitPointMax(GameDataHolderAccessor);
    static bool isPlayerHitPointMaxWithItem(GameDataHolderAccessor);
    static void recoveryPlayer(al::LiveActor const*);
    static void recoveryPlayerForDebug(al::LiveActor const*);
    static void recoveryPlayerMax(al::LiveActor const*);
    static void recoveryPlayerForSystem(GameDataHolder const*);
    static void recoveryPlayerMaxForSystem(GameDataHolder const*);
    static void initPlayerHitPointForSystem(GameDataHolder const*);
    static void damagePlayer(GameDataHolderWriter);
    static bool isPlayerLifeZero(GameDataHolderAccessor);
    static bool isGotShine(GameDataHolderAccessor, ShineInfo const*);
    static bool isGotShine(GameDataHolderAccessor, al::PlacementId const*);
    static bool isGotShine(GameDataHolderAccessor, const char*, const char*);
    static bool isGotShine(Shine const*);
    static void setGotShine(GameDataHolderWriter, ShineInfo const*);
    static void getGotShineNum(GameDataHolderAccessor, s32);
    static void getLatestGetShineInfo(GameDataHolderAccessor);
    static void getTotalShopShineNum(GameDataHolderAccessor, s32);
    static void tryGetNextMainScenarioLabel(sead::BufferedSafeStringBase<char>*,
                                            sead::BufferedSafeStringBase<char>*,
                                            al::IUseSceneObjHolder const*);
    static void getMainScenarioNumMax(GameDataHolderAccessor);
    static void tryGetNextMainScenarioPos(sead::Vector3<f32>*, GameDataHolderAccessor);
    static bool isPlayScenarioCamera(GameDataHolderAccessor, QuestInfo const*);
    static bool isPlayAlreadyScenarioStartCamera(GameDataHolderAccessor, s32);
    static bool isEnterStageFirst(GameDataHolderAccessor);
    static bool isNextMainShine(GameDataHolderAccessor, s32);
    static bool isMainShine(GameDataHolderAccessor, s32);
    static bool isLatestGetMainShine(GameDataHolderAccessor, ShineInfo const*);
    static void tryFindLinkedShineIndex(al::LiveActor const*, al::ActorInitInfo const&);
    static void tryFindLinkedShineIndex(al::LiveActor const*, al::ActorInitInfo const&, s32);
    static void tryFindLinkedShineIndexByLinkName(al::LiveActor const*, al::ActorInitInfo const&,
                                                  const char*);
    static void calcLinkedShineNum(al::LiveActor const*, al::ActorInitInfo const&);
    static void tryFindShineIndex(al::LiveActor const*, al::ActorInitInfo const&);
    static void tryFindShineIndex(al::LiveActor const*, const char*, const char*);
    static void tryFindShineUniqueId(al::Scene const*, ShineInfo const*);
    static void disableHintByShineIndex(al::LiveActor const*, s32);
    static void enableHintByShineIndex(al::LiveActor const*, s32);
    static void calcIsGetMainShineAll(al::IUseSceneObjHolder const*);
    static void calcIsGetShineAllInWorld(GameDataHolderAccessor, s32);
    static void calcIsGetShineAllInAllWorld(GameDataHolderAccessor);
    static void tryFindAndInitShineInfoByOptionalId(ShineInfo*, GameDataHolderAccessor,
                                                    const char*);
    static bool isGotLinkedShineBeforeInitActor(al::ActorInitInfo const&, const char*);
    static void checkIsComplete(al::IUseSceneObjHolder const*, s32);
    static bool isExistInHackDictionary(GameDataHolderAccessor, const char*, s32);
    static void getCollectedBgmNum(GameDataHolderAccessor, s32);
    static void getCollectedBgmMaxNum(GameDataHolderWriter);
    static bool isEnableOpenMoonRock(al::LiveActor const*);
    static void openMoonRock(al::LiveActor const*, al::PlacementId const*);
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
    static void openDoorLock(GameDataHolderWriter, al::PlacementId const*);
    static bool isOpenDoorLock(GameDataHolderAccessor, al::PlacementId const*);
    static void setObjStarted(GameDataHolder*, al::PlacementId const*);
    static bool isObjStarted(GameDataHolder const*, al::PlacementId const*);
    static bool isObjStarted(GameDataHolderAccessor, const char*, const char*);
    static void saveObjS32(GameDataHolderWriter, al::PlacementId const*, s32);
    static void tryFindSaveObjS32Value(s32*, GameDataHolderAccessor, al::PlacementId const*);
    static void onObjNoWriteSaveData(GameDataHolderWriter, al::PlacementId const*);
    static void offObjNoWriteSaveData(GameDataHolderWriter, al::PlacementId const*);
    static bool isOnObjNoWriteSaveData(GameDataHolderAccessor, al::PlacementId const*);
    static void onObjNoWriteSaveDataResetMiniGame(GameDataHolderWriter, al::PlacementId const*);
    static void offObjNoWriteSaveDataResetMiniGame(GameDataHolderWriter, al::PlacementId const*);
    static bool isOnObjNoWriteSaveDataResetMiniGame(GameDataHolderAccessor, al::PlacementId const*);
    static void onObjNoWriteSaveDataInSameScenario(GameDataHolder*, al::PlacementId const*);
    static bool isOnObjNoWriteSaveDataInSameScenario(GameDataHolder const*, al::PlacementId const*);
    static void setSessionEventProgress(GameDataHolderWriter, SessionEventProgress const&);
    static void getSessionEventProgress(GameDataHolderAccessor);
    static bool isPayCoinToSphinx(al::LiveActor const*);
    static void payCoinToSphinx(al::LiveActor const*);
    static bool isRemovedCapByJango(al::LiveActor const*);
    static bool isGetCapFromJango(al::LiveActor const*);
    static void removeCapByJango(al::LiveActor const*);
    static void getCapFromJango(al::LiveActor const*);
    static void addJangoCount(al::LiveActor const*);
    static void getJangoCount(al::LiveActor const*);
    static bool isFirstWorldTravelingStatus(WorldTravelingNpc const*);
    static void saveWorldTravelingStatus(WorldTravelingNpc const*, const char*);
    static void getWorldTravelingStatus(WorldTravelingNpc const*);
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
    static void getRaceRivalLevel(al::LiveActor const*);
    static void getRaceRivalLevel(al::Scene const*);
    static void setRaceRivalLevel(al::LiveActor const*, s32);
    static void setLastRaceRanking(GameDataHolderAccessor, s32);
    static void getLastRaceRanking(GameDataHolderWriter);
    static void incrementRaceLoseCount(al::Scene const*, s32);
    static void getRaceLoseCount(al::LiveActor const*, s32);
    static void addCoinCollect(GameDataHolderWriter, al::PlacementId const*);
    static void useCoinCollect(GameDataHolderWriter, s32);
    static bool isGotCoinCollect(GameDataHolderAccessor, al::ActorInitInfo const&);
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
    static void registerCheckpointTrans(GameDataHolderWriter, al::PlacementId const*,
                                        sead::Vector3<f32> const&);
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
                                      sead::Vector3<f32> const&);
    static void checkHintSeaOfTree(GameDataHolderAccessor, s32);
    static void checkHintSeaOfTreeMoonRock(GameDataHolderAccessor, s32);
    static void findUnlockShineNum(bool*, GameDataHolderAccessor);
    static void findUnlockShineNumByWorldId(bool*, GameDataHolderAccessor, s32);
    static bool isUnlockedWorld(GameDataHolderAccessor, s32);
    static bool isUnlockedNextWorld(GameDataHolderAccessor);
    static bool isUnlockedAllWorld(GameDataHolderAccessor);
    static void getWorldIndexSpecial2();
    static bool isUnlockedCurrentWorld(GameDataHolderAccessor);
    static bool isCollectShineForNextWorldAndNotUnlockNextWorld(al::LiveActor const*);
    static void tryGetCurrentMainStageName(GameDataHolderAccessor);
    static void tryGetWorldName(al::LayoutActor const*, s32);
    static void tryGetWorldNameCurrent(al::LayoutActor const*);
    static void tryGetRegionNameCurrent(al::LayoutActor const*);
    static void getWorldDevelopNameCurrent(GameDataHolderAccessor);
    static void getWorldScenarioNum(GameDataHolderAccessor, s32);
    static void findMainStageNameByDevelopName(GameDataHolderAccessor, const char*);
    static void findWorldIdByDevelopName(GameDataHolderAccessor, const char*);
    static void tryFindWorldIdByMainStageName(al::Scene const*, const char*);
    static void tryFindWorldIdByMainStageName(al::IUseSceneObjHolder const*, const char*);
    static void checkEnableUnlockWorldSpecial1(al::LiveActor const*);
    static void getWorldIndexSpecial1();
    static void checkEnableUnlockWorldSpecial2(al::LiveActor const*);
    static void tryGetWorldNameByFileId(al::LayoutActor const*, s32);
    static bool isNewSaveDataByFileId(al::LayoutActor const*, s32);
    static void getLastUpdateFileTime(al::LayoutActor const*, s32);
    static void makeTextureSaveDataFileName(sead::BufferedSafeStringBase<char>*,
                                            nn::g3d::ResFile const*, GameDataHolder const*, s32);
    static void getWorldIndexPeach();
    static void getWorldIndexCity();
    static bool isCityWorldCeremonyAll(s32, s32);
    static void getWorldIndexSea();
    static void getUnlockWorldIdForWorldMap(al::LayoutActor const*, s32);
    static void getUnlockWorldIdForWorldMap(al::LiveActor const*, s32);
    static void getUnlockWorldIdForWorldMap(al::Scene const*, s32);
    static void getUnlockWorldIdForWorldMap(GameDataHolder const*, s32);
    static void getUnlockWorldNumForWorldMap(al::Scene const*);
    static void getUnlockWorldNumForWorldMap(al::LiveActor const*);
    static void getUnlockWorldNumForWorldMap(al::LayoutActor const*);
    static void calcNextLockedWorldIdForWorldMap(al::LayoutActor const*, s32);
    static void calcNextLockedWorldIdForWorldMap(al::Scene const*, s32);
    static void calcNextLockedWorldNumForWorldMap(al::Scene const*);
    static void calcNextLockedWorldNumForWorldMap(al::LayoutActor const*);
    static void getWorldIdForShineList(al::LayoutActor const*, s32);
    static void calcWorldNumForShineList(al::LayoutActor const*);
    static void getLatestUnlockWorldIdForShineTowerMeter(al::LiveActor const*);
    static bool isClearSandWorldScenario1(al::Scene const*);
    static bool isClearCityWorldScenario1(al::Scene const*);
    static bool isClearSkyWorldScenario1(al::Scene const*);
    static bool isCityWorldScenario2(al::IUseSceneObjHolder const*);
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
    static bool isShowBindTutorial(GameDataHolderAccessor, BindInfo const&);
    static void setShowBindTutorial(GameDataHolderWriter, BindInfo const&);
    static bool isOpenShineName(al::LayoutActor const*, s32, s32);
    static void checkAchievementShine(al::LayoutActor const*, s32, s32);
    static void calcShineNumInOneShine(al::LayoutActor const*, s32, s32);
    static void tryFindShineMessage(al::LayoutActor const*, s32, s32);
    static void tryFindShineMessage(al::LiveActor const*, al::IUseMessageSystem const*, s32, s32);
    static void findShineGetTime(al::LayoutActor const*, s32, s32);
    static void checkMoonRockShineForShineList(al::LayoutActor const*, s32, s32);
    static void checkUnlockHintByHintNpcForShineList(al::LayoutActor const*, s32, s32);
    static void checkUnlockHintByAmiiboForShineList(al::LayoutActor const*, s32, s32);
    static void checkShineSeaOfTreeForShineList(al::LayoutActor const*, s32, s32);
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
    static void setHomeTrans(al::LiveActor const*, sead::Vector3<f32> const&);
    static void setRaceStartNpcTrans(al::LiveActor const*);
    static void setRaceStartTrans(al::LiveActor const*);
    static void setRaceGoalTrans(al::LiveActor const*);
    static void setHintNpcTrans(al::LiveActor const*);
    static void setJangoTrans(al::LiveActor const*);
    static void disableJangoTrans(al::LiveActor const*);
    static void setAmiiboNpcTrans(al::LiveActor const*);
    static void setShopNpcTrans(al::LiveActor const*, const char*, s32);
    static void setShopNpcTrans(GameDataHolderAccessor, al::PlacementInfo const&);
    static void setMiniGameInfo(GameDataHolderAccessor, al::PlacementInfo const&);
    static void setTimeBalloonTrans(GameDataHolderAccessor, sead::Vector3<f32> const&);
    static void setPoetterTrans(GameDataHolderAccessor, sead::Vector3<f32> const&);
    static void setStageMapPlayerPos(GameDataHolderWriter, sead::Vector3<f32> const&);
    static void setMoonRockTrans(GameDataHolderWriter, sead::Vector3<f32> const&);
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
    static void disableCapByPlacement(al::LiveActor const*);
    static bool isTalkedCapNearHomeInWaterfall(al::LiveActor const*);
    static bool isFlagOnTalkMessageInfo(al::IUseSceneObjHolder const*, s32);
    static void onFlagTalkMessageInfo(al::IUseSceneObjHolder const*, s32);
    static void getWorldIndexHat();
    static void getWorldIndexSand();
    static void getWorldIndexForest();
    static void getWorldIndexLake();
    static void getWorldIndexCloud();
    static void getWorldIndexClash();
    static void getWorldIndexSnow();
    static void getWorldIndexLava();
    static void getWorldIndexBoss();
    static void getWorldIndexSky();
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
    static bool isSnowMainScenario1(al::IUseSceneObjHolder const*);
    static bool isSnowMainScenario2(al::IUseSceneObjHolder const*);
    static void getHomeShipStageName();
    static bool isCityWorldCeremonyAgain(s32, s32);
    static bool isGoToCeremonyFromInsideHomeShip(GameDataHolderAccessor);
    static void registerScenarioStartCameraInfo(ScenarioStartCamera const*, s32, al::CameraTicket*);
    static bool isEnableExplainAmiibo(GameDataHolderAccessor);
    static void endExplainAmiibo(GameDataHolderWriter);
    static void startSearchHintByAmiibo(GameDataHolderWriter);
    static void endSearchHintByAmiibo(GameDataHolderWriter);
    static void setKidsModeOn(al::Scene const*);
    static void setKidsModeOff(al::Scene const*);
    static bool isCollectedBgm(GameDataHolderAccessor, const char*, const char*);
    static void getCollectBgmByIndex(GameDataHolderAccessor, s32);
    static void trySetCollectedBgm(GameDataHolderWriter, const char*, const char*);
    static void getCollectedBgmNum(GameDataHolderWriter);
    static void registerShineInfo(GameDataHolderAccessor, ShineInfo const*,
                                  sead::Vector3<f32> const&);
    static void setHintTrans(GameDataHolderAccessor, s32, sead::Vector3<f32> const&);
    static void resetHintTrans(GameDataHolderAccessor, s32);
    static bool isKoopaLv3(GameDataHolderAccessor);
};
