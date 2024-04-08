#pragma once

#include <heap/seadHeap.h>

namespace al {
class FileLoader;
class MemorySystem;
class ResourceSystem;
class SaveDataDirector;

class SystemKit {
public:
    SystemKit();

    void createFileLoader(s32 threadPriority);
    void createMemorySystem(sead::Heap* heap);
    void createResourceSystem(const char* archivePath, s32, s32, bool);
    void createSaveDataSystem();

    MemorySystem* getMemorySystem() { return mMemorySystem; }
    FileLoader* getFileLoader() { return mFileLoader; }
    ResourceSystem* getResourceSystem() { return mResourceSystem; }
    SaveDataDirector* getSaveDataDirector() { return mSaveDataDirector; }

protected:
    friend class alProjectInterface;

private:
    MemorySystem* mMemorySystem;
    FileLoader* mFileLoader;
    ResourceSystem* mResourceSystem;
    SaveDataDirector* mSaveDataDirector;
};
}  // namespace al
