#include "Library/Yaml/ByamlIter.h"

#include "Library/Yaml/ByamlContainerHeader.h"
#include "Library/Yaml/ByamlData.h"
#include "Library/Yaml/ByamlHeader.h"

namespace al {
ByamlIter::ByamlIter() = default;

ByamlIter::ByamlIter(const u8* data) : mData(data) {
    if (!data)
        return;
    if (!alByamlLocalUtil::verifiByaml(data)) {
        mData = nullptr;
        mRootNode = nullptr;
        return;
    }

    const ByamlHeader* header = getHeader();
    if (!header->getDataOffset())
        return;

    mRootNode = &mData[header->getDataOffset()];
}

ByamlIter::ByamlIter(const u8* data, const u8* root_node) : mData(data), mRootNode(root_node) {}

bool ByamlIter::isValid() const {
    return mData != nullptr;
}

bool ByamlIter::isTypeHash() const {
    return mRootNode ? getContainerHeader()->getTypeCode() == ByamlDataType::Hash : false;
}

bool ByamlIter::isTypeArray() const {
    return mRootNode ? getContainerHeader()->getTypeCode() == ByamlDataType::Array : false;
}

bool ByamlIter::isTypeContainer() const {
    return isTypeHash() || isTypeArray();
}

bool ByamlIter::isExistKey(const char* key) const {
    if (!mRootNode || getContainerHeader()->getTypeCode() != ByamlDataType::Hash)
        return false;

    s32 index = getKeyIndex(key);
    if (index < 0)
        return false;

    ByamlHashIter iter = {mRootNode, isInvertOrder()};
    return iter.findPair(index);
}

s32 ByamlIter::getKeyIndex(const char* key) const {
    ByamlStringTableIter hash = alByamlLocalUtil::getHashKeyTable(mData);
    if (!hash.isValidate())
        return -1;

    return hash.findStringIndex(key);
}

bool ByamlIter::isInvertOrder() const {
    return getHeader()->isInvertOrder();
}

s32 ByamlIter::getSize() const {
    if (!mRootNode)
        return false;

    ByamlDataType type = getContainerHeader()->getTypeCode();
    if (type == ByamlDataType::Array || type == ByamlDataType::Hash)
        return getContainerHeader()->getCount(isInvertOrder());
    return 0;
}

ByamlIter ByamlIter::getIterByIndex(s32 index) const {
    ByamlData data;
    if (!getByamlDataByIndex(&data, index))
        return nullptr;
    if (data.getType() != ByamlDataType::Array && data.getType() != ByamlDataType::Hash) {
        if (data.getType() == ByamlDataType::Null)
            return {mData, nullptr};
        return {};
    }
    return {mData, &mData[data.getValue()]};
}

bool ByamlIter::getByamlDataByIndex(ByamlData* data, s32 index) const {
    if (!mRootNode)
        return false;
    if (getContainerHeader()->getTypeCode() == ByamlDataType::Array) {
        ByamlArrayIter iter = {mRootNode, isInvertOrder()};
        return iter.getDataByIndex(data, index);
    }
    if (getContainerHeader()->getTypeCode() == ByamlDataType::Hash) {
        ByamlHashIter iter = {mRootNode, isInvertOrder()};
        return iter.getDataByIndex(data, index);
    }
    return false;
}

ByamlIter ByamlIter::getIterByKey(const char* key) const {
    ByamlData data;
    if (!getByamlDataByKey(&data, key))
        return nullptr;
    if (data.getType() != ByamlDataType::Array && data.getType() != ByamlDataType::Hash) {
        if (data.getType() == ByamlDataType::Null)
            return {mData, nullptr};
        return {};
    }
    return {mData, &mData[data.getValue()]};
}

bool ByamlIter::getByamlDataByKey(ByamlData* data, const char* key) const {
    if (!mRootNode || getContainerHeader()->getTypeCode() != ByamlDataType::Hash)
        return false;
    ByamlStringTableIter hash_table = alByamlLocalUtil::getHashKeyTable(mData);
    if (!hash_table.isValidate())
        return false;

    bool isRev = isInvertOrder();
    ByamlHashIter iter = {mRootNode, isRev};
    s32 lowerBound = 0;
    s32 upperBound = iter.getSize();
    while (lowerBound < upperBound) {
        s32 avg = (lowerBound + upperBound) / 2;
        const ByamlHashPair* pair = iter.getPairByIndex(avg);
        s32 result = strcmp(key, hash_table.getString(pair->getKey(isRev)));
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

bool ByamlIter::getByamlDataByKeyIndex(ByamlData* data, s32 index) const {
    if (!mRootNode || getContainerHeader()->getTypeCode() != ByamlDataType::Hash)
        return false;

    ByamlHashIter iter = {mRootNode, isInvertOrder()};
    return iter.getDataByKey(data, index);
}

bool ByamlIter::getByamlDataAndKeyName(ByamlData* data, const char** key, s32 index) const {
    if (!mRootNode || getContainerHeader()->getTypeCode() != ByamlDataType::Hash)
        return false;

    ByamlHashIter iter = {mRootNode, isInvertOrder()};
    const ByamlHashPair* pair = iter.getPairByIndex(index);
    if (!pair)
        return false;

    if (data)
        data->set(pair, isInvertOrder());
    ByamlStringTableIter hash_table = alByamlLocalUtil::getHashKeyTable(mData);
    if (!hash_table.isValidate())
        return false;

    *key = hash_table.getString(pair->getKey(isInvertOrder()));
    return true;
}

bool ByamlIter::getKeyName(const char** key, s32 index) const {
    return getByamlDataAndKeyName(nullptr, key, index);
}

bool ByamlIter::tryGetIterByIndex(ByamlIter* iter, s32 index) const {
    *iter = getIterByIndex(index);
    return iter->isValid();
}

bool ByamlIter::tryGetIterAndKeyNameByIndex(ByamlIter* iter, const char** key, s32 index) const {
    ByamlData data;
    if (!getByamlDataAndKeyName(&data, key, index)) {
        *key = nullptr;
        *iter = getIterByIndex(index);
        return iter->isValid();
    }

    if (data.getType() == ByamlDataType::Array || data.getType() == ByamlDataType::Hash)
        *iter = {mData, &mData[data.getValue()]};
    if (data.getType() == ByamlDataType::Null)
        *iter = {mData, nullptr};
    return true;
}

bool ByamlIter::tryGetIterByKey(ByamlIter* iter, const char* key) const {
    *iter = getIterByKey(key);
    return iter->isValid();
}

bool ByamlIter::tryConvertIter(ByamlIter* iter, const ByamlData* data) const {
    if (data->getType() == ByamlDataType::Array || data->getType() == ByamlDataType::Hash) {
        *iter = {mData, &mData[data->getValue()]};
        return true;
    }
    if (data->getType() == ByamlDataType::Null) {
        *iter = {mData, nullptr};
        return true;
    }
    return false;
}

bool ByamlIter::tryGetStringByIndex(const char** value, s32 index) const {
    ByamlData data;
    if (!getByamlDataByIndex(&data, index))
        return false;

    return tryConvertString(value, &data);
}

bool ByamlIter::tryGetStringByKey(const char** value, const char* key) const {
    ByamlData data;
    if (!getByamlDataByKey(&data, key))
        return false;

    if (data.getType() == ByamlDataType::Null)
        return false;
    return tryConvertString(value, &data);
}

bool ByamlIter::tryConvertString(const char** value, const ByamlData* data) const {
    if (data->getType() != ByamlDataType::String)
        return false;

    ByamlStringTableIter string_table = alByamlLocalUtil::getStringTable(mData);
    if (!string_table.isValidate())
        return false;
    *value = string_table.getString(data->getValue());

    return true;
}

bool ByamlIter::tryGetBinaryByIndex(const u8** value, s32* size, s32 index) const {
    ByamlData data;
    if (!getByamlDataByIndex(&data, index))
        return false;

    return tryConvertBinary(value, size, &data);
}

bool ByamlIter::tryGetBinaryByKey(const u8** value, s32* size, const char* key) const {
    ByamlData data;
    if (!getByamlDataByKey(&data, key))
        return false;

    return tryConvertBinary(value, size, &data);
}

bool ByamlIter::tryConvertBinary(const u8** value, s32* size, const ByamlData* data) const {
    if (data->getType() != ByamlDataType::String)
        return false;

    ByamlStringTableIter string_table = alByamlLocalUtil::getStringTable(mData);
    if (!string_table.isValidate())
        return false;
    *value = (const u8*)string_table.getString(data->getValue());
    *size = string_table.getStringSize(data->getValue());

    return true;
}

bool ByamlIter::tryGetIntByIndex(s32* value, s32 index) const {
    ByamlData data;
    if (!getByamlDataByIndex(&data, index))
        return false;

    return tryConvertInt(value, &data);
}

bool ByamlIter::tryGetIntByKey(s32* value, const char* key) const {
    ByamlData data;
    if (!getByamlDataByKey(&data, key))
        return false;

    if (data.getType() == ByamlDataType::Null)
        return false;
    return tryConvertInt(value, &data);
}

bool ByamlIter::tryConvertInt(s32* value, const ByamlData* data) const {
    if (data->getType() != ByamlDataType::Int)
        return false;

    *value = data->getValue();
    return true;
}

// NON_MATCHING: mismatch in inlined convert (https://decomp.me/scratch/BnTuu)
bool ByamlIter::tryGetUIntByIndex(u32* value, s32 index) const {
    ByamlData data;
    if (!getByamlDataByIndex(&data, index))
        return false;

    return tryConvertUInt(value, &data);
}

// NON_MATCHING: mismatch in inlined convert (https://decomp.me/scratch/d7Ooh)
bool ByamlIter::tryGetUIntByKey(u32* value, const char* key) const {
    ByamlData data;
    if (!getByamlDataByKey(&data, key))
        return false;

    if (data.getType() == ByamlDataType::Null)
        return false;
    return tryConvertUInt(value, &data);
}

bool ByamlIter::tryConvertUInt(u32* value, const ByamlData* data) const {
    s32 val = data->getValue<s32>();
    if (data->getType() == ByamlDataType::Int) {
        bool result = val >= 0;
        *value = val < 0 ? 0 : val;
        return result;
    }
    if (data->getType() == ByamlDataType::UInt) {
        *value = val;
        return true;
    }

    return false;
}

bool ByamlIter::tryGetFloatByIndex(f32* value, s32 index) const {
    ByamlData data;
    if (!getByamlDataByIndex(&data, index))
        return false;

    return tryConvertFloat(value, &data);
}

bool ByamlIter::tryGetFloatByKey(f32* value, const char* key) const {
    ByamlData data;
    if (!getByamlDataByKey(&data, key))
        return false;

    if (data.getType() == ByamlDataType::Null)
        return false;
    return tryConvertFloat(value, &data);
}

bool ByamlIter::tryConvertFloat(f32* value, const ByamlData* data) const {
    if (data->getType() != ByamlDataType::Float)
        return false;

    *value = data->getValue<f32>();
    return true;
}

bool ByamlIter::tryGetBoolByIndex(bool* value, s32 index) const {
    ByamlData data;
    if (!getByamlDataByIndex(&data, index))
        return false;

    return tryConvertBool(value, &data);
}

bool ByamlIter::tryGetBoolByKey(bool* value, const char* key) const {
    ByamlData data;
    if (!getByamlDataByKey(&data, key))
        return false;

    if (data.getType() == ByamlDataType::Null)
        return false;
    return tryConvertBool(value, &data);
}

bool ByamlIter::tryConvertBool(bool* value, const ByamlData* data) const {
    if (data->getType() != ByamlDataType::Bool)
        return false;

    *value = data->getValue() != 0;
    return true;
}

bool ByamlIter::tryGetInt64ByIndex(s64* value, s32 index) const {
    ByamlData data;
    if (!getByamlDataByIndex(&data, index))
        return false;

    if (data.getType() == ByamlDataType::Null)
        return false;
    return tryConvertInt64(value, &data);
}

bool ByamlIter::tryGetInt64ByKey(s64* value, const char* key) const {
    ByamlData data;
    if (!getByamlDataByKey(&data, key))
        return false;

    if (data.getType() == ByamlDataType::Null)
        return false;
    return tryConvertInt64(value, &data);
}

bool ByamlIter::tryConvertInt64(s64* value, const ByamlData* data) const {
    u32 val = data->getValue();
    if (data->getType() == ByamlDataType::Int) {
        *value = *reinterpret_cast<s32*>(&val);
        return true;
    }
    if (data->getType() == ByamlDataType::UInt) {
        *value = val;
        return true;
    }
    if (data->getType() == ByamlDataType::Int64) {
        *value = alByamlLocalUtil::getData64Bit(mData, val, getHeader()->isInvertOrder());
        return true;
    }

    return false;
}

bool ByamlIter::tryGetUInt64ByIndex(u64* value, s32 index) const {
    ByamlData data;
    if (!getByamlDataByIndex(&data, index))
        return false;

    if (data.getType() == ByamlDataType::Null)
        return false;
    return tryConvertUInt64(value, &data);
}

bool ByamlIter::tryGetUInt64ByKey(u64* value, const char* key) const {
    ByamlData data;
    if (!getByamlDataByKey(&data, key))
        return false;

    if (data.getType() == ByamlDataType::Null)
        return false;
    return tryConvertUInt64(value, &data);
}

bool ByamlIter::tryConvertUInt64(u64* value, const ByamlData* data) const {
    s32 val = data->getValue<s32>();
    if (data->getType() == ByamlDataType::Int) {
        bool result = val >= 0;
        *value = val > 0 ? val : 0;
        return result;
    }
    if (data->getType() == ByamlDataType::UInt) {
        *value = *reinterpret_cast<u32*>(&val);
        return true;
    }

    s64 realVal = alByamlLocalUtil::getData64Bit(mData, val, getHeader()->isInvertOrder());
    if (data->getType() == ByamlDataType::Int64) {
        bool result = realVal >= 0;
        *value = realVal < 0 ? 0 : realVal;
        return result;
    }
    if (data->getType() == ByamlDataType::UInt64) {
        *value = realVal;
        return true;
    }

    return false;
}

bool ByamlIter::tryGetDoubleByIndex(f64* value, s32 index) const {
    ByamlData data;
    if (!getByamlDataByIndex(&data, index))
        return false;

    if (data.getType() == ByamlDataType::Null)
        return false;
    return tryConvertDouble(value, &data);
}

bool ByamlIter::tryGetDoubleByKey(f64* value, const char* key) const {
    ByamlData data;
    if (!getByamlDataByKey(&data, key))
        return false;

    if (data.getType() == ByamlDataType::Null)
        return false;
    return tryConvertDouble(value, &data);
}

bool ByamlIter::tryConvertDouble(f64* value, const ByamlData* data) const {
    u32 val = data->getValue();
    if (data->getType() == ByamlDataType::Float) {
        *value = *reinterpret_cast<f32*>(&val);
        return true;
    }
    if (data->getType() == ByamlDataType::Double) {
        u64 bigVal = alByamlLocalUtil::getData64Bit(mData, val, getHeader()->isInvertOrder());
        *value = *reinterpret_cast<f64*>(&bigVal);
        return true;
    }

    return false;
}

bool ByamlIter::isEqualData(const ByamlIter& other) const {
    if (!mData || !other.mData)
        return false;

    return mData == other.mData && mRootNode == other.mRootNode;
}

}  // namespace al
