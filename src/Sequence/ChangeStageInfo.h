#pragma once

#include <prim/seadSafeString.h>

#include "Library/Placement/PlacementInfo.h"

#include "System/GameDataHolder.h"

class GameDataHolder;

class ChangeStageInfo {
public:
    // Based on usages to rs::createChangeStageInfo and ChangeStageInfo member functions
    enum SubScenarioType : u32 {
        NO_SUB_SCENARIO,
        DEFAULT_SUB_SCENARIO,
        RESET_MINI_GAME1,
        SNOW_MAN_RACE,
        RESET_MINI_GAME2
    };

    ChangeStageInfo(const GameDataHolder* holder, const al::PlacementInfo& info);
    ChangeStageInfo(const GameDataHolder* holder, const al::PlacementInfo& info,
                    const char* entranceName, const char* stageName, bool, s32 scenario,
                    ChangeStageInfo::SubScenarioType);
    ChangeStageInfo(const GameDataHolder*, const char* entranceName, const char* stageName, bool,
                    s32 scenario, SubScenarioType subScenarioType);
    void copy(const ChangeStageInfo& other);
    void findScenarioNoByList(const GameDataHolder*);  // Doesn't return anything
    void init();
    void init(const al::PlacementInfo& info, const GameDataHolder* holder);
    bool isSubScenarioTypeLifeRecover() const;
    bool isSubScenarioTypeResetMiniGame() const;
    void setWipeType(const char* wipeType);

private:
    sead::FixedSafeString<0x80> mChangeStageId;
    sead::FixedSafeString<0x80> mChangeStageName;
    sead::FixedSafeString<0x80> mPlacementString;
    bool mIsReturn;
    s32 mScenarioNo;
    SubScenarioType mSubScenarioType;
    sead::FixedSafeString<0x80> mWipeType;
    s32 mHintPriority;
};

static_assert(sizeof(ChangeStageInfo) == 0x278, "ChangeStageInfo size");
