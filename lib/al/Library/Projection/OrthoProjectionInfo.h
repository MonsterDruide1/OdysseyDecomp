#pragma once

namespace al {

struct OrthoProjectionInfo {
    f32 nearClipWidth = -1.0f;
    f32 nearClipHeight = -1.0f;
};

static_assert(sizeof(OrthoProjectionInfo) == 0x8);

}  // namespace al
