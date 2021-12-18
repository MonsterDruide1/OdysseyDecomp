#include "al/byaml/ByamlStringTableIter.h"
#include "byteswap.h"

namespace al {
ByamlStringTableIter::ByamlStringTableIter() = default;

ByamlStringTableIter::ByamlStringTableIter(const u8* data, bool isRev)
    : mData(data), isRev(isRev) {}

int ByamlStringTableIter::getSize() const {
    u32 type_and_size = *reinterpret_cast<const u32*>(mData);
    return isRev ? bswap_32_ignore_last(type_and_size) : type_and_size >> 8;
}

const u32* ByamlStringTableIter::getAddressTable() const {
    // mData is an integer pointer, so getting to the table is just increasing the pointer by 1
    // (which is + 4)
    return reinterpret_cast<const u32*>(mData + 4);
}

int ByamlStringTableIter::getStringAddress(int idx) const {
    if (isRev)
        return bswap_32(getAddressTable()[idx]);

    return getAddressTable()[idx];
}

bool ByamlStringTableIter::isValidate() const {
    return mData != nullptr;
}
}  // namespace al
