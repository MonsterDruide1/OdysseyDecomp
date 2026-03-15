#pragma once

#include <container/seadPtrArray.h>

#include "Library/HostIO/HioNode.h"
#include "Library/Scene/ISceneObj.h"

namespace al {
class LayoutTextureRenderObj;

class LayoutTextureRenderer : public HioNode, public ISceneObj {
public:
    LayoutTextureRenderer();
    ~LayoutTextureRenderer() override;

    void addRenderObj(LayoutTextureRenderObj* renderObj);
    void drawToTexture() const;

    const char* getSceneObjName() const override { return "レイアウトテクスチャ生成"; }

private:
    sead::PtrArray<LayoutTextureRenderObj> mRenderObjs;
};
}  // namespace al

static_assert(sizeof(al::LayoutTextureRenderer) == 0x18, "LayoutTextureRenderer size");
