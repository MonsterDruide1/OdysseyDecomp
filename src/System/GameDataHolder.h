/**
 * @file GameDataHolder.h
 * @brief Holds scenario / game data.
 */

#pragma once

#include <prim/seadSafeString.h>
#include "Library/Message/MessageSystem.h"
#include "Library/Scene/GameDataHolderBase.h"
#include "System/GameDataFile.h"
#include "System/WorldList.h"

namespace al {
class PlacementId;
}

class GameDataHolder : public al::GameDataHolderBase {
public:
    GameDataHolder(al::MessageSystem const*);
    GameDataHolder();

    virtual ~GameDataHolder();

    virtual char* getSceneObjName() const;
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

    void changeNextStage(const struct ChangeStageInfo*, s32);

    s32 findUseScenarioNo(const char*);

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
    bool mChangeStageRelated;
    u8 field_4A;
    u8 field_4B;
    u32 field_4C;
    sead::BufferedSafeString mLanguage;
    u8 gap_58[0x28];
    sead::Heap* field_90;
    u8 gap_98[0x20];
    u64* field_B8;  // TempSaveData*
    u8 gap_C0[0xD0];
    WorldList* mWorldList;
};
