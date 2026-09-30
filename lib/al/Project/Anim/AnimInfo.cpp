#include "Project/Anim/AnimInfo.h"

#include "Library/Base/StringUtil.h"

namespace al {

AnimResInfo::AnimResInfo() = default;

s32 AnimResInfo::getFrameMax() const {
    return static_cast<s32>(frameMax);
}

bool AnimResInfo::isLoop() const {
    return isLooping;
}

AnimInfoTable::AnimInfoTable(s32 capacity) {
    mInfoEntries = new AnimResInfo[capacity];
}

void AnimInfoTable::add(const char* name, void* buffer, f32 frameMax, bool isLoop) {
    // BUG: Is sorted flag is not cleared and no bounds check
    AnimResInfo* info = &mInfoEntries[mSize];
    info->name = createStringIfInStack(name);
    info->buffer = buffer;
    info->frameMax = frameMax;
    info->isLooping = isLoop;
    mSize++;
}

AnimResInfo* AnimInfoTable::findAnimInfo(const char* name) const {
    if (mIsSorted) {
        s32 low = 0;
        s32 high = mSize;
        while (low < high) {
            s32 mid = (high + low - 1) >> 1;
            AnimResInfo* info = &mInfoEntries[mid];
            s32 cmp = strcmp(info->name, name);

            if (cmp > 0) {
                high = mid;
                continue;
            }

            if (cmp == 0)
                return info;

            low = mid + 1;
        }
        return nullptr;
    }

    for (s32 i = 0; i < mSize; i++) {
        AnimResInfo* info = &mInfoEntries[i];
        if (isEqualString(info->name, name))
            return info;
    }
    return nullptr;
}

// NON-MATCHING: Different register somehow https://decomp.me/scratch/ZxdNk
AnimResInfo* AnimInfoTable::tryFindAnimInfo(const char* name) const {
    return findAnimInfo(name);
}

inline void heapify(AnimResInfo* entries, s32 n, const AnimResInfo& temp, s32 parent) {
    s32 child = parent * 2;
    while (child <= n) {
        if (child < n && strcmp(entries[child - 1].name, entries[child].name) < 0)
            child++;

        if (strcmp(temp.name, entries[child - 1].name) >= 0)
            break;

        entries[parent - 1] = entries[child - 1];
        parent = child;
        child = parent * 2;
    }
    entries[parent - 1] = temp;
}

void AnimInfoTable::sort() {
    s32 n = mSize;
    AnimResInfo* entries = mInfoEntries;

    if (n < 2 || !entries) {
        mIsSorted = true;
        return;
    }

    AnimResInfo temp;

    for (s32 i = n / 2; i >= 1; i--) {
        temp = entries[i - 1];
        heapify(entries, n, temp, i);
    }

    // TODO: Figure out how to apply heapify here as well
    for (s32 i = n; i >= 2; i--) {
        temp = entries[i - 1];
        entries[i - 1] = entries[0];
        s32 parent = 1;
        s32 child = parent * 2;
        while (child < i) {
            if (child < i - 1 && strcmp(entries[child - 1].name, entries[child].name) < 0)
                child++;

            if (strcmp(temp.name, entries[child - 1].name) >= 0)
                break;

            entries[parent - 1] = entries[child - 1];
            parent = child;
            child = parent * 2;
        }
        entries[parent - 1] = temp;
    }

    mIsSorted = true;
}

}  // namespace al
