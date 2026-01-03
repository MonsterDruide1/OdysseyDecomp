#include "Library/Yaml/ByamlContainerHeader.h"

#include <byteswap.h>

namespace al {
s32 ByamlContainerHeader::getType() const {
    return type;
}

s32 ByamlContainerHeader::getCount(bool isRev) const {
    if (isRev)
        return bswap_32_ignore_last(type);

    return (type >> 8);
}
}  // namespace al
