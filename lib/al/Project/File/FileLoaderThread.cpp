#include "Project/File/FileLoaderThread.h"

#include <thread/seadDelegateThread.h>
#include <thread/seadMessageQueue.h>

#include "Project/FileEntryBase.h"

namespace al {

FileLoaderThread::FileLoaderThread(s32 priority) {
    mThread = new sead::DelegateThread("FileLoadThread",
                                       new sead::Delegate2<FileLoaderThread, sead::Thread*, s64>(
                                           this, &FileLoaderThread::threadFunction),
                                       nullptr, priority, sead::MessageQueue::BlockType::Blocking,
                                       0x7fffffff, 0x4000, 0x800);
    mThread->start();
}

void FileLoaderThread::threadFunction(sead::Thread* thread, s64 fileEntryPtr) {
    FileEntryBase* fileEntry = (FileEntryBase*)fileEntryPtr;
    fileEntry->load();
}

void FileLoaderThread::requestLoadFile(FileEntryBase* fileEntry) {
    mThread->sendMessage((s64)fileEntry, sead::MessageQueue::BlockType::NonBlocking);
}

}  // namespace al
