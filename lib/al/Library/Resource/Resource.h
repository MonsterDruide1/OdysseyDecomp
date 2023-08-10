#pragma once

#include <filedevice/seadArchiveFileDevice.h>
#include <filedevice/seadFileDevice.h>
#include <nn/g3d/ResFile.h>
#include <resource/seadArchiveRes.h>
#include <resource/seadResource.h>

namespace al {
class ActorInitResourceData;

class Resource {
public:
    Resource(const sead::SafeString& path);
    Resource(const sead::SafeString& path, sead::ArchiveRes* archive);

    bool isExistFile(const sead::SafeString& name) const;
    bool isExistByml(const char*) const;
    u32 getSize() const;

    u32 getFileSize(const sead::SafeString&) const;

    const u8* getByml(const sead::SafeString&);

    bool tryCreateResGraphicsFile(const sead::SafeString& name, nn::g3d::ResFile* resFile);
    void cleanupResGraphicsFile();

    u32 getEntryNum(const sead::SafeString&) const;
    const char* getEntryName(const sead::BufferedSafeString* outName, const sead::SafeString&,
                             u32) const;
    const u8* getByml(const sead::SafeString& name) const;
    void* getFile(const sead::SafeString& name) const;
    void tryGetByml(const sead::SafeString& name) const;
    void getKcl(const sead::SafeString& name) const;
    void tryGetKcl(const sead::SafeString& name) const;
    void getPa(const sead::SafeString& name) const;
    void tryGetPa(const sead::SafeString& name) const;
    void getOtherFile(const sead::SafeString& name) const;
    const char* getArchiveName() const;

private:
    sead::ArchiveRes* mArchive;
    sead::ArchiveFileDevice* mDevice;
    sead::FixedSafeString<0x80> mName;
    sead::Heap* mHeap;
    ActorInitResourceData* mData;
    nn::g3d::ResFile* mResFile;
};
}  // namespace al
