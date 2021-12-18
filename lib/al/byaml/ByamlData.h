#pragma once

#include <basis/seadTypes.h>
#include "byteswap.h"

namespace al {
class ByamlHashPair;

enum ByamlDataType : u8 {
    TYPE_INVALID = 0,
    TYPE_STRING = 0xA0,
    TYPE_BINARY = 0xA1,
    TYPE_ARRAY = 0xC0,
    TYPE_HASH = 0xC1,
    TYPE_STRING_TABLE = 0xC2,
    TYPE_BOOL = 0xD0,
    TYPE_INT = 0xD1,
    TYPE_FLOAT = 0xD2,
    TYPE_UINT = 0xD3,
    TYPE_LONG = 0xD4,
    TYPE_ULONG = 0xD5,
    TYPE_DOUBLE = 0xD6,
    TYPE_NULL = 0xFF
};

class ByamlData {
public:
    ByamlData();

    void set(const ByamlHashPair* hash_pair, bool isRev);
    void set(u8 type, u32 value, bool isRev);
    ByamlDataType getType() const;
    u32 getValue() const;

private:
    u32 mValue = 0;
    ByamlDataType mType = TYPE_INVALID;
};

class ByamlHashPair {
public:
    int getKey(bool isRev) const;
    ByamlDataType getType() const;
    int getValue(bool isRev) const;

private:
    union {
        const int mData;
        struct {
            const int mKey : 24;  // unusable due to different loading mechanisms
            ByamlDataType mType;
        };
    };
    const int mValue;
};
}  // namespace al
