#pragma once

#include <math/seadVector.h>

#include "Library/Draw/IUsePartsGraphics.h"
#include "Library/HostIO/HioNode.h"

namespace agl {
class DrawContext;
class RenderBuffer;
}  // namespace agl

namespace sead {
class Camera;
class Viewport;
}  // namespace sead

namespace agl::pfx {
class FilterAA;
}

namespace al {
class EffectSystem;
class ExecuteDirector;
class GraphicsSystemInfo;
class Projection;
class SimpleModelEnv;

class ChromakeyDrawer : public IUsePartsGraphics, public HioNode {
public:
    ChromakeyDrawer(GraphicsSystemInfo*, const ExecuteDirector*, const EffectSystem*);

    void finalize() override;
    void drawChromakey(agl::DrawContext*, const Projection&, const sead::Camera&,
                       const agl::RenderBuffer*, const sead::Viewport&, const char*, const char*,
                       const char*) const;
    void update(const GraphicsUpdateInfo&) override;
    void calcGpu(const GraphicsCalcGpuInfo&) override;
    const char* getName() const override;

    void setPhysicalArea(sead::Vector2f area) { mPhysicalArea = area; }

private:
    GraphicsSystemInfo* mGraphicsSystemInfo;
    agl::pfx::FilterAA* mFilterAA;
    const ExecuteDirector* mExecuteDirector;
    const EffectSystem* mEffectSystem;
    sead::Vector2f mPhysicalArea;
    bool _30;
    SimpleModelEnv* mSimpleModelEnv;
};
}  // namespace al
