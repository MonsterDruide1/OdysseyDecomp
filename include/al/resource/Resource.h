#pragma once

#include "sead/seadFileDevice.h"
#include "nn/g3d.h"
#include "sead/seadSafeString.h"

namespace al
{
    class Resource
    {
    public:
        Resource(const sead::SafeStringBase<char> &);

        bool isExistFile(const sead::SafeStringBase<char> &) const;

        unsigned int getFileSize(const sead::SafeStringBase<char> &) const;

        void cleanupResGraphicsFile();

        unsigned long _0;
        sead::ArchiveFileDevice* mDevice; // _8
        char _10[0xB8-0x10]; // _10
        nn::g3d::ResFile* mResFile; // _B8
    };  
};