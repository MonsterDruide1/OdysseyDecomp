#pragma once

#include <basis/seadTypes.h>
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

    s32 getWorldNum() const;
    s32 getMainQuestMin(s32, s32) const;
    const char* getMainStageName(s32) const;
    s32 tryFindWorldIndexByMainStageName(const char*) const;
    s32 tryFindWorldIndexByStageName(const char*) const;
    s32 tryFindWorldIndexByDevelopName(const char*) const;
    bool isEqualClearMainScenarioNo(s32, s32) const;
    s32 getAfterEndingScenarioNo(s32) const;
    bool isEqualAfterEndingScenarioNo(s32, s32) const;
    s32 getMoonRockScenarioNo(s32) const;
    bool isEqualMoonRockScenarioNo(s32, s32) const;
    const char* getWorldDevelopName(s32) const;
    s32 getWorldScenarioNum(s32) const;
    s32 findUseScenarioNo(const char*) const;
    bool checkNeedTreasureMessageStage(const char*) const;
    bool checkIsMainStage(const char*) const;
    bool tryFindTransOnMainStageByStageName(sead::Vector3f*, const char*, s32) const;
    bool tryFindHintTransByScenarioNo(sead::Vector3f*, s32, s32) const;

private:
    sead::PtrArray<WorldListEntry> mWorldList;
    sead::StrTreeMap<128, StagePosInfo*> mStagePosList;
    sead::PtrArrayImpl field_30;
};

static_assert(sizeof(WorldList) == 0x40, "WorldList Size");
