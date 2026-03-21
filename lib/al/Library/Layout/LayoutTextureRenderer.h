#pragma once

#include <container/seadPtrArray.h>

#include "Library/HostIO/HioNode.h"
#include "Library/Scene/ISceneObj.h"

#include "Scene/SceneObjFactory.h"

namespace al {
class LayoutTextureRenderObj;

class LayoutTextureRenderer : public HioNode, public ISceneObj {
public:
    static constexpr s32 sSceneObjId = SceneObjID_alLayoutTextureRenderer;

    LayoutTextureRenderer();
    ~LayoutTextureRenderer() override;

    void addRenderObj(LayoutTextureRenderObj* renderObj);
    void drawToTexture() const;

    const char* getSceneObjName() const override { return "レイアウトテクスチャ生成"; }

private:
    sead::PtrArray<LayoutTextureRenderObj> mRenderObjs;
};

static_assert(sizeof(LayoutTextureRenderer) == 0x18);
}  // namespace al
