#pragma once

#include <filedevice/seadArchiveFileDevice.h>
#include <filedevice/seadFileDevice.h>
#include <heap/seadHeapMgr.h>
#include <nn/g3d/ResFile.h>
#include <resource/seadArchiveRes.h>
#include <resource/seadResource.h>

namespace al {
class InitResourceDataAction;
class InitResourceDataAnim;
struct ActionAnimCtrlInfo;
class ActorInitResourceData;

class Resource {
public:
    Resource(const sead::SafeString& path);
    Resource(const sead::SafeString& path, sead::ArchiveRes* archive);

    bool isExistFile(const sead::SafeString& name) const;
    bool isExistByml(const char*) const;
    u32 getSize() const;
    u32 getEntryNum(const sead::SafeString& directoryPath) const;
    void getEntryName(sead::BufferedSafeString* outName, const sead::SafeString& directoryPath,
                      u32 entryNum) const;
    u32 getFileSize(const sead::SafeString& filePath) const;
    const u8* getByml(const sead::SafeString& filePath) const;
    void* getFile(const sead::SafeString& name) const;

    bool tryCreateResGraphicsFile(const sead::SafeString& name, nn::g3d::ResFile* resFile);
    void cleanupResGraphicsFile();

    void* tryGetByml(const sead::SafeString& name) const;
    void* getKcl(const sead::SafeString& name) const;
    void* tryGetKcl(const sead::SafeString& name) const;
    void* getPa(const sead::SafeString& name) const;
    void* tryGetPa(const sead::SafeString& name) const;
    void* getOtherFile(const sead::SafeString& name) const;
    const char* getArchiveName() const;

    ActorInitResourceData* getResData() const { return mData; }

    nn::g3d::ResFile* getResFile() const { return mResFile; }

private:
    sead::ArchiveRes* mArchive = nullptr;
    sead::ArchiveFileDevice* mDevice = nullptr;
    sead::FixedSafeString<0x80> mName;
    sead::Heap* mHeap = sead::HeapMgr::instance()->getCurrentHeap();
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
