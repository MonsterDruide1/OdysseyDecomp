#pragma once

#include <basis/seadTypes.h>

namespace al {

struct AnimResInfo {
    AnimResInfo();

    s32 getFrameMax() const;
    bool isLoop() const;

    const char* name = nullptr;
    void* buffer = nullptr;
    f32 frameMax = 0.0f;
    bool isLooping = false;
};

static_assert(sizeof(AnimResInfo) == 0x18);

class AnimInfoTable {
public:
    AnimInfoTable(u32 capacity);

    const AnimResInfo& findAnimInfo(const char* name) const;
    bool tryFindAnimInfo(const char* name) const;

    void add(const char* name, void* buffer, f32 frameMax, bool isLoop);
    void sort();

    s32 getInfoCount() const { return mSize; }

    const AnimResInfo& getResInfo(s32 index) const { return mInfoEntries[index]; }

private:
    s32 mSize = 0;
    AnimResInfo* mInfoEntries = nullptr;
    bool mIsSorted = false;
};

static_assert(sizeof(AnimInfoTable) == 0x18);

}  // namespace al
