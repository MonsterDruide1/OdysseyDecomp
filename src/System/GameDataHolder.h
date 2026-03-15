#pragma once

#include <container/seadPtrArray.h>
#include <math/seadVector.h>
#include <prim/seadSafeString.h>

#include "Library/HostIO/HioNode.h"
#include "Library/Message/IUseMessageSystem.h"
#include "Library/Scene/GameDataHolderBase.h"
#include "Library/Scene/ISceneObj.h"

#include "Scene/SceneObjFactory.h"

namespace al {
class PlacementId;
class PlacementInfo;
class LayoutInitInfo;
}  // namespace al

namespace ShopItem {
struct ItemInfo;
struct ShopItemInfo;
}  // namespace ShopItem

class AchievementInfoReader;
class AchievementHolder;
class CapMessageBossData;
class ChangeStageInfo;
class Coin;
class GameDataFile;
class GameConfigData;
class GameSequenceInfo;
class MapDataHolder;
class QuestInfoHolder;
class SaveDataAccessSequence;
class TemporaryScenarioCameraHolder;
class TempSaveData;
class TimeBalloonSequenceInfo;
class UniqObjInfo;
class WorldList;

struct HackObjInfo {
    const char* hackName = nullptr;
    f32 guideHeight = 200.0f;
    f32 stayGravityMargine = 0.0f;
    bool isScare = false;
    bool isNoCollisionMsg = false;
    bool isNoSeparateCameraInput = false;
    bool isUsePlayerCollision = false;
    bool isUseCollisionPartsFilterActor = false;
    bool isGuideEnable = true;
    const char* tutorialName = nullptr;
};

static_assert(sizeof(HackObjInfo) == 0x20);

class GameDataHolder : public al::GameDataHolderBase,
                       public al::ISceneObj,
                       public al::HioNode,
                       public al::IUseMessageSystem {
public:
    static constexpr s32 sSceneObjId = SceneObjID_GameDataHolder;

    struct ChangeStageItem {
        sead::FixedSafeString<128> srcStageName;
        sead::FixedSafeString<128> srcLabel;
        sead::FixedSafeString<128> destStageName;
        sead::FixedSafeString<128> destLabel;
    };

    static_assert(sizeof(ChangeStageItem) == 0x260);

    struct ExStageItem {
        sead::FixedSafeString<128> name;
    };

    static_assert(sizeof(ExStageItem) == 0x98);

    struct ShowHackTutorialInfo {
        sead::FixedSafeString<128> label;
    };

    static_assert(sizeof(ShowHackTutorialInfo) == 0x98);

    struct WorldWarpHoleInfo {
        sead::FixedSafeString<128> stageName;
        s32 worldId;
        s32 scenarioNo;
        sead::FixedSafeString<128> name;
    };

    static_assert(sizeof(WorldWarpHoleInfo) == 0x138);

    struct WorldItemTypeInfo {
        sead::FixedSafeString<128> coinCollect;
        sead::FixedSafeString<128> coinCollectEmpty;
        sead::FixedSafeString<128> coinCollect2D;
        sead::FixedSafeString<128> coinCollectEmpty2D;
        s32 shineAnimFrame;
    };

    static_assert(sizeof(WorldItemTypeInfo) == 0x268);

    struct StageLockInfo {
        s32* shineNumInfo = nullptr;
        s32 shineNumInfoNum = 0;
        bool isCountTotal = false;
        bool isCrash = false;
    };

    static_assert(sizeof(StageLockInfo) == 0x10);

    struct InvalidOpenMapInfo {
        const char* name = nullptr;
        s32 scenario = 0;
    };

    static_assert(sizeof(InvalidOpenMapInfo) == 0x10);

    GameDataHolder(const al::MessageSystem* messageSystem);
    GameDataHolder();

    ~GameDataHolder() override;

    const char* getSceneObjName() const override;
    const al::MessageSystem* getMessageSystem() const override;

    void setPlayingFileId(s32 fileId);
    void initializeData();
    void initializeDataCommon();
    void resetTempSaveData(bool isSwapTempSaveData);
    void initializeDataId(s32 fileId);
    void readByamlData(s32 fileId, const char* fileName);
    s32 tryFindEmptyFileId() const;
    void createSaveDataAccessSequence(const al::LayoutInitInfo& layoutInitInfo);
    void createSaveDataAccessSequenceDevelop(const al::LayoutInitInfo& layoutInitInfo);
    bool isRequireSave() const;
    void setRequireSave();
    void setRequireSaveFalse();
    void setRequireSaveFrame();
    void updateRequireSaveFrame();
    bool isInvalidSaveForMoonGet() const;
    void invalidateSaveForMoonGet();
    void validateSaveForMoonGet();
    void setLanguage(const char* language);
    const char* getLanguage() const;
    void changeNextStage(const ChangeStageInfo* changeStageInfo, s32 raceType = 0);
    void resetLocationName();
    void changeNextStageWithDemoWorldWarp(const char* stageName);
    bool tryChangeNextStageWithWorldWarpHole(const char* changeStageId);
    void returnPrevStage();
    const char* getNextStageName() const;
    const char* getNextStageName(s32 fileId) const;
    GameDataFile* getGameDataFile(s32 fileId) const;
    const char* getNextPlayerStartId() const;
    const char* getCurrentStageName() const;
    const char* tryGetCurrentStageName() const;
    const char* getCurrentStageName(s32 fileId) const;
    void setCheckpointId(const al::PlacementId* placementId);
    const char* tryGetRestartPointIdString() const;
    void endStage();
    void startStage(const char* stageName, s32 scenarioNo);
    void onObjNoWriteSaveData(const al::PlacementId* placementId);
    void offObjNoWriteSaveData(const al::PlacementId* placementId);
    bool isOnObjNoWriteSaveData(const al::PlacementId* placementId) const;
    void onObjNoWriteSaveDataResetMiniGame(const al::PlacementId* placementId);
    void offObjNoWriteSaveDataResetMiniGame(const al::PlacementId* placementId);
    bool isOnObjNoWriteSaveDataResetMiniGame(const al::PlacementId* placementId) const;
    void onObjNoWriteSaveDataInSameScenario(const al::PlacementId* placementId);
    bool isOnObjNoWriteSaveDataInSameScenario(const al::PlacementId* placementId) const;
    void writeTempSaveDataToHash(const char* hashName, bool value);
    bool findValueFromTempSaveDataHash(const char* hashName);
    void resetMiniGameData();
    s32 getPlayingFileId() const;
    s32 getPlayingOrNextFileId() const;
    void requestSetPlayingFileId(s32 fileId);
    void receiveSetPlayingFileIdMsg();
    GameDataFile* findGameDataFile(const char* fileName) const;
    GameDataFile* findFileByName(const char* fileName) const;
    void resetScenarioStartCamera();
    void resetTempSaveDataInSameScenario();
    void readFromSaveDataBuffer(const char* fileName);
    bool tryReadByamlDataCommon(const u8* byamlData);
    void readFromSaveDataBufferCommonFileOnlyLanguage();
    void writeToSaveDataBuffer(const char* fileName);
    void updateSaveInfoForDisp(const char* fileName);
    void updateSaveTimeForDisp(const char* fileName);
    s32 findUnlockShineNum(bool* isCountTotal, s32 worldId) const;
    s32 calcBeforePhaseWorldNumMax(s32 worldId) const;
    bool isFindKoopaNext(s32 worldId) const;
    bool isBossAttackedHomeNext(s32 worldId) const;
    void playScenarioStartCamera(s32 questNo);
    bool isPlayAlreadyScenarioStartCamera(s32 questNo) const;
    const sead::PtrArray<ShopItem::ShopItemInfo>& getShopItemInfoList() const;
    bool checkNeedTreasureMessageStage(const char* stageName) const;
    bool tryFindLinkDestStageInfo(const char** destStageName, const char** destLabel,
                                  const char* srcStageName, const char* srcLabel) const;
    bool isShowHackTutorial(const char* hackName, const char* suffix) const;
    void setShowHackTutorial(const char* hackName, const char* suffix);
    bool isShowBindTutorial(const char* bindName) const;
    const char* getCoinCollectArchiveName(s32 worldId) const;
    const char* getCoinCollectEmptyArchiveName(s32 worldId) const;
    const char* getCoinCollect2DArchiveName(s32 worldId) const;
    const char* getCoinCollect2DEmptyArchiveName(s32 worldId) const;
    s32 getShineAnimFrame(s32 worldId) const;
    s32 getCoinCollectNumMax(s32 worldId) const;
    bool isInvalidOpenMapStage(const char* stageName, s32 scenarioNo) const;
    void setShowBindTutorial(const char* bindName);
    s32 tryCalcWorldWarpHoleSrcId(s32 destId) const;
    s32 calcWorldWarpHoleDestId(s32 srcId) const;
    s32 calcWorldWarpHoleIdFromWorldId(s32 worldId) const;
    s32 calcWorldIdFromWorldWarpHoleId(s32 worldWarpHoleId) const;
    void calcWorldWarpHoleLabelAndStageName(sead::BufferedSafeString* label,
                                            sead::BufferedSafeString* stageName,
                                            const char* srcLabel, s32 worldId) const;
    // TODO: parameter name
    const WorldWarpHoleInfo* findWorldWarpHoleInfo(s32 worldId, s32, const char* label) const;
    bool checkIsOpenWorldWarpHoleInScenario(s32 worldId, s32 scenarioNo) const;
    void setLocationName(const al::PlacementInfo* placementInfo);
    bool isPrevLocation(const al::PlacementInfo* placementInfo) const;
    void setCoinTransForDeadPlayer(const sead::PtrArray<Coin>& coins, s32 coinNum);
    const sead::Vector3f& getCoinTransForDeadPlayer(s32 coinIdx);
    void setSeparatePlay(bool isSeparatePlay);
    CapMessageBossData* getCapMessageBossData() const;
    s32 findUseScenarioNo(const char* stageName) const;

    GameDataFile* getGameDataFile() const { return mPlayingFile; }

    s64 getPlayTimeAcrossFile() const { return mPlayTimeAcrossFile; }

    TemporaryScenarioCameraHolder* getTemporaryScenarioCameraHolder() const {
        return mTemporaryScenarioCameraHolder;
    }

    void resetTemporaryScenarioCameraHolder() { mTemporaryScenarioCameraHolder = nullptr; }

    const sead::PtrArray<ShopItem::ItemInfo>& getClothList() const { return mItemCloth; }

    const sead::PtrArray<ShopItem::ItemInfo>& getCapList() const { return mItemCap; }

    const sead::PtrArray<ShopItem::ItemInfo>& getGiftList() const { return mItemGift; }

    const sead::PtrArray<ShopItem::ItemInfo>& getStickerList() const { return mItemSticker; }

    const sead::PtrArray<HackObjInfo>& getHackObjList() const { return mHackObjList; }

    const sead::PtrArray<sead::FixedSafeString<64>>& getWorldsForNewReleaseShop() const {
        return mWorldsForNewReleaseShop;
    }

    AchievementInfoReader* getAchievementInfoReader() const { return mAchievementInfoReader; }

    WorldList* getWorldList() const { return mWorldList; }

    MapDataHolder* getMapDataHolder() const { return mMapDataHolder; }

    bool isEnableExplainAmiibo() const { return !mIsDisableExplainAmiibo; }

    void enableExplainAmiibo() { mIsDisableExplainAmiibo = false; }

    void disableExplainAmiibo() { mIsDisableExplainAmiibo = true; }

    void startSearchHintByAmiibo() { mSearchHintByAmiiboCount++; }

    void endSearchHintByAmiibo() { mSearchHintByAmiiboCount--; }

    void setIsStageEnding() { mIsStageEnding = true; }

    bool isEnableCheckpointWarp() const { return mIsValidCheckpointWarp; }

    void validateCheckpointWarp() { mIsValidCheckpointWarp = true; }

    void invalidateCheckpointWarp() { mIsValidCheckpointWarp = false; }

    const sead::Vector3f& getStageMapPlayerPos() const { return mStageMapPlayerPos; }

    void setStageMapPlayerPos(const sead::Vector3f& pos) { mStageMapPlayerPos = pos; }

    void resetDeadPlayerCoinIdx() { mDeadPlayerCoinIdx = 0; }

    bool isSeparatePlay() const { return mIsSeparatePlay; }

    bool isPlayDemoLavaErupt() const { return mIsPlayDemoLavaErupt; }

    bool isExistKoopaShip() const { return mIsExistKoopaShip; }

    void setExistKoopaShip(bool isExistKoopaShip) { mIsExistKoopaShip = isExistKoopaShip; }

    GameSequenceInfo* getSequenceInfo() const { return mSequenceInfo; }

    GameConfigData* getGameConfigData() const { return mGameConfigData; }

private:
    const al::MessageSystem* mMessageSystem = nullptr;
    GameDataFile** mFiles = nullptr;
    GameDataFile* mPlayingFile = nullptr;
    GameDataFile* mNextFile = nullptr;
    s32 mPlayingFileId = 0;
    SaveDataAccessSequence* mSaveDataAccessSequence = nullptr;
    bool mIsRequireSave = false;
    u32 mRequireSaveFrame = 0;
    bool mIsInvalidSaveForMoonGet = false;
    bool mIsStageChanging = false;  // Similar to mutex lock
    bool mIsStageEnding = false;    // Similar to mutex lock
    sead::FixedSafeString<32> mLanguage;

    u64 mPlayTimeAcrossFile = 0;
    sead::Heap* mSaveDataWriteHeap = nullptr;
    u8* mSaveDataWorkBuffer = nullptr;
    GameConfigData* mGameConfigData = nullptr;
    TempSaveData* mTempSaveData = nullptr;
    TempSaveData* mTempSaveDataBackup = nullptr;
    CapMessageBossData* mCapMessageBossData = nullptr;
    sead::Vector3f _c0 = sead::Vector3f::zero;

    TemporaryScenarioCameraHolder* mTemporaryScenarioCameraHolder = nullptr;
    bool* mIsPlayAlreadyScenarioStartCamera = nullptr;
    sead::PtrArray<StageLockInfo> mStageLockList;
    sead::PtrArray<ShopItem::ShopItemInfo> mShopItemList;
    sead::PtrArray<ShopItem::ShopItemInfo> mShopItemListE3;
    sead::PtrArray<ShopItem::ItemInfo> mItemCloth;
    sead::PtrArray<ShopItem::ItemInfo> mItemCap;
    sead::PtrArray<ShopItem::ItemInfo> mItemGift;
    sead::PtrArray<ShopItem::ItemInfo> mItemSticker;
    sead::PtrArray<HackObjInfo> mHackObjList;
    sead::PtrArray<sead::FixedSafeString<64>> mWorldsForNewReleaseShop;
    s32* mShopTalkDataInfos = nullptr;
    s32 mShopTalkDataSize = 0;
    AchievementInfoReader* mAchievementInfoReader = nullptr;
    AchievementHolder* mAchievementHolder = nullptr;
    WorldList* mWorldList = nullptr;
    sead::PtrArray<ChangeStageItem> mChangeStageList;
    sead::PtrArray<ExStageItem> mExStageList;
    sead::PtrArray<InvalidOpenMapInfo> mInvalidOpenMapList;
    sead::PtrArray<sead::FixedSafeString<128>> mShowHackTutorialList;
    bool* mIsShowBindTutorial = nullptr;
    MapDataHolder* mMapDataHolder = nullptr;
    sead::PtrArray<WorldItemTypeInfo> mWorldItemTypeInfo;
    s32* mCoinCollectNumMax = nullptr;
    s32* mWorldWarpHoleDestIds = nullptr;
    WorldWarpHoleInfo* mWorldWarpHoleInfos = nullptr;
    s32 mWorldWarpHoleInfoNum = 0;
    UniqObjInfo* mLocationName = nullptr;
    bool mIsDisableExplainAmiibo = false;
    s32 mSearchHintByAmiiboCount = 0;
    bool mIsValidCheckpointWarp = true;
    sead::Vector3f mStageMapPlayerPos = sead::Vector3f::zero;
    sead::Vector3f* mCoinTransForDeadPlayer = nullptr;
    s32 mDeadPlayerCoinIdx = 0;
    bool _244 = false;  // Part of E3Sequence
    bool mIsSeparatePlay = false;
    bool mIsPlayDemoLavaErupt = false;
    QuestInfoHolder* mQuestInfoHolder = nullptr;
    bool mIsExistKoopaShip = false;
    GameSequenceInfo* mSequenceInfo = nullptr;
    TimeBalloonSequenceInfo* mTimeBalloonSequenceInfo = nullptr;
};

static_assert(sizeof(GameDataHolder) == 0x268);
