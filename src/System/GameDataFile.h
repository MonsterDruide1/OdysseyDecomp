#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>
#include <prim/seadBitFlag.h>
#include <prim/seadSafeString.h>
#include <stream/seadStream.h>
#include <time/seadDateTime.h>

#include "System/UniqObjInfo.h"

constexpr s32 sNumWorlds = 20;

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

class ChangeStageInfo;
class CollectBgm;
class GameDataHolder;
class GameProgressData;
class HintPhotoData;
struct ItemInfo;
class NetworkUploadFlag;
class PlayerHitPointData;
class QuestInfo;
class RaceRecord;
class SessionMusicianType;
struct ShineInfo;
class ShopTalkData;
class SphinxQuizData;
class TimeBalloonSaveData;
class UniqObjInfo;
class VisitStageData;
class WorldList;
class WorldWarpTalkData;

class GameDataFile {
public:
    struct SaveObjS32 {
        UniqObjInfo objInfo;
        s32 value;
    };

    static_assert(sizeof(GameDataFile::SaveObjS32) == 0x138);

    enum class AchievementStatus { None, Unlocked, Got };

    struct HintInfo {
        enum class HintStatus { None, Npc, Amiibo };

        void clear();

        bool isDisableByWorldWarpHole(bool condition) const;
        bool isEnableUnlock(s32 curWorldId, bool isGameClear, s32 scenarioNo, bool isInWorld) const;
        bool isHintStatusUnlock(s32 curWorldId, s32 scenarioNo, bool isInWorld) const;
        bool isHintStatusUnlockByNpc() const;
        bool isHintStatusUnlockByAmiibo() const;
        bool isEnableNameUnlockByScenario(s32 curWorldId, s32 scenarioNo, bool isInWorld) const;

        bool testFunc(s32 curWorldId, bool isGameClear, s32 scenarioNo, bool isInWorld) const;

        sead::FixedSafeString<0x80> stageName;
        sead::FixedSafeString<0x80> objId;
        sead::FixedSafeString<0x40> scenarioName;
        const char* objectName;
        sead::Vector3f trans;
        sead::Vector3f transAgain;
        void* unkPtr1;
        void* unkPtr2;
        void* unkPtr3;
        void* unkPtr4;
        s32 mainScenarioNo;
        s32 worldIndex;
        bool isMoonRock;
        bool unkBool1;
        bool isAchievement;
        bool isGrand;
        bool isShopMoon;
        s32 unkInt;
        HintStatus hintStatus;
        AchievementStatus achievementStatus;
        s32 unkInt4;
        void* unkPtr6;
        s32 uniqueID;
        s32 hintIdx;
        sead::FixedSafeString<0x20> optionalID;
        sead::BitFlag32 progressBitflag;
        bool isDisabled;
        bool unkBool3;
    };

    struct CoinCollectInfo;

    template <typename T, s32 Size>
    class FixedHeapArray {
    public:
        void alloc() { mPtr = new T[Size]; }

        s32 size() const { return Size; }

        T& operator[](s32 index) { return mPtr[index]; }

        const T& operator[](s32 index) const { return mPtr[index]; }

        T* begin() const { return mPtr; }

        T* end() const { return mPtr + Size; }

    private:
        T* mPtr = nullptr;
    };

    enum class CountType : s32 {
        Value_0 = 0,
        Value_1 = 1,
        Value_2 = 2,
    };

    GameDataFile(GameDataHolder*);
    void initializeData();
    bool tryReadByamlData(const u8*);
    CoinCollectInfo* tryFindCoinCollectInfo(const u8*, const u8*) const;
    s32 tryFindShineIndexByUniqueId(s32) const;
    s32 tryFindCoinCollectIndexByUniqueId(s32) const;
    void buyDefaultItem();
    void unlockAchievementShineName();
    bool isKidsMode() const;
    void updateWorldMapIndex();
    void updateWorldWarpIndex();
    s32 getScenarioNo(s32 worldId) const;
    s32 getMainScenarioNo(s32 worldId) const;
    bool isEmpty() const;
    void initializeCheckpointTable();
    void generateSaveDataIdForPrepo();
    void resetMapIcon();
    void wearDefault();
    void initializeHintList();
    void initializeCoinCollectList();
    void resetTempData();
    bool isGameClear() const;
    bool isUnlockedWorld(s32 worldId) const;
    bool isAlreadyGoWorld(s32 worldId) const;
    void addPlayTime(s32, const al::IUseSceneObjHolder*);
    s64 getPlayTimeTotal() const;
    void updateSaveTime();
    void updateSaveTimeForDisp();
    void updateSaveInfoForDisp();
    sead::DateTime getLastUpdateTime() const;
    void generateSaveDataIdForPrepoForWrite();
    void resetSaveDataIdForPrepoForWrite();
    void startStage(const char*, s32);
    bool isRaceStart() const;
    bool checkIsHomeStage(const char* stageName) const;
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
    bool isUseMissRestartInfo(const al::PlacementInfo&) const;
    bool isFirstTimeNextWorld() const;
    void getAchievement(const char*);
    bool isGotShine(const ShineInfo*) const;
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
    s32 getPayShineNum(s32 worldId = -1) const;
    s32 getTotalPayShineNum() const;
    bool isPayShineAllInAllWorld() const;
    void addKey(s32);
    s32 getKeyNum() const;
    void addOpenDoorLockNum(s32);
    void setStartedObj(const al::PlacementId*);
    bool isStartedObj(const al::PlacementId*, const char*) const;
    bool isStartedObj(const char* stageName, const char* objId) const;
    void setSaveObjS32(const al::PlacementId*, s32);
    const SaveObjS32* tryFindSaveObjS32(const al::PlacementId* placementId);
    bool isExistSessionMember(const SessionMusicianType&) const;
    void addSessionMember(const SessionMusicianType&) const;
    void addCoinCollect(const al::PlacementId*);
    bool isGotCoinCollect(const al::PlacementId*) const;
    s32 getCoinCollectNum() const;
    s32 getCoinCollectGotNum() const;
    s32 getCoinCollectGotNum(s32) const;
    void useCoinCollect(s32 purplesUsed);
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
    void saveWorldTravelingStatus();
    const char* getWorldTravelingStatus() const;
    bool isStartWorldTravelingPeach() const;
    void startWorldTravelingPeach();
    void setGrowFlowerTime(const al::PlacementId*, const al::PlacementId*, u64 time);
    void setGrowFlowerTime(const al::PlacementId*, u64 time);
    u64 getGrowFlowerTime(const al::PlacementId*) const;
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
    bool tryReadByamlDataFromStream(sead::ReadStream*, u8*, s32);
    void writeToStream(sead::WriteStream*, sead::Heap*) const;
    bool tryWriteByByaml(sead::WriteStream*, sead::Heap*) const;
    bool isPlayDemoPlayerDownForBattleKoopaAfter() const;
    s32 getCheckpointNumMaxInWorld() const;
    const sead::Vector3f& getCheckpointTransInWorld(const char*) const;
    bool isGotCheckpointInWorld(s32) const;
    s32 calcCheckpointIndexInScenario(s32) const;
    const char* getCheckpointObjIdInWorld(s32) const;
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
    const sead::Vector3f& calcHintTrans(s32) const;
    HintInfo* findHint(s32) const;
    const sead::Vector3f& calcHintTransMostEasy() const;
    HintInfo* findHintInfoMostEasy() const;
    s32 calcHintMoonRockNum() const;
    const sead::Vector3f& calcHintMoonRockTrans(s32) const;
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
    const sead::FixedSafeString<64>* tryFindItemList(ItemInfo*) const;
    s32 calcHaveClothNum() const;
    s32 calcHaveCapNum() const;
    s32 calcHaveStickerNum() const;
    s32 calcHaveGiftNum() const;
    bool isBuyItem(const ShopItem::ItemInfo*) const;
    bool isBuyItem(const char*, const sead::FixedSafeString<64>*) const;
    void buyItemAll();
    void wearCostume(const char* name);
    void wearCap(const char* name);
    void addHackDictionary(const char*);
    bool isExistInHackDictionary(const char*) const;
    HintInfo* findShine(s32, s32) const;
    bool isGotShine(s32, s32) const;
    bool isOpenShineName(s32, s32) const;
    s32 calcShineNumInOneShine(s32, s32) const;
    bool checkAchievementShine(s32, s32) const;
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
    void setShopNpcTrans(const sead::Vector3f&, const char*, s32);
    void setMoonRockTrans(const sead::Vector3f&);
    void setMiniGameInfo(const sead::Vector3f&, const char*);
    s32 calcMiniGameNum() const;
    s32 getMiniGameNumMax() const;
    const sead::Vector3f& getMiniGameTrans(s32) const;
    const char* getMiniGameName(s32) const;
    bool isExistTimeBalloonNpc() const;
    const sead::Vector3f& getTimeBalloonNpcTrans() const;
    bool isExistPoetter() const;
    const sead::Vector3f& getPoetterTrans() const;
    bool isAlreadyShowExplainCheckpointFlag() const;
    void showExplainCheckpointFlag();
    const sead::Vector3f& getShopNpcTrans(s32) const;
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
    void setTokimekiMayorNpcFavorabilityRating(s32);
    bool isFirstNetwork() const;
    void noFirstNetwork();
    void setKidsMode(bool);
    bool isPlayScenarioCamera(const QuestInfo*) const;
    bool isNextMainShine(const QuestInfo*) const;
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
    void disableHintById(s32 shineIndex);
    void enableHintById(s32 shineIndex);
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
    const CollectBgm* getCollectBgmByIndex(s32) const;
    bool trySetCollectedBgm(const char*, const char*);
    s32 getCollectedBgmNum() const;
    s32 getCollectedBgmNumMax() const;
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

    s32 getCurrentWorldId() const { return mCurrentWorldId; }

    void startKoopaCapture() { mIsStartKoopaCapture = true; }

    bool isStartKoopaCapture() const { return mIsStartKoopaCapture; }

    ShopTalkData* getShopTalkData() const { return mShopTalkData; }

    bool isEnableCapMessageLifeOneKidsMode() const { return mIsEnableCapMessageLifeOneKidsMode; }

    void disableCapMessageLifeOneKidsMode() { mIsEnableCapMessageLifeOneKidsMode = false; }

    sead::FixedSafeString<64>* getCapName() { return mCapNamePtr; }

    sead::FixedSafeString<64>* getClothName() { return mClothNamePtr; }

    sead::FixedSafeString<64>* getStickerName() { return mStickerNamePtr; }

    sead::FixedSafeString<64>* getGiftName() { return mGiftNamePtr; }

private:
    char _0[0x18];
    FixedHeapArray<s32, sNumWorlds> mShopShineNum;
    FixedHeapArray<s32, sNumWorlds> mMainScenarioNo;
    s32 mStartShineIndex;
    char _2c[0x98];
    sead::FixedSafeString<128> mPlayerStartIdForSave;
    char _160[0x30];
    WorldList* mWorldList;
    char _198[0x60];
    sead::FixedSafeString<128> mCheckpointName;
    char _290[0x98];
    sead::FixedSafeString<128> mCurrentStageName;
    char _3c0[0xa0];
    FixedHeapArray<bool, sNumWorlds> mIsWorldWarpHoleThrough;
    sead::DateTime mSaveTimeForDisp;
    sead::DateTime mSaveTime;
    s64 mSaveDataIdForPrepo;
    s64 mSaveDataIdForPrepoForWrite;
    FixedHeapArray<UniqObjInfo, 320> mGotCheckpoint;
    char _490[0x118];
    FixedHeapArray<s32, sNumWorlds> mPayShineNum;
    bool mIsPayShineInAllWorld;
    s32 mKeyNum;
    char _5b8[0x8];
    FixedHeapArray<UniqObjInfo, 512> mStartedObj;
    FixedHeapArray<SaveObjS32, 256> mSaveObjS32;
    char _5d0[0x8];
    s32 mCoinNum;
    s32 mTotalCoinNum;
    s32 mPlayerJumpCount;
    s32 mPlayerThrowCapCount;
    FixedHeapArray<s32, sNumWorlds> mUseCoinCollectNum;
    FixedHeapArray<s32, sNumWorlds> mUnlockedWorldId;
    bool mIsPlayDemoOpening;
    bool mIsEnableCap;
    char _5fa[0xe];
    s32 mSessionEventProgress;
    bool mIsPayCoinToSphinx;
    SphinxQuizData* mSphinxQuizData;
    FixedHeapArray<bool, sNumWorlds> mIsTalkLocalLanguage;
    bool mIsHintNpcFirstTalk;
    bool mIsKinopioBrigadeNpcFirstTalk;
    s32 mCapStatusForJango;
    char _628[0x4];
    s32 mJangoCount;
    TimeBalloonSaveData* mTimeBalloonSaveData;
    sead::FixedSafeString<64> mWorldTravelingStatus;
    bool mIsStartWorldTravelingPeach;
    bool mIsPlayAlreadyWorldWarp;
    bool mIsTalkFirstAmiiboNpc;
    bool mIsUnlockAchievement;
    WorldWarpTalkData* mWorldWarpTalkData;
    VisitStageData* mVisitStageData;
    GameProgressData* mGameProgressData;
    char _6b0[0x20];
    NetworkUploadFlag* mNetworkUploadFlag;
    char _6d8[0x8];
    HintPhotoData* mHintPhotoData;
    s64 mPlayTimeTotal;
    sead::FixedSafeString<64>* mCapNamePtr;
    sead::FixedSafeString<64>* mClothNamePtr;
    sead::FixedSafeString<64>* mStickerNamePtr;
    sead::FixedSafeString<64>* mGiftNamePtr;
    sead::FixedSafeString<64> mCurrentCostumeName;
    sead::FixedSafeString<64> mCurrentCapName;
    bool mIsCostumeRandom;
    bool mIsCapRandom;
    char _7c2[0xe];
    bool mIsRideSphinx;
    bool mIsRideMotorcycle;
    FixedHeapArray<s32, sNumWorlds> mScenarioNo;
    char _7e0[0x10];
    s32 mRaceLoseCountLv1;
    s32 mRaceLoseCountLv2;
    s32 mJumpingRopeBestCount;
    s32 mJumpingRopeDayCount;
    bool mIsExistRecordJumpingRope;
    bool mIsExistRecordInDayJumpingRope;
    s32 mVolleyballBestCount;
    s32 mVolleyballDayCount;
    bool mIsExistRecordVolleyball;
    bool mIsExistRecordInDayVolleyball;
    s32 mNextWorldId;
    s32 mPrevWorldId;
    bool mIsPlayDemoPlayerDownForBattleKoopaAfter;
    char _819[0xf];
    bool mIsKidsMode;
    char _829[0x17];
    bool mIsTalkKakku;
    bool mIsTalkWorldTravelingPeach;
    s32 mTokimekiMayorNpcFavorabilityRating;
    bool mIsFirstNetwork;
    GameDataHolder* mGameDataHolder;
    char _858[0x8];
    PlayerHitPointData* mPlayerHitPointData;
    sead::BufferedSafeString mNextStageName;
    char _880[0x11c];
    FixedHeapArray<HintInfo, 1024> mHintList;
    char _9a8[0x48];
    s32 mCurrentWorldId;
    s32 mWorldId;
    char _9f8[0x38];
    ChangeStageInfo* mChangeStageInfo;
    char _a38[0x9];
    bool mIsStartKoopaCapture;
    char _a42[0x1e];
    ShopTalkData* mShopTalkData;
    char _a68[0x8];
    bool mIsShowExplainCheckpointFlag;
    char _a71[0xcf];
    FixedHeapArray<s32, sNumWorlds> mWorldMapIndex;
    s32 mWorldMapNum;
    FixedHeapArray<s32, sNumWorlds> mWorldWarpIndex;
    s32 mWorldWarpNum;
    char _b5c[0x9];
    bool mIsEnableCapMessageLifeOneKidsMode;
    char _b66[0x2];

    inline s32 getCurrentWorldIdNoDevelop() const;
};

static_assert(sizeof(GameDataFile) == 0xb68);
