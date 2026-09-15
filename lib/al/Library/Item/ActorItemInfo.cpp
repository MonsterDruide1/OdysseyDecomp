#include "Library/Item/ActorItemInfo.h"

#include "Library/Base/StringUtil.h"

namespace al {

ActorItemInfo::ActorItemInfo(const char* name, const char* timing, const char* factor, s32 value) {
    mName = name;
    mTiming = timing;
    mFactor = factor;
    _18 = 0;
    mValue = value;
}

bool ActorItemInfo::isEqualTiming(const char* timing) const {
    if (timing != nullptr && mTiming != nullptr)
        return isEqualString(timing, mTiming);
    return timing == nullptr && mTiming == nullptr;
}

bool ActorItemInfo::isEqualFactor(const char* factor) const {
    if (factor != nullptr && mFactor != nullptr)
        return isEqualString(factor, mFactor);
    return factor == nullptr && mFactor == nullptr;
}

}  // namespace al
