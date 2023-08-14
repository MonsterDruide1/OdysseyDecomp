#pragma once

#include "Library/Yaml/ByamlData.h"
#include "Library/Yaml/ByamlHeader.h"

namespace al {
class ByamlIter {
public:
    ByamlIter();
    ByamlIter(const u8* data);
    ByamlIter(const u8* data, const u8* root_node);

    bool isValid() const;
    bool isTypeHash() const;
    bool isTypeArray() const;
    bool isTypeContainer() const;
    bool isExistKey(const char* key) const;
    s32 getKeyIndex(const char* key) const;
    bool isInvertOrder() const;
    s32 getSize() const;
    ByamlIter getIterByIndex(s32 index) const;
    bool getByamlDataByIndex(al::ByamlData* data, s32 index) const;
    ByamlIter getIterByKey(const char* key) const;
    bool getByamlDataByKey(al::ByamlData* data, const char* key) const;
    bool getByamlDataByKeyIndex(al::ByamlData* data, s32 index) const;
    bool getByamlDataAndKeyName(al::ByamlData* data, const char** key, s32 index) const;
    bool getKeyName(const char** key, s32 index) const;
    bool tryGetIterByIndex(ByamlIter* iter, s32 index) const;
    bool tryGetIterAndKeyNameByIndex(ByamlIter* iter, const char** key, s32 index) const;
    bool tryGetIterByKey(ByamlIter* iter, const char* key) const;
    bool tryGetStringByKey(const char** string, const char* key) const;
    bool tryConvertString(const char** string, const ByamlData* data) const;
    bool tryGetBinaryByKey(const u8** binary, s32* size, const char* key) const;
    bool tryConvertBinary(const u8** binary, s32* size, const ByamlData* data) const;
    bool tryGetBoolByKey(bool* val, const char* key) const;
    bool tryConvertBool(bool* val, const ByamlData* data) const;
    bool tryGetIntByKey(s32* val, const char* key) const;
    bool tryConvertInt(s32* val, const ByamlData* data) const;
    bool tryGetUIntByKey(u32* val, const char* key) const;
    bool tryConvertUInt(u32* val, const ByamlData* data) const;
    bool tryGetFloatByKey(f32* val, const char* key) const;
    bool tryConvertFloat(f32* val, const ByamlData* data) const;
    bool tryGetInt64ByKey(long* val, const char* key) const;
    bool tryConvertInt64(long* val, const ByamlData* data) const;
    bool tryGetUInt64ByKey(unsigned long* val, const char* key) const;
    bool tryConvertUInt64(unsigned long* val, const ByamlData* data) const;
    bool tryGetDoubleByKey(double* val, const char* key) const;
    bool tryConvertDouble(double* val, const ByamlData* data) const;
    bool tryGetStringByIndex(const char** string, s32 index) const;
    bool tryGetBinaryByIndex(const u8** binary, s32* size, s32 index) const;
    bool tryGetBoolByIndex(bool* val, s32 index) const;
    bool tryGetIntByIndex(s32* val, s32 index) const;
    bool tryGetUIntByIndex(u32* val, s32 index) const;
    bool tryGetFloatByIndex(f32* val, s32 index) const;
    bool tryGetInt64ByIndex(long* val, s32 index) const;
    bool tryGetUInt64ByIndex(unsigned long* val, s32 index) const;
    bool tryGetDoubleByIndex(double* val, s32 index) const;
    bool tryConvertIter(ByamlIter* iter, const ByamlData* data) const;
    bool isEqualData(ByamlIter const& other) const;

    const ByamlHeader* getHeader() const { return mHeader; }

private:
    union {
        const u8* mData;
        const ByamlHeader* mHeader;
    };
    const u8* mRootNode;
};
}  // namespace al
