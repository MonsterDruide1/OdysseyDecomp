#pragma once

#include <container/seadObjArray.h>
#include <container/seadPtrArray.h>
#include <container/seadStrTreeMap.h>
#include <math/seadVector.h>
#include <prim/seadSafeString.h>

struct StagePosInfo {
    sead::ObjArray<sead::Vector3f> posList;
    s32 mask = 0;
};

static_assert(sizeof(StagePosInfo) == 0x28);

struct ShinePosInfo {
    sead::ObjArray<sead::Vector3f> posList;
    s32 uniqueId = 0;
    s32 mask = 0;
};

static_assert(sizeof(ShinePosInfo) == 0x28);

struct StageDBEntry {
    sead::FixedSafeString<0x80> name;
    sead::FixedSafeString<0x40> category;
    s32 useScenarioNo = 0;
};

static_assert(sizeof(StageDBEntry) == 0xf8);

struct WorldListEntry {
    const char* mainStageName = nullptr;
    const char* worldDevelopName = nullptr;
    s32 worldScenarioNum = 0;
    s32 clearMainScenarioNo = 0;
    s32 afterEndingScenarioNo = 0;
    s32 moonRockScenarioNo = 0;
    s32* mainQuestIndexes = 0;
    sead::PtrArray<StageDBEntry> stageList;
};

static_assert(sizeof(WorldListEntry) == 0x38);

class WorldList {
public:
    WorldList();

    s32 getWorldNum() const;
    s32 getMainQuestMin(s32 worldId, s32 questId) const;
    const char* getMainStageName(s32 worldId) const;
    s32 tryFindWorldIndexByMainStageName(const char* mainStageName) const;
    s32 tryFindWorldIndexByStageName(const char* stageName) const;
    s32 tryFindWorldIndexByDevelopName(const char* developName) const;
    bool isEqualClearMainScenarioNo(s32 worldId, s32 scenarioNo) const;
    s32 getAfterEndingScenarioNo(s32 worldId) const;
    bool isEqualAfterEndingScenarioNo(s32 worldId, s32 scenarioNo) const;
    s32 getMoonRockScenarioNo(s32 worldId) const;
    bool isEqualMoonRockScenarioNo(s32 worldId, s32 scenarioNo) const;
    const char* getWorldDevelopName(s32 worldId) const;
    s32 getWorldScenarioNum(s32 worldId) const;
    s32 findUseScenarioNo(const char* stageName) const;
    bool checkNeedTreasureMessageStage(const char* stageName) const;
    bool checkIsMainStage(const char* mainStageName) const;
    bool tryFindTransOnMainStageByStageName(sead::Vector3f* outTrans, const char* stageName,
                                            s32 index) const;
    bool tryFindHintTransByScenarioNo(sead::Vector3f* outTrans, s32 scenarioNo, s32 index) const;

private:
    s32 findHintByScenarioNo(s32 scenarioNo) const;

    sead::PtrArray<WorldListEntry> mWorldList;
    sead::StrTreeMap<128, StagePosInfo*> mStagePosList;
    sead::PtrArray<ShinePosInfo> mShinePosList;
};

static_assert(sizeof(WorldList) == 0x40);
