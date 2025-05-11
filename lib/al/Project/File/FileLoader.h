#pragma once

#include <prim/seadSafeString.hpp>
#include <thread/seadCriticalSection.h>

namespace sead {
class ArchiveRes;
struct DirectoryEntry;
class FileDevice;
}  // namespace sead

namespace al {
class ArchiveEntry;
class ArchiveHolder;
class ByamlIter;
class FileEntryBase;
class FileLoaderThread;
class IAudioResourceLoader;
class SoundItemEntry;
class SoundItemHolder;

class FileLoader {
public:
    FileLoader(s32 threadPriority);

    static u32 listSubdirectories(sead::FixedSafeString<256>* out, s32 outSize, const char* path);
    static u32 listFiles(sead::FixedSafeString<256>* out, s32 outSize, const char* path,
                         const char* suffix);

    bool isExistFile(const sead::SafeString& path, sead::FileDevice* device) const;
    sead::FileDevice* getFileDevice(const sead::SafeString& path, sead::FileDevice* device) const;
    bool isExistArchive(const sead::SafeString& path, sead::FileDevice* device) const;
    bool isExistDirectory(const sead::SafeString& path, sead::FileDevice* device) const;
    u32 getFileSize(const sead::SafeString& path, sead::FileDevice* device) const;
    u8* loadFile(const sead::SafeString&, s32, sead::FileDevice*);
    bool tryLoadFileToBuffer(const sead::SafeString& path, u8* buffer, u32 bufferSize,
                             s32 alignment, sead::FileDevice* device);
    sead::ArchiveRes* loadArchive(const sead::SafeString& path, sead::FileDevice* device);
    sead::ArchiveRes* loadArchiveLocal(const sead::SafeString& path, const char* suffix,
                                       sead::FileDevice* device);
    sead::ArchiveRes* loadArchiveWithExt(const sead::SafeString& path, const char* suffix,
                                         sead::FileDevice* device);
    bool tryRequestLoadArchive(const sead::SafeString& path, sead::Heap* heap,
                               sead::FileDevice* device);
    ArchiveEntry* requestLoadArchive(const sead::SafeString& path, sead::Heap* heap,
                                     sead::FileDevice* device);
    // TODO: unknown parameter name
    bool loadSoundItem(u32 itemId, u32 unknown, IAudioResourceLoader* loader);
    // TODO: unknown parameter name
    SoundItemEntry* requestLoadSoundItem(u32 itemId, u32 unknown, IAudioResourceLoader* loader);
    bool tryRequestLoadSoundItem(u32 itemId, IAudioResourceLoader* loader);
    void requestPreLoadFile(const ByamlIter& preLoadList, sead::Heap* heap,
                            IAudioResourceLoader* loader);
    void waitLoadDoneAllFile();
    void clearAllEntry();
    void setThreadPriority(s32 priority);

private:
    FileLoaderThread* mLoaderThread = nullptr;
    ArchiveHolder* mArchiveHolder = nullptr;
    SoundItemHolder* mSoundItemHolder = nullptr;
    bool mIsFilePreloaded = false;
    sead::FileDevice* mFileDevice = nullptr;
    sead::CriticalSection mCriticalSection;
};

static_assert(sizeof(FileLoader) == 0x68, "FileLoader size");

}  // namespace al
