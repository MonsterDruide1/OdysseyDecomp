#include "Library/Resource/Resource.h"

#include <g3d/aglNW4FToNN.h>
#include <heap/seadHeapMgr.h>
#include <nn/g3d/ResFile.h>
#include <resource/seadArchiveRes.h>

#include "Library/Base/HashCodeUtil.h"
#include "Library/Base/StringUtil.h"
#include "Library/File/FileUtil.h"

const sead::DirectoryEntry sEntries[0x1000];

namespace al {
Resource::Resource(const sead::SafeString& path)
    : mName(path), mHeap(sead::HeapMgr::instance()->getCurrentHeap()) {
    mArchive = loadArchive(path);
    mDevice = new sead::ArchiveFileDevice(mArchive);
}

Resource::Resource(const sead::SafeString& path, sead::ArchiveRes* archive)
    : mName(path), mHeap(sead::HeapMgr::instance()->getCurrentHeap()) {
    mArchive = archive;
    mDevice = new sead::ArchiveFileDevice(mArchive);
}

bool Resource::isExistFile(const sead::SafeString& filePath) const {
    bool exists = false;
    mDevice->tryIsExistFile(&exists, filePath);

    return exists;
}

bool Resource::isExistByml(const char* filePath) const {
    StringTmp<0x80> filePathExt;
    filePathExt.copy(filePath);
    filePathExt.append(".byml");

    return isExistFile(filePathExt.cstr());
}

u32 Resource::getSize() const {
    return mArchive->getRawSize();
}

u32 Resource::getEntryNum(const sead::SafeString& directoryPath) const {
    sead::DirectoryHandle handle;

    if (!mDevice->tryOpenDirectory(&handle, directoryPath))
        return 0;

    u32 entriesRead = mDevice->readDirectory(&handle, (sead::DirectoryEntry*)sEntries, 0x1000);
    mDevice->tryCloseDirectory(&handle);

    return entriesRead;
}

void Resource::getEntryName(sead::BufferedSafeString* outName,
                            const sead::SafeString& directoryPath, u32 entryNum) const {
    sead::DirectoryHandle handle;

    mDevice->tryOpenDirectory(&handle, directoryPath);
    mDevice->readDirectory(&handle, (sead::DirectoryEntry*)sEntries, 0x1000);
    mDevice->tryCloseDirectory(&handle);

    outName->format("%s", sEntries[entryNum].name.cstr());
}

u32 Resource::getFileSize(const sead::SafeString& filePath) const {
    return mDevice->getFileSize(filePath);
}

const u8* Resource::getByml(const sead::SafeString& filePath) const {
    StringTmp<0x80> filePathExt;
    filePathExt.copy(filePath);
    filePathExt.append(".byml");

    return (const u8*)getFile(filePathExt);
}

const void* Resource::getFile(const sead::SafeString& filePath) const {
    return mArchive->getFile(filePath);
}

const u8* Resource::tryGetByml(const sead::SafeString& filePath) const {
    StringTmp<0x80> filePathExt;
    filePathExt.copy(filePath);
    filePathExt.append(".byml");

    if (!isExistFile(filePathExt.cstr()))
        return nullptr;

    return (const u8*)getFile(filePathExt);
}

const void* Resource::getKcl(const sead::SafeString& filePath) const {
    StringTmp<0x80> filePathExt;
    filePathExt.copy(filePath);
    filePathExt.append(".kcl");

    return getFile(filePathExt);
}

const void* Resource::tryGetKcl(const sead::SafeString& filePath) const {
    StringTmp<0x80> filePathExt;
    filePathExt.copy(filePath);
    filePathExt.append(".kcl");

    if (!isExistFile(filePathExt.cstr()))
        return nullptr;

    return getFile(filePathExt);
}

const void* Resource::getPa(const sead::SafeString& filePath) const {
    StringTmp<0x80> filePathExt = {"%s.pa", filePath.cstr()};

    return getFile(filePathExt);
}

const void* Resource::tryGetPa(const sead::SafeString& filePath) const {
    StringTmp<0x80> filePathExt;
    filePathExt.copy(filePath);
    filePathExt.append(".pa");

    if (!isExistFile(filePathExt.cstr()))
        return nullptr;

    return getFile(filePathExt);
}

const void* Resource::getOtherFile(const sead::SafeString& filePath) const {
    return getFile(filePath);
}

const char* Resource::getArchiveName() const {
    return getBaseName(mName.cstr());
}

bool Resource::tryCreateResGraphicsFile(const sead::SafeString& filePath,
                                        nn::g3d::ResFile* resFile) {
    if (mResFile)
        return false;

    mResFile = nn::g3d::ResFile::ResCast((void*)getFile(filePath));
    if (resFile)
        agl::g3d::ResFile::BindTexture(mResFile, resFile);
    agl::g3d::ResFile::Setup(mResFile);
    agl::g3d::ResFile::BindTexture(mResFile, mResFile);

    return true;
}

void Resource::cleanupResGraphicsFile() {
    if (!mResFile)
        return;

    mResFile->ReleaseTexture();
    mResFile->Reset();
    agl::g3d::ResFile::Cleanup(mResFile);

    mResFile = nullptr;
}
}  // namespace al
