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
        int _0;
        unsigned short mTag, mVersion;  // unusable due to different loading mechanisms
    };

    int mHashKeyOffset;
    int mStringTableOffset;
    int mDataOffset;
};

class ByamlStringTableIter {
public:
    ByamlStringTableIter();
    ByamlStringTableIter(const u8* data, bool isRev);

    int getSize() const;
    const u32* getAddressTable() const;
    u32 getStringAddress(int index) const;
    u32 getEndAddress() const;
    const char* getString(int index) const;
    int getStringSize(int index) const;
    int findStringIndex(const char* str) const;
    bool isValidate() const;

private:
    const u8* mData = nullptr;
    bool isRev = false;
};
}  // namespace al

namespace alByamlLocalUtil {

const char* getDataTypeString(int type);
al::ByamlStringTableIter getHashKeyTable(const u8* data);
al::ByamlStringTableIter getStringTable(const u8* data);
u64 getData64Bit(const u8* data, u32 off, bool isRev);
void writeU24(sead::WriteStream* stream, int val);
bool verifiByaml(const u8* data);
bool verifiByamlHeader(const u8* data);
bool verifiByamlStringTable(const u8* data, bool isRev);

}  // namespace alByamlLocalUtil
