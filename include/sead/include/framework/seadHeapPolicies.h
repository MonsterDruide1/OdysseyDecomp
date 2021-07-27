#ifndef SEAD_HEAP_POLICIES_H_
#define SEAD_HEAP_POLICIES_H_

#include <basis/seadTypes.h>

namespace sead
{
class Heap;

class HeapArray
{
public:
    HeapArray(HeapArray& other)
    {
        mHeaps[0] = other.mHeaps[0];
        mHeaps[1] = other.mHeaps[1];
        mHeaps[2] = other.mHeaps[2];
        mHeaps[3] = other.mHeaps[3];
        *reinterpret_cast<u32*>(mAdjusted) = *reinterpret_cast<u32*>(other.mAdjusted);
        mPrimaryIndex = other.mPrimaryIndex;
    }

    Heap* getHeap(s32 index) const { return mHeaps[index]; }
    Heap* getPrimaryHeap() const { return mHeaps[mPrimaryIndex]; }

    Heap* mHeaps[4];
    bool mAdjusted[4];
    s32 mPrimaryIndex;
};

class HeapPolicy
{
public:
    HeapPolicy();

    Heap* parent;
    u32 size;
    u32 create_slack;
    u32 adjust_slack;
    u8 adjust;
    u8 temporary;
    u8 dont_create;
};

class HeapPolicies
{
public:
    HeapPolicy mPolicies[4];
    s32 mPrimaryIndex;
};

}  // namespace sead

#endif  // SEAD_HEAP_POLICIES_H_
