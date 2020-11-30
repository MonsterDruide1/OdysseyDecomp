#ifndef SEAD_LIST_IMPL_H_
#define SEAD_LIST_IMPL_H_

#include <basis/seadTypes.h>

namespace sead
{
class Random;

class ListNode
{
public:
    ListNode* next() const { return mNext; }
    ListNode* prev() const { return mPrev; }
    bool isLinked() const { return mNext || mPrev; }

private:
    friend class ListImpl;

    void init_() { *this = {}; }
    void insertBack_(ListNode* node);
    void insertFront_(ListNode* node);
    void erase_();

    ListNode* mPrev = nullptr;
    ListNode* mNext = nullptr;
};

class ListImpl
{
public:
    __attribute__((always_inline)) ListImpl() : mStartEnd(), mCount(0)
    {
        mStartEnd.mNext = &mStartEnd;
        mStartEnd.mPrev = &mStartEnd;
    }

    bool isEmpty() const { return mCount == 0; }
    s32 size() const { return mCount; }

    void reverse();
    void shuffle();
    void shuffle(Random* random);
    bool checkLinks() const;

protected:
    using CompareCallbackImpl = int (*)(const void*, const void*);

    void sort(s32 offset, CompareCallbackImpl cmp);
    void mergeSort(s32 offset, CompareCallbackImpl comp);

    void pushBack(ListNode* item)
    {
        mStartEnd.insertFront_(item);
        ++mCount;
    }

    void pushFront(ListNode* item)
    {
        mStartEnd.insertBack_(item);
        ++mCount;
    }

    ListNode* popBack();
    ListNode* popFront();

    void insertBefore(ListNode* node, ListNode* node_to_insert)
    {
        node->insertFront_(node_to_insert);
        ++mCount;
    }

    void insertAfter(ListNode* node, ListNode* node_to_insert)
    {
        node->insertBack_(node_to_insert);
        ++mCount;
    }

    void erase(ListNode* item)
    {
        item->erase_();
        --mCount;
    }

    ListNode* front() const { return mStartEnd.mNext; }
    ListNode* back() const { return mStartEnd.mPrev; }
    ListNode* nth(int n) const;
    s32 indexOf(const ListNode*) const;

    void swap(ListNode* n1, ListNode* n2);
    void moveAfter(ListNode* basis, ListNode* n);
    void moveBefore(ListNode* basis, ListNode* n);

    ListNode* find(const void* ptr, s32 offset, CompareCallbackImpl cmp) const;
    void uniq(s32 offset, CompareCallbackImpl cmp);

    void clear();

    static void mergeSortImpl(ListNode* front, ListNode* back, s32 num, s32 offset,
                              CompareCallbackImpl cmp);

protected:
    ListNode mStartEnd;
    s32 mCount;
};

}  // namespace sead

#endif  // SEAD_LIST_IMPL_H_
