#pragma once

#include "sead/seadDisposer.h"
#include "sead/seadListImpl.h"
#include "sead/seadSafeString.h"
#include "sead/seadRuntimeTypeInfo.h"

namespace sead
{
    class ArchiveRes;

    class FileDevice : public TListNode<FileDevice *>, public IDisposer
    {
        SEAD_RTTI_BASE(FileDevice)
    public:

        bool tryGetFileSize(unsigned int *, const sead::SafeStringBase<char> &);
        bool tryIsExistFile(bool *, const sead::SafeStringBase<char> &);

        FixedSafeString<32> mName;
        bool unk;
    };

    class ArchiveFileDevice : public FileDevice
    {
        SEAD_RTTI_OVERRIDE(ArchiveFileDevice, FileDevice)
    public:
        ArchiveFileDevice(ArchiveRes *);

        ArchiveRes* mArchiveRes;
    };
};