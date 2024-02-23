#include "Library/Yaml/Writer/ByamlWriterData.h"

#include <stream/seadStream.h>

#include "Library/Yaml/ByamlUtil.h"
#include "Library/Yaml/Writer/ByamlWriterBigDataList.h"
#include "Library/Yaml/Writer/ByamlWriterStringTable.h"

namespace al {

u32 ByamlWriterData::calcPackSize() const {
    return 4;
}
u8 ByamlWriterData::getTypeCode() const {
    return 0;
}
bool ByamlWriterData::isContainer() const {
    return false;
}
void ByamlWriterData::makeIndex() {}
void ByamlWriterData::print(s32) const {}
void ByamlWriterData::printIndent(s32) const {}
ByamlWriterData::~ByamlWriterData() {}

ByamlWriterBool::ByamlWriterBool(bool value) : mValue(value) {}
u8 ByamlWriterBool::getTypeCode() const {
    return 0xD0;
}
void ByamlWriterBool::print(s32) const {}
void ByamlWriterBool::write(sead::WriteStream* stream) const {
    stream->writeU32(mValue);
}

ByamlWriterInt::ByamlWriterInt(s32 value) : mValue(value) {}
u8 ByamlWriterInt::getTypeCode() const {
    return 0xD1;
}
void ByamlWriterInt::print(s32) const {}
void ByamlWriterInt::write(sead::WriteStream* stream) const {
    stream->writeS32(mValue);
}

ByamlWriterFloat::ByamlWriterFloat(f32 value) : mValue(value) {}
u8 ByamlWriterFloat::getTypeCode() const {
    return 0xD2;
}
void ByamlWriterFloat::print(s32) const {}
void ByamlWriterFloat::write(sead::WriteStream* stream) const {
    stream->writeF32(mValue);
}

ByamlWriterUInt::ByamlWriterUInt(u32 value) : mValue(value) {}
u8 ByamlWriterUInt::getTypeCode() const {
    return 0xD3;
}
void ByamlWriterUInt::print(s32) const {}
void ByamlWriterUInt::write(sead::WriteStream* stream) const {
    stream->writeU32(mValue);
}

ByamlWriterNull::ByamlWriterNull() = default;
u8 ByamlWriterNull::getTypeCode() const {
    return 0xFF;
}
void ByamlWriterNull::print(s32) const {}
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
void ByamlWriterString::print(s32) const {}
void ByamlWriterString::write(sead::WriteStream* stream) const {
    stream->writeU32(mStringTable->calcIndex(mString));
}

ByamlWriterBigData::ByamlWriterBigData(ByamlWriterBigDataList* list) : mList(list) {
    mList->addData(this);
}
ByamlWriterBigData::~ByamlWriterBigData() = default;
void ByamlWriterBigData::write(sead::WriteStream* stream) const {
    stream->writeU32(mOffset);
}
u32 ByamlWriterBigData::calcBigDataSize() const {
    return 8;
}
void ByamlWriterBigData::writeBigData(sead::WriteStream*) const {}

ByamlWriterInt64::ByamlWriterInt64(long value, ByamlWriterBigDataList* list)
    : ByamlWriterBigData(list), mValue(value) {}
ByamlWriterInt64::~ByamlWriterInt64() = default;
u8 ByamlWriterInt64::getTypeCode() const {
    return 0xD4;
}
void ByamlWriterInt64::writeBigData(sead::WriteStream* stream) const {
    stream->writeS64(mValue);
}
void ByamlWriterInt64::print(s32) const {}

ByamlWriterUInt64::ByamlWriterUInt64(u64 value, ByamlWriterBigDataList* list)
    : ByamlWriterBigData(list), mValue(value) {}
ByamlWriterUInt64::~ByamlWriterUInt64() = default;
u8 ByamlWriterUInt64::getTypeCode() const {
    return 0xD5;
}
void ByamlWriterUInt64::writeBigData(sead::WriteStream* stream) const {
    stream->writeU64(mValue);
}
void ByamlWriterUInt64::print(s32) const {}

ByamlWriterDouble::ByamlWriterDouble(double value, ByamlWriterBigDataList* list)
    : ByamlWriterBigData(list), mValue(value) {}
ByamlWriterDouble::~ByamlWriterDouble() = default;
u8 ByamlWriterDouble::getTypeCode() const {
    return 0xD6;
}
void ByamlWriterDouble::writeBigData(sead::WriteStream* stream) const {
    stream->writeU64(*reinterpret_cast<const u64*>(&mValue));
}
void ByamlWriterDouble::print(s32) const {}

bool ByamlWriterContainer::isContainer() const {
    return true;
}

void ByamlWriterContainer::addBool(const char*, bool) {}
void ByamlWriterContainer::addInt(const char*, s32) {}
void ByamlWriterContainer::addUInt(const char*, u32) {}
void ByamlWriterContainer::addFloat(const char*, f32) {}
void ByamlWriterContainer::addInt64(const char*, s64, ByamlWriterBigDataList*) {}
void ByamlWriterContainer::addUInt64(const char*, u64, ByamlWriterBigDataList*) {}
void ByamlWriterContainer::addDouble(const char*, double, ByamlWriterBigDataList*) {}
void ByamlWriterContainer::addString(const char*, const char*) {}
void ByamlWriterContainer::addHash(const char*, ByamlWriterHash*) {}
void ByamlWriterContainer::addArray(const char*, ByamlWriterArray*) {}
void ByamlWriterContainer::addNull(const char*) {}

void ByamlWriterContainer::addBool(bool) {}
void ByamlWriterContainer::addInt(s32) {}
void ByamlWriterContainer::addUInt(u32) {}
void ByamlWriterContainer::addFloat(f32) {}
void ByamlWriterContainer::addInt64(s64, ByamlWriterBigDataList*) {}
void ByamlWriterContainer::addUInt64(u64, ByamlWriterBigDataList*) {}
void ByamlWriterContainer::addDouble(double, ByamlWriterBigDataList*) {}
void ByamlWriterContainer::addString(const char*) {}
void ByamlWriterContainer::addHash(ByamlWriterHash*) {}
void ByamlWriterContainer::addArray(ByamlWriterArray*) {}
void ByamlWriterContainer::addNull() {}

void ByamlWriterContainer::writeContainer(sead::WriteStream*) const {}
bool ByamlWriterContainer::isHash() const {
    return false;
}
bool ByamlWriterContainer::isArray() const {
    return false;
}
void ByamlWriterContainer::deleteData() {}

ByamlWriterArray::ByamlWriterArray(ByamlWriterStringTable* stringTable)
    : mStringTable(stringTable) {}
ByamlWriterArray::~ByamlWriterArray() {
    while (auto* node = mList.popBack()) {
        delete node;
    }
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
void ByamlWriterArray::addDouble(double value, ByamlWriterBigDataList* list) {
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

    for (auto& node : mList) {
        stream->writeU8(node->getTypeCode());
    }

    s32 i = mList.size();
    s32 v12 = i < 0 ? i + 3 : i;
    s32 v14 = i - (v12 & 0xFFFFFFFC);
    s32 v15 = 4 - v14;
    s32 v16 = v14 == 0 ? 0 : v15;
    for (s32 j = 0; j < v16; j++) {
        stream->writeU8(0);
    }
    for (auto& node : mList) {
        node->write(stream);
    }
}
void ByamlWriterArray::write(sead::WriteStream* stream) const {
    stream->writeU32(getOffset());
}
void ByamlWriterArray::print(s32 unknown) const {
    for (auto node : mList) {
        node->print(unknown + 1);
    }
}
bool ByamlWriterArray::isArray() const {
    return true;
}

ByamlWriterHashPair::ByamlWriterHashPair(const char* key, ByamlWriterData* value)
    : mKey(key), mValue(value) {}

ByamlWriterHash::ByamlWriterHash(ByamlWriterStringTable* stringTable1,
                                 ByamlWriterStringTable* stringTable2)
    : mStringTable1(stringTable1), mStringTable2(stringTable2) {}
ByamlWriterHash::~ByamlWriterHash() {
    while (auto* node = mList.popBack()) {
        delete node;
    }
}
u32 ByamlWriterHash::calcPackSize() const {
    return mList.size() * 8 + 4;
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
void ByamlWriterHash::addInt64(const char* key, long value, ByamlWriterBigDataList* list) {
    addData(key, new ByamlWriterInt64(value, list));
}
void ByamlWriterHash::addUInt64(const char* key, u64 value, ByamlWriterBigDataList* list) {
    addData(key, new ByamlWriterUInt64(value, list));
}
void ByamlWriterHash::addDouble(const char* key, double value, ByamlWriterBigDataList* list) {
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
    return 0xC1;
}
void ByamlWriterHash::write(sead::WriteStream* stream) const {
    stream->writeU32(getOffset());
}
bool ByamlWriterHash::isHash() const {
    return true;
}

}  // namespace al
