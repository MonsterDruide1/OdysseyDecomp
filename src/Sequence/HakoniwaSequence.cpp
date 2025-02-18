#include "Sequence/HakoniwaSequence.h"

#include <common/aglDrawContext.h>
#include <common/aglRenderBuffer.h>
#include <gfx/seadViewport.h>

#include "Library/Layout/LayoutUtil.h"
#include "Library/Screen/ScreenFunction.h"
#include "Library/Sequence/Sequence.h"
#include "Library/System/GameSystemInfo.h"

void HakoniwaSequence::drawMain() const {
    al::Sequence::drawMain();
    al::DrawSystemInfo* info = getDrawInfo();
    agl::DrawContext* context = info->mDrawContext;
    const agl::RenderBuffer* buffer = info->mDockedRenderBuffer;
    if (!info->mIsDocked)
        buffer = info->mHandheldRenderBuffer;

    mScreenCaptureExecutor->tryCaptureAndDraw(context, buffer, 0);
    sead::Viewport viewport = sead::Viewport(*buffer);
    viewport.apply(context, *buffer);
    buffer->bind(context);
    al::setRenderBuffer(mLayoutKit, buffer);
    al::executeDraw(mLayoutKit, "２Ｄバック（メイン画面）");
    al::executeDraw(mLayoutKit, "２Ｄベース（メイン画面）");
    al::executeDraw(mLayoutKit, "２Ｄオーバー（メイン画面）");
}
