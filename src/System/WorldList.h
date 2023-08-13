#pragma once

#include <container/seadPtrArray.h>
#include <container/seadStrTreeMap.h>
#include <math/seadVector.h>
#include <prim/seadSafeString.h>

struct StagePosInfo {};

struct StageDBEntry {
    sead::FixedSafeString<0x80> stageName;
    sead::FixedSafeString<0x40> stageCategory;
    int useScenario;
};

struct WorldListEntry {
    const char* mainStageName;
    const char* worldDevelopName;
    int questInfoCount;
    int clearMainScenario;
    int endingScenario;
    int moonRockScenario;
    int* mainQuestIndexes;
    sead::PtrArray<StageDBEntry> stageNames;
};

class WorldList {
public:
    WorldList();

    void getWorldNum();
    void getMainQuestMin(int, int);
    const char* getMainStageName(int) const;
    int tryFindWorldIndexByMainStageName(const char* stageName) const;
    int tryFindWorldIndexByStageName(const char* stageName) const;
    int tryFindWorldIndexByDevelopName(const char* stageName) const;
    void isEqualClearMainScenarioNo(int, int);
    void getAfterEndingScenarioNo(int);
    void isEqualAfterEndingScenarioNo(int, int);
    int getMoonRockScenarioNo(int worldId) const;
    bool isEqualMoonRockScenarioNo(int, int) const;
    const char* getWorldDevelopName(int) const;
    void getWorldScenarioNum(int);
    int findUseScenarioNo(const char* stageName) const;
    void checkNeedTreasureMessageStage(const char*);
    bool checkIsMainStage(const char* stageName) const;
    void tryFindTransOnMainStageByStageName(sead::Vector3f*, const char*, int);
    void tryFindHintTransByScenarioNo(sead::Vector3f*, int, int);
    
private:
    sead::PtrArray<WorldListEntry> mWorldList;
    sead::StrTreeMap<128, StagePosInfo*> mStagePosList;
    sead::PtrArrayImpl field_30;
};
static_assert(sizeof(WorldList) == 0x40, "WorldList Size");
