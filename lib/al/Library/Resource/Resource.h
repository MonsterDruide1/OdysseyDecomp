#pragma once

#include <prim/seadSafeString.h>

namespace sead {
class ArchiveFileDevice;
class ArchiveRes;
}  // namespace sead

namespace nn::g3d {
class ResFile;
}

namespace al {
class InitResourceDataAction;
class InitResourceDataAnim;
struct ActionAnimCtrlInfo;
class ActorInitResourceData;

class Resource {
public:
    Resource(const sead::SafeString& path);
    Resource(const sead::SafeString& path, sead::ArchiveRes* archive);

    bool isExistFile(const sead::SafeString& filePath) const;
    bool isExistByml(const char* filePath) const;
    u32 getSize() const;
    u32 getEntryNum(const sead::SafeString& directoryPath) const;
    void getEntryName(sead::BufferedSafeString* outName, const sead::SafeString& directoryPath,
                      u32 entryNum) const;
    u32 getFileSize(const sead::SafeString& filePath) const;
    const u8* getByml(const sead::SafeString& filePath) const;
    const void* getFile(const sead::SafeString& filePath) const;
    const u8* tryGetByml(const sead::SafeString& filePath) const;
    const void* getKcl(const sead::SafeString& filePath) const;
    const void* tryGetKcl(const sead::SafeString& filePath) const;
    const void* getPa(const sead::SafeString& filePath) const;
    const void* tryGetPa(const sead::SafeString& filePath) const;
    const void* getOtherFile(const sead::SafeString& filePath) const;
    const char* getArchiveName() const;
    bool tryCreateResGraphicsFile(const sead::SafeString& filePath, nn::g3d::ResFile* resFile);
    void cleanupResGraphicsFile();

    sead::ArchiveRes* getFileArchive() const { return mArchive; }

    sead::ArchiveFileDevice* getFileDevice() const { return mDevice; }

    const char* getPath() const { return mPath.cstr(); }

    ActorInitResourceData* getResData() const { return mData; }

    void setActorInitResourceData(ActorInitResourceData* data) { mData = data; }

    nn::g3d::ResFile* getResFile() const { return mResFile; }

private:
    sead::ArchiveRes* mArchive = nullptr;
    sead::ArchiveFileDevice* mDevice = nullptr;
    sead::FixedSafeString<0x80> mPath;
    sead::Heap* mHeap;
    ActorInitResourceData* mData = nullptr;
    nn::g3d::ResFile* mResFile = nullptr;
};

}  // namespace al
