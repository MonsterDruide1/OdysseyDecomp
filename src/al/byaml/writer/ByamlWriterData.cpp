#include "al/byaml/writer/ByamlWriterData.h"

#include <stream/seadStream.h>

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

}
