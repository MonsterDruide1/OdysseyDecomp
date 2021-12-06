#include "al/byaml/ByamlData.h"

namespace al {

ByamlData::ByamlData() = default;

void ByamlData::set(const ByamlHashPair* hash_pair, bool isRev) {
    mType = hash_pair->getType();
    mValue = isRev ? bswap_32(hash_pair->getValue(false)) : hash_pair->getValue(false);
}

void ByamlData::set(u8 type, u32 value, bool isRev) {
    mType = (ByamlDataType)type;
    mValue = isRev ? bswap_32(value) : value;
}

ByamlDataType ByamlData::getType() const {
    return mType;
}

unsigned int ByamlData::getValue() const {
    return mValue;
}

int ByamlHashPair::getKey(bool isRev) const {
    return isRev ? bswap_24(mData) : mData & 0xFFFFFF;
}

ByamlDataType ByamlHashPair::getType() const {
    return mType;
}

int ByamlHashPair::getValue(bool isRev) const {
    return isRev ? bswap_32(mValue) : mValue;
}

}  // namespace al
