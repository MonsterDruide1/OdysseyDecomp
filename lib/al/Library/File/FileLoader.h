#pragma once

#include <prim/seadSafeString.hpp>
#include <thread/seadCriticalSection.h>

namespace sead {
class FileDevice;
class Resource;
}  // namespace sead

namespace al {
class IAudioResourceLoader;
class ByamlIter;
class SoundItemHolder;
class FileEntryBase;
}  // namespace al

namespace al {

class FileLoaderThread;
class ArchiveHolder;

class FileLoader {
public:
    FileLoader(s32 threadPriority);
    s32 listSubdirectories(sead::FixedSafeString<256>* out, s32 unk,
                           const char* compareToInCallback);
    s32 listFiles(sead::FixedSafeString<256>* out, s32 unk, const char* compareToInCallback,
                  const char* unused);
    bool isExistFile(const sead::SafeString& path, sead::FileDevice* device) const;
    sead::FileDevice* getFileDevice(const sead::SafeString& unused,
                                    sead::FileDevice* maybeNullDevice) const;
    bool isExistArchive(const sead::SafeString& path, sead::FileDevice* device) const;
    bool isExistDirectory(const sead::SafeString& path, sead::FileDevice* device) const;
    u32 getFileSize(const sead::SafeString& path, sead::FileDevice* device) const;
    u8* loadfile(const sead::SafeString&, s32, sead::FileDevice*);
    bool tryLoadFileToBuffer(const sead::SafeString& path, u8* buffer, u32, s32,
                             sead::FileDevice* device);
    sead::Resource* loadArchive(const sead::SafeString& path, sead::FileDevice* device);
    sead::Resource* loadArchiveLocal(const sead::SafeString& path, const char* suffix,
                                     sead::FileDevice* device);
    sead::Resource* loadArchiveWithExt(const sead::SafeString& path, const char* suffix,
                                      sead::FileDevice* device);
    bool tryRequestLoadArchive(const sead::SafeString& path, sead::Heap* heap,
                               sead::FileDevice* device);
    FileEntryBase* requestLoadArchive(const sead::SafeString& path, sead::Heap* heap,
                                      sead::FileDevice* device);
    bool loadSoundItem(u32 soundEntryId, u32 unused, IAudioResourceLoader* loader);
    FileEntryBase* requestLoadSoundItem(u32 unused, u32 unused2, IAudioResourceLoader* unused3);
    bool tryRequestLoadSoundItem(u32 soundEntryId, IAudioResourceLoader* loader);
    void requestPreLoadFile(const ByamlIter& preLoadList, sead::Heap* heap,
                            IAudioResourceLoader* loader);
    void waitLoadDoneAllFile();
    void clearAllEntry();
    void setThreadPriority(s32 priority);

private:
    FileLoaderThread* mLoaderThread;
    ArchiveHolder* mArchiveHolder;
    SoundItemHolder* mSoundItemHolder;
    void* _18;
    sead::FileDevice* mFileDevice;
    sead::CriticalSection mCriticalSection;
};
}  // namespace al

static_assert(sizeof(al::FileLoader) == 0x68, "al::FileLoader size");
