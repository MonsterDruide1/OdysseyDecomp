#include "Library/Yaml/ByamlContainerHeader.h"

#include <byteswap.h>

namespace al {
s32 ByamlContainerHeader::getType() const {
    return mType;
}

s32 ByamlContainerHeader::getCount(bool isRev) const {
    if (isRev)
        return bswap_32_ignore_last(mType);

    return (mType >> 8);
}
}  // namespace al
