#include "al/Library/Yaml/ByamlContainerHeader.h"

#include <byteswap.h>

namespace al {
int ByamlContainerHeader::getType() const {
    return mType;
}

int ByamlContainerHeader::getCount(bool isRev) const {
    if (isRev)
        return bswap_32_ignore_last(mType);

    return (mType >> 8);
}
}  // namespace al
