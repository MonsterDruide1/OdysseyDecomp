#pragma once

#include <basis/seadTypes.h>

namespace al {
class ByamlHashPair;

enum class ByamlDataType : const u8 {
    None = 0,
    String = 0xa0,
    Binary = 0xa1,
    Array = 0xc0,
    Hash = 0xc1,
    StringTable = 0xc2,
    Bool = 0xd0,
    Int = 0xd1,
    Float = 0xd2,
    UInt = 0xd3,
    Int64 = 0xd4,
    UInt64 = 0xd5,
    Double = 0xd6,
    Null = 0xff,
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
    ByamlDataType mType = ByamlDataType::None;
};

class ByamlHashPair {
public:
    s32 getKey(bool isRev) const;
    ByamlDataType getType() const;
    s32 getValue(bool isRev) const;

private:
    const s32 mData = 0;
    const s32 mValue = 0;
};

class ByamlHashIter {
public:
    ByamlHashIter(const u8* data, bool isRev);
    ByamlHashIter();

    const ByamlHashPair* findPair(s32 key) const;
    bool getDataByIndex(ByamlData* data, s32 index) const;
    bool getDataByKey(ByamlData* data, s32 key) const;
    const u8* getOffsetData(u32 off) const;
    const ByamlHashPair* getPairByIndex(s32 index) const;
    const ByamlHashPair* getPairTable() const;
    u32 getSize() const;

private:
    const u8* mData;
    bool mIsRev;
};

class ByamlArrayIter {
public:
    ByamlArrayIter(const u8* data, bool isRev);
    ByamlArrayIter();

    bool getDataByIndex(ByamlData* data, s32 index) const;
    const u32* getDataTable() const;
    const u8* getOffsetData(u32 off) const;
    u32 getSize() const;
    const u8* getTypeTable() const;

private:
    const u8* mData;
    bool mIsRev;
};
}  // namespace al
