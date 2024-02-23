#include "Library/Yaml/ByamlHeader.h"

#include <byteswap.h>
#include <prim/seadEndian.h>
#include <stream/seadStream.h>

#include "Library/Yaml/ByamlData.h"

#define BYAML_LE_TAG 'YB'

namespace al {
u16 ByamlHeader::getTag() const {
    return isInvertOrder() ? bswap_16(mTag) : mTag;
}

bool ByamlHeader::isInvertOrder() const {
    return mTag == BYAML_LE_TAG;
}

u16 ByamlHeader::getVersion() const {
    if ((_0 & 0xFFFF) == BYAML_LE_TAG)  // isInvertOrder()
        return bswap_16(_0 >> 16);
    return _0 >> 16;
}

u32 ByamlHeader::getHashKeyTableOffset() const {
    return isInvertOrder() ? __bswap_32(mHashKeyOffset) : mHashKeyOffset;
}

u32 ByamlHeader::getStringTableOffset() const {
    return isInvertOrder() ? __bswap_32(mStringTableOffset) : mStringTableOffset;
}

u32 ByamlHeader::getDataOffset() const {
    return isInvertOrder() ? __bswap_32(mDataOffset) : mDataOffset;
}

ByamlStringTableIter::ByamlStringTableIter() = default;

ByamlStringTableIter::ByamlStringTableIter(const u8* data, bool isRev)
    : mData(data), isRev(isRev) {}

s32 ByamlStringTableIter::getSize() const {
    u32 type_and_size = *reinterpret_cast<const u32*>(mData);
    return isRev ? bswap_24(type_and_size >> 8) : type_and_size >> 8;
}

const u32* ByamlStringTableIter::getAddressTable() const {
    // mData is an integer pointer, so getting to the table is just increasing the pointer by 1
    // (which is + 4)
    return reinterpret_cast<const u32*>(mData + 4);
}

u32 ByamlStringTableIter::getStringAddress(s32 idx) const {
    if (isRev)
        return bswap_32(getAddressTable()[idx]);

    return getAddressTable()[idx];
}

// NON_MATCHING: regalloc
u32 ByamlStringTableIter::getEndAddress() const {
    u32 val = getAddressTable()[getSize()];
    return isRev ? bswap_32(val) : val;
}
const char* ByamlStringTableIter::getString(s32 index) const {
    return reinterpret_cast<const char*>(&mData[getStringAddress(index)]);
}
s32 ByamlStringTableIter::getStringSize(s32 index) const {
    return getStringAddress(index + 1) - getStringAddress(index) - 1;
}
s32 ByamlStringTableIter::findStringIndex(const char* str) const {
    s32 lowerBound = 0;
    s32 upperBound = getSize();
    while (lowerBound < upperBound) {
        s32 avg = (lowerBound + upperBound) / 2;
        s32 result = strcmp(str, getString(avg));
        if (result == 0)
            return avg;

        if (result > 0)
            lowerBound = avg + 1;
        else
            upperBound = avg;
    }
    return -1;
}

bool ByamlStringTableIter::isValidate() const {
    return mData != nullptr;
}

}  // namespace al

namespace alByamlLocalUtil {

const char* getDataTypeString(s32 type) {
    switch (type) {
    case al::ByamlDataType::TYPE_INVALID:
        return "None";
    case al::ByamlDataType::TYPE_STRING:
        return "String";
    case al::ByamlDataType::TYPE_ARRAY:
        return "Array";
    case al::ByamlDataType::TYPE_HASH:
        return "Hash";
    case al::ByamlDataType::TYPE_STRING_TABLE:
        return "StringTable";
    case al::ByamlDataType::TYPE_BOOL:
        return "Bool";
    case al::ByamlDataType::TYPE_INT:
        return "Int";
    case al::ByamlDataType::TYPE_FLOAT:
        return "Float";
    case al::ByamlDataType::TYPE_UINT:
        return "UInt";
    case al::ByamlDataType::TYPE_LONG:
        return "Int64";
    case al::ByamlDataType::TYPE_ULONG:
        return "UInt64";
    case al::ByamlDataType::TYPE_DOUBLE:
        return "Double";
    case al::ByamlDataType::TYPE_NULL:
        return "NULL";
    case al::ByamlDataType::TYPE_BINARY:
    default:
        return "Unknown";
    };
}
al::ByamlStringTableIter getHashKeyTable(const u8* data) {
    const al::ByamlHeader* header = reinterpret_cast<const al::ByamlHeader*>(data);
    s32 off = header->getHashKeyTableOffset();
    if (off == 0)
        return {};
    return {&data[off], header->isInvertOrder()};
}
al::ByamlStringTableIter getStringTable(const u8* data) {
    const al::ByamlHeader* header = reinterpret_cast<const al::ByamlHeader*>(data);
    s32 off = header->getStringTableOffset();
    if (off == 0)
        return {};
    return {&data[off], header->isInvertOrder()};
}

u64 getData64Bit(const u8* data, u32 off, bool isRev) {
    u64 val = *reinterpret_cast<const u64*>(&data[off]);
    return isRev ? bswap_32_64(val) : val;
}

void writeU24(sead::WriteStream* stream, s32 val) {
    if (sead::Endian::getHostEndian() == sead::Endian::cBig) {
        stream->writeU8(val >> 16);
        stream->writeU8(val >> 8);
        stream->writeU8(val);
    } else {
        stream->writeU8(val);
        stream->writeU8(val >> 8);
        stream->writeU8(val >> 16);
    }
}

// NON_MATCHING: inlined verifiHeader, splitted loads for unswappedAfterOffset and diff in final
// logic
bool verifiByaml(const u8* data) {
    if (!verifiByamlHeader(data))
        return false;

    bool isRev = *((const u16*)data) == BYAML_LE_TAG;
    const u32* biggerData = (const u32*)data;

    s32 afterHashOffset = 0;
    u32 hashOffset = isRev ? bswap_32(biggerData[1]) : biggerData[1];
    if (hashOffset) {
        const u8* hashData = &data[hashOffset];
        if (!verifiByamlStringTable(hashData, isRev))
            return false;
        u32 type_and_size = *reinterpret_cast<const u32*>(hashData);
        s32 hash_size = isRev ? bswap_24(type_and_size >> 8) : type_and_size >> 8;
        s32 unswappedAfterOffset = *(s32*)&hashData[(hash_size * 4) + 4];
        u32 swappedAfterOffset = bswap_32(unswappedAfterOffset);
        afterHashOffset = isRev ? swappedAfterOffset : unswappedAfterOffset;
    }

    s32 afterStringOffset = 0;
    u32 stringOffset = isRev ? bswap_32(biggerData[2]) : biggerData[2];
    if (stringOffset) {
        const u8* stringData = &data[stringOffset];
        if (!verifiByamlStringTable(stringData, isRev))
            return false;
        u32 type_and_size = *reinterpret_cast<const u32*>(stringData);
        s32 string_size = isRev ? bswap_24(type_and_size >> 8) : type_and_size >> 8;
        s32 unswappedAfterOffset = *(s32*)&stringData[4 * string_size + 4];
        u32 swappedAfterOffset = bswap_32(unswappedAfterOffset);
        afterStringOffset = isRev ? swappedAfterOffset : unswappedAfterOffset;
    }

    u32 rootOffset = isRev ? bswap_32(biggerData[3]) : biggerData[3];

    return (((!hashOffset && !stringOffset) || rootOffset) &&
            (!hashOffset || ((!stringOffset || afterHashOffset <= stringOffset) &&
                             (!rootOffset || afterHashOffset <= rootOffset)))) &&
           (afterStringOffset <= rootOffset || !stringOffset || !rootOffset);
}

// NON_MATCHING: missing & 0xFFFF
bool verifiByamlHeader(const u8* data) {
    const al::ByamlHeader* header = reinterpret_cast<const al::ByamlHeader*>(data);
    return header->getTag() == 'BY' && (u32)(header->getVersion() - 1) < 3;
}

bool verifiByamlStringTable(const u8* data, bool isRev) {
    const al::ByamlStringTableIter* strings =
        reinterpret_cast<const al::ByamlStringTableIter*>(data);
    const s32* address_table = reinterpret_cast<const s32*>(data + 4);

    u32 type_and_size = *reinterpret_cast<const u32*>(data);
    if ((type_and_size & 0xff) != al::ByamlDataType::TYPE_STRING_TABLE)
        return false;
    s32 size = isRev ? bswap_24(type_and_size >> 8) : type_and_size >> 8;
    if (size < 1)
        return false;

    // check that strings are null-terminated
    for (s32 i = 1; i <= size; i++) {
        s32 off = isRev ? bswap_32(address_table[i]) : address_table[i];
        if (data[off - 1])
            return false;
    }

    for (s32 i = 0; i < size; i++) {
        s32 val1 = isRev ? bswap_32(address_table[i]) : address_table[i];
        s32 val2 = isRev ? bswap_32(address_table[i + 1]) : address_table[i + 1];
        if (val1 >= val2)
            return false;
    }

    // check for correct length of address table
    s32 calcFirstStringPos = 4 * size + 8;
    s32 dataFirstStringPos = isRev ? bswap_32(address_table[0]) : address_table[0];
    if (dataFirstStringPos != calcFirstStringPos)
        return false;

    // TODO: improve this. Matching, but ugly
    if (isRev) {
        for (s32 i = 0; i < size - 1; i++) {
            const char* c1 =
                (const char*)&data[isRev ? bswap_32(address_table[i]) : address_table[i]];
            const char* c2 =
                (const char*)&data[isRev ? bswap_32(address_table[i + 1]) : address_table[i + 1]];
            if (strcmp(c1, c2) > 0)
                return false;
        }
    } else {
        for (s32 i = 0; i < size - 1; i++) {
            const char* c1 =
                (const char*)&data[isRev ? bswap_32(address_table[i]) : address_table[i]];
            const char* c2 =
                (const char*)&data[false ? bswap_32(address_table[i + 1]) : address_table[i + 1]];
            if (strcmp(c1, c2) > 0)
                return false;
        }
    }
    return true;
}

}  // namespace alByamlLocalUtil
