#pragma once

#include <basis/seadTypes.h>

namespace sead {
class DelegateThread;
class Thread;
}  // namespace sead

namespace al {
class FileEntryBase;

class FileLoaderThread {
public:
    FileLoaderThread(s32 priority);

    void threadFunction(sead::Thread* thread, s64 fileEntryPtr);
    void requestLoadFile(FileEntryBase* fileEntry);

    sead::DelegateThread* getThread() const { return mThread; }

private:
    sead::DelegateThread* mThread = nullptr;
};

}  // namespace al
