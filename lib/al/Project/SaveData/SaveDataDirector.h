#pragma once

#include <basis/seadTypes.h>

namespace al {

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

private:
    void* _0[0xa8 / 8];
};

static_assert(sizeof(SaveDataDirector) == 0xa8);

}  // namespace al
