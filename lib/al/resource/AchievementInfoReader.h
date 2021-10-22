#pragma once

#include <container/seadPtrArray.h>

#include "al/util/StringUtil.h"

class AchievementInfo {
public:
    AchievementInfo(const char* name, const char* note, int num, int level) : mName(name), mNote(note), mNum(num), mLevel(level) {}

    const char* mName;
    const char* mNote;
    int mNum;
    int mLevel;
};

class AchievementInfoReader {
public:

    AchievementInfoReader();
    void init();
    int tryFindIndexByName(const char*) const;

private:
    sead::PtrArray<AchievementInfo> array;

};
