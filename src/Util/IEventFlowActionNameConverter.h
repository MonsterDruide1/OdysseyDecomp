#pragma once

#include <prim/seadSafeString.h>

namespace al {
class IEventFlowActionNameConverter {
public:
    virtual void convertActionName(sead::BufferedSafeStringBase<char>* outActionName,
                                   const char* actionName) const = 0;
};

}  // namespace al
