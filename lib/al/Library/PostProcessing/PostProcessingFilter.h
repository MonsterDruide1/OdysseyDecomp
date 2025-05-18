#pragma once

#include <basis/seadTypes.h>

namespace sead {
class Camera;
}  // namespace sead

namespace agl {
class DrawContext;
class TextureData;
class RenderBuffer;
}  // namespace agl

namespace al {
class CartoonDrawer;
class ColorClampDrawer;
class DepthOfFieldDrawer;
class EdgeDrawer;
class GraphicsParamRequesterImpl;
class NoiseTextureKeeper;
class PencilSketchDrawer;
class Projection;
class RetroColorDrawer;
class ScreenBlurDrawer;
class SimpleModelEnv;
class ShaderHolder;
class VignettingDrawer;
class ViewDepthDrawer;

class PostProcessingFilter {
public:
    PostProcessingFilter(ShaderHolder*, NoiseTextureKeeper*);

    void incrementPreset();
    void decrementPreset();
    void initProjectResource();
    void endInit();
    void clearRequest();
    void update();
    void findPreset(s32) const;
    void updateViewGpu(s32, const sead::Camera*, const Projection*);
    void drawFilter(agl::DrawContext*, s32, SimpleModelEnv*, agl::RenderBuffer&,
                    const agl::TextureData&, const agl::TextureData&, const agl::TextureData&,
                    const sead::Camera&, const Projection&, f32, f32, f32);

    void validate() { mIsValid = true; }

    void invalidate() { mIsValid = false; }

    s32 getPresetId() { return mPresetId; }

private:
    bool mIsValid;
    ShaderHolder* mShaderHolder;
    DepthOfFieldDrawer* mDepthOfFieldDrawer;
    GraphicsParamRequesterImpl* mGraphicsParamRequesterImpl;
    ViewDepthDrawer* mViewDepthDrawer;
    VignettingDrawer* mVignettingDrawer;
    EdgeDrawer* mEdgeDrawer;
    CartoonDrawer* mCartoonDrawer;
    RetroColorDrawer* mRetroColorDrawer;
    ScreenBlurDrawer* mScreenBlurDrawer;
    PencilSketchDrawer* mPencilSketchDrawer;
    ColorClampDrawer* mColorClampDrawer;
    s32 _60;
    void** _68;
    s32 mPresetId;
};

}  // namespace al
