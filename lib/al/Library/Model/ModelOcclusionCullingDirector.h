#pragma once

#include <heap/seadHeap.h>
#include <math/seadMatrix.h>

namespace agl {
class DrawContext;
}

namespace al {
class ModelOcclusionQuery;

class ModelOcclusionCullingDirector {
public:
    ModelOcclusionCullingDirector();
    ~ModelOcclusionCullingDirector();

    void initialize(s32, sead::Heap*);
    bool isEnable() const;
    void addQuery(ModelOcclusionQuery*);
    void removeQuery(ModelOcclusionQuery*);
    void clearRequest();
    void update();
    void calc();
    void updateViewGpu(s32, const sead::Matrix34f&, const sead::Matrix44f&);
    void draw(agl::DrawContext*, s32) const;

private:
    void* _0[4];
};

static_assert(sizeof(ModelOcclusionCullingDirector) == 0x20);

}  // namespace al
