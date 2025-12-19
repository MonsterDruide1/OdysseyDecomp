#pragma once

#include <basis/seadTypes.h>

#include "Library/Yaml/ByamlData.h"

namespace al {
struct ByamlContainerHeader {
public:
    s32 getType() const;
    s32 getCount(bool isRev) const;

    ByamlDataType getTypeCode() const { return (ByamlDataType)type; }

private:
    u32 type;
};
}  // namespace al
