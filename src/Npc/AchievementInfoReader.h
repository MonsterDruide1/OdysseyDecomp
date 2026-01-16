#pragma once

#include <container/seadPtrArray.h>

struct AchievementInfo {
public:
    AchievementInfo(const char* name, s32 num, s32 level, const char* note)
        : name(name), num(num), level(level), note(note) {}

    const char* name;
    s32 num;
    s32 level;
    const char* note;
};

class AchievementInfoReader {
public:
    AchievementInfoReader();
    void init();
    s32 tryFindIndexByName(const char* name) const;

    AchievementInfo* get(s32 index) { return mAchievements[index]; }

    AchievementInfo* unsafeAt(s32 index) { return mAchievements.unsafeAt(index); }

    s32 size() { return mAchievements.size(); }

    s32 capacity() { return mAchievements.capacity(); }

private:
    sead::PtrArray<AchievementInfo> mAchievements;
};
