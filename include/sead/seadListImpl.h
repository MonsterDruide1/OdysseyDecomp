#pragma once

namespace sead
{
    class ListNode
    {
    public:
        void insertBack(ListNode *);
        void insertFront(ListNode *);
        void erase_();

        ListNode* mPrev; // _0
        ListNode* mNext; // _8
    };

    class ListImpl
    {
    public:
        ListNode* popBack();
        ListNode* popFront();
        ListNode* nth(int);
        int indexOf(const ListNode *) const;
        void clear();
    
        ListNode mStartEnd; // _0
        int mCount; // _10
    };
};