#include "al/byaml/ByamlHeader.h"
#include "byteswap.h"

#define BYAML_LE_TAG 'YB'

namespace al {
short ByamlHeader::getTag() const {
    return isInvertOrder() ? bswap_16(mTag) : mTag;
}

bool ByamlHeader::isInvertOrder() const {
    return mTag == BYAML_LE_TAG;
}

short ByamlHeader::getVersion() const {
    if ((_0 & 0xFFFF) == BYAML_LE_TAG)  // isInvertOrder()
        return bswap_16(_0 >> 16);
    return _0 >> 16;
}

int ByamlHeader::getHashKeyTableOffset() const {
    return isInvertOrder() ? __bswap_32(mHashKeyOffset) : mHashKeyOffset;
}

int ByamlHeader::getStringTableOffset() const {
    return isInvertOrder() ? __bswap_32(mStringTableOffset) : mStringTableOffset;
}

int ByamlHeader::getDataOffset() const {
    return isInvertOrder() ? __bswap_32(mDataOffset) : mDataOffset;
}
}  // namespace al
