#pragma once

#include <basis/seadTypes.h>

#include "Project/SaveDataSequenceBase.h"

namespace sead {
class FileDevice;
}

namespace al {

class SaveDataSequenceWrite : public SaveDataSequenceBase {
public:
    // TODO: Rename parameter
    SaveDataSequenceWrite(u8 a);

    s32 threadFunc(const char* fileName) override;

    void start(u8* buffer, u32 bufferSize, u32 version, bool isFlushNeeded);
    void startFlushOnly();
    bool write(sead::FileDevice* fileDevice, const char* fileName);

private:
    u8* mBuffer = nullptr;
    u32 mBufferSize = 0;
    bool mIsWriteNeeded = false;
    bool mIsFlushNeeded = false;
    u8 _16 = 0;
};

static_assert(sizeof(SaveDataSequenceWrite) == 0x18);

}  // namespace al
