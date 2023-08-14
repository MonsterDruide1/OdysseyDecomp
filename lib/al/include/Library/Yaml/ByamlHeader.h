#pragma once

#include <basis/seadTypes.h>
#include <stream/seadStream.h>

namespace al {
class ByamlHeader {
public:
    u16 getTag() const;
    bool isInvertOrder() const;
    u16 getVersion() const;
    u32 getHashKeyTableOffset() const;
    u32 getStringTableOffset() const;
    u32 getDataOffset() const;

private:
    union {
        s32 _0;
        u16 mTag, mVersion;  // unusable due to different loading mechanisms
    };

    s32 mHashKeyOffset;
    s32 mStringTableOffset;
    s32 mDataOffset;
};

class ByamlStringTableIter {
public:
    ByamlStringTableIter();
    ByamlStringTableIter(const u8* data, bool isRev);

    s32 getSize() const;
    const u32* getAddressTable() const;
    u32 getStringAddress(s32 index) const;
    u32 getEndAddress() const;
    const char* getString(s32 index) const;
    s32 getStringSize(s32 index) const;
    s32 findStringIndex(const char* str) const;
    bool isValidate() const;

private:
    const u8* mData = nullptr;
    bool isRev = false;
};
}  // namespace al

namespace alByamlLocalUtil {

const char* getDataTypeString(s32 type);
al::ByamlStringTableIter getHashKeyTable(const u8* data);
al::ByamlStringTableIter getStringTable(const u8* data);
u64 getData64Bit(const u8* data, u32 off, bool isRev);
void writeU24(sead::WriteStream* stream, s32 val);
bool verifiByaml(const u8* data);
bool verifiByamlHeader(const u8* data);
bool verifiByamlStringTable(const u8* data, bool isRev);

}  // namespace alByamlLocalUtil
