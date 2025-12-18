#include "Library/Yaml/Writer/ByamlWriterData.h"

#include <stream/seadStream.h>

#include "Library/Yaml/ByamlUtil.h"
#include "Library/Yaml/Writer/ByamlWriterBigDataList.h"
#include "Library/Yaml/Writer/ByamlWriterStringTable.h"

namespace al {

void ByamlWriterData::printIndent(s32) const {}

ByamlWriterBool::ByamlWriterBool(bool value) : mValue(value) {}

u8 ByamlWriterBool::getTypeCode() const {
    return 0xD0;
}

void ByamlWriterBool::print(s32 recursionDepth) const {}

void ByamlWriterBool::write(sead::WriteStream* stream) const {
    stream->writeU32(mValue);
}

ByamlWriterInt::ByamlWriterInt(s32 value) : mValue(value) {}

u8 ByamlWriterInt::getTypeCode() const {
    return 0xD1;
}

void ByamlWriterInt::print(s32 recursionDepth) const {}

void ByamlWriterInt::write(sead::WriteStream* stream) const {
    stream->writeS32(mValue);
}

ByamlWriterFloat::ByamlWriterFloat(f32 value) : mValue(value) {}

u8 ByamlWriterFloat::getTypeCode() const {
    return 0xD2;
}

void ByamlWriterFloat::print(s32 recursionDepth) const {}

void ByamlWriterFloat::write(sead::WriteStream* stream) const {
    stream->writeF32(mValue);
}

ByamlWriterUInt::ByamlWriterUInt(u32 value) : mValue(value) {}

u8 ByamlWriterUInt::getTypeCode() const {
    return 0xD3;
}

void ByamlWriterUInt::print(s32 recursionDepth) const {}

void ByamlWriterUInt::write(sead::WriteStream* stream) const {
    stream->writeU32(mValue);
}

ByamlWriterNull::ByamlWriterNull() = default;

u8 ByamlWriterNull::getTypeCode() const {
    return 0xFF;
}

void ByamlWriterNull::print(s32 recursionDepth) const {}

void ByamlWriterNull::write(sead::WriteStream* stream) const {
    stream->writeU32(0);
}

ByamlWriterString::ByamlWriterString(const char* string, ByamlWriterStringTable* stringTable)
    : mString(nullptr), mStringTable(stringTable) {
    mString = mStringTable->tryAdd(string);
}

u8 ByamlWriterString::getTypeCode() const {
    return 0xA0;
}

void ByamlWriterString::print(s32 recursionDepth) const {}

void ByamlWriterString::write(sead::WriteStream* stream) const {
    stream->writeU32(mStringTable->calcIndex(mString));
}

ByamlWriterBigData::ByamlWriterBigData(ByamlWriterBigDataList* list) : mList(list) {
    mList->addData(this);
}

void ByamlWriterBigData::write(sead::WriteStream* stream) const {
    stream->writeU32(mOffset);
}

ByamlWriterInt64::ByamlWriterInt64(s64 value, ByamlWriterBigDataList* list)
    : ByamlWriterBigData(list), mValue(value) {}

u8 ByamlWriterInt64::getTypeCode() const {
    return 0xD4;
}

void ByamlWriterInt64::writeBigData(sead::WriteStream* stream) const {
    stream->writeS64(mValue);
}

void ByamlWriterInt64::print(s32 recursionDepth) const {}

ByamlWriterUInt64::ByamlWriterUInt64(u64 value, ByamlWriterBigDataList* list)
    : ByamlWriterBigData(list), mValue(value) {}

u8 ByamlWriterUInt64::getTypeCode() const {
    return 0xD5;
}

void ByamlWriterUInt64::writeBigData(sead::WriteStream* stream) const {
    stream->writeU64(mValue);
}

void ByamlWriterUInt64::print(s32 recursionDepth) const {}

ByamlWriterDouble::ByamlWriterDouble(f64 value, ByamlWriterBigDataList* list)
    : ByamlWriterBigData(list), mValue(value) {}

u8 ByamlWriterDouble::getTypeCode() const {
    return 0xD6;
}

void ByamlWriterDouble::writeBigData(sead::WriteStream* stream) const {
    stream->writeU64(*reinterpret_cast<const u64*>(&mValue));
}

void ByamlWriterDouble::print(s32 recursionDepth) const {}

ByamlWriterArray::ByamlWriterArray(ByamlWriterStringTable* stringTable)
    : mStringTable(stringTable) {}

ByamlWriterArray::~ByamlWriterArray() {
    while (auto* node = mList.popBack())
        delete node;
}

void ByamlWriterArray::deleteData() {
    for (auto it = mList.robustBegin(); it != mList.robustEnd(); ++it) {
        if (!it->mData->isContainer()) {
            delete it->mData;
            it->mData = nullptr;
        }
    }
}

u32 ByamlWriterArray::calcPackSize() const {
    return mList.size() * 4 + ((mList.size() + 7) & 0xFFFFFFFC);
}

void ByamlWriterArray::addData(ByamlWriterData* data) {
    mList.pushBack(new sead::TListNode<ByamlWriterData*>(data));
}

void ByamlWriterArray::addBool(bool value) {
    addData(new ByamlWriterBool(value));
}

void ByamlWriterArray::addInt(s32 value) {
    addData(new ByamlWriterInt(value));
}

void ByamlWriterArray::addUInt(u32 value) {
    addData(new ByamlWriterUInt(value));
}

void ByamlWriterArray::addFloat(f32 value) {
    addData(new ByamlWriterFloat(value));
}

void ByamlWriterArray::addInt64(s64 value, ByamlWriterBigDataList* list) {
    addData(new ByamlWriterInt64(value, list));
}

void ByamlWriterArray::addUInt64(u64 value, ByamlWriterBigDataList* list) {
    addData(new ByamlWriterUInt64(value, list));
}

void ByamlWriterArray::addDouble(f64 value, ByamlWriterBigDataList* list) {
    addData(new ByamlWriterDouble(value, list));
}

void ByamlWriterArray::addString(const char* value) {
    addData(new ByamlWriterString(value, mStringTable));
}

void ByamlWriterArray::addHash(ByamlWriterHash* hash) {
    addData(hash);
}

void ByamlWriterArray::addArray(ByamlWriterArray* array) {
    addData(array);
}

void ByamlWriterArray::addNull() {
    addData(new ByamlWriterNull());
}

u8 ByamlWriterArray::getTypeCode() const {
    return 0xC0;
}

void ByamlWriterArray::writeContainer(sead::WriteStream* stream) const {
    stream->writeU8(0xC0);
    alByamlLocalUtil::writeU24(stream, mList.size());

    for (auto& node : mList)
        stream->writeU8(node->getTypeCode());

    s32 i = mList.size();
    s32 v12 = i < 0 ? i + 3 : i;
    s32 v14 = i - (v12 & 0xFFFFFFFC);
    s32 v15 = 4 - v14;
    s32 v16 = v14 == 0 ? 0 : v15;
    for (s32 j = 0; j < v16; j++)
        stream->writeU8(0);
    for (auto& node : mList)
        node->write(stream);
}

void ByamlWriterArray::write(sead::WriteStream* stream) const {
    stream->writeU32(getOffset());
}

void ByamlWriterArray::print(s32 recursionDepth) const {
    for (auto node : mList)
        node->print(recursionDepth + 1);
}

ByamlWriterHashPair::ByamlWriterHashPair(const char* key, ByamlWriterData* value)
    : mKey(key), mValue(value) {}

ByamlWriterHash::ByamlWriterHash(ByamlWriterStringTable* stringTable1,
                                 ByamlWriterStringTable* stringTable2)
    : mStringTable1(stringTable1), mStringTable2(stringTable2) {}

ByamlWriterHash::~ByamlWriterHash() {
    while (auto* node = mList.popBack())
        delete node;
}

void ByamlWriterHash::deleteData() {
    for (auto it = mList.robustBegin(); it != mList.robustEnd(); ++it)
        if (!it->mData->getValue()->isContainer())
            delete it->mData->getValue();
}

u32 ByamlWriterHash::calcPackSize() const {
    return mList.size() * 8 + 4;
}

void ByamlWriterHash::addData(const char* key, ByamlWriterData* data) {
    const char* str = mStringTable1->tryAdd(key);
    ByamlWriterHashPair* pair = new ByamlWriterHashPair(str, data);
    auto* node = reinterpret_cast<sead::TListNode<ByamlWriterHashPair*>*>(pair);

    for (auto it = mList.robustBegin(); it != mList.robustEnd(); ++it) {
        s32 cmp = strcmp(str, it->mData->getKey());
        if (cmp == 0)
            return;
        if (cmp < 0) {
            mList.insertBefore(&(*it), node);
            return;
        }
    }
    mList.pushBack(node);
}

void ByamlWriterHash::addBool(const char* key, bool value) {
    addData(key, new ByamlWriterBool(value));
}

void ByamlWriterHash::addInt(const char* key, s32 value) {
    addData(key, new ByamlWriterInt(value));
}

void ByamlWriterHash::addUInt(const char* key, u32 value) {
    addData(key, new ByamlWriterUInt(value));
}

void ByamlWriterHash::addFloat(const char* key, f32 value) {
    addData(key, new ByamlWriterFloat(value));
}

void ByamlWriterHash::addInt64(const char* key, s64 value, ByamlWriterBigDataList* list) {
    addData(key, new ByamlWriterInt64(value, list));
}

void ByamlWriterHash::addUInt64(const char* key, u64 value, ByamlWriterBigDataList* list) {
    addData(key, new ByamlWriterUInt64(value, list));
}

void ByamlWriterHash::addDouble(const char* key, f64 value, ByamlWriterBigDataList* list) {
    addData(key, new ByamlWriterDouble(value, list));
}

void ByamlWriterHash::addString(const char* key, const char* value) {
    addData(key, new ByamlWriterString(value, mStringTable2));
}

void ByamlWriterHash::addHash(const char* key, ByamlWriterHash* value) {
    addData(key, value);
}

void ByamlWriterHash::addArray(const char* key, ByamlWriterArray* value) {
    addData(key, value);
}

void ByamlWriterHash::addNull(const char* key) {
    addData(key, new ByamlWriterNull());
}

u8 ByamlWriterHash::getTypeCode() const {
    return 0xc1;
}

void ByamlWriterHash::writeContainer(sead::WriteStream* stream) const {
    stream->writeU8(0xc1);
    alByamlLocalUtil::writeU24(stream, mList.size());

    for (auto it = mList.begin(); it != mList.end(); ++it) {
        alByamlLocalUtil::writeU24(stream, mStringTable1->calcIndex((*it)->getKey()));
        stream->writeU8((*it)->getValue()->getTypeCode());
        (*it)->getValue()->write(stream);
    }
}

void ByamlWriterHash::write(sead::WriteStream* stream) const {
    stream->writeU32(getOffset());
}

void ByamlWriterHash::print(s32 recursionDepth) const {
    for (auto it = mList.begin(); it != mList.end(); ++it)
        (*it)->getValue()->print(recursionDepth + 1);
}

}  // namespace al
