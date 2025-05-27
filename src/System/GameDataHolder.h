#pragma once

#include <container/seadPtrArray.h>
#include <math/seadVector.h>
#include <prim/seadSafeString.h>

#include "Library/Scene/GameDataHolderBase.h"

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
        s32* shineNumInfo;
        s32 shineNumInfoNum;
        bool isCountTotal;
        bool isCrash;
    };

    static_assert(sizeof(StageLockInfo) == 0x10);

    struct HackObjInfo {
        const char* hackName;
        f32 guideHeight;
        f32 stayGravityMargine;
        bool isScare;
        bool isNoCollisionMsg;
        bool isNoSeparateCameraInput;
        bool isUsePlayerCollision;
        bool isUseCollisionPartsFilterActor;
        bool isGuideEnable;
        const char* tutorialName;
    };

    static_assert(sizeof(HackObjInfo) == 0x20);

    struct InvalidOpenMapInfo {
        const char* name;
        s32 scenario;
    };

    static_assert(sizeof(InvalidOpenMapInfo) == 0x10);

    GameDataHolder(const al::MessageSystem* messageSystem);
    GameDataHolder();

    ~GameDataHolder() override = default;

    const char* getSceneObjName() const override;
    al::MessageSystem* getMessageSystem() const override;

    void setPlayingFileId(s32 fileId);
    void initializeData();
    void initializeDataCommon();
    void resetTempSaveData(bool isSwap);
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
    void writeToSaveBuffer(const char* fileName);
    void updateSaveInfoForDisp(const char* fileName);
    void updateSaveTimeForDisp(const char* fileName);
    s32 findUnlockShineNum(bool* isCountTotal, s32 worldId) const;
    s32 calcBeforePhaseWorldNumMax(s32 worldId) const;
    bool isFindKoopaNext(s32 worldId) const;
    bool isBossAttackedHomeNext(s32 worldId) const;
    void playScenarioStartCamera(s32 questNo);
    bool isPlayAlreadyScenarioStartCamera(s32 questNo) const;
    const sead::PtrArray<ShopItem::ItemInfo>& getShopItemInfoList() const;
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

    const sead::PtrArray<ShopItem::ItemInfo>& getClothList() const { return mItemCloth; }

    const sead::PtrArray<ShopItem::ItemInfo>& getCapList() const { return mItemCap; }

    const sead::PtrArray<ShopItem::ItemInfo>& getGiftList() const { return mItemGift; }

    const sead::PtrArray<ShopItem::ItemInfo>& getStickerList() const { return mItemSticker; }

    WorldList* getWorldList() const { return mWorldList; }

    bool isExistKoopaShip() const { return mIsExistKoopaShip; }

    void setExistKoopaShip(bool isExistKoopaShip) { mIsExistKoopaShip = isExistKoopaShip; }

    GameSequenceInfo* getSequenceInfo() const { return mSequenceInfo; }

    GameConfigData* getGameConfigData() const { return mGameConfigData; }

private:
    al::MessageSystem* mMessageSystem;
    GameDataFile** mFiles;
    GameDataFile* mPlayingFile;
    GameDataFile* mNextFile;
    s32 mPlayingFileId;
    SaveDataAccessSequence* mSaveDataAccessSequence;
    bool mIsRequireSave;
    u32 mRequireSaveFrame;
    bool mIsInvalidSaveForMoonGet;
    bool _49;  // related to changeNextStage(WithWorldDemoWarp)
    bool _4a;  // related to endStage
    sead::FixedSafeString<32> mLanguage;
    u64 mPlayTimeAcrossFiles;
    sead::Heap* mSaveDataWriteThread;
    const u8* mSaveDataWorkBuffer;
    GameConfigData* mGameConfigData;
    TempSaveData* mTempSaveData;
    TempSaveData* mTempSaveDataBackup;
    CapMessageBossData* mCapMessageBossData;
    void* _c0;
    void* _c8;
    TemporaryScenarioCameraHolder* mTemporaryScenarioCameraHolder;
    bool* mIsPlayAlreadyScenarioStartCamera;
    sead::PtrArray<StageLockInfo> mStageLockList;
    sead::PtrArray<ShopItem::ItemInfo> mShopItemList;
    sead::PtrArray<ShopItem::ItemInfo> mShopItemListE3;
    sead::PtrArray<ShopItem::ItemInfo> mItemCloth;
    sead::PtrArray<ShopItem::ItemInfo> mItemCap;
    sead::PtrArray<ShopItem::ItemInfo> mItemGift;
    sead::PtrArray<ShopItem::ItemInfo> mItemSticker;
    sead::PtrArray<HackObjInfo> mHackObjList;
    sead::PtrArray<sead::FixedSafeString<64>> _160;
    void* _170;
    s32 _178;
    AchievementInfoReader* mAchievementInfoReader;
    AchievementHolder* mAchievementHolder;
    WorldList* mWorldList;
    sead::PtrArray<ChangeStageItem> mChangeStageList;
    sead::PtrArray<sead::FixedSafeString<128>> mExStageList;
    sead::PtrArray<InvalidOpenMapInfo> mInvalidOpenMapList;
    sead::PtrArray<sead::FixedSafeString<128>> mShowHackTutorialList;
    bool* mIsShowBindTutorial;
    MapDataHolder* mMapDataHolder;
    sead::PtrArray<WorldItemTypeInfo> mWorldItemTypeInfo;
    s32* mCoinCollectNumMax;
    s32* mWorldWarpHoleDestIds;
    WorldWarpHoleInfo* mWorldWarpHoleInfos;
    s32 mWorldWarpHoleInfoNum;
    UniqObjInfo* mLocationName;
    bool _220;
    s32 _224;
    bool mIsValidCheckpointWarp;
    sead::Vector3f mStageMapPlayerPos;
    sead::Vector3f* mCoinTransForDeadPlayer;
    s32 mDeadPlayerCoinIdx;
    bool _244;
    bool mIsSeparatePlay;
    bool mIsPlayDemoLavaErupt;
    QuestInfoHolder* mQuestInfoHolder;
    bool mIsExistKoopaShip;
    GameSequenceInfo* mSequenceInfo;
    TimeBalloonSequenceInfo* mTimeBalloonSequenceInfo;
};

static_assert(sizeof(GameDataHolder) == 0x268);
