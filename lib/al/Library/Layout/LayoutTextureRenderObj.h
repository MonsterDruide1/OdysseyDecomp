#pragma once

#include <basis/seadTypes.h>
#include <common/aglGPUMemBlock.h>

#include "Library/HostIO/HioNode.h"
#include "Library/Layout/IUseLayout.h"
#include "Library/Layout/IUseLayoutAction.h"

namespace nn::ui2d {
class DrawInfo;
}

namespace agl {
class TextureData;
class RenderBuffer;
class RenderTargetColor;
class DrawContext;
}  // namespace agl

namespace al {
class TextureReplacer;
class LayoutInitInfo;

class LayoutTextureRenderObj : public HioNode, public IUseLayout, public IUseLayoutAction {
public:
    LayoutTextureRenderObj(const char* name, const LayoutInitInfo& initInfo,
                           const char* archiveName, const char*);
    ~LayoutTextureRenderObj();

    void destroy();
    void updateLayout();
    void requestRenderTexture();
    void drawToTexture() const;

    const char* getName() const override { return mName; }

    LayoutKeeper* getLayoutKeeper() const override { return mLayoutKeeper; }

    LayoutActionKeeper* getLayoutActionKeeper() const override { return mLayoutActionKeeper; }

private:
    const char* mName;
    bool mIsAlreadyRendered;
    u32 mWidth;
    u32 mHeight;
    agl::TextureData* mTextureData;
    u32 mTextureStorageSize;
    agl::GPUMemBlock<u8> mTextureStorage;
    LayoutKeeper* mLayoutKeeper;
    LayoutActionKeeper* mLayoutActionKeeper;
    agl::RenderBuffer* mRenderBuffer;
    agl::RenderTargetColor* mRenderTargetColor;
    TextureReplacer* mTextureReplacer;
    agl::DrawContext* mDrawContext;
    nn::ui2d::DrawInfo* mDrawInfo;
};
}  // namespace al

static_assert(sizeof(al::LayoutTextureRenderObj) == 0xa8, "LayoutTextureRenderObj size");
