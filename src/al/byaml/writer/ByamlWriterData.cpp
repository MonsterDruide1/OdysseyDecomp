#include "al/byaml/writer/ByamlWriterData.h"

#include <stream/seadStream.h>
#include "al/byaml/writer/ByamlWriterBigDataList.h"

namespace al {

u32 ByamlWriterData::calcPackSize() const {return 4;}
u8 ByamlWriterData::getTypeCode() const {return 0;}
bool ByamlWriterData::isContainer() const {return false;}
void ByamlWriterData::makeIndex() {}
void ByamlWriterData::print(int) const {}
void ByamlWriterData::printIndent(int) const {}
ByamlWriterData::~ByamlWriterData() {}

ByamlWriterBool::ByamlWriterBool(bool value) : mValue(value) {}
u8 ByamlWriterBool::getTypeCode() const {return 0xD0;}
void ByamlWriterBool::print(int) const {}
void ByamlWriterBool::write(sead::WriteStream* stream) const {stream->writeU32(mValue);}

ByamlWriterInt::ByamlWriterInt(int value) : mValue(value) {}
u8 ByamlWriterInt::getTypeCode() const {return 0xD1;}
void ByamlWriterInt::print(int) const {}
void ByamlWriterInt::write(sead::WriteStream* stream) const {stream->writeS32(mValue);}

ByamlWriterFloat::ByamlWriterFloat(float value) : mValue(value) {}
u8 ByamlWriterFloat::getTypeCode() const {return 0xD2;}
void ByamlWriterFloat::print(int) const {}
void ByamlWriterFloat::write(sead::WriteStream* stream) const {stream->writeF32(mValue);}

ByamlWriterUInt::ByamlWriterUInt(u32 value) : mValue(value) {}
u8 ByamlWriterUInt::getTypeCode() const {return 0xD3;}
void ByamlWriterUInt::print(int) const {}
void ByamlWriterUInt::write(sead::WriteStream* stream) const {stream->writeU32(mValue);}

ByamlWriterNull::ByamlWriterNull() = default;
u8 ByamlWriterNull::getTypeCode() const {return 0xFF;}
void ByamlWriterNull::print(int) const {}
void ByamlWriterNull::write(sead::WriteStream* stream) const {stream->writeU32(0);}



ByamlWriterBigData::ByamlWriterBigData(al::ByamlWriterBigDataList* list) : mList(list) {
    mList->addData(this);
}
ByamlWriterBigData::~ByamlWriterBigData() = default;
void ByamlWriterBigData::write(sead::WriteStream* stream) const {
    stream->writeU32(mOffset);
}
u32 ByamlWriterBigData::calcBigDataSize() const {return 8;}
void ByamlWriterBigData::writeBigData(sead::WriteStream*) const {}

ByamlWriterInt64::ByamlWriterInt64(long value, ByamlWriterBigDataList* list) : ByamlWriterBigData(list), mValue(value) {}
ByamlWriterInt64::~ByamlWriterInt64() = default;
u8 ByamlWriterInt64::getTypeCode() const {return 0xD4;}
void ByamlWriterInt64::writeBigData(sead::WriteStream* stream) const {stream->writeS64(mValue);}
void ByamlWriterInt64::print(int) const {}

ByamlWriterUInt64::ByamlWriterUInt64(u64 value, ByamlWriterBigDataList* list) : ByamlWriterBigData(list), mValue(value) {}
ByamlWriterUInt64::~ByamlWriterUInt64() = default;
u8 ByamlWriterUInt64::getTypeCode() const {return 0xD5;}
void ByamlWriterUInt64::writeBigData(sead::WriteStream* stream) const {stream->writeU64(mValue);}
void ByamlWriterUInt64::print(int) const {}

ByamlWriterDouble::ByamlWriterDouble(double value, ByamlWriterBigDataList* list) : ByamlWriterBigData(list), mValue(value) {}
ByamlWriterDouble::~ByamlWriterDouble() = default;
u8 ByamlWriterDouble::getTypeCode() const {return 0xD6;}
void ByamlWriterDouble::writeBigData(sead::WriteStream* stream) const {stream->writeU64(*reinterpret_cast<const u64*>(&mValue));}
void ByamlWriterDouble::print(int) const {}




bool ByamlWriterContainer::isContainer() const {return true;}

void ByamlWriterContainer::addBool(const char*,bool) {}
void ByamlWriterContainer::addInt(const char*,s32) {}
void ByamlWriterContainer::addUInt(const char*,u32) {}
void ByamlWriterContainer::addFloat(const char*,float) {}
void ByamlWriterContainer::addInt64(const char*,s64, ByamlWriterBigDataList*) {}
void ByamlWriterContainer::addUInt64(const char*,u64, ByamlWriterBigDataList*) {}
void ByamlWriterContainer::addDouble(const char*,double, ByamlWriterBigDataList*) {}
void ByamlWriterContainer::addString(const char*,const char*) {}
void ByamlWriterContainer::addHash(const char*,ByamlWriterHash*) {}
void ByamlWriterContainer::addArray(const char*,ByamlWriterArray*) {}
void ByamlWriterContainer::addNull(const char*) {}

void ByamlWriterContainer::addBool(bool) {}
void ByamlWriterContainer::addInt(s32) {}
void ByamlWriterContainer::addUInt(u32) {}
void ByamlWriterContainer::addFloat(float) {}
void ByamlWriterContainer::addInt64(s64, ByamlWriterBigDataList*) {}
void ByamlWriterContainer::addUInt64(u64, ByamlWriterBigDataList*) {}
void ByamlWriterContainer::addDouble(double, ByamlWriterBigDataList*) {}
void ByamlWriterContainer::addString(const char*) {}
void ByamlWriterContainer::addHash(ByamlWriterHash*) {}
void ByamlWriterContainer::addArray(ByamlWriterArray*) {}
void ByamlWriterContainer::addNull() {}

void ByamlWriterContainer::writeContainer(sead::WriteStream*) const {}
bool ByamlWriterContainer::isHash() const {return false;}
bool ByamlWriterContainer::isArray() const {return false;}
void ByamlWriterContainer::deleteData() {}

}
