#pragma once

#include <prim/seadSafeString.h>

namespace al {

class IEventFlowActionNameConverter {
public:
    virtual s32 convertActionName(sead::BufferedSafeStringBase<char>*, const char*) const;
};

}  // namespace al
