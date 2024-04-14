#pragma once

namespace sead {
class Camera;
}

namespace agl {
class DrawContext;
class RenderTargetDepth;
class TextureData;
}  // namespace agl

namespace al {
class GraphicsSystemInfo;
class AreaObjDirector;
class ExecuteDirector;
class EffectSystem;
class SceneCameraInfo;
class PlayerHolder;
class Projection;
class ShaderHolder;
class DepthShadowParam;
class DepthShadowClipParam;

class ShadowDirector {
public:
    ShadowDirector(int, GraphicsSystemInfo*, AreaObjDirector*, ExecuteDirector*, EffectSystem*,
                   SceneCameraInfo*, const PlayerHolder*);
    ~ShadowDirector();

    void initShader(ShaderHolder*);
    void endInit();
    void initAfterPlacement();
    void clearRequest();
    void update();
    void* getCurrentShadowParam();  // unknown return type
    void* getCurrentClipParam();    // unknown return type
    bool isUsingShadowCamera() const;
    void preDrawGraphics();
    void isEnableShadowPrePass() const;
    void updateViewGpu(int, const sead::Camera*, const Projection*);
    void* getDepthShadow();  // unknown return type
    void drawShadowPrePass(agl::DrawContext*, int, const agl::RenderTargetDepth*,
                           const agl::RenderTargetDepth*, const agl::TextureData*);
    void* getShadowPrePassSampler(int) const;  // unknown return type
    void* getShadowPrePassTexture(int) const;  // unknown return type
    bool isEnableDepthShadow() const;
    void* getCurrentShadowParam() const;  // unknown return type
    void* getCurrentClipParam() const;    // unknown return type
    void* getCurrentNear() const;         // unknown return type
    void* getCurrentFar() const;          // unknown return type
    bool requestParam(int, int, const DepthShadowParam&);
    bool requestParam(int, int, const DepthShadowClipParam&);
};

}  // namespace al
