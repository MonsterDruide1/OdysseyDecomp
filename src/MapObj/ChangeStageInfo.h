#pragma once

#include <math/seadVector.h>
#include <prim/seadSafeString.h>

namespace al {
class LiveActor;
class PlacementInfo;
}  // namespace al

class GameDataHolder;

class ChangeStageInfo {
public:
    enum SubScenarioType : u32 {
        NO_SUB_SCENARIO,
        DEFAULT_SUB_SCENARIO,
        RESET_MINI_GAME1,
        SNOW_MAN_RACE,
        RESET_MINI_GAME2
    };

    ChangeStageInfo(const GameDataHolder* holder, const al::PlacementInfo& info);
    ChangeStageInfo(const GameDataHolder* holder, const al::PlacementInfo& info,
                    const char* entranceName, const char* stageName, bool isReturn = false,
                    s32 scenario = -1, SubScenarioType = SubScenarioType::NO_SUB_SCENARIO);
    ChangeStageInfo(const GameDataHolder* holder, const char* entranceName, const char* stageName,
                    bool isReturn = false, s32 scenario = -1,
                    SubScenarioType subScenarioType = SubScenarioType::NO_SUB_SCENARIO);

    void init();
    void init(const al::PlacementInfo& info, const GameDataHolder* holder);
    void copy(const ChangeStageInfo& other);
    void findScenarioNoByList(const GameDataHolder* holder);
    bool isSubScenarioTypeLifeRecover() const;
    bool isSubScenarioTypeResetMiniGame() const;
    void setWipeType(const char* wipeType);

    bool isEmpty() const { return mChangeStageName.isEmpty(); }

    const char* getChangeStageId() const { return mChangeStageId.cstr(); }

    const char* getStageName() const { return mChangeStageName.cstr(); }

    const char* getPlacementString() const { return mPlacementString.cstr(); }

    bool isReturn() const { return mIsReturn; }

    s32 getScenarioNo() const { return mScenarioNo; }

    SubScenarioType getSubScenarioType() const { return mSubScenarioType; }

private:
    void calcTrans(sead::Vector3f* out, const al::PlacementInfo& info);

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

namespace rs {
ChangeStageInfo* createChangeStageInfo(const al::LiveActor* actor, const al::PlacementInfo& info);
ChangeStageInfo* createChangeStageInfo(
    const al::LiveActor* actor, const al::PlacementInfo& info, const char* entranceName,
    const char* stageName, bool isReturn = false, s32 scenario = -1,
    ChangeStageInfo::SubScenarioType subScenarioType = ChangeStageInfo::NO_SUB_SCENARIO);
ChangeStageInfo* createChangeStageInfo(
    const al::LiveActor* actor, const char* entranceName, const char* stageName,
    bool isReturn = false, s32 scenario = -1,
    ChangeStageInfo::SubScenarioType subScenarioType = ChangeStageInfo::NO_SUB_SCENARIO);
}  // namespace rs
