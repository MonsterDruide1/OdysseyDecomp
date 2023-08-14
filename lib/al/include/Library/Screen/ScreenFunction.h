#pragma once

#include <basis/seadTypes.h>
#include <common/aglRenderBuffer.h>
#include <container/seadPtrArray.h>
#include <gfx/seadCamera.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/Camera/IUseCamera.h"
#include "Library/HostIO/HioNode.h"

namespace al {

class SceneCameraInfo;
class ScreenCapture;

class ScreenCaptureExecutor : IUseHioNode {
private:
    sead::PtrArray<ScreenCapture> mArray;
    bool mIsCaptured;

public:
    ScreenCaptureExecutor(s32);
    ~ScreenCaptureExecutor();

    void createScreenCapture(s32, s32, s32);
    void draw(agl::DrawContext*, const agl::RenderBuffer*, s32) const;
    void tryCapture(agl::DrawContext*, const agl::RenderBuffer*, s32);
    void tryCaptureAndDraw(agl::DrawContext*, const agl::RenderBuffer*, s32);

    void requestCapture(bool, s32);
    void onDraw(s32 screenCaptureIndex);
    void offDraw(s32 screenCaptureIndex);
    void offDraw();

    bool isDraw(s32) const;
};

class ScreenCoverCtrl {
private:
    s32 mFrameTimer;
    bool mIsActive;

public:
    ScreenCoverCtrl();
    void requestCaptureScreenCover(s32 totalFrames);
    void update();
};

u32 getDisplayWidth();
u32 getDisplayHeight();
u32 getLayoutDisplayWidth();
u32 getLayoutDisplayHeight();
u32 getVirtualDisplayWidth();
u32 getVirtualDisplayHeight();
u32 getSubDisplayWidth();
u32 getSubDisplayHeight();

bool isInScreen(const sead::Vector2f&, f32);

bool calcWorldPosFromScreen(sead::Vector3f* output, const sead::Vector2f&, const sead::Matrix34f&,
                            f32);  // Always returns true
void calcWorldPosFromScreenPos(sead::Vector3f* output, const IUseCamera*, const sead::Vector2f,
                               f32);
void calcWorldPosFromScreenPos(sead::Vector3f* output, const IUseCamera*, const sead::Vector2f,
                               const sead::Vector3f);
void calcWorldPosFromScreenPos(sead::Vector3f* output, const SceneCameraInfo*, const sead::Vector2f,
                               f32, s32);
void calcWorldPosFromScreenPos(sead::Vector3f* output, const SceneCameraInfo*, const sead::Vector2f,
                               const sead::Vector3f, s32);
void calcWorldPosFromScreenPosSub(sead::Vector3f* output, const IUseCamera*, const sead::Vector2f,
                                  f32);
void calcWorldPosFromScreenPosSub(sead::Vector3f* output, const IUseCamera*, const sead::Vector2f,
                                  const sead::Vector3f);
void calcWorldPosFromLayoutPos(sead::Vector3f* output, const IUseCamera*, const sead::Vector2f,
                               f32);
void calcWorldPosFromLayoutPos(sead::Vector3f* output, const IUseCamera*, const sead::Vector2f,
                               const sead::Vector3f);
void calcWorldPosFromLayoutPos(sead::Vector3f* output, const SceneCameraInfo*, const sead::Vector2f,
                               s32, s32);
void calcWorldPosFromLayoutPos(sead::Vector3f* output, const SceneCameraInfo*, const sead::Vector2f,
                               const sead::Vector3f, s32);
void calcWorldPosFromLayoutPosSub(sead::Vector3f* output, const IUseCamera*, const sead::Vector2f,
                                  f32);
void calcWorldPosFromLayoutPosSub(sead::Vector3f* output, const IUseCamera*, const sead::Vector2f,
                                  const sead::Vector3f);

void calcScreenPosFromWorldPos(sead::Vector2f* output, const IUseCamera*, const sead::Vector3f&);
void calcScreenPosFromWorldPosSub(sead::Vector2f* output, const IUseCamera*, const sead::Vector3f&);
void calcScreenPosFromLayoutPos(sead::Vector2f* output, const sead::Vector2f&);
f32 calcScreenRadiusFromWorldRadius(const sead::Vector3f&, const IUseCamera*, f32);
f32 calcScreenRadiusFromWorldRadiusSub(const sead::Vector3f&, const IUseCamera*, f32);

void calcLayoutPosFromScreenPos(sead::Vector2f* output, const sead::Vector2f&);
void calcLayoutPosFromWorldPos(sead::Vector2f* output, const IUseCamera*, const sead::Vector3f&);
void calcLayoutPosFromWorldPos(sead::Vector3f* output, const IUseCamera*, const sead::Vector3f&);
void calcLayoutPosFromWorldPos(sead::Vector2f* output, const SceneCameraInfo*,
                               const sead::Vector3f&, s32);
void calcLayoutPosFromWorldPos(sead::Vector3f* output, const SceneCameraInfo*,
                               const sead::Vector3f&, s32);
void calcLayoutPosFromWorldPosSub(sead::Vector2f* output, const IUseCamera*, const sead::Vector3f&);
void calcLayoutPosFromWorldPosWithClampOutRange(sead::Vector3f* output, const IUseCamera*,
                                                const sead::Vector3f&, f32, s32);
void calcLayoutPosFromWorldPosWithClampOutRange(sead::Vector3f* output, const SceneCameraInfo*,
                                                const sead::Vector3f&, f32, s32);
void calcLayoutPosFromWorldPosWithClampByScreen(sead::Vector3f* output, const IUseCamera*,
                                                const sead::Vector3f&);
f32 calcLayoutRadiusFromWorldRadius(const sead::Vector3f&, const IUseCamera*, f32);

bool calcCameraPosToWorldPosDirFromScreenPos(sead::Vector3f* output, const IUseCamera*,
                                             const sead::Vector2f&, f32);
bool calcCameraPosToWorldPosDirFromScreenPos(sead::Vector3f* output, const IUseCamera*,
                                             const sead::Vector2f&, const sead::Vector3f&);
bool calcCameraPosToWorldPosDirFromScreenPos(sead::Vector3f* output, const SceneCameraInfo*,
                                             const sead::Vector2f&, f32, s32);
bool calcCameraPosToWorldPosDirFromScreenPos(sead::Vector3f* output, const SceneCameraInfo*,
                                             const sead::Vector2f&, const sead::Vector3f&, s32);
void calcCameraPosToWorldPosDirFromScreenPosSub(sead::Vector3f* output, const IUseCamera*,
                                                const sead::Vector2f&, f32);
void calcCameraPosToWorldPosDirFromScreenPosSub(sead::Vector3f* output, const IUseCamera*,
                                                const sead::Vector2f&, const sead::Vector3f&);

void calcLineCameraToWorldPosFromScreenPos(sead::Vector3f* output1, sead::Vector3f* output2,
                                           const IUseCamera*, const sead::Vector2f&);
void calcLineCameraToWorldPosFromScreenPos(sead::Vector3f* output1, sead::Vector3f* output2,
                                           const IUseCamera*, const sead::Vector2f&, f32, f32);
void calcLineCameraToWorldPosFromScreenPos(sead::Vector3f* output1, sead::Vector3f* output2,
                                           const SceneCameraInfo*, const sead::Vector2f&, s32);
void calcLineCameraToWorldPosFromScreenPos(sead::Vector3f* output1, sead::Vector3f* output2,
                                           const SceneCameraInfo*, const sead::Vector2f&, f32, f32,
                                           s32);
void calcLineCameraToWorldPosFromScreenPosSub(sead::Vector3f* output1, sead::Vector3f* output2,
                                              const IUseCamera*, const sead::Vector2f&);
void calcLineCameraToWorldPosFromScreenPosSub(sead::Vector3f* output1, sead::Vector3f* output2,
                                              const IUseCamera*, const sead::Vector2f&, f32, f32);
}  // namespace al

namespace ScreenFunction {
void calcWorldPositionFromCenterScreen(sead::Vector3f*, const sead::Vector2f&,
                                       const sead::Vector3f&, const sead::Camera&,
                                       const sead::Projection&, const sead::Viewport&);
}  // namespace ScreenFunction
