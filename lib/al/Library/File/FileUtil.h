#pragma once

#include <basis/seadTypes.h>
#include <prim/seadSafeString.h>

namespace sead {
class ArchiveRes;
class Heap;
}  // namespace sead

namespace al {
class IAudioResourceLoader;
class Resource;

bool isExistFile(const sead::SafeString& fileName);
bool isExistDirectory(const sead::SafeString& fileName);
bool isExistArchive(const sead::SafeString& fileName);
bool isExistArchive(const sead::SafeString& fileName, const char* ext);
u32 getFileSize(const sead::SafeString& fileName);
u32 calcFileAlignment(const sead::SafeString& fileName);
u32 calcBufferSizeAlignment(const sead::SafeString& fileName);
u8* loadFile(const sead::SafeString& fileName, s32 size);
bool tryLoadFileToBuffer(const sead::SafeString& fileName, u8* buffer, u32 bufferSize, s32 size);
sead::ArchiveRes* loadArchive(const sead::SafeString& fileName);
sead::ArchiveRes* loadArchiveWithExt(const sead::SafeString& fileName, const char* ext);
bool tryRequestLoadArchive(const sead::SafeString& fileName, sead::Heap* heap);

// TODO: Replace unknown with a proper name
void loadSoundItem(u32 soundEntryId, u32 unknown, IAudioResourceLoader* resLoader);

bool tryRequestLoadSoundItem(u32 soundEntryId);
bool tryRequestPreLoadFile(const Resource* res, s32 id, sead::Heap* heap,
                           IAudioResourceLoader* resLoader);
bool tryRequestPreLoadFile(const Resource* res, const sead::SafeString& fileName, sead::Heap* heap,
                           IAudioResourceLoader* resLoader);
void waitLoadDoneAllFile();
void clearFileLoaderEntry();
void makeLocalizedArchivePath(sead::BufferedSafeString* outPath, const sead::SafeString& fileName);
void makeLocalizedArchivePathByCountryCode(sead::BufferedSafeString* outPath,
                                           const sead::SafeString& fileName);
void setFileLoaderThreadPriority(s32 priority);
}  // namespace al
