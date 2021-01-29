#pragma once

#include "sead/seadDisposer.h"
#include "sead/seadListImpl.h"
#include "sead/seadSafeString.h"

namespace sead
{
    class FileDevice : public TList<FileDevice *>, public IDisposer
    {
    public:

        bool tryGetFileSize(unsigned int *, const sead::SafeStringBase<char> &);
        bool tryIsExistFile(bool *, const sead::SafeStringBase<char> &);

        FixedSafeString<32> mName;
        bool unk;
    };

    class ArchiveFileDevice : public FileDevice
    {

    };
};