#pragma once

#include <basis/seadTypes.h>
#include "byteswap.h"

namespace al {
class ByamlHashPair;

enum ByamlDataType : const u8 {
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

    template <typename T = u32>
    T getValue() const {
        u32 val = getValue();
        return *reinterpret_cast<const T*>(&val);
    }

private:
    u32 mValue = 0;
    ByamlDataType mType = ByamlDataType::TYPE_INVALID;
};

class ByamlHashPair {
public:
    int getKey(bool isRev) const;
    ByamlDataType getType() const;
    int getValue(bool isRev) const;

private:
    const int mData = 0;
    const int mValue = 0;
};

class ByamlHashIter {
public:
    ByamlHashIter(const u8* data, bool isRev);
    ByamlHashIter();

    const ByamlHashPair* findPair(int key) const;
    bool getDataByIndex(ByamlData* data, int index) const;
    bool getDataByKey(ByamlData* data, int key) const;
    const u8* getOffsetData(u32 off) const;
    const ByamlHashPair* getPairByIndex(int index) const;
    const ByamlHashPair* getPairTable() const;
    u32 getSize() const;

private:
    const u8* mData;
    bool isRev;
};

class ByamlArrayIter {
public:
    ByamlArrayIter(const u8* data, bool isRev);
    ByamlArrayIter();

    bool getDataByIndex(ByamlData* data, int index) const;
    const u32* getDataTable() const;
    const u8* getOffsetData(u32 off) const;
    u32 getSize() const;
    const u8* getTypeTable() const;

private:
    const u8* mData;
    bool isRev;
};
}  // namespace al
