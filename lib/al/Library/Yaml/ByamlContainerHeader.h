#pragma once

#include <basis/seadTypes.h>

#include "Library/Yaml/ByamlData.h"

namespace al {
class ByamlContainerHeader {
public:
    s32 getType() const;
    s32 getCount(bool isRev) const;

    ByamlDataType getTypeCode() const { return (ByamlDataType)mType; }

private:
    u32 mType;
};
}  // namespace al
