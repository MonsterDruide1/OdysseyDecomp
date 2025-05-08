#include "Library/File/FileUtil.h"

#include <filedevice/seadPath.h>

#include "Library/Base/StringUtil.h"
#include "Library/Memory/HeapUtil.h"
#include "Library/Message/MessageHolder.h"
#include "Library/Resource/Resource.h"
#include "Library/System/SystemKit.h"
#include "Library/Yaml/ByamlIter.h"
#include "Project/File/FileLoader.h"

#include "System/ProjectInterface.h"

namespace al {
inline FileLoader* getFileLoader() {
    return alProjectInterface::getSystemKit()->getFileLoader();
}

bool isExistFile(const sead::SafeString& fileName) {
    return getFileLoader()->isExistFile(fileName, nullptr);
}

bool isExistDirectory(const sead::SafeString& fileName) {
    return getFileLoader()->isExistDirectory(fileName, nullptr);
}

bool isExistArchive(const sead::SafeString& fileName) {
    return getFileLoader()->isExistArchive(StringTmp<256>("%s.szs", fileName.cstr()), nullptr);
}

bool isExistArchive(const sead::SafeString& fileName, const char* ext) {
    return getFileLoader()->isExistArchive(StringTmp<256>("%s.%s", fileName.cstr(), ext), nullptr);
}

u32 getFileSize(const sead::SafeString& fileName) {
    return getFileLoader()->getFileSize(fileName, nullptr);
}

u32 calcFileAlignment(const sead::SafeString& fileName) {
    StringTmp<32> ext("");
    sead::Path::getExt(&ext, fileName);

    return isEqualString("aras", ext.cstr()) ? 0x1000 : 0;
}

u32 calcBufferSizeAlignment(const sead::SafeString& fileName) {
    StringTmp<32> ext("");
    sead::Path::getExt(&ext, fileName);

    return isEqualString("aras", ext.cstr()) ? 0x1000 : 0;
}

u8* loadFile(const sead::SafeString& fileName, s32 size) {
    return getFileLoader()->loadFile(fileName, size, nullptr);
}

bool tryLoadFileToBuffer(const sead::SafeString& fileName, u8* buffer, u32 bufferSize, s32 size) {
    return getFileLoader()->tryLoadFileToBuffer(fileName, buffer, bufferSize, size, nullptr);
}

sead::ArchiveRes* loadArchive(const sead::SafeString& fileName) {
    return getFileLoader()->loadArchive(StringTmp<256>("%s.szs", fileName.cstr()), nullptr);
}

sead::ArchiveRes* loadArchiveWithExt(const sead::SafeString& fileName, const char* ext) {
    return getFileLoader()->loadArchiveWithExt(StringTmp<256>("%s.%s", fileName.cstr(), ext), ext,
                                               nullptr);
}

bool tryRequestLoadArchive(const sead::SafeString& fileName, sead::Heap* heap) {
    return getFileLoader()->tryRequestLoadArchive(StringTmp<256>("%s.szs", fileName.cstr()), heap,
                                                  nullptr);
}

void loadSoundItem(u32 soundEntryId, u32 unknown, IAudioResourceLoader* resLoader) {
    getFileLoader()->loadSoundItem(soundEntryId, unknown, resLoader);
}

bool tryRequestLoadSoundItem(u32 soundEntryId) {
    return false;
}

bool tryRequestPreLoadFile(const Resource* res, s32 id, sead::Heap* heap,
                           IAudioResourceLoader* resLoader) {
    return tryRequestPreLoadFile(res, StringTmp<256>("PreLoadFileList%d", id), heap, resLoader);
}

bool tryRequestPreLoadFile(const Resource* res, const sead::SafeString& fileName, sead::Heap* heap,
                           IAudioResourceLoader* resLoader) {
    if (!res->isExistFile(StringTmp<256>("%s.byml", fileName.cstr())))
        return false;

    ByamlIter byml = res->getByml(fileName);
    FileLoader* fileLoader = getFileLoader();
    if (!heap)
        heap = getSceneResourceHeap();

    fileLoader->requestPreLoadFile(byml, heap, resLoader);
    return true;
}

void waitLoadDoneAllFile() {
    getFileLoader()->waitLoadDoneAllFile();
}

void clearFileLoaderEntry() {
    getFileLoader()->clearAllEntry();
}

void makeLocalizedArchivePath(sead::BufferedSafeString* outPath, const sead::SafeString& fileName) {
    outPath->format("LocalizedData/%s/%s", getLanguage(), fileName.cstr());
}

void makeLocalizedArchivePathByCountryCode(sead::BufferedSafeString* outPath,
                                           const sead::SafeString& fileName) {
    outPath->format("LocalizedData/%s/%s", "EuEnglish", fileName.cstr());
}

void setFileLoaderThreadPriority(s32 priority) {
    getFileLoader()->setThreadPriority(priority);
}
}  // namespace al
