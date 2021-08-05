#pragma once

namespace sead
{
    class ListNode
    {
    public:
        ListNode* mPrev; // _0
        ListNode* mNext; // _8
    };

    class ListImpl
    {
    public:
        ListNode mStart; // _0
        int mNodeCount; // _10
    };

    template <typename T>
    class TListNode;

    template<typename T>
    class TList : ListImpl
    {
    public:
        TListNode<T>* mCur; // _18
        TListNode<T>* mNext; // _20
    };

    template<typename T>
    class TListNode : ListNode
    {
    public:
        T mData; // _10
        TList<T>* mParentList; // _18
    };
};