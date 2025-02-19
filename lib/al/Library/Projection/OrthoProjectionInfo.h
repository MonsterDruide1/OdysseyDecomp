#pragma once

#include <basis/seadTypes.h>

namespace al {

struct OrthoProjectionInfo {
    f32 mNearClipWidth = -1.0f;
    f32 mNearClipHeight = -1.0f;
};

static_assert(sizeof(OrthoProjectionInfo) == 0x8);

}  // namespace al
