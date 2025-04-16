#pragma once

namespace agl {
class DrawContext;
class RenderBuffer;
class TextureData;
}  // namespace agl

namespace sead {
class Camera;
class Viewport;
}  // namespace sead

namespace al {
class DeferredRendering;
class EffectSystem;
class ExecuteDirector;
class GraphicsQualityInfo;
class GraphicsRenderInfo;
class GraphicsSystemInfo;
class Projection;
class RenderVariables;
class SceneCameraInfo;
class ScreenFader;
class ShaderHolder;
class SimpleModelEnv;
class ViewInfo;
struct DrawSystemInfo;

class ViewRenderer {
public:
    ViewRenderer(GraphicsSystemInfo*, ExecuteDirector*, EffectSystem*, SceneCameraInfo*);
    ~ViewRenderer();

    void clearRequest();
    void calcView(s32, const sead::Camera*, const Projection*);
    void preDrawGraphics();
    void drawView(s32, DrawSystemInfo*, const Projection&, const sead::Camera&,
                  const agl::RenderBuffer*, const sead::Viewport&, bool, bool, bool) const;
    void drawView(const ViewInfo&, DrawSystemInfo*, const Projection&, const sead::Camera&,
                  const agl::RenderBuffer*, const sead::Viewport&, bool, bool, bool) const;
    void drawSystem(const GraphicsRenderInfo&) const;
    void drawMirror(agl::DrawContext*, s32, RenderVariables*) const;
    void drawHdr(const GraphicsRenderInfo&, const RenderVariables&, bool, bool) const;
    void captureIndirectTexture(agl::DrawContext*, const agl::TextureData*,
                                const agl::TextureData*) const;
    void startForwardPlayerScreenFader(s32, s32, f32);
    void endForwardPlayerScreenFader(s32);

private:
    GraphicsSystemInfo* mGraphicsSystemInfo;
    ExecuteDirector* mExecuteDirector;
    EffectSystem* mEffectSystem;
    SimpleModelEnv* mSimpleModelEnv;
    DeferredRendering* mDeferredRendering;
    SceneCameraInfo* mSceneCameraInfo;
    GraphicsQualityInfo* mGraphicsQualityInfo;
    ScreenFader* mScreenFader;
    void* _40;
    void* _48;
    bool _50;
    bool mIsWorldMap;
    const agl::TextureData* mEffectTextureColor;
    void* _60;
};
}  // namespace al

namespace alViewRendererFunction {
void createLinearDepthFromDepthBuffer(agl::DrawContext*, const al::ShaderHolder*,
                                      const agl::TextureData*, const agl::TextureData*);
}
