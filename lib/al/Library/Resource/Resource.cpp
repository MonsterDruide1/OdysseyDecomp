#include "Library/Resource/Resource.h"

#include <g3d/aglNW4FToNN.h>
#include <heap/seadHeap.h>

#include "Library/File/FileUtil.h"

const sead::DirectoryEntry sEntries[0x1000];

namespace al {
Resource::Resource(const sead::SafeString& path) : mName(path) {
    mArchive = loadArchive(path);
    mDevice = new sead::ArchiveFileDevice(mArchive);
}

Resource::Resource(const sead::SafeString& path, sead::ArchiveRes* archive) : mName(path) {
    mArchive = archive;
    mDevice = new sead::ArchiveFileDevice(mArchive);
}

bool Resource::isExistFile(const sead::SafeString& path) const {
    bool exists = false;
    mDevice->tryIsExistFile(&exists, path);

    return exists;
}

// bool Resource::isExistByml(const char*) const {}

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

void Resource::cleanupResGraphicsFile() {
    if (!mResFile)
        return;

    mResFile->ReleaseTexture();
    mResFile->Reset();
    agl::g3d::ResFile::Cleanup(mResFile);
    mResFile = nullptr;
}
}  // namespace al
