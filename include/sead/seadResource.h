#pragma once

#include "sead/seadBitFlag.h"
#include "sead/seadRuntimeTypeInfo.h"

namespace sead
{
    class Heap;

    class Resource
    {
    public:
        SEAD_RTTI_BASE(Resource)

        Resource();

        virtual ~Resource();
    };

    class DirectResource : public Resource
    {
    public:
        SEAD_RTTI_OVERRIDE(DirectResource, Resource)

        DirectResource();

        virtual ~DirectResource();
        virtual int getLoadDataAlignment() const;
        virtual void doCreate_(unsigned char *, unsigned int, sead::Heap *);

        unsigned char* mData; // _8
        unsigned int mSize; // _10
        unsigned int mBufferSize; // _14
        sead::BitFlag<unsigned long> mSettings; // _18
    };

    class ArchiveRes : public DirectResource
    {
    public:
        SEAD_RTTI_OVERRIDE(ArchiveRes, DirectResource)
    };
};