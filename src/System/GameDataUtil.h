#pragma once

#include <basis/seadTypes.h>

namespace al {
struct ActorInitInfo;
class PlacementInfo;
class PlacementId;
class LiveActor;
class Scene;
class LayoutActor;
class IUseSceneObjHolder;
}  // namespace al

class SaveObjInfo;
class GameDataHolderAccessor;
class NpcEventDirector;
class KoopaShip;
class QuestInfo;
class ShineTowerRocket;
class WorldWarpHole;
class GameDataHolder;
class CapMessageBossData;

namespace rs {
SaveObjInfo* createSaveObjInfoWriteSaveData(const al::ActorInitInfo& actorInitInfo);
SaveObjInfo* createSaveObjInfoWriteSaveData(const al::ActorInitInfo& actorInitInfo,
                                            const al::PlacementInfo& placementInfo);
SaveObjInfo* createSaveObjInfoNoWriteSaveDataInSameWorld(const al::ActorInitInfo& actorInitInfo);
SaveObjInfo* createSaveObjInfoNoWriteSaveDataInSameWorld(const al::ActorInitInfo& actorInitInfo,
                                                         const al::PlacementInfo& placementInfo);
SaveObjInfo*
createSaveObjInfoNoWriteSaveDataInSameWorldResetMiniGame(const al::ActorInitInfo& actorInitInfo);
SaveObjInfo*
createSaveObjInfoNoWriteSaveDataInSameWorldResetMiniGame(const al::ActorInitInfo& actorInitInfo,
                                                         const al::PlacementInfo& placementInfo);
SaveObjInfo* createSaveObjInfoNoWriteSaveDataInSameScenario(const al::ActorInitInfo& actorInitInfo);
SaveObjInfo* createSaveObjInfoNoWriteSaveDataInSameScenario(const al::ActorInitInfo& actorInitInfo,
                                                            const al::PlacementInfo& placementInfo);
SaveObjInfo* createSaveObjInfoWriteSaveData(const al::ActorInitInfo& actorInitInfo,
                                            const al::PlacementId* placementId);
SaveObjInfo* createSaveObjInfoNoWriteSaveDataInSameWorld(const al::ActorInitInfo& actorInitInfo,
                                                         const al::PlacementId* placementId);
SaveObjInfo*
createSaveObjInfoNoWriteSaveDataInSameWorldResetMiniGame(const al::ActorInitInfo& actorInitInfo,
                                                         const al::PlacementId* placementId);
SaveObjInfo* createSaveObjInfoNoWriteSaveDataInSameScenario(const al::ActorInitInfo& actorInitInfo,
                                                            const al::PlacementId* placementId);
void onSaveObjInfo(SaveObjInfo* saveObjInfo);
void offSaveObjInfo(SaveObjInfo* saveObjInfo);
bool isOnSaveObjInfo(const SaveObjInfo* saveObjInfo);
void setGrowFlowerTime(const al::LiveActor* actor, const al::PlacementId*, const al::PlacementId*,
                       u64 time);
void setGrowFlowerTime(const al::LiveActor* actor, const al::PlacementId*, u64 time);
u64 getGrowFlowerTime(const al::LiveActor* actor, const al::PlacementId*);
bool isUsedGrowFlowerSeed(const al::LiveActor* actor, const al::PlacementId*);
void addGrowFlowerGrowLevel(const al::LiveActor* actor, const al::PlacementId*, u32 growLevel);
u32 getGrowFlowerGrowLevel(const al::LiveActor* actor, const al::PlacementId*);
const char* findGrowFlowerPotIdFromSeedId(const al::LiveActor* actor, const al::PlacementId*);
bool isPlayFirstTimeBalloon(const al::LiveActor* actor);
void playTimeBalloonFirst(const al::LiveActor* actor);
void onShortExplainGet(const al::LiveActor* actor);
void onShortExplainSet(const al::LiveActor* actor);
bool isShortExplainGet(const al::LiveActor* actor);
bool isShortExplainSet(const al::LiveActor* actor);
void onReactedLuigiCostume(const al::LiveActor* actor);
bool isReactedLuigiCostume(const al::LiveActor* actor);
s32 getPlayerRank(const al::LiveActor* actor);
s32 getPlayerRank(GameDataHolderAccessor accessor);
void setPlayerRank(const al::LiveActor* actor, s32 rank);
void setPlayerRank(GameDataHolderAccessor accessor, s32 rank);
u64 getPrincipalId(const al::LiveActor* actor);
void setPrincipalId(const al::LiveActor* actor, u64 principalId);
bool isAnswerCorrectSphinxQuiz(GameDataHolderAccessor accessor, const char* worldName);
void answerCorrectSphinxQuiz(const al::LiveActor* actor);
bool isAnswerCorrectSphinxQuizAll(GameDataHolderAccessor accessor, const char* worldName);
void answerCorrectSphinxQuizAll(const al::LiveActor* actor);
bool isTalkAlreadyLocalLanguage(const NpcEventDirector* npcEventDirector);
void talkLocalLanguage(const al::LiveActor* actor);
const char* getLocalLanguageName(const al::LiveActor* actor);
const char* getLocalLanguageName(const NpcEventDirector* npcEventDirector);
bool isOnFlagKinopioBrigadeNpcFirstTalk(const al::LiveActor* actor);
void onFlagKinopioBrigadeNpcFirstTalk(const al::LiveActor* actor);
bool isOnFlagHintNpcFirstTalk(const al::LiveActor* actor);
void onFlagHintNpcFirstTalk(const al::LiveActor* actor);
void playScenarioStartCamera(const al::LiveActor* actor, const QuestInfo* questInfo);
void playScenarioStartCamera(const al::LiveActor* actor, s32 idx);
void setExistKoopaShip(const KoopaShip* koopaShip);
void resetExistKoopaShip(const KoopaShip* koopaShip);
bool isExistKoopaShip(const ShineTowerRocket* shineTowerRocket);
bool isExistKoopaShipInSky(const ShineTowerRocket* shineTowerRocket);
bool tryUnlockShineName(const al::LiveActor* actor, s32 shineIdx);
void calcShineIndexTableNameAvailable(s32*, s32*, const al::LiveActor* actor);
void calcShineIndexTableNameUnlockable(s32*, s32*, const al::LiveActor* actor);
s32 calcWorldIdByWorldWarpHole(const WorldWarpHole* worldWarpHole, const char* changeStageId);
bool checkIsOpenWorldWarpHoleCurrentScenario(const WorldWarpHole* worldWarpHole);
bool isKidsMode(const al::LiveActor* actor);
bool isKidsMode(const al::LayoutActor* layout);
bool isKidsMode(const al::Scene* scene);
bool isKidsMode(const al::IUseSceneObjHolder* user);
bool isKidsMode(const GameDataHolder* holder);
bool isEnableCapMessageLifeOneKidsMode(const al::LiveActor* actor);
void disableCapMessageLifeOneKidsMode(const al::LiveActor* actor);
bool isInvalidChangeStage(const al::LiveActor* actor);
bool isSequenceTimeBalloonOrRace(const al::LiveActor* actor);
bool isSequenceTimeBalloonOrRace(const al::LiveActor* actor);
bool isTreasureBoxDeadStage(const al::LiveActor* actor);
void findRaceRecord(bool* isExistRecord, bool*, s32* record, s32*, s32* lapRecord,
                    const al::LiveActor* actor, const char* raceName);
void findRaceRecord(bool* isExistRecord, bool*, s32* record, s32*, s32* lapRecord,
                    const al::Scene* actor, const char* raceName);
void findRaceRecordRaceManRace(bool* isExistRecord, bool*, s32* record, s32*, s32* lapRecord,
                               const al::Scene* scene);
void findRaceRecordRaceManRace(bool* isExistRecord, bool*, s32* record, s32*, s32* lapRecord,
                               const al::LiveActor* actor);
void findRaceRecordRaceManRaceWithWorldId(bool* isExistRecord, bool*, s32* record, s32*,
                                          s32* lapRecord, const al::Scene* scene, s32 worldId);
void findRaceRecordRaceManRaceWithWorldId(bool* isExistRecord, bool*, s32* record, s32*,
                                          s32* lapRecord, const al::LiveActor* actor, s32 worldId);
void findRaceRecordRadicon(bool* isExistRecord, bool*, s32* record, s32*, s32* lapRecord,
                           const al::Scene* scene);
void findRaceRecordRadicon(bool* isExistRecord, bool*, s32* record, s32*, s32* lapRecord,
                           const al::LiveActor* actor);
void findRaceRecordYukimaru(bool* isExistRecord, bool*, s32* record, s32*, s32* lapRecord,
                            const al::Scene* scene, s32 level);
void findRaceRecordYukimaru(bool* isExistRecord, bool*, s32* record, s32*, s32* lapRecord,
                            const al::LiveActor* actor, s32 level);
void writeRaceRecord(const al::LiveActor* actor, const char* raceName, s32 record, s32,
                     s32 lapRecord);
void writeRaceRecord(const al::Scene* scene, const char* raceName, s32 record, s32, s32 lapRecord);
s32 getJumpingRopeBestCount(const al::LiveActor* actor);
s32 getJumpingRopeBestCount(const al::Scene* scene);
void setJumpingRopeBestCount(const al::LiveActor* actor, s32 count);
void setJumpingRopeBestCount(const al::Scene* scene, s32 count);
s32 getJumpingRopeDayCount(const al::LiveActor* actor);
void setJumpingRopeDayCount(const al::LiveActor* actor, s32 count);
void setJumpingRopeUpdateScoreFlag(const al::LiveActor* actor);
bool isExistRecordJumpingRope(const al::LiveActor* actor);
bool isExistRecordJumpingRope(const al::Scene* scene);
bool isExistRecordInDayJumpingRope(const al::LayoutActor* layout);
s32 getVolleyballBestCount(const al::LiveActor* actor);
s32 getVolleyballBestCount(const al::Scene* scene);
void setVolleyballBestCount(const al::LiveActor* actor, s32 count);
void setVolleyballBestCount(const al::Scene* scene, s32 count);
s32 getVolleyballDayCount(const al::LiveActor* actor);
void setVolleyballDayCount(const al::LiveActor* actor, s32 count);
void setVolleyballUpdateScoreFlag(const al::LiveActor* actor);
bool isExistRecordVolleyball(const al::LiveActor* actor);
bool isExistRecordVolleyball(const al::Scene* scene);
bool isExistRecordInDayVolleyball(const al::LayoutActor* layout);
void writeRaceRecordRaceManRace(const al::LiveActor* actor, s32 record);
bool isTalkFirstAmiiboNpc(const al::LiveActor* actor);
void offFlagTalkFirstAmiiboNpc(const al::LiveActor* actor);
bool checkGetShineForWorldTravelingPeach(GameDataHolderAccessor accessor, const char* worldName);
bool checkEnableAppearMoonWorldTravelingPeach(const al::LiveActor* actor);
bool isStartWorldTravelingPeach(const al::LiveActor* actor);
void startWorldTravelingPeach(const al::LiveActor* actor);
bool checkGetEnoughShine(const al::LiveActor* actor, s32 count);
bool checkGetEnoughShineInCurrentWorld(const al::LiveActor* actor);
void setFlagOnStartKoopaCapture(const al::LiveActor* actor);
bool isStartKoopaCapture(const al::LiveActor* actor);
bool isAlreadyTalkPrevWorldInWorldWarp(const al::LiveActor* actor, s32);
bool isAlreadyTalkNextWorldInWorldWarp(const al::LiveActor* actor, s32);
void setAlreadyTalkPrevWorldInWorldWarp(const al::LiveActor* actor, s32);
void setAlreadyTalkNextWorldInWorldWarp(const al::LiveActor* actor, s32);
bool checkNeedTalkPrevWorld(const al::LiveActor* actor);
s32 getTipsIdInWorldWarp(const al::LiveActor* actor);
void addTipsIdInWorldWarp(const al::LiveActor* actor);
void setTalkSpecialInWorldWarp(const al::LiveActor* actor, bool, bool);
bool isTalkFindKoopaInWorldWarp(const al::LiveActor* actor);
bool isTalkBossRaidInWorldWarp(const al::LiveActor* actor);
bool isClearMainScenario(const al::LiveActor* actor, s32 worldId);
bool checkAlreadyVisitStage(const GameDataHolder* holder, const char* stageName);
void visitStage(const GameDataHolder* holder, const char* stageName);
bool isSequenceGoToNextWorld(const al::IUseSceneObjHolder* user);
bool isSequenceCollectShineBeforeGameClear(const al::IUseSceneObjHolder* user);
bool isSequenceCollectShineForRepairHome(const al::IUseSceneObjHolder* user);
bool isSequenceCollectShine(const al::IUseSceneObjHolder* user);
bool isSequenceGoToMoonRock(const al::IUseSceneObjHolder* user);
bool isEnableExplainShopCoin(const al::LiveActor* actor);
void explainShopCoin(const al::LiveActor* actor);
bool checkShowNewItemAndPrepareSave(const al::LiveActor* actor);
bool isInNormalCoinShopFirst(const al::LiveActor* actor);
void offInNormalCoinShopFirst(const al::LiveActor* actor);
bool checkExistNewShopItem(const al::LiveActor* actor, s32, s32, bool);
CapMessageBossData* getCapMessageBossData(const al::LiveActor* actor);
void getYoshiFruit(SaveObjInfo* saveObjInfo);
bool isGetYoshiFruit(const SaveObjInfo* saveObjInfo);
bool isTalkKakku(const al::LiveActor* actor);
void talkKakku(const al::LiveActor* actor);
bool isTalkWorldTravelingPeach(const al::LiveActor* actor);
void talkWorldTravelingPeach(const al::LiveActor* actor);
bool isTalkCollectBgmNpc(const al::LiveActor* actor);
void talkCollectBgmNpc(const al::LiveActor* actor);
s32 getTokimekiMayorNpcFavorabilityRating(const al::LiveActor* actor);
void setTokimekiMayorNpcFavorabilityRating(const al::LiveActor* actor, s32 rating);
bool tryFindCoinStackSave(s32*, const al::LiveActor* actor, const al::PlacementId*);
void saveCoinStack(const al::LiveActor* actor, const al::PlacementId*, s32);
bool isCollectedBgmCityWorldCelemony(const al::LiveActor* actor);
bool isCollectedBgmCityWorldCelemony2D(const al::LiveActor* actor);
s32 calcGetShineNumCollectedBgm(GameDataHolderAccessor accessor);
bool checkGetShineCollectedBgmAll(const al::LiveActor* actor);
bool checkSavedHintPhoto(const al::LiveActor* actor, const char* name);
void saveHintPhoto(const al::LiveActor* actor, const char* name);
bool isRideSphinx(GameDataHolderAccessor accessor);
void rideSphinx(const al::LiveActor* actor);
bool isRideMotorcycle(GameDataHolderAccessor accessor);
void rideMotorcycle(const al::LiveActor* actor);
void setShowInformationPoleGrab(const al::IUseSceneObjHolder* user);
bool isNeedShowInformationPoleGrab(const al::IUseSceneObjHolder* user);
void setShowInformationPoleClimb(const al::IUseSceneObjHolder* user);
bool isNeedShowInformationPoleClimb(const al::IUseSceneObjHolder* user);
void setShowInformationPoleClimbTop(const al::IUseSceneObjHolder* user);
bool isNeedShowInformationPoleClimbTop(const al::IUseSceneObjHolder* user);
bool isNeedConfirmNetworkFirst(const al::IUseSceneObjHolder* user);
void saveConfirmNetworkFirst(const al::IUseSceneObjHolder* user);
}  // namespace rs
