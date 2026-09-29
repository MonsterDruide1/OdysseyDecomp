#pragma once

#include <prim/seadSafeString.h>

namespace al {

class IEventFlowActionNameConverter {
public:
    virtual s32 convertActionName(sead::BufferedSafeString*, const char*) const = 0;
};

}  // namespace al
