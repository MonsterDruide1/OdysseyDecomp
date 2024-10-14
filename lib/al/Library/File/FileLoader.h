#pragma once

#include <prim/seadSafeString.hpp>
#include <thread/seadCriticalSection.h>

namespace sead {
class FileDevice;
}

namespace al {
class IAudioResourceLoader;
class ByamlIter;
class SoundItemHolder;
}  // namespace al

namespace al {

class FileLoaderThread;
class ArchiveHolder;

class FileLoader {
public:
    FileLoader(s32 threadPriority);
    void listSubdirectories(sead::FixedSafeString<256>* out, s32 unk,
                            const char* compareToInCallback);
    void listFiles(sead::FixedSafeString<256>* out, s32 unk, const char* compareToInCallback,
                   const char* unused);
    void isExistFile(const sead::SafeString& path, sead::FileDevice* device) const;
    sead::FileDevice* getFileDevice(const sead::SafeString& unused,
                                    sead::FileDevice* maybeNullDevice) const;
    void isExistArchive(const sead::SafeString& path, sead::FileDevice* device) const;
    void isExistDirectory(const sead::SafeString& path, sead::FileDevice* device) const;
    u32 getFileSize(const sead::SafeString& path, sead::FileDevice* device) const;
    void loadfile(const sead::SafeString&, s32, sead::FileDevice*);
    void tryLoadFileToBuffer(const sead::SafeString&, u8*, u32, s32, sead::FileDevice*);
    void loadArchive(const sead::SafeString& path, sead::FileDevice* device);
    void loadArchiveLocal(const sead::SafeString& path, const char* suffix,
                          sead::FileDevice* device);
    void loadArchiveWithExt(const sead::SafeString& path, const char* suffix,
                            sead::FileDevice* device);
    bool tryRequestLoadArchive(const sead::SafeString& path, sead::Heap* heap,
                               sead::FileDevice* device);
    void requestLoadArchive(const sead::SafeString& path, sead::Heap* heap,
                            sead::FileDevice* device);
    void loadSoundItem(u32 soundEntryId, u32 unused, IAudioResourceLoader* loader);
    void requestLoadSoundItem(u32 unused, u32 unused2, IAudioResourceLoader* unused3);
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
