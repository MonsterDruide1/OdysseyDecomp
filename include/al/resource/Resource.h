#pragma once

#include "nn/g3d.h"
#include "sead/seadFileDevice.h"
#include "sead/seadResource.h"
#include "sead/seadSafeString.hpp"

namespace al
{
    class Resource
    {
    public:
        Resource(const sead::SafeStringBase<char> &);
        Resource(const sead::SafeStringBase<char> &, sead::ArchiveRes *);

        bool isExistFile(const sead::SafeStringBase<char> &) const;
        bool isExistByml(const char *) const;
        unsigned int getSize() const;
        
        unsigned int getFileSize(const sead::SafeStringBase<char> &) const;

        bool tryCreateResGraphicsFile(const sead::SafeStringBase<char> &, nn::g3d::ResFile *);
        void cleanupResGraphicsFile();

        sead::ArchiveRes* mArchive; // _0
        sead::ArchiveFileDevice* mDevice; // _8
        sead::FixedSafeString<0x80> mName; // _10
        sead::Heap* mHeap; // _A8
        unsigned long _B0;
        nn::g3d::ResFile* mResFile; // _B8
    };  
};