#pragma once

#include <basis/seadTypes.h>

namespace al {

struct OrthoProjectionInfo {
    f32 mNearClipWidth;
    f32 mNearClipHeight;
};

static_assert(sizeof(OrthoProjectionInfo) == 0x8);

}  // namespace al
