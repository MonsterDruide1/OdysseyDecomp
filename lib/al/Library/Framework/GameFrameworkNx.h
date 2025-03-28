#pragma once

#include <basis/seadTypes.h>
#include <framework/nx/seadGameFrameworkNx.h>

#include "Library/HostIO/HioNode.h"

namespace agl {
class DisplayList;
class DrawContext;
class RenderBuffer;
class RenderTargetColor;
}  // namespace agl

namespace sead {
class Event;
}

namespace al {

class GpuPerf;

class GameFrameworkNx : public sead::GameFrameworkNx, public HioNode {
    SEAD_RTTI_OVERRIDE(GameFrameworkNx, sead::GameFrameworkNx);

public:
    GameFrameworkNx();
    ~GameFrameworkNx() override;
    void createControllerMgr(sead::TaskBase* base) override;
    void createHostIOMgr(sead::TaskBase* base, sead::HostIOMgr::Parameter* hostioParam,
                         sead::Heap* heap) override;
    void createInfLoopChecker(sead::TaskBase* base, const sead::TickSpan&, s32) override;

    void clearFrameBuffer();
    void initAgl(sead::Heap* heap, s32 virtWidth, s32 virtHeight, s32 dockedWidth, s32 dockedHeight,
                 s32 handheldWidth, s32 handheldHeight);

private:
    void procFrame_() override;
    void procDraw_() override;
    void present_() override;

    agl::DrawContext* mDrawContext;
    agl::RenderBuffer* mDockedRenderBuffer;
    agl::RenderTargetColor* mDockedClearColor;
    agl::RenderBuffer* mHandheldRenderBuffer;
    agl::RenderTargetColor* mHandheldClearColor;
    agl::DisplayList* mDisplayList[2];
    GpuPerf* mGpuPerf;
    s32 mCurDisplayListIdx;
    void* mDisplayControlMemory[2];
    bool mIsClearRenderBuffer;
    bool mIsDocked;
    sead::Event* mDrawReadyEvent;
};

static_assert(sizeof(GameFrameworkNx) == 0x278);

}  // namespace al
