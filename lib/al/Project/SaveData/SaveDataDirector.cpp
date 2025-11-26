#include "Project/SaveData/SaveDataDirector.h"

#include "Library/Thread/AsyncFunctorThread.h"
#include "Project/SaveData/SaveDataFunction.h"
#include "Project/SaveData/SaveDataSequenceFormat.h"
#include "Project/SaveData/SaveDataSequenceInitDir.h"
#include "Project/SaveData/SaveDataSequenceRead.h"
#include "Project/SaveData/SaveDataSequenceWrite.h"

namespace al {
SaveDataDirector::SaveDataDirector(u32 workBufferSize, s32 threadPriority) {
    using SaveDataDirectorFunctor = FunctorV0M<SaveDataDirector*, void (SaveDataDirector::*)()>;

    mInitDirSequence = new SaveDataSequenceInitDir(0);
    mFormatSequence = new SaveDataSequenceFormat();
    mReadSequence = new SaveDataSequenceRead(0);
    mWriteSequence = new SaveDataSequenceWrite(0);

    u32 bufferSize = workBufferSize + sizeof(SaveDataFunction::SaveDataHeader);
    mBuffer.tryAllocBuffer(bufferSize, nullptr);

    mSaveDataThread = new AsyncFunctorThread(
        "セーブデータスレッド", SaveDataDirectorFunctor{this, &SaveDataDirector::threadFunc},
        threadPriority, 0x2000, sead::CoreId::cMain);
    initCheckSaveData();
}

void SaveDataDirector::threadFunc() {
    mResult = mRunningSequence->threadFunc(mCurrentFileName.cstr());
}

void SaveDataDirector::initCheckSaveData() {
    mIsInitialized = false;
}

bool SaveDataDirector::requestInitSaveDir(const char* fileName, u32 dirSize, u32 version) {
    mCurrentFileName = fileName;
    mInitDirSequence->start(mBuffer.getBufferPtr(),
                            dirSize + sizeof(SaveDataFunction::SaveDataHeader), version);
    mRunningSequence = mInitDirSequence;

    mResult = SaveDataFunction::makeInvalidResult();
    mIsInitialized = true;
    mSaveDataThread->start();
    return true;
}

bool SaveDataDirector::initSaveDirSync(const char* fileName, u32 dirSize, u32 version) {
    mCurrentFileName = fileName;
    mInitDirSequence->start(mBuffer.getBufferPtr(),
                            dirSize + sizeof(SaveDataFunction::SaveDataHeader), version);
    mRunningSequence = mInitDirSequence;

    mResult = SaveDataFunction::makeInvalidResult();
    mIsInitialized = true;
    threadFunc();
    mRunningSequence = nullptr;
    return true;
}

bool SaveDataDirector::requestFormat(s32 a, s32 b) {
    mFormatSequence->start(a, b);
    mRunningSequence = mFormatSequence;

    mResult = SaveDataFunction::makeInvalidResult();
    mSaveDataThread->start();
    return true;
}

bool SaveDataDirector::formatSync(s32 a, s32 b) {
    mFormatSequence->start(a, b);
    mRunningSequence = mFormatSequence;

    mResult = SaveDataFunction::makeInvalidResult();
    threadFunc();
    mRunningSequence = nullptr;
    return true;
}

bool SaveDataDirector::requestRead(const char* fileName, u32 readSize, u32 version) {
    if (_29)
        return true;

    mCurrentFileName = fileName;
    mReadSequence->start(mBuffer.getBufferPtr(),
                         readSize + sizeof(SaveDataFunction::SaveDataHeader), version);
    mRunningSequence = mReadSequence;

    mResult = SaveDataFunction::makeInvalidResult();
    mSaveDataThread->start();
    return true;
}

bool SaveDataDirector::readSync(const char* fileName, u32 readSize, u32 version) {
    if (_29)
        return true;

    mCurrentFileName = fileName;
    mReadSequence->start(mBuffer.getBufferPtr(),
                         readSize + sizeof(SaveDataFunction::SaveDataHeader), version);
    mRunningSequence = mReadSequence;

    mResult = SaveDataFunction::makeInvalidResult();
    threadFunc();
    mRunningSequence = nullptr;
    return true;
}

bool SaveDataDirector::requestWrite(const char* fileName, u32 writeSize, u32 version,
                                    bool isFlushNeeded) {
    if (_29)
        return true;

    mCurrentFileName = fileName;
    mWriteSequence->start(mBuffer.getBufferPtr(),
                          writeSize + sizeof(SaveDataFunction::SaveDataHeader), version,
                          isFlushNeeded);
    mRunningSequence = mWriteSequence;

    mResult = SaveDataFunction::makeInvalidResult();
    mSaveDataThread->start();
    return true;
}

bool SaveDataDirector::requestFlush() {
    if (_29)
        return true;

    mWriteSequence->startFlushOnly();
    mRunningSequence = mWriteSequence;

    mResult = SaveDataFunction::makeInvalidResult();
    mSaveDataThread->start();
    return true;
}

bool SaveDataDirector::writeSync(const char* fileName, u32 writeSize, u32 version) {
    if (_29)
        return true;

    mCurrentFileName = fileName;
    mWriteSequence->start(mBuffer.getBufferPtr(),
                          writeSize + sizeof(SaveDataFunction::SaveDataHeader), version, true);
    mRunningSequence = mWriteSequence;

    mResult = SaveDataFunction::makeInvalidResult();
    threadFunc();
    mRunningSequence = nullptr;
    return true;
}

bool SaveDataDirector::updateSequence() {
    if (_29)
        return true;

    if (!mSaveDataThread->isDone())
        return false;

    mRunningSequence = nullptr;
    return true;
}

bool SaveDataDirector::isDoneSequence() const {
    return _29 || !mRunningSequence;
}

u8* SaveDataDirector::getWorkBuffer() {
    return mBuffer.getBufferPtr() + sizeof(SaveDataFunction::SaveDataHeader);
}

s32 SaveDataDirector::getResult() {
    return mResult;
}
}  // namespace al
