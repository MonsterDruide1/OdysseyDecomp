#pragma once

namespace al {
class GraphicsCopyInfo;
class GraphicsComputeInfo;
class GraphicsUpdateInfo;
class GraphicsRenderInfo;
class GraphicsCalcGpuInfo;
class RenderVariables;

class IUsePartsGraphics {
public:
    virtual void finalize() = 0;
    virtual void endInit();
    virtual void doCommandBufferCopy(const GraphicsCopyInfo*) const;
    virtual void doComputeShader(const GraphicsComputeInfo*) const;
    virtual void drawSystem(const GraphicsRenderInfo*) const;
    virtual void update(const GraphicsUpdateInfo&) = 0;
    virtual void calcGpu(const GraphicsCalcGpuInfo&) = 0;
    virtual void drawGBufferAfterSky(const GraphicsRenderInfo&) const;
    virtual void drawForward(const GraphicsRenderInfo&, const RenderVariables&) const;
    virtual void drawDeferred(const GraphicsRenderInfo&) const;
    virtual void drawLdr(const GraphicsRenderInfo&) const;
    virtual void drawIndirect(const GraphicsRenderInfo&, const RenderVariables&) const;
    virtual void drawCubemap(const GraphicsRenderInfo&) const;
    virtual const char* getName() const = 0;
};
}  // namespace al
