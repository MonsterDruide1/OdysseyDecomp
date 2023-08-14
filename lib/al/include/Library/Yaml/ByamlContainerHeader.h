#pragma once

#include <basis/seadTypes.h>

namespace al {
class ByamlContainerHeader {
public:
    s32 getType() const;
    s32 getCount(bool isRev) const;

private:
    u32 mType;
};
}  // namespace al
