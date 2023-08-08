#pragma once

#include <filedevice/seadArchiveFileDevice.h>
#include <filedevice/seadFileDevice.h>
#include <nn/g3d/ResFile.h>
#include <resource/seadArchiveRes.h>
#include <resource/seadResource.h>

namespace al {
class Resource {
public:
    Resource(const sead::SafeStringBase<char>&);
    Resource(const sead::SafeStringBase<char>&, sead::ArchiveRes*);

    bool isExistFile(const sead::SafeStringBase<char>&) const;
    bool isExistByml(const char*) const;
    unsigned int getSize() const;

    unsigned int getFileSize(const sead::SafeStringBase<char>&) const;

    const u8* getByml(const sead::SafeString&);

    bool tryCreateResGraphicsFile(const sead::SafeStringBase<char>&, nn::g3d::ResFile*);
    void cleanupResGraphicsFile();

    sead::ArchiveRes* mArchive;         // _0
    sead::ArchiveFileDevice* mDevice;   // _8
    sead::FixedSafeString<0x80> mName;  // _10
    sead::Heap* mHeap;                  // _A8
    unsigned long _B0;
    nn::g3d::ResFile* mResFile;  // _B8
};
};  // namespace al