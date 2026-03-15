#include "Library/Layout/LayoutTextureRenderer.h"

#include "Library/Layout/LayoutTextureRenderObj.h"

namespace al {
LayoutTextureRenderer::LayoutTextureRenderer() {
    mRenderObjs.allocBuffer(0x80, nullptr);
}

LayoutTextureRenderer::~LayoutTextureRenderer() {
    for (s32 i = 0, size = mRenderObjs.size(); i < size; i++)
        if (LayoutTextureRenderObj* renderObj = mRenderObjs[i])
            renderObj->destroy();
}

void LayoutTextureRenderer::addRenderObj(LayoutTextureRenderObj* renderObj) {
    mRenderObjs.pushBack(renderObj);
}

void LayoutTextureRenderer::drawToTexture() const {
    for (s32 i = 0, size = mRenderObjs.size(); i < size; i++)
        mRenderObjs[i]->drawToTexture();
}
}  // namespace al
