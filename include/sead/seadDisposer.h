#pragma once

#include "sead/seadListImpl.h"

namespace sead
{
    class Heap;

    class IDisposer
    {
    public:
        IDisposer();

        virtual ~IDisposer();

        enum HeapNullOption
        {
            UseGivenHeap = 0x0,
            UseGivenOrContainedHeap = 0x1,
            NoDisposerIfNoSpecifiedHeap = 0x2,
            UseCurrentOrSpecifiedHeap = 0x3
        };

        IDisposer(sead::Heap *, HeapNullOption);

        sead::Heap* mDisposerHeap; // _8
        sead::ListNode mNode; // _10
    };
};