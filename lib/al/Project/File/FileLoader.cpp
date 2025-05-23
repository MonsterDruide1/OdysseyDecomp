#include "Project/File/FileLoader.h"

#include <filedevice/seadFileDeviceMgr.h>
#include <resource/seadArchiveRes.h>
#include <thread/seadDelegateThread.h>

#include "Library/Base/StringUtil.h"
#include "Library/File/FileUtil.h"
#include "Library/Yaml/ByamlIter.h"
#include "Project/ArchiveEntry.h"
#include "Project/ArchiveHolder.h"
#include "Project/File/FileLoaderThread.h"
#include "Project/File/SoundItemEntry.h"
#include "Project/File/SoundItemHolder.h"

namespace al {
static sead::FixedSafeString<256> sDeviceName{"main"};

FileLoader::FileLoader(s32 threadPriority) {
    mLoaderThread = new FileLoaderThread(threadPriority);
    mArchiveHolder = new ArchiveHolder();
    mSoundItemHolder = new SoundItemHolder();
    mFileDevice = sead::FileDeviceMgr::instance()->findDevice(sDeviceName);
}

u32 getFileList(sead::FixedSafeString<256> out[], s32 outSize, const char* path,
                bool isPathValid(const sead::DirectoryEntry&, const char*), const char* suffix) {
    u32 entriesRead;
    sead::FileDevice* mainFileDevice = sead::FileDeviceMgr::instance()->getMainFileDevice();
    sead::DirectoryHandle handle;
    sead::FileDevice* fileDevice = mainFileDevice->tryOpenDirectory(&handle, path);

    if (!fileDevice)
        return 0;

    u32 entryCount = 0;
    sead::DirectoryEntry entry;
    for (s32 i = 0; i < outSize; i++) {
        entriesRead = 0;
        fileDevice->tryReadDirectory(&entriesRead, &handle, &entry, 1);
        if (entriesRead == 0)
            break;
        if (isPathValid(entry, suffix)) {
            out[entryCount] = entry.name;
            entryCount++;
        }
    }

    fileDevice->tryCloseDirectory(&handle);
    return entryCount;
}

static bool isDirectoryValid(const sead::DirectoryEntry& entry, const char* suffix) {
    return entry.is_directory;
}

u32 FileLoader::listSubdirectories(sead::FixedSafeString<256> out[], s32 outSize,
                                   const char* path) {
    return getFileList(out, outSize, path, &isDirectoryValid, nullptr);
}

static bool isFileValid(const sead::DirectoryEntry& entry, const char* suffix) {
    if (!suffix)
        return !entry.is_directory;

    s32 suffixLen = strlen(suffix);
    if (entry.name.getBufferSize() < suffixLen)
        return false;

    const char* pathStr = entry.name.cstr();
    const char* pathSuffixStart = strrchr(pathStr, '.');
    const char* pathSuffix = "";
    if (pathSuffixStart && pathSuffixStart != pathStr)
        pathSuffix = pathSuffixStart + 1;

    return strcasecmp(pathSuffix, suffix) == 0;
}

u32 FileLoader::listFiles(sead::FixedSafeString<256> out[], s32 outSize, const char* path,
                          const char* suffix) {
    return getFileList(out, outSize, path, &isFileValid, suffix);
}

bool FileLoader::isExistFile(const sead::SafeString& path, sead::FileDevice* device) const {
    sead::FileDevice* fileDevice = getFileDevice(path, device);

    bool isExist = false;
    fileDevice->tryIsExistFile(&isExist, path);
    return isExist;
}

sead::FileDevice* FileLoader::getFileDevice(const sead::SafeString& path,
                                            sead::FileDevice* device) const {
    return device ?: mFileDevice;
}

bool FileLoader::isExistArchive(const sead::SafeString& path, sead::FileDevice* device) const {
    return isExistFile(path, device);
}

bool FileLoader::isExistDirectory(const sead::SafeString& path, sead::FileDevice* device) const {
    sead::FileDevice* fileDevice = getFileDevice(path, device);

    bool isExist = false;
    fileDevice->tryIsExistDirectory(&isExist, path);
    return isExist;
}

u32 FileLoader::getFileSize(const sead::SafeString& path, sead::FileDevice* device) const {
    sead::FileDevice* fileDevice = getFileDevice(path, device);

    u32 fileSize = 0;
    fileDevice->tryGetFileSize(&fileSize, path);
    return fileSize;
}

u8* FileLoader::loadFile(const sead::SafeString& path, s32 alignment, sead::FileDevice* device) {
    sead::FileDevice::LoadArg loadArg;
    loadArg.alignment = alignment;
    loadArg.path = path;

    return getFileDevice(path, device)->tryLoad(loadArg);
}

bool FileLoader::tryLoadFileToBuffer(const sead::SafeString& path, u8* buffer, u32 bufferSize,
                                     s32 alignment, sead::FileDevice* device) {
    sead::FileDevice::LoadArg loadArg;
    loadArg.path = path;
    loadArg.buffer = buffer;
    loadArg.buffer_size = bufferSize;
    loadArg.buffer_size_alignment = alignment;

    return getFileDevice(path, device)->tryLoad(loadArg) != nullptr;
}

sead::ArchiveRes* FileLoader::loadArchive(const sead::SafeString& path, sead::FileDevice* device) {
    return loadArchiveLocal(path, nullptr, device);
}

sead::ArchiveRes* FileLoader::loadArchiveLocal(const sead::SafeString& path, const char* suffix,
                                               sead::FileDevice* device) {
    ArchiveEntry* entry = mArchiveHolder->tryFindEntry(path);
    if (entry) {
        if (entry->getFileState() != FileState::IsLoadDone)
            entry->waitLoadDone();
    } else if (!sead::ThreadMgr::instance()->isMainThread()) {
        sead::Heap* heap = sead::HeapMgr::instance()->getCurrentHeap();
        entry = requestLoadArchive(path, heap, getFileDevice(path, device));
        entry->waitLoadDone();
    } else {
        sead::ResourceMgr::LoadArg loadArg;
        loadArg.device = getFileDevice(path, device);
        loadArg.path = path;
        loadArg.load_data_alignment = calcFileAlignment(path);
        loadArg.load_data_buffer_alignment = calcBufferSizeAlignment(path);
        sead::Resource* resource =
            sead::ResourceMgr::instance()->tryLoad(loadArg, suffix ? suffix : "sarc", nullptr);
        return sead::DynamicCast<sead::ArchiveRes>(resource);
    }

    return entry->getArchiveRes();
}

sead::ArchiveRes* FileLoader::loadArchiveWithExt(const sead::SafeString& path, const char* suffix,
                                                 sead::FileDevice* device) {
    return loadArchiveLocal(path, suffix, device);
}

bool FileLoader::tryRequestLoadArchive(const sead::SafeString& path, sead::Heap* heap,
                                       sead::FileDevice* device) {
    if (mArchiveHolder->tryFindEntry(path))
        return false;
    requestLoadArchive(path, heap, device);
    return true;
}

ArchiveEntry* FileLoader::requestLoadArchive(const sead::SafeString& path, sead::Heap* heap,
                                             sead::FileDevice* device) {
    ArchiveEntry* entry =
        mArchiveHolder->addNewLoadRequestEntry(path, heap, getFileDevice(path, device));
    mLoaderThread->requestLoadFile(entry);
    return entry;
}

bool FileLoader::loadSoundItem(u32 itemId, u32 unknown, IAudioResourceLoader* loader) {
    SoundItemEntry* entry = mSoundItemHolder->tryFindEntry(itemId, loader);
    if (!entry) {
        entry = requestLoadSoundItem(itemId, unknown, loader);
        entry->waitLoadDone();
    } else if (entry->getFileState() != FileState::IsLoadDone) {
        entry->waitLoadDone();
    }

    return entry->isLoadSuccess();
}

SoundItemEntry* FileLoader::requestLoadSoundItem(u32 itemId, u32 unknown,
                                                 IAudioResourceLoader* loader) {
    SoundItemEntry* entry = mSoundItemHolder->addNewLoadRequestEntry(itemId, unknown, loader);
    mLoaderThread->requestLoadFile(entry);
    return entry;
}

bool FileLoader::tryRequestLoadSoundItem(u32 itemId, IAudioResourceLoader* loader) {
    if (mSoundItemHolder->tryFindEntry(itemId, loader))
        return false;

    requestLoadSoundItem(itemId, -1, loader);
    return true;
}

void FileLoader::requestPreLoadFile(const ByamlIter& preLoadList, sead::Heap* heap,
                                    IAudioResourceLoader* loader) {
    if (mIsFilePreloaded)
        return;

    for (s32 i = 0; i < preLoadList.getSize(); i++) {
        ByamlIter iter;
        preLoadList.tryGetIterByIndex(&iter, i);

        const char* type;
        iter.tryGetStringByKey(&type, "Type");

        if (!isEqualString(type, "SoundItem") && isEqualString(type, "Archive")) {
            const char* path;
            iter.tryGetStringByKey(&path, "Path");
            tryRequestLoadArchive(path, heap, nullptr);
        }
    }

    mIsFilePreloaded = true;
}

void FileLoader::waitLoadDoneAllFile() {
    mArchiveHolder->waitLoadDoneAll();
}

void FileLoader::clearAllEntry() {
    mArchiveHolder->clearEntry();
    mSoundItemHolder->clearEntry();
    mIsFilePreloaded = false;
}

void FileLoader::setThreadPriority(s32 priority) {
    mLoaderThread->getThread()->setPriority(priority);
}

}  // namespace al
