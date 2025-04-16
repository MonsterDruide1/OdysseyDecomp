#pragma once

#include <prim/seadSafeString.h>

namespace nn::g3d {
class ResFile;
}  // namespace nn::g3d

namespace sead {
class ArchiveFileDevice;
class ArchiveRes;
class Heap;
}  // namespace sead

namespace al {
class ActorInitResourceData;
class InitResourceDataAction;
class InitResourceDataAnim;

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

    ActorInitResourceData* getResData() const { return mData; }

    nn::g3d::ResFile* getResFile() const { return mResFile; }

private:
    sead::ArchiveRes* mArchive = nullptr;
    sead::ArchiveFileDevice* mDevice = nullptr;
    sead::FixedSafeString<0x80> mName;
    sead::Heap* mHeap;
    ActorInitResourceData* mData = nullptr;
    nn::g3d::ResFile* mResFile = nullptr;
};

class ActorResource {
public:
    ActorResource(const sead::SafeString&, Resource*, Resource*);
    virtual ~ActorResource();

    void initResourceData(const char*, bool);

    bool hasAnimData() const { return mHasAnimData; }

    Resource* getModelRes() const { return mModelRes; }

    Resource* getAnimRes() const { return mAnimRes; }

private:
    sead::FixedSafeString<0x80> mName;
    Resource* mModelRes;
    Resource* mAnimRes;
    bool mHasAnimData;
    InitResourceDataAnim* mAnimResData;
    InitResourceDataAction* mActionResData;
};

}  // namespace al
