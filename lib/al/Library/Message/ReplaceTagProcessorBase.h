#pragma once

#include <basis/seadTypes.h>

namespace al {
class IUseMessageSystem;
class MessageTag;

class ReplaceTagProcessorBase {
public:
    virtual s32 replaceProjectTag(char16*, const MessageTag&, const IUseMessageSystem*) const;
};

static_assert(sizeof(ReplaceTagProcessorBase) == 0x8);
}  // namespace al
