#pragma once

namespace agl {
class RenderBuffer;

}  // namespace agl

namespace al {
class LayoutKit;

void setRenderBuffer(LayoutKit*, const agl::RenderBuffer*);
void executeDraw(const LayoutKit*, const char*);

}  // namespace al
