#pragma once

#include <basis/seadTypes.h>

#include "Project/SaveDataSequenceBase.h"

namespace al {

class SaveDataSequenceInitDir : public SaveDataSequenceBase {
public:
    // TODO: Rename parameter
    SaveDataSequenceInitDir(u8 a);

    s32 threadFunc(const char* filename) override;

    void start(u8* buffer, u32 bufferSize, u32 version);

private:
    u8* mBuffer = nullptr;
    u32 mBufferSize = 0;
    u32 mVersion = 0;
    u8 _18;
};

static_assert(sizeof(SaveDataSequenceInitDir) == 0x20);

}  // namespace al
