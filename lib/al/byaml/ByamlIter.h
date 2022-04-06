#pragma once

#include "al/byaml/ByamlData.h"
#include "al/byaml/ByamlHeader.h"

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
    int getKeyIndex(const char* key) const;
    bool isInvertOrder() const;
    int getSize() const;
    ByamlIter getIterByIndex(int index) const;
    bool getByamlDataByIndex(al::ByamlData* data, int index) const;
    ByamlIter getIterByKey(const char* key) const;
    bool getByamlDataByKey(al::ByamlData* data, const char* key) const;
    bool getByamlDataByKeyIndex(al::ByamlData* data, int index) const;
    bool getByamlDataAndKeyName(al::ByamlData* data, const char** key, int index) const;
    bool getKeyName(const char** key, int index) const;
    bool tryGetIterByIndex(ByamlIter* iter, int index) const;
    bool tryGetIterAndKeyNameByIndex(ByamlIter* iter, const char** key, int index) const;
    bool tryGetIterByKey(ByamlIter* iter, const char* key) const;
    bool tryGetStringByKey(const char** string, const char* key) const;
    bool tryConvertString(const char** string, const ByamlData* data) const;
    bool tryGetBinaryByKey(const u8** binary, int* size, const char* key) const;
    bool tryConvertBinary(const u8** binary, int* size, const ByamlData* data) const;
    bool tryGetBoolByKey(bool* val, const char* key) const;
    bool tryConvertBool(bool* val, const ByamlData* data) const;
    bool tryGetIntByKey(int* val, const char* key) const;
    bool tryConvertInt(int* val, const ByamlData* data) const;
    bool tryGetUIntByKey(unsigned int* val, const char* key) const;
    bool tryConvertUInt(unsigned int* val, const ByamlData* data) const;
    bool tryGetFloatByKey(float* val, const char* key) const;
    bool tryConvertFloat(float* val, const ByamlData* data) const;
    bool tryGetInt64ByKey(long* val, const char* key) const;
    bool tryConvertInt64(long* val, const ByamlData* data) const;
    bool tryGetUInt64ByKey(unsigned long* val, const char* key) const;
    bool tryConvertUInt64(unsigned long* val, const ByamlData* data) const;
    bool tryGetDoubleByKey(double* val, const char* key) const;
    bool tryConvertDouble(double* val, const ByamlData* data) const;
    bool tryGetStringByIndex(const char** string, int index) const;
    bool tryGetBinaryByIndex(const u8** binary, int* size, int index) const;
    bool tryGetBoolByIndex(bool* val, int index) const;
    bool tryGetIntByIndex(int* val, int index) const;
    bool tryGetUIntByIndex(unsigned int* val, int index) const;
    bool tryGetFloatByIndex(float* val, int index) const;
    bool tryGetInt64ByIndex(long* val, int index) const;
    bool tryGetUInt64ByIndex(unsigned long* val, int index) const;
    bool tryGetDoubleByIndex(double* val, int index) const;
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
