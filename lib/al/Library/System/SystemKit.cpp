#include "Library/System/SystemKit.h"

#include <heap/seadHeapMgr.h>
#include <resource/seadParallelSZSDecompressor.h>
#include <resource/seadResourceMgr.h>
#include <resource/seadSZSDecompressor.h>
#include <resource/seadSharcArchiveRes.h>

#include "Project/File/FileLoader.h"
#include "Project/Memory/MemorySystem.h"
#include "Project/Resource/ResourceSystem.h"
#include "Project/SaveData/SaveDataDirector.h"

namespace al {

SystemKit::SystemKit() = default;

void SystemKit::createFileLoader(s32 threadPriority) {
    mFileLoader = new FileLoader(threadPriority);
}

void SystemKit::createMemorySystem(sead::Heap* heap) {
    sead::ScopedCurrentHeapSetter setter(heap);
    mMemorySystem = new MemorySystem(heap);
}

void SystemKit::createResourceSystem(const char* archivePath, s32 threadPriority,
                                     s32 decompressDestinationSize, bool useSubCore) {
    sead::ResourceMgr::instance()->registerFactory(
        new sead::DirectResourceFactory<sead::SharcArchiveRes>(), "sarc");
    sead::ResourceMgr::instance()->registerFactory(
        new sead::DirectResourceFactory<sead::SharcArchiveRes>(), "aras");

    decompressDestinationSize =
        decompressDestinationSize >= 0 ? decompressDestinationSize : 0x400000;
    u8* decompressDestination = new (0x20) u8[decompressDestinationSize];

    sead::ResourceMgr* instance = sead::ResourceMgr::instance();
    if (threadPriority == -1) {
        instance->registerDecompressor(
            new sead::SZSDecompressor(decompressDestinationSize / 2, decompressDestination), "szs");
    } else {
        sead::CoreId mask = useSubCore ? sead::CoreId::cSub2 : sead::CoreId::cMain;
        instance->registerDecompressor(new sead::ParallelSZSDecompressor(
                                           decompressDestinationSize / 2, threadPriority, nullptr,
                                           decompressDestination, sead::CoreIdMask(mask)),
                                       "szs");
    }

    mResourceSystem = new ResourceSystem(archivePath);
}

void SystemKit::createSaveDataSystem(u32 workBufferSize, s32 threadPriority) {
    mSaveDataDirector = new SaveDataDirector(workBufferSize, threadPriority);
}

}  // namespace al
