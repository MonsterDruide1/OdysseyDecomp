#pragma once

#include <container/seadPtrArray.h>

#include "al/util/StringUtil.h"

class AchievementInfo {
public:
    AchievementInfo(const char* name, int num, int level, const char* note)
        : mName(name), mNum(num), mLevel(level), mNote(note) {}

    const char* mName;
    int mNum;
    int mLevel;
    const char* mNote;
};

class AchievementInfoReader {
public:
    AchievementInfoReader();
    void init();
    int tryFindIndexByName(const char*) const;

    AchievementInfo* get(int index) { return array[index]; }
    int size() { return array.size(); }
    int capacity() { return array.capacity(); }

private:
    sead::PtrArray<AchievementInfo> array;
};
