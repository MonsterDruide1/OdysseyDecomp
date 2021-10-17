#pragma once

#include <container/seadPtrArray.h>

#include "al/util/StringUtil.h"
class AchievementInfoReader {
public:
    struct Achievement {
    public:
        Achievement(const char* name, const char* note, int num, int level) : mName(name), mNote(note), mNum(num), mLevel(level) {}

        const char* mName;
        const char* mNote;
        int mNum;
        int mLevel;
    };

    AchievementInfoReader();
    void init();
    int tryFindIndexByName(const char*) const;

private:
    sead::PtrArray<Achievement> array;

};
