#pragma once

#include <container/seadPtrArray.h>
#include <container/seadStrTreeMap.h>
#include <math/seadVector.h>
#include <prim/seadSafeString.h>

struct StagePosInfo {};

struct StageDBEntry {
    sead::FixedSafeString<0x80> stageName;
    sead::FixedSafeString<0x40> stageCategory;
    s32 useScenario;
};

struct WorldListEntry {
    const char* mainStageName;
    const char* worldDevelopName;
    s32 questInfoCount;
    s32 clearMainScenario;
    s32 endingScenario;
    s32 moonRockScenario;
    s32* mainQuestIndexes;
    sead::PtrArray<StageDBEntry> stageNames;
};

class WorldList {
public:
    WorldList();

    void getWorldNum();
    void getMainQuestMin(s32, s32);
    const char* getMainStageName(s32) const;
    s32 tryFindWorldIndexByMainStageName(const char* stageName) const;
    s32 tryFindWorldIndexByStageName(const char* stageName) const;
    s32 tryFindWorldIndexByDevelopName(const char* stageName) const;
    void isEqualClearMainScenarioNo(s32, s32);
    void getAfterEndingScenarioNo(s32);
    void isEqualAfterEndingScenarioNo(s32, s32);
    s32 getMoonRockScenarioNo(s32 worldId) const;
    bool isEqualMoonRockScenarioNo(s32, s32) const;
    const char* getWorldDevelopName(s32) const;
    void getWorldScenarioNum(s32);
    s32 findUseScenarioNo(const char* stageName) const;
    void checkNeedTreasureMessageStage(const char*);
    bool checkIsMainStage(const char* stageName) const;
    void tryFindTransOnMainStageByStageName(sead::Vector3f*, const char*, s32);
    void tryFindHintTransByScenarioNo(sead::Vector3f*, s32, s32);

private:
    sead::PtrArray<WorldListEntry> mWorldList;
    sead::StrTreeMap<128, StagePosInfo*> mStagePosList;
    sead::PtrArrayImpl field_30;
};
static_assert(sizeof(WorldList) == 0x40, "WorldList Size");
