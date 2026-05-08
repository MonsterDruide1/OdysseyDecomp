#pragma once

#include <container/seadPtrArray.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/Nerve/NerveExecutor.h"

namespace agl {
class DrawContext;
class RenderTargetDepth;
class TextureData;

namespace pfx {
class ColorCorrection;
}
}  // namespace agl

namespace al {
class AreaObj;
class ExecuteDirector;
class GraphicsSystemInfo;
class SceneCameraInfo;

struct CameraSubAreaScreenInfo {
    CameraSubAreaScreenInfo(const sead::Vector3f& scale, const sead::Matrix34f& mtx, AreaObj* obj)
        : screenScale(scale), screenMatrix(mtx), areaObj(obj) {}

    sead::Vector3f screenScale;
    sead::Matrix34f screenMatrix;
    AreaObj* areaObj;
};

static_assert(sizeof(CameraSubAreaScreenInfo) == 0x48);

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
    CameraSubAreaScreenInfo* findCameraSubAreaScreenInfo(const sead::Vector3f&) const;
    void calcOnScreenPos(sead::Vector3f*, const sead::Vector3f&) const;

    void addCameraSub() { mNumCameraSub++; }

    void addCameraSubAreaScreenInfo(CameraSubAreaScreenInfo* screenInfo) {
        mCameraSubAreaScreenInfos.pushBack(screenInfo);
    }

private:
    char _10[0x220];
    SceneCameraInfo* mSceneCameraInfo;
    s32 mNumCameraSub;
    s32 _23c;
    GraphicsSystemInfo* mGraphicsSystemInfo;
    agl::pfx::ColorCorrection* mColorCorrection;
    sead::PtrArray<CameraSubAreaScreenInfo> mCameraSubAreaScreenInfos;
};

static_assert(sizeof(SubCameraRenderer) == 0x260);

}  // namespace al
