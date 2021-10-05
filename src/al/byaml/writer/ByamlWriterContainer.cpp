#include "al/byaml/writer/ByamlWriterContainer.h"

namespace al {

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
