#pragma once

#include <common/aglDrawContext.h>
#include <common/aglRenderBuffer.h>
#include <framework/nx/seadGameFrameworkNx.h>

#include "Library/HostIO/HioNode.h"

namespace sead {
class Event;
}

namespace al {
class GpuPerf;

class GameFrameworkNx : public sead::GameFrameworkNx, public HioNode {
    SEAD_RTTI_OVERRIDE(GameFrameworkNx, sead::GameFramework);

public:
    GameFrameworkNx(const sead::GameFrameworkNx::CreateArg&);
    virtual void createControllerMgr(sead::TaskBase*) override;
    void initAgl(sead::Heap*, s32, s32, s32, s32, s32, s32);
    virtual void createInfLoopChecker(sead::TaskBase*, const sead::TickSpan&, s32) override;
    virtual void createHostIOMgr(sead::TaskBase*, sead::HostIOMgr::Parameter*,
                                 sead::Heap*) override;
    void clearFrameBuffer();

    void enableRendering() { mIsNotRendering = false; }

    void disableRendering() { mIsNotRendering = true; }

private:
    void procFrame_();
    void procDraw_();
    void present_();

    agl::DrawContext* mDrawContext;
    agl::RenderBuffer* mRenderBuffer;
    agl::RenderTargetColor* mRenderTargetColor;
    agl::RenderBuffer* mRenderBuffer2;
    agl::RenderTargetColor* mRenderTargetColor2;
    agl::DisplayList* mDisplayList[2];
    GpuPerf* mGpuPerf;
    s32 mDisplaySelection;
    void* mDisplayControlMem[2];
    bool mIsNotRendering;
    bool mIsBufferSelection;
    sead::Event* mEvent;
};
}  // namespace al
