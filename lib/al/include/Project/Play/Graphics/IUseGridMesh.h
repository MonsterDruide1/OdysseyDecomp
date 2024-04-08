#pragma once

#include <basis/seadTypes.h>
#include <common/aglDrawContext.h>

#include "Library/HostIO/HioNode.h"

namespace agl {
class IndexStream;
}

namespace al {
class QuadNode;
class MeshQuadtree;

class IUseGridMesh : public IUseHioNode {
public:
    virtual void finalize() = 0;
    virtual s32 getGridMeshType() const = 0;
    virtual s32 getGridNum() const = 0;
    virtual s32 getLodNum() const = 0;
    virtual void calcMeshScaleLod(const QuadNode& node, s32) const = 0;
    virtual agl::IndexStream* getIndexStream() const = 0;
    virtual const char* getName() const = 0;
    virtual void activateVertexAttribute(agl::DrawContext* drawContext, s32) const = 0;
    virtual void drawTreeMesh(agl::DrawContext*, const MeshQuadtree&, const QuadNode&, f32,
                              s32) = 0;
    virtual void drawArrayMesh(agl::DrawContext* drawContext, const QuadNode& node, s32, s32, s32,
                               s32, s32) = 0;
};
}  // namespace al
