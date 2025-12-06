#include "Project/SaveData/SaveDataFunction.h"

#include <codec/seadHashCRC32.h>

namespace al::SaveDataFunction {

SaveDataHeader* getSaveDataHeader(u8* buffer) {
    return (SaveDataHeader*)buffer;
}

const SaveDataHeader* getSaveDataHeader(const u8* buffer) {
    return (const SaveDataHeader*)buffer;
}

u32 calcSaveDataCheckSum(const u8* buffer) {
    return sead::HashCRC32::calcHash(buffer + 4, getSaveDataHeader(buffer)->fileSize - 4);
}

s32 makeInvalidResult() {
    return 0;
}

}  // namespace al::SaveDataFunction
