#include "Library/Screen/ScreenPointCheckGroup.h"

namespace al {
ScreenPointCheckGroup::ScreenPointCheckGroup(s32 size) : mSize(size) {
    mScreenPointTargets = new ScreenPointTarget*[mSize];

    for (s32 i = 0; i < mSize; i++)
        mScreenPointTargets[i] = nullptr;
}

void ScreenPointCheckGroup::setValid(ScreenPointTarget* target) {
    for (s32 i = mValidCount; i < mCount; i++) {
        if (mScreenPointTargets[i] == target) {
            mScreenPointTargets[i] = mScreenPointTargets[mValidCount];
            mScreenPointTargets[mValidCount] = target;
            mValidCount++;

            break;
        }
    }
}

void ScreenPointCheckGroup::setInvalid(ScreenPointTarget* target) {
    for (s32 i = 0; i < mValidCount; i++) {
        if (mScreenPointTargets[i] == target) {
            mScreenPointTargets[i] = mScreenPointTargets[mValidCount - 1];
            mScreenPointTargets[mValidCount - 1] = target;
            mValidCount--;
            break;
        }
    }
}

ScreenPointTarget* ScreenPointCheckGroup::getTarget(s32 index) const {
    return mScreenPointTargets[index];
}

void ScreenPointCheckGroup::setTarget(ScreenPointTarget* target) {
    mScreenPointTargets[mCount] = target;
    mCount++;
}

}  // namespace al
