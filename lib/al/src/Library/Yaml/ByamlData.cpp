#include "Library/Yaml/ByamlData.h"

#include <byteswap.h>

namespace al {

ByamlData::ByamlData() = default;

void ByamlData::set(const ByamlHashPair* hash_pair, bool isRev) {
    mType = hash_pair->getType();
    mValue = isRev ? bswap_32(hash_pair->getValue(false)) : hash_pair->getValue(false);
}

void ByamlData::set(u8 type, u32 value, bool isRev) {
    mType = (ByamlDataType)type;
    mValue = isRev ? bswap_32(value) : value;
}

ByamlDataType ByamlData::getType() const {
    return mType;
}

u32 ByamlData::getValue() const {
    return mValue;
}

s32 ByamlHashPair::getKey(bool isRev) const {
    return isRev ? bswap_24(mData) : mData & 0xFFFFFF;
}

ByamlDataType ByamlHashPair::getType() const {
    return (ByamlDataType)(mData >> 24);
}

s32 ByamlHashPair::getValue(bool isRev) const {
    return isRev ? bswap_32(mValue) : mValue;
}

ByamlHashIter::ByamlHashIter(const u8* data, bool isRev_) : mData(data), isRev(isRev_) {}
ByamlHashIter::ByamlHashIter() : mData(nullptr), isRev(false) {}

// NON_MATCHING: mismatch during bound calculation
const ByamlHashPair* ByamlHashIter::findPair(s32 key) const {
    const ByamlHashPair* pairTable = getPairTable();
    if (!mData)
        return nullptr;

    s32 lowerBound = 0;
    s32 upperBound = getSize();
    while (lowerBound < upperBound) {
        s32 avg = (lowerBound + upperBound) / 2;
        const ByamlHashPair* pair = &pairTable[avg];
        s32 result = key - pair->getKey(isRev);
        if (result == 0) {
            return pair;
        }

        if (result > 0)
            lowerBound = avg + 1;
        else
            upperBound = avg;
    }
    return nullptr;
}
bool ByamlHashIter::getDataByIndex(ByamlData* data, s32 index) const {
    if (!mData)
        return false;
    if (((s32)getSize()) < 1)
        return false;

    data->set(&getPairTable()[index], isRev);
    return true;
}
// NON_MATCHING: missing cbz on matching result
bool ByamlHashIter::getDataByKey(ByamlData* data, s32 key) const {
    if (!mData)
        return false;
    if (((s32)getSize()) < 1)
        return false;

    s32 lowerBound = 0;
    s32 upperBound = getSize();
    while (lowerBound < upperBound) {
        s32 avg = (lowerBound + upperBound) / 2;
        const ByamlHashPair* pair = &getPairTable()[avg];
        s32 result = key - pair->getKey(isRev);
        if (result == 0) {
            data->set(pair, isRev);
            return true;
        }

        if (result > 0)
            lowerBound = avg + 1;
        else
            upperBound = avg;
    }
    return false;
}
const u8* ByamlHashIter::getOffsetData(u32 off) const {
    return &mData[off];
}
const ByamlHashPair* ByamlHashIter::getPairByIndex(s32 index) const {
    if (index < 0)
        return nullptr;
    if (((s32)getSize()) <= index)
        return nullptr;

    return &getPairTable()[index];
}
const ByamlHashPair* ByamlHashIter::getPairTable() const {
    if (!mData)
        return nullptr;
    return reinterpret_cast<const ByamlHashPair*>(mData + 4);
}
u32 ByamlHashIter::getSize() const {
    if (!mData)
        return 0;
    u32 val = *reinterpret_cast<const u32*>(mData);
    return isRev ? bswap_24(val >> 8) : val >> 8;
}

ByamlArrayIter::ByamlArrayIter(const u8* data, bool isRev_) : mData(data), isRev(isRev_) {}
ByamlArrayIter::ByamlArrayIter() : mData(nullptr), isRev(false) {}

bool ByamlArrayIter::getDataByIndex(ByamlData* data, s32 index) const {
    if (index < 0)
        return false;
    if (((s32)getSize()) <= index)
        return false;

    data->set(getTypeTable()[index], getDataTable()[index], isRev);
    return true;
}
// NON_MATCHING: regalloc
const u32* ByamlArrayIter::getDataTable() const {
    return reinterpret_cast<const u32*>(getOffsetData((getSize() + 7) & 0xFFFFFFFC));
}
const u8* ByamlArrayIter::getOffsetData(u32 off) const {
    return &mData[off];
}
u32 ByamlArrayIter::getSize() const {
    u32 val = *reinterpret_cast<const u32*>(mData);
    return isRev ? bswap_24(val >> 8) : val >> 8;
}
const u8* ByamlArrayIter::getTypeTable() const {
    return mData + 4;
}
}  // namespace al
