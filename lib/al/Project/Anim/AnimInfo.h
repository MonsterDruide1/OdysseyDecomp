#pragma once

#include <basis/seadTypes.h>

namespace al {
struct AnimResInfo {
    const char* name;
    void* resMaterialAnim;
    f32 frameMax;
    bool isLoop;
};

class AnimInfoTable {
public:
    AnimInfoTable(u32);

    AnimResInfo* findAnimInfo(const char* name);
    bool tryFindAnimInfo(const char* name);

    void add(const char* name, void*, f32 frameMax, bool isLoop);
    void sort();

    s32 getInfoCount() const { return mInfoCount; }

    AnimResInfo* getResInfo(s32 index) const { return &mResInfos[index]; }

private:
    s32 mInfoCount;
    AnimResInfo* mResInfos;
    bool mIsSorted;
};
}  // namespace al
