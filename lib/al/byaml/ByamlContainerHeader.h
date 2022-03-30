#pragma once

#include <basis/seadTypes.h>

namespace al {
class ByamlContainerHeader {
public:
    int getType() const;
    int getCount(bool isRev) const;

private:
    u32 mType;
};
}  // namespace al
