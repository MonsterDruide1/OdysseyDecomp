#pragma once

#include <prim/seadSafeString.h>

#include "Library/Message/MessageSystem.h"
#include "Library/Scene/GameDataHolderBase.h"

#include "System/WorldList.h"

namespace al {
class PlacementId;
}

class GameDataFile;
class ChangeStageInfo;

struct ItemAmiiboInfo {
    s32 mCharacterId;
    s32 mNumberingId;
};

struct ItemListInfo {
    s32 mIndex;
    char mName[0x80];
    s32 mType;
    u32 mAmiiboCount;
    ItemAmiiboInfo* mAmiiboInfo;
    bool mIsAOC;
};

class GameDataHolder : public al::GameDataHolderBase {
public:
    GameDataHolder(const al::MessageSystem*);
    GameDataHolder();

    virtual ~GameDataHolder();

    virtual const char* getSceneObjName() const;
    virtual al::MessageSystem* getMessageSystem() const;

    void setPlayingFileId(s32 file);
    void intitalizeData();
    void initialzeDataCommon();
    void resetTempSaveData(bool);
    void initializeDataId(s32);
    void readByamlData(s32, const char*);
    s32 tryFindEmptyFileId() const;

    bool isRequireSave() const;
    void setRequireSave();
    void setRequireSaveFalse();
    void setRequireSaveFrame();
    void updateRequireSaveFrame();
    bool isInvalidSaveForMoonGet() const;
    void invalidateSaveForMoonGet();
    void validateSaveForMoonGet();
    void setLanguage(const char*);
    char* getLanguage() const;

    void resetLocationName();
    void changeNextStageWithDemoWorldWarp(const char*);
    bool tryChangeNextStageWithWorldWarpHole(const char*);
    void returnPrevStage();
    char* getNextStageName() const;
    char* getNextStageName(s32 idx) const;
    GameDataFile* getGameDataFile(s32 idx) const;
    u64 getNextPlayerStartId() const;
    char* getCurrentStageName() const;
    char* tryGetCurrentStageName() const;
    char* getCurrentStageName(s32 idx) const;
    void setCheckpointId(const al::PlacementId*);
    char* tryGetRestartPointIdString() const;
    void endStage();
    void startStage(const char*, s32);
    void onObjNoWriteSaveData(const al::PlacementId*);
    void offObjNoWriteSaveData(const al::PlacementId*);
    bool isOnObjNoWriteSaveData(const al::PlacementId*) const;
    void onObjNoWriteSaveDataResetMiniGame(const al::PlacementId);
    void offObjNoWriteSaveDataResetMiniGame(const al::PlacementId);
    bool isOnObjNoWriteSaveDataResetMiniGame(const al::PlacementId) const;
    void onObjNoWriteSaveDataInSameScenario(const al::PlacementId);
    bool isOnObjNoWriteSaveDataInSameScenario(const al::PlacementId) const;
    void writeTempSaveDataToHash(const char*, bool);

    void resetMiniGameData();
    s32 getPlayingFileId() const;
    void requestSetPlayingFileId(s32);

    s32 findUnlockShineNum(bool*, s32) const;
    s32 calcBeforePhaseWorldNumMax(s32) const;
    bool isFindKoopaNext(s32) const;
    bool isBossAttackedHomeNext(s32) const;
    void playScenarioStartCamera(s32);
    bool isPlayAlreadyScenarioStartCamera() const;

    s32 getShineAnimFrame(s32) const;
    s32 getCoinCollectNumMax(s32) const;

    void readFromSaveDataBufferCommonFileOnlyLanguage();
    void readFromSaveDataBuffer(const char* bufferName);

    void changeNextStage(const ChangeStageInfo*, s32);

    s32 findUseScenarioNo(const char*);

    const sead::PtrArray<ItemListInfo>& getClothList() const { return mItemCloth; }

    const sead::PtrArray<ItemListInfo>& getCapList() const { return mItemCap; }

    const sead::PtrArray<ItemListInfo>& getGiftList() const { return mItemGift; }

    const sead::PtrArray<ItemListInfo>& getStickerList() const { return mItemSticker; }

private:
    s32 padding;
    GameDataFile** mDataFileArr;
    GameDataFile* mGameDataFile;
    u64 field_28;
    u64 field_30;
    u64* field_38;  // SaveDataAccessSequence*
    u32 field_40;
    u32 mRequireSaveFrame;
    bool mIsInvalidSaveForMoonGet;
    bool unk_changeStageRelated;
    u8 field_4a;
    u8 field_4b;
    u32 field_4c;
    sead::BufferedSafeString mLanguage;
    u8 gap_58[0x28];
    sead::Heap* field_90;
    u8 gap_98[0x20];
    u64* field_b8;  // TempSaveData*
    u8 gap_C0[0x110 - 0xc0];
    sead::PtrArray<ItemListInfo> mItemCloth;
    sead::PtrArray<ItemListInfo> mItemCap;
    sead::PtrArray<ItemListInfo> mItemGift;
    sead::PtrArray<ItemListInfo> mItemSticker;
    u8 gap_150[0x190 - 0x150];
    WorldList* mWorldList;
    u8 gap_198[0x268 - 0x198];
};

static_assert(sizeof(GameDataHolder) == 0x268);
