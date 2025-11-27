#pragma once

#include <basis/seadTypes.h>

#include "Project/SaveDataSequenceBase.h"

namespace sead {
class FileDevice;
}

namespace al {

class SaveDataSequenceRead : public SaveDataSequenceBase {
public:
    // TODO: Rename parameter
    SaveDataSequenceRead(u8 a);

    s32 threadFunc(const char* fileName) override;

    void start(u8* buffer, u32 bufferSize, u32 version);
    s32 read(sead::FileDevice* fileDevice, const char* fileName, u32* readSize);

    bool isCorrupted() const { return mIsCorrupted; }

private:
    u8* mBuffer = nullptr;
    u32 mBufferSize = 0;
    u32 mVersion = 0;
    bool mIsCorrupted = false;
    u8 _19 = 0;
};

static_assert(sizeof(SaveDataSequenceRead) == 0x20);

}  // namespace al
