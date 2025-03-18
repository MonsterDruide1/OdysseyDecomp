#pragma once

#include <common/aglDrawContext.h>
#include <common/aglRenderBuffer.h>
#include <gfx/seadCamera.h>
#include <gfx/seadViewport.h>

#include "Library/Draw/IUsePartsGraphics.h"

namespace agl::pfx {
class FilterAA;
}

namespace al {
class GraphicsSystemInfo;
class ExecuteDirector;
class EffectSystem;
class Projection;
class SimpleModelEnv;

class ChromakeyDrawer : public IUsePartsGraphics {
public:
    ChromakeyDrawer(GraphicsSystemInfo*, const ExecuteDirector*, const EffectSystem*);

    virtual void finalize();
    void drawChromakey(agl::DrawContext*, const Projection&, const sead::Camera&,
                       const agl::RenderBuffer*, const sead::Viewport&, const char*, const char*,
                       const char*);
    virtual void update(const GraphicsUpdateInfo&);
    virtual void calcGpu(const GraphicsCalcGpuInfo&);
    void getName() const;

    void setPhysicalArea(sead::Vector2f area) { mPhysicalArea = area; }

private:
    GraphicsSystemInfo* mGraphicsSystemInfo;
    agl::pfx::FilterAA* mFilterAA;
    const ExecuteDirector* mExecuteDirector;
    const EffectSystem* mEffectSystem;
    sead::Vector2f mPhysicalArea;
    bool field_30;
    SimpleModelEnv* mSimpleModelEnv;
};
}  // namespace al
