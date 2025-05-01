#pragma once

#include <math/seadVector.h>

#include "Library/Nerve/NerveExecutor.h"

namespace agl {
class DrawContext;
class RenderTargetDepth;
class TextureData;
}  // namespace agl

namespace al {
class ExecuteDirector;
class GraphicsSystemInfo;
class SceneCameraInfo;

class SubCameraRenderer : public NerveExecutor {
public:
    SubCameraRenderer(agl::DrawContext*, GraphicsSystemInfo*, ExecuteDirector*, SceneCameraInfo*);
    ~SubCameraRenderer() override;

    void endInit();
    void draw(agl::DrawContext*, const agl::TextureData*, const agl::TextureData*,
              const agl::TextureData*, const agl::TextureData*, const agl::RenderTargetDepth*,
              bool);
    void tryCapture();
    void update();

    void exeCapture();
    void exeCaptureFinish();
    bool isCaptureFinish() const;
    void* findCameraSubAreaScreenInfo(const sead::Vector3f&) const;  // TODO unknown return type
    void calcOnScreenPos(sead::Vector3f*, const sead::Vector3f&) const;

private:
    void* _padding[0x4a];
};

static_assert(sizeof(SubCameraRenderer) == 0x260);

}  // namespace al
