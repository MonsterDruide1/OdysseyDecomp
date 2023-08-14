#pragma once

#include <container/seadPtrArray.h>

#include "Library/Base/String.h"

class AchievementInfo {
public:
    AchievementInfo(const char* name, s32 num, s32 level, const char* note)
        : mName(name), mNum(num), mLevel(level), mNote(note) {}

    const char* mName;
    s32 mNum;
    s32 mLevel;
    const char* mNote;
};

class AchievementInfoReader {
public:
    AchievementInfoReader();
    void init();
    s32 tryFindIndexByName(const char*) const;

    AchievementInfo* get(s32 index) { return array[index]; }
    s32 size() { return array.size(); }
    s32 capacity() { return array.capacity(); }

private:
    sead::PtrArray<AchievementInfo> array;
};
