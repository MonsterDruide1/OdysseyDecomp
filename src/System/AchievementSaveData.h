#pragma once

#include <basis/seadTypes.h>

#include "System/ByamlSave.h"

class AchievementInfoReader;

struct AchievementEntry {
    bool isGotten = false;
    u64 getTime = 0;
};

static_assert(sizeof(AchievementEntry) == 0x10);

class AchievementSaveData : public ByamlSave {
public:
    AchievementSaveData(const AchievementInfoReader* reader);
    void init();
    void getAchievement(const char* name);
    s32 findAchievementIndex(const char* name) const;
    bool checkGetAchievement(const char* name) const;
    bool checkGetAchievement(s32 index) const;
    s64 findAchievementGetTime(s32 index) const;
    s32 calcGetAchievementNum() const;
    s32 getAchievementNum() const;
    void write(al::ByamlWriter* writer) override;
    void read(const al::ByamlIter& save) override;

private:
    AchievementEntry* mEntries = nullptr;
    const AchievementInfoReader* mInfoReader;
};

static_assert(sizeof(AchievementSaveData) == 0x18);
