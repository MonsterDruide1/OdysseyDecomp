#pragma once

#include <heap/seadHeap.h>

namespace al {
class FileLoader;
class ResourceSystem;
class SaveDataDirector;
class MemorySystem;

class SystemKit {
public:
    SystemKit();

    void createFileLoader(s32 threadPriority);
    void createMemorySystem(sead::Heap* heap);
    void createResourceSystem(const char* archivePath, s32 threadPriority,
                              s32 decompressDestinationSize, bool useSubCore);
    void createSaveDataSystem(u32 workBufferSize, s32 threadPriority);

    MemorySystem* getMemorySystem() { return mMemorySystem; }

    FileLoader* getFileLoader() { return mFileLoader; }

    ResourceSystem* getResourceSystem() { return mResourceSystem; }

    SaveDataDirector* getSaveDataDirector() { return mSaveDataDirector; }

protected:
    friend class alProjectInterface;

private:
    MemorySystem* mMemorySystem = nullptr;
    FileLoader* mFileLoader = nullptr;
    ResourceSystem* mResourceSystem = nullptr;
    SaveDataDirector* mSaveDataDirector = nullptr;
};

static_assert(sizeof(SystemKit) == 0x20);
}  // namespace al
