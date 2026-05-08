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

    const AnimResInfo& findAnimInfo(const char* name) const;
    bool tryFindAnimInfo(const char* name) const;

    void add(const char* name, void*, f32 frameMax, bool isLoop);
    void sort();

    s32 getInfoCount() const { return mInfoCount; }

    const AnimResInfo& getResInfo(s32 index) const { return mResInfos[index]; }

private:
    s32 mInfoCount;
    AnimResInfo* mResInfos;
    bool mIsSorted;
};
}  // namespace al
