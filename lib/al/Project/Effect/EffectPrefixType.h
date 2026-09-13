#pragma once

#include <basis/seadTypes.h>

namespace al {
class EffectPrefixType {
public:
    explicit constexpr EffectPrefixType(s32 type) : mType(type) {}

private:
    static const char* text_(s32);

    s32 mType;
};

static_assert(sizeof(EffectPrefixType) == 0x4);
}  // namespace al
