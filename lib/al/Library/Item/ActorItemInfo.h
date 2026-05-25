#pragma once

#include <basis/seadTypes.h>

namespace al {

class ActorItemInfo {
public:
    ActorItemInfo(const char* name, const char* timing, const char* factor, s32 value);

    bool isEqualTiming(const char* timing) const;
    bool isEqualFactor(const char* factor) const;

private:
    const char* mName;
    const char* mTiming;
    const char* mFactor;
    s32 _18;
    s32 mValue;
};

static_assert(sizeof(ActorItemInfo) == 0x20);

}  // namespace al
