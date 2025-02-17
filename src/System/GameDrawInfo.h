#pragma once

namespace agl {
class RenderBuffer;
class DrawContext;
}

namespace al {
struct GameDrawInfo {
    agl::RenderBuffer* mFirstRenderBuffer;
    agl::RenderBuffer* mSecondRenderBuffer;
    bool mIsFirst;
    agl::DrawContext* mDrawContext;
};
}  // namespace al
