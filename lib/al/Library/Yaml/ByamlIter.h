#pragma once

#include "Library/Yaml/ByamlData.h"
#include "Library/Yaml/ByamlHeader.h"

namespace al {
class ByamlContainerHeader;

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
    bool getByamlDataByIndex(ByamlData* data, s32 index) const;
    ByamlIter getIterByKey(const char* key) const;
    bool getByamlDataByKey(ByamlData* data, const char* key) const;
    bool getByamlDataByKeyIndex(ByamlData* data, s32 index) const;
    bool getByamlDataAndKeyName(ByamlData* data, const char** key, s32 index) const;
    bool getKeyName(const char** key, s32 index) const;
    bool tryGetIterByIndex(ByamlIter* iter, s32 index) const;
    bool tryGetIterAndKeyNameByIndex(ByamlIter* iter, const char** key, s32 index) const;
    bool tryGetIterByKey(ByamlIter* iter, const char* key) const;
    bool tryGetStringByKey(const char** value, const char* key) const;
    bool tryConvertString(const char** value, const ByamlData* data) const;
    bool tryGetBinaryByKey(const u8** value, s32* size, const char* key) const;
    bool tryConvertBinary(const u8** value, s32* size, const ByamlData* data) const;
    bool tryGetBoolByKey(bool* value, const char* key) const;
    bool tryConvertBool(bool* value, const ByamlData* data) const;
    bool tryGetIntByKey(s32* value, const char* key) const;
    bool tryConvertInt(s32* value, const ByamlData* data) const;
    bool tryGetUIntByKey(u32* value, const char* key) const;
    bool tryConvertUInt(u32* value, const ByamlData* data) const;
    bool tryGetFloatByKey(f32* value, const char* key) const;
    bool tryConvertFloat(f32* value, const ByamlData* data) const;
    bool tryGetInt64ByKey(s64* value, const char* key) const;
    bool tryConvertInt64(s64* value, const ByamlData* data) const;
    bool tryGetUInt64ByKey(u64* value, const char* key) const;
    bool tryConvertUInt64(u64* value, const ByamlData* data) const;
    bool tryGetDoubleByKey(f64* value, const char* key) const;
    bool tryConvertDouble(f64* value, const ByamlData* data) const;
    bool tryGetStringByIndex(const char** value, s32 index) const;
    bool tryGetBinaryByIndex(const u8** value, s32* size, s32 index) const;
    bool tryGetBoolByIndex(bool* value, s32 index) const;
    bool tryGetIntByIndex(s32* value, s32 index) const;
    bool tryGetUIntByIndex(u32* value, s32 index) const;
    bool tryGetFloatByIndex(f32* value, s32 index) const;
    bool tryGetInt64ByIndex(s64* value, s32 index) const;
    bool tryGetUInt64ByIndex(u64* value, s32 index) const;
    bool tryGetDoubleByIndex(f64* value, s32 index) const;
    bool tryConvertIter(ByamlIter* iter, const ByamlData* data) const;
    bool isEqualData(const ByamlIter& other) const;

    const ByamlHeader* getHeader() const { return reinterpret_cast<const ByamlHeader*>(mData); }

    const ByamlContainerHeader* getContainerHeader() const {
        return reinterpret_cast<const ByamlContainerHeader*>(mRootNode);
    }

private:
    const u8* mData = nullptr;
    const u8* mRootNode = nullptr;
};
}  // namespace al
