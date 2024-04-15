#pragma once

#include <basis/seadTypes.h>

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
    ShadowDirector(s32, GraphicsSystemInfo*, AreaObjDirector*, ExecuteDirector*, EffectSystem*,
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
    void updateViewGpu(s32, const sead::Camera*, const Projection*);
    void* getDepthShadow();  // unknown return type
    void drawShadowPrePass(agl::DrawContext*, s32, const agl::RenderTargetDepth*,
                           const agl::RenderTargetDepth*, const agl::TextureData*);
    void* getShadowPrePassSampler(s32) const;  // unknown return type
    void* getShadowPrePassTexture(s32) const;  // unknown return type
    bool isEnableDepthShadow() const;
    void* getCurrentShadowParam() const;  // unknown return type
    void* getCurrentClipParam() const;    // unknown return type
    void* getCurrentNear() const;         // unknown return type
    void* getCurrentFar() const;          // unknown return type
    bool requestParam(s32, s32, const DepthShadowParam&);
    bool requestParam(s32, s32, const DepthShadowClipParam&);
};

}  // namespace al
