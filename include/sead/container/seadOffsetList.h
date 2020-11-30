#ifndef SEAD_OFFSET_LIST_H_
#define SEAD_OFFSET_LIST_H_

#include <basis/seadRawPrint.h>
#include <basis/seadTypes.h>
#include <container/seadListImpl.h>
#include <prim/seadPtrUtil.h>

namespace sead
{
template <typename T>
class OffsetList : public ListImpl
{
public:
    OffsetList() = default;

    void initOffset(s32 offset) { mOffset = offset; }

    void clear() { ListImpl::clear(); }

    void pushBack(T* item)
    {
        SEAD_ASSERT(mOffset >= 0);
        ListImpl::pushBack(objToListNode(item));
    }

    void pushFront(T* item)
    {
        SEAD_ASSERT(mOffset >= 0);
        ListImpl::pushFront(objToListNode(item));
    }

    T* popBack() { return listNodeToObj(ListImpl::popBack()); }
    T* popFront() { return listNodeToObj(ListImpl::popFront()); }

    void insertBefore(const T* obj, T* obj_to_insert)
    {
        ListImpl::insertBefore(const_cast<ListNode*>(objToListNode(obj)),
                               objToListNode(obj_to_insert));
    }
    void insertAfter(const T* obj, T* obj_to_insert)
    {
        ListImpl::insertAfter(const_cast<ListNode*>(objToListNode(obj)),
                              objToListNode(obj_to_insert));
    }

    void erase(T* item) { ListImpl::erase(objToListNode(item)); }

    T* front() const
    {
        auto* node = ListImpl::front();
        return node ? listNodeToObj(node) : nullptr;
    }

    T* back() const
    {
        auto* node = ListImpl::back();
        return node ? listNodeToObj(node) : nullptr;
    }

    T* prev(const T* obj) const
    {
        ListNode* prev_node = objToListNode(obj)->prev();
        if (prev_node == &mStartEnd)
            return nullptr;
        return listNodeToObj(prev_node);
    }

    T* next(const T* obj) const
    {
        ListNode* next_node = objToListNode(obj)->next();
        if (next_node == &mStartEnd)
            return nullptr;
        return listNodeToObj(next_node);
    }

    T* nth(s32 n) const { return listNodeToObj(ListImpl::nth(n)); }
    s32 indexOf(const T* obj) const { return ListImpl::indexOf(objToListNode(obj)); }

    bool isNodeLinked(const T* obj) const { return objToListNode(obj)->isLinked(); }

    void swap(T* obj1, T* obj2) { ListImpl::swap(objToListNode(obj1), objToListNode(obj2)); }
    void moveAfter(T* basis, T* obj)
    {
        ListImpl::moveAfter(objToListNode(basis), objToListNode(obj));
    }
    void moveBefore(T* basis, T* obj)
    {
        ListImpl::moveBefore(objToListNode(basis), objToListNode(obj));
    }

    using CompareCallback = int (*)(const T*, const T*);

    void sort() { sort(compareT); }
    void sort(CompareCallback cmp) { ListImpl::sort(mOffset, cmp); }
    void mergeSort() { mergeSort(compareT); }
    void mergeSort(CompareCallback cmp) { ListImpl::mergeSort(mOffset, cmp); }

    T* find(const T* obj) const { return find(obj, compareT); }
    T* find(const T* obj, CompareCallback cmp) const
    {
        return listNodeToObj(ListImpl::find(obj, mOffset, cmp));
    }

    void uniq() { uniq(compareT); }
    void uniq(CompareCallback cmp) { ListImpl::uniq(mOffset, cmp); }

    class iterator
    {
    public:
        iterator(T* ptr, s32 offset) : mPtr{ptr}, mOffset{offset} {}
        bool operator==(const iterator& other) const { return mPtr == other.mPtr; }
        bool operator!=(const iterator& other) const { return !(*this == other); }
        iterator& operator++()
        {
            ListNode* node = static_cast<ListNode*>(PtrUtil::addOffset(mPtr, mOffset))->next();
            mPtr = static_cast<T*>(PtrUtil::addOffset(node, -mOffset));
            return *this;
        }
        T& operator*() const { return *mPtr; }
        T* operator->() const { return mPtr; }

    private:
        T* mPtr;
        s32 mOffset;
    };

    iterator begin() const { return iterator(listNodeToObj(mStartEnd.next()), mOffset); }
    iterator end() const
    {
        return iterator(listNodeToObj(const_cast<ListNode*>(&mStartEnd)), mOffset);
    }
    iterator begin(T* ptr) const { return iterator(ptr, mOffset); }

    class robustIterator
    {
    public:
        robustIterator(T* ptr, s32 offset)
            : mPtr{ptr}, mNextNode(static_cast<ListNode*>(PtrUtil::addOffset(ptr, offset))->next()),
              mOffset{offset}
        {
        }
        bool operator==(const robustIterator& other) const { return mPtr == other.mPtr; }
        bool operator!=(const robustIterator& other) const { return !operator==(other); }
        robustIterator& operator++()
        {
            mPtr = static_cast<T*>(PtrUtil::addOffset(mNextNode, -mOffset));
            mNextNode = mNextNode->next();
            return *this;
        }
        T& operator*() const { return *mPtr; }
        T* operator->() const { return mPtr; }

    private:
        T* mPtr;
        ListNode* mNextNode;
        s32 mOffset;
    };

    robustIterator robustBegin() const
    {
        return robustIterator(listNodeToObj(mStartEnd.next()), mOffset);
    }

    robustIterator robustEnd() const
    {
        return robustIterator(listNodeToObj(const_cast<ListNode*>(&mStartEnd)), mOffset);
    }

    robustIterator robustBegin(T* ptr) const { return robustIterator(ptr, mOffset); }

    struct RobustRange
    {
        auto begin() const { return mList.robustBegin(); }
        auto end() const { return mList.robustEnd(); }
        const OffsetList& mList;
    };
    RobustRange robustRange() const { return {*this}; }

protected:
    static int compareT(const T* lhs, const T* rhs)
    {
        if (lhs < rhs)
            return -1;
        if (lhs > rhs)
            return 1;
        return 0;
    }

    ListNode* objToListNode(T* obj) const
    {
        return static_cast<ListNode*>(PtrUtil::addOffset(obj, mOffset));
    }

    const ListNode* objToListNode(const T* obj) const
    {
        return static_cast<const ListNode*>(PtrUtil::addOffset(obj, mOffset));
    }

    T* listNodeToObj(ListNode* node) const
    {
        return static_cast<T*>(PtrUtil::addOffset(node, -mOffset));
    }

    const T* listNodeToObj(const ListNode* node) const
    {
        return static_cast<const T*>(PtrUtil::addOffset(node, -mOffset));
    }

    s32 mOffset = -1;
};

}  // namespace sead

#endif  // SEAD_OFFSET_LIST_H_
