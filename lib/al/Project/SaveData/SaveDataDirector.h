#pragma once

#include <basis/seadTypes.h>
#include <prim/seadSafeString.h>

namespace al {
class SaveDataSequenceBase;
class SaveDataSequenceInitDir;
class SaveDataSequenceFormat;
class SaveDataSequenceRead;
class SaveDataSequenceWrite;
class AsyncFunctorThread;

class SaveDataDirector {
public:
    SaveDataDirector(u32 workBufferSize, s32 threadPriority);

    void threadFunc();
    void initCheckSaveData();
    bool requestInitSaveDir(const char*, u32, u32);
    bool initSaveDirSync(const char*, u32, u32);
    bool requestFormat(s32, s32);
    bool formatSync(s32, s32);
    bool requestRead(const char*, u32, u32);
    bool readSync(const char*, u32, u32);
    bool requestWrite(const char*, u32, u32, bool);
    bool requestFlush();
    bool writeSync(const char*, u32, u32);
    bool updateSequence();
    bool isDoneSequence() const;
    u8* getWorkBuffer();
    s32 getResult();

    SaveDataSequenceRead* getReadSequence() const { return mReadSequence; }

    bool isInitialized() const { return mIsInitialized; }

    s32 getFSErrorCode() const { return mFileSystemErrorCode; }

private:
    SaveDataSequenceBase* _0;
    SaveDataSequenceInitDir* mInitDirSequence;
    SaveDataSequenceFormat* mFormatSequence;
    SaveDataSequenceRead* mReadSequence;
    SaveDataSequenceWrite* mWriteSequence;
    bool mIsInitialized;
    bool _29;
    void* filler;
    u8* mWorkBuffer;
    sead::FixedSafeString<64> _40;
    AsyncFunctorThread* _98;
    s32 mResult;
    s32 mFileSystemErrorCode;
};

static_assert(sizeof(SaveDataDirector) == 0xa8);

}  // namespace al
