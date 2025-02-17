#pragma once

#include <common/aglDrawContext.h>
#include <common/aglRenderBuffer.h>

namespace al {
struct GameDrawInfo {
    agl::RenderBuffer* mFirstRenderBuffer;
    agl::RenderBuffer* mSecondRenderBuffer;
    bool mIsFirst;
    agl::DrawContext* mDrawContext;
};
}  // namespace al
