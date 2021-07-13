#pragma once

#include "seadDisposer.h"
#include "seadNamable.h"
#include "seadHostIO.h"

namespace sead
{
    class Heap;

    class HeapMgr
    {
    public:
        HeapMgr();

        virtual ~HeapMgr();

        Heap* getCurrentHeap();

        static HeapMgr sInstance;
        static HeapMgr* sInstancePtr;
    };

    class Heap : public IDisposer, INamable, hostio::Reflexible
    {
    public:
        
    };
};