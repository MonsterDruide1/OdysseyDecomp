#pragma once

#include "filedevice/seadArchiveFileDevice.h"
#include "prim/seadSafeString.h"

namespace al
{
    class Resource
    {
    public:
        Resource(const sead::SafeStringBase<char> &);

        unsigned long _0;
        sead::ArchiveFileDevice* mDevice; // _8
    };  
};