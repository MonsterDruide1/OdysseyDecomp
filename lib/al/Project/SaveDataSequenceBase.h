#pragma once

#include <basis/seadTypes.h>

namespace al {

class SaveDataSequenceBase {
public:
    SaveDataSequenceBase();

    virtual s32 threadFunc(const char* fileName) = 0;
};

static_assert(sizeof(SaveDataSequenceBase) == 0x8);

}  // namespace al
