#pragma once

#include <basis/seadTypes.h>

namespace al::SaveDataFunction {

struct SaveDataHeader {
    u32 checkSum;
    u32 version;
    u32 fileSize;
};

static_assert(sizeof(SaveDataHeader) == 0xc);

SaveDataHeader* getSaveDataHeader(u8*);
const SaveDataHeader* getSaveDataHeader(const u8*);
u32 calcSaveDataCheckSum(const u8*);
s32 makeInvalidResult();

}  // namespace al::SaveDataFunction
