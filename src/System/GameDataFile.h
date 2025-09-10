#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>
#include <prim/seadBitFlag.h>
#include <prim/seadSafeString.h>
#include <stream/seadStream.h>

namespace al {
struct ActorInitInfo;
class ByamlWriter;
class IUseSceneObjHolder;
class PlacementId;
class PlacementInfo;
}  // namespace al

namespace ShopItem {
struct ItemInfo;
}

class GameProgressData;
class GameDataHolder;
class HintPhotoData;
struct ItemInfo;
class NetworkUploadFlag;
class PlayerHitPointData;
class QuestInfo;
class RaceRecord;
class SessionMusicianType;
struct ShineInfo;
class ShopTalkData;
class TimeBalloonSaveData;
class UniqObjInfo;
class VisitStageData;
class WorldList;
class WorldWarpTalkData;

class GameDataFile {
public:
    class HintInfo {
    public:
        enum class HintStatus { NONE, NPC, AMIIBO };

        void clear();

        bool isDisableByWorldWarpHole(bool condition) const;
        bool isEnableUnlock(s32 curWorldId, bool isGameClear, s32 scenarioNo, bool isInWorld) const;
        bool isHintStatusUnlock(s32 curWorldId, s32 scenarioNo, bool isInWorld) const;
        bool isHintStatusUnlockByNpc() const;
        bool isHintStatusUnlockByAmiibo() const;
        bool isEnableNameUnlockByScenario(s32 curWorldId, s32 scenarioNo, bool isInWorld) const;

        bool testFunc(s32 curWorldId, bool isGameClear, s32 scenarioNo, bool isInWorld) const;

    private:
        sead::FixedSafeString<0x80> mStageName;
        sead::FixedSafeString<0x80> mObjId;
        sead::FixedSafeString<0x40> mScenarioName;
        const char* mObjectName;
        sead::Vector3f mTrans;
        sead::Vector3f mTransAgain;
        void* unkPtr1;
        void* unkPtr2;
        void* unkPtr3;
        void* unkPtr4;
        s32 mMainScenarioNo;
        s32 mWorldIndex;
        bool mIsMoonRock;
        bool unkBool1;
        bool mIsAchievement;
        bool mIsGrand;
        bool mIsShopMoon;
        s32 unkInt;
        HintStatus mHintStatus;
        s32 unkInt3;
        s32 unkInt4;
        void* unkPtr6;
        s32 mUniqueID;
        s32 mHintIdx;
        sead::FixedSafeString<0x20> mOptionalID;
        sead::BitFlag32 mProgressBitflag;
        bool unkBool2;
        bool unkBool3;
    };

    enum class CountType : s32 {
        Value_0 = 0,
        Value_1 = 1,
        Value_2 = 2,
    };

    GameDataFile(GameDataHolder*);
    void initializeData();
    bool tryReadByamlData(const u8*);
    bool tryFindCoinCollectInfo(const u8*, const u8*) const;
    bool tryFindShineIndexByUniqueId(s32) const;
    bool tryFindCoinCollectIndexByUniqueId(s32) const;
    void buyDefaultItem();
    void unlockAchievementShineName();
    bool isKidsMode() const;
    void updateWorldMapIndex();
    void updateWorldWarpIndex();
    s32 getScenarioNo(s32) const;
    s32 getMainScenarioNo(s32) const;
    bool isEmpty();
    void initializeCheckpointTable();
    void generateSaveDataIdForPrepo();
    void resetMapIcon();
    void wearDefault();
    void initializeHintList();
    void initializeCoinCollectList();
    void resetTempData();
    bool isGameClear() const;
    bool isUnlockedWorld(s32 index) const;
    bool isAlreadyGoWorld(s32 index) const;
    void addPlayTime(s32, const al::IUseSceneObjHolder*);
    s64 getPlayTimeTotal() const;
    void updateSaveTime();
    void updateSaveTimeForDisp();
    void updateSaveInfoForDisp();
    u64 getLastUpdateTime() const;
    void generateSaveDataIdForPrepoForWrite();
    void resetSaveDataIdForPrepoForWrite();
    void startStage(const char*, s32);
    bool isRaceStart() const;
    bool checkIsHomeStage(const char*) const;
    void setGameClear();
    void setOriginalHintTrans(s32);
    PlayerHitPointData* getPlayerHitPointData() const;
    void startDemoStage(const char*);
    void changeNextStage(const char*);
    void returnPrevStage();
    void changeNextStageWithDemoWorldWarp(const char*);
    void changeNextStageWithWorldWarpHole(const char*);
    void restartStage();
    s32 calcNextScenarioNo() const;
    const char* getStageNameCurrent() const;
    const char* tryGetStageNameCurrent() const;
    const char* getStageNameNext() const;
    void changeWipeType(const char*);
    void setActivateHome();
    bool isGoToCeremonyFromInsideHomeShip() const;
    const char* getPlayerStartId() const;
    void setCheckpointId(const al::PlacementId*);
    void setRestartPointId(const al::PlacementId*);
    void clearStartId();
    const char* tryGetRestartPointIdString() const;
    void endStage();
    void missAndRestartStage();
    void setMissRestartInfo(const al::PlacementInfo&);
    bool isUseMissRestartInfo(const al::PlacementInfo&);
    bool isFirstTimeNextWorld() const;
    void getAchievement(const char*);
    bool isGotShine(const ShineInfo*);
    bool isGotShine(s32) const;
    bool checkGotShine(const char*) const;
    void setGotShine(const ShineInfo*);
    s32 getShineNum() const;
    s32 getShineNum(s32) const;
    s32 getTotalShineNum() const;
    s32 getTotalShopShineNum() const;
    bool tryGetNextMainScenarioLabel(sead::BufferedSafeStringBase<char>*,
                                     sead::BufferedSafeStringBase<char>*) const;
    bool tryGetNextMainScenarioPos(sead::Vector3f*) const;
    HintInfo* tryFindNextMainScenarioInfo() const;
    void addPayShine(s32);
    void addPayShineCurrentAll();
    s32 getPayShineNum(s32) const;
    s32 getTotalPayShineNum() const;
    bool isPayShineAllInAllWorld() const;
    void addKey(s32);
    s32 getKeyNum() const;
    void addOpenDoorLockNum(s32);
    void setStartedObj(const al::PlacementId*);
    bool isStartedObj(const al::PlacementId*, const char*) const;
    bool isStartedObj(const char*, const char*) const;
    void setSaveObjS32(const al::PlacementId*, s32);
    UniqObjInfo* tryFindSaveObjS32(const al::PlacementId*);
    bool isExistSessionMember(const SessionMusicianType&);
    void addCoinCollect(const al::PlacementId*);
    bool isGotCoinCollect(const al::PlacementId*) const;
    s32 getCoinCollectNum() const;
    s32 getCoinCollectGotNum() const;
    s32 getCoinCollectGotNum(s32) const;
    void useCoinCollect();
    const char* tryFindCoinCollectStageName(s32) const;
    void payCoinToSphinx();
    bool isPayCoinToSphinx() const;
    void answerCorrectSphinxQuiz();
    void answerCorrectSphinxQuizAll();
    bool isAnswerCorrectSphinxQuiz(s32 index) const;
    bool isAnswerCorrectSphinxQuizAll(s32 index) const;
    bool isTalkAlreadyLocalLanguage() const;
    void talkLocalLanguage();
    bool isFirstWorldTravelingStatus() const;
    void saveWorldTravelingStatus() const;
    void getWorldTravelingStatus() const;
    bool isStartWorldTravelingPeach();
    void startWorldTravelingPeach();
    void setGrowFlowerTime(const al::PlacementId*, const al::PlacementId*, u64 time);
    void setGrowFlowerTime(const al::PlacementId*, u64 time);
    u64 getGrowFlowerTime(const al::PlacementId*);
    void addGrowFlowerGrowLevel(const al::PlacementId*, u32 growLevel);
    u32 getGrowFlowerGrowLevel(const al::PlacementId*) const;
    bool isUsedGrowFlowerSeed(const al::PlacementId*) const;
    const char* findGrowFlowerPotIdFromSeedId(const al::PlacementId*);
    void addCoin(s32);
    void addPlayerJumpCount();
    s32 getPlayerJumpCount() const;
    void addPlayerThrowCapCount();
    s32 getPlayerThrowCapCount() const;
    bool readFromStream(sead::ReadStream*, u8*);
    void tryReadByamlDataFromStream(sead::ReadStream*, u8*, s32);
    void writeToStream(sead::WriteStream*, sead::Heap*) const;
    bool tryWriteByByaml(sead::WriteStream*, sead::Heap*) const;
    bool isPlayDemoPlayerDownForBattleKoopaAfter() const;
    s32 getCheckpointNumMaxInWorld() const;
    sead::Vector3f* getCheckpointTransInWorld(const char*) const;
    bool isGotCheckpointInWorld(s32) const;
    s32 calcCheckpointIndexInScenario(s32) const;
    s32 getCheckpointObjIdInWorld(s32) const;
    bool isGotCheckpoint(al::PlacementId*) const;
    void changeNextSceneByGotCheckpoint(s32);
    void changeNextSceneByWarp();
    void changeNextSceneByHome();
    void startYukimaruRace();
    void startYukimaruRaceTutorial();
    void startRaceManRace();
    void registerCheckpointTrans(const al::PlacementId*, const sead::Vector3f&);
    s32 calcGetCheckpointNum() const;
    bool isEnableUnlockHint() const;
    s32 calcRestHintNum() const;
    void unlockHint();
    void unlockHintImpl(s32);
    void unlockHintAmiibo();
    void unlockHintAddByMoonRock();
    s32 calcHintNum() const;
    sead::Vector3f* calcHintTrans(s32) const;
    HintInfo* findHint(s32) const;
    sead::Vector3f* calcHintTransMostEasy() const;
    HintInfo* findHintInfoMostEasy() const;
    s32 calcHintMoonRockNum() const;
    sead::Vector3f* calcHintMoonRockTrans(s32) const;
    HintInfo* findHintMoonRock(s32) const;
    bool tryUnlockShineName(s32, s32);
    bool isOpenMoonRock(s32) const;
    void calcShineIndexTableNameAvailable(s32*, s32*, s32);
    void calcShineIndexTableNameUnlockable(s32*, s32*, s32);
    bool isUnlockAchievementShineName() const;
    void unlockWorld(s32);
    void noPlayDemoWorldWarp();
    s32 calcWorldWarpHoleThroughNum() const;
    s32 getWorldWarpHoleThroughNumMax() const;
    void enteredStage();
    void buyItem(const ShopItem::ItemInfo*, bool);
    void tryFindItemList(ItemInfo*);
    s32 calcHaveClothNum() const;
    s32 calcHaveCapNum() const;
    s32 calcHaveStickerNum() const;
    s32 calcHaveGiftNum() const;
    bool isBuyItem(const ShopItem::ItemInfo*) const;
    bool isBuyItem(const char*, const sead::FixedSafeString<64>*) const;
    void buyItemAll();
    void wearCostume(const char*);
    void wearCap(const char*);
    void addHackDictionary(const char*);
    bool isExistInHackDictionary(const char*) const;
    void findShine(s32, s32) const;
    bool isGotShine(s32, s32) const;
    bool isOpenShineName(s32, s32) const;
    s32 calcShineNumInOneShine(s32, s32) const;
    void checkAchievementShine(s32, s32) const;
    s32 getWorldTotalShineNum(s32) const;
    s32 getWorldTotalShineNumMax(s32) const;
    void winRace();
    RaceRecord* findRaceRecord(const char*) const;
    void incrementRaceLoseCount(s32);
    s32 getRaceLoseCount(s32) const;
    void setJumpingRopeBestCount(s32);
    void setUpdateJumpingRopeScoreFlag();
    void setVolleyballBestCount(s32);
    void setUpdateVolleyballScoreFlag();
    bool isExistJango() const;
    void setJangoTrans(const sead::Vector3f&);
    void setAmiiboNpcTrans(const sead::Vector3f&);
    void setTimeBalloonNpcTrans(const sead::Vector3f&);
    void setPoetterTrans(const sead::Vector3f&);
    void setShopNpcTrans(const sead::Vector3f&);
    void setMoonRockTrans(const sead::Vector3f&);
    void setMiniGameInfo(const sead::Vector3f&, const char*);
    s32 calcMiniGameNum() const;
    s32 getMiniGameNumMax() const;
    sead::Vector3f getMiniGameTrans(s32) const;
    const char* getMiniGameName(s32) const;
    bool isExistTimeBalloonNpc() const;
    sead::Vector3f getTimeBalloonNpcTrans() const;
    bool isExistPoetter() const;
    sead::Vector3f getPoetterTrans() const;
    bool isAlreadyShowExplainCheckpointFlag() const;
    void showExplainCheckpointFlag();
    sead::Vector3f getShopNpcTrans(s32) const;
    bool isShopSellout() const;
    s32 calcShopNum() const;
    s32 getShopNpcIconNumMax() const;
    s32 getScenarioNo() const;
    s32 getScenarioNoPlacement() const;
    bool isClearWorldMainScenario(s32) const;
    bool isFlagOnTalkMessageInfo(s32) const;
    void setFlagOnTalkMessageInfo(s32);
    bool isTalkKakku() const;
    void talkKakku();
    bool isTalkWorldTravelingPeach() const;
    void talkWorldTravelingPeach();
    bool isTalkCollectBgmNpc() const;
    void talkCollectBgmNpc();
    s32 getTokimekiMayorNpcFavorabilityRating() const;
    void setTokimekiMayorNpcFavorabilityRating(s32) const;
    bool isFirstNetwork() const;
    void noFirstNetwork();
    void setKidsMode(bool);
    bool isPlayScenarioCamera(const QuestInfo*) const;
    bool isNextMainShine(s32) const;
    bool isMainShine(s32) const;
    bool isLatestGetMainShine(const ShineInfo*) const;
    bool calcIsGetMainShineAll(const al::IUseSceneObjHolder*) const;
    bool calcIsGetShineAllInWorld(s32) const;
    s32 tryFindLinkedShineIndex(const al::ActorInitInfo&, const al::IUseSceneObjHolder*) const;
    s32 tryFindLinkedShineIndex(const al::ActorInitInfo&, s32, const al::IUseSceneObjHolder*) const;
    s32 tryFindLinkedShineIndexByLinkName(const al::IUseSceneObjHolder*, const al::ActorInitInfo&,
                                          const char*) const;
    s32 calcLinkedShineNum(const al::ActorInitInfo&) const;
    s32 tryFindShineIndex(const al::ActorInitInfo&) const;
    s32 tryFindShineIndex(const char*, const char*) const;
    void disableHintById(s32);
    void enableHintById(s32);
    void setStartShine(const ShineInfo*);
    s32 getStartShineNextIndex() const;
    void setHintTrans(s32, const sead::Vector3f&);
    void resetHintTrans(s32);
    void registerShineInfo(const ShineInfo*, const sead::Vector3f&);
    s32 calcRestShineInStageWithWorldProgress(const char*) const;
    s32 calcGetShineNumByObjectNameOrOptionalId(const char*, CountType) const;
    s32 calcGetShineNumByObjectNameWithWorldId(const char*, s32) const;
    s32 calcAllShineNumByObjectNameOrOptionalId(const char*) const;
    s32 calcGetShineNumByStageName(const char*) const;
    bool tryFindAndInitShineInfoByOptionalId(ShineInfo*, const char*);
    s32 tryFindUniqueId(const ShineInfo*) const;
    s32 findUnlockShineNumCurrentWorld(bool*) const;
    s32 getMainScenarioNoCurrent() const;
    void setMainScenarioNo(s32);
    bool isCollectedBgm(const char*, const char*) const;
    void getCollectBgmByIndex(s32);
    bool trySetCollectedBgm(const char*, const char*);
    s32 getCollectedBgmNum() const;
    void setGotShine(s32);
    void setGotShine(const HintInfo*);
    bool isEnableOpenMoonRock(s32) const;
    bool tryWriteByByaml(al::ByamlWriter*) const;

    bool isHintNpcFirstTalk() const { return mIsHintNpcFirstTalk; }

    void hintNpcFirstTalk() { mIsHintNpcFirstTalk = true; }

    bool isKinopioBrigadeNpcFirstTalk() const { return mIsKinopioBrigadeNpcFirstTalk; }

    void kinopioBrigadeNpcFirstTalk() { mIsKinopioBrigadeNpcFirstTalk = true; }

    TimeBalloonSaveData* getTimeBalloonSaveData() const { return mTimeBalloonSaveData; }

    bool isTalkFirstAmiiboNpc() const { return mIsTalkFirstAmiiboNpc; }

    void offTalkFirstAmiiboNpc() { mIsTalkFirstAmiiboNpc = false; }

    WorldWarpTalkData* getWorldWarpTalkData() const { return mWorldWarpTalkData; }

    VisitStageData* getVisitStageData() const { return mVisitStageData; }

    NetworkUploadFlag* getNetworkUploadFlag() const { return mNetworkUploadFlag; }

    HintPhotoData* getHintPhotoData() const { return mHintPhotoData; }

    bool isRideSphinx() const { return mIsRideSphinx; }

    void rideSphinx() { mIsRideSphinx = true; }

    bool isRideMotorcycle() const { return mIsRideMotorcycle; }

    void rideMotorcycle() { mIsRideMotorcycle = true; }

    s32 getJumpingRopeBestCount() const { return mJumpingRopeBestCount; }

    s32 getJumpingRopeDayCount() const { return mJumpingRopeDayCount; }

    void setJumpingRopeDayCount(s32 count) { mJumpingRopeDayCount = count; }

    bool isExistRecordJumpingRope() const { return mIsExistRecordJumpingRope; }

    void existRecordJumpingRope() { mIsExistRecordJumpingRope = true; }

    bool isExistRecordInDayJumpingRope() const { return mIsExistRecordInDayJumpingRope; }

    void existRecordInDayJumpingRope() { mIsExistRecordInDayJumpingRope = true; }

    s32 getVolleyballBestCount() const { return mVolleyballBestCount; }

    s32 getVolleyballDayCount() const { return mVolleyballDayCount; }

    void setVolleyballDayCount(s32 count) { mVolleyballDayCount = count; }

    bool isExistRecordVolleyball() const { return mIsExistRecordVolleyball; }

    void existRecordVolleyball() { mIsExistRecordVolleyball = true; }

    bool isExistRecordInDayVolleyball() const { return mIsExistRecordInDayVolleyball; }

    void existRecordInDayVolleyball() { mIsExistRecordInDayVolleyball = true; }

    s32 getCurWorldId() const { return mCurWorldId; }

    void startKoopaCapture() { mIsStartKoopaCapture = true; }

    bool isStartKoopaCapture() const { return mIsStartKoopaCapture; }

    ShopTalkData* getShopTalkData() const { return mShopTalkData; }

    bool isEnableCapMessageLifeOneKidsMode() const { return mIsEnableCapMessageLifeOneKidsMode; }

    void disableCapMessageLifeOneKidsMode() { mIsEnableCapMessageLifeOneKidsMode = false; }

private:
    char filler_0[0x620];
    bool mIsHintNpcFirstTalk;
    bool mIsKinopioBrigadeNpcFirstTalk;
    char filler_622[0xe];
    TimeBalloonSaveData* mTimeBalloonSaveData;
    char filler_638[0x58];
    bool mIsStartWorldTravelingPeach;
    bool mIsPlayAlreadyWorldWarp;
    bool mIsTalkFirstAmiiboNpc;
    bool mIsUnlockAchievement;
    WorldWarpTalkData* mWorldWarpTalkData;
    VisitStageData* mVisitStageData;
    GameProgressData* mGameProgressData;
    char filler_6b0[0x20];
    NetworkUploadFlag* mNetworkUploadFlag;
    char filler_6d8[0x8];
    HintPhotoData* mHintPhotoData;
    s64 mPlayTimeTotal;
    char filler_6f0[0xe0];
    bool mIsRideSphinx;
    bool mIsRideMotorcycle;
    char filler_7d2[0x26];
    s32 mJumpingRopeBestCount;
    s32 mJumpingRopeDayCount;
    bool mIsExistRecordJumpingRope;
    bool mIsExistRecordInDayJumpingRope;
    s32 mVolleyballBestCount;
    s32 mVolleyballDayCount;
    bool mIsExistRecordVolleyball;
    bool mIsExistRecordInDayVolleyball;
    char filler_80e[0x42];
    GameDataHolder* mGameDataHolder;
    char filler_858[0x198];
    s32 mCurWorldId;
    s32 mWorldId;
    char filler_9f8[0x49];
    bool mIsStartKoopaCapture;
    char filler_a42[0x1e];
    ShopTalkData* mShopTalkData;
    char filler_a68[0xfd];
    bool mIsEnableCapMessageLifeOneKidsMode;
    char filler_b66[0x2];
};

static_assert(sizeof(GameDataFile) == 0xb68);
