#pragma once

#include <basis/seadTypes.h>
#include <math/seadMatrix.h>

#include "Library/Effect/EffectSystemInfo.h"

namespace agl {
class DrawContext;
}

namespace agl::sdw {
class DepthShadow;
}

namespace sead {
class Heap;
}

namespace al {
class CameraDirector;
class CollisionCodeList;
class EffectCameraHolder;
class EffectEnvParam;
class EffectLayoutDrawer;
class EffectShaderHolder;
class EffectSystemInfo;
class ExecuteDirector;
class GraphicsSystemInfo;
class IUseExecutor;

class EffectSystem {
public:
    static EffectSystem* initializeSystem(agl::DrawContext*, sead::Heap*);

    EffectSystem();

    const char* getDefaultDataBaseResourcePath();
    const char* getDefaultPtclResourcePath();
    const char* getDefaultPtclPatchResourcePath();
    EffectSystem* createSystem(agl::DrawContext*, sead::Heap*);
    EffectSystem* createSystemWithPatchResouce(agl::DrawContext*, sead::Heap*);
    void loadEffectResource(EffectSystem*);
    void loadPtclResource(sead::Heap*);
    EffectSystem* initializeSystemWithPatchResouce(agl::DrawContext*, sead::Heap*);
    bool isEnableBatchCompute();
    s32 getPauseForceCalcFrame();
    void setDrawContext(agl::DrawContext*);
    void addResourcePath(const char*);
    void init();
    void loadDbResource(sead::Heap*);
    void initScene();
    void endInit();
    void startScene(ExecuteDirector*);
    void preprocess();
    void postprocess();
    void endScene();
    void setCameraDirector(CameraDirector*);
    void setMaterialCodeList(CollisionCodeList*);
    void calcParticle(u64);
    void setGraphicsSystemInfo(const GraphicsSystemInfo*);
    void updateEffect(const char*) const;
    IUseExecutor* findGroupDrawer(const char*) const;
    void calcEffectCompute() const;
    void drawEffectWithRenderPathAndCamPos(const sead::Matrix44f&, const sead::Matrix34f&,
                                           const sead::Vector3f&, f32, f32, f32, const char*,
                                           u32) const;
    void drawEffectWithRenderPath(const sead::Matrix44f&, const sead::Matrix34f&, f32, f32, f32,
                                  const char*, u32) const;
    void calcShadowClipVolume(agl::sdw::DepthShadow*, const char*, u32) const;
    void addCalcEffect(u64);
    bool isHasRenderingEmitter(u32) const;
    void checkCalculateFlag(s32);
    void calcParticle(s32);
    void calcChildParticle(s32);

    void set_69(bool val) { _69 = val; }

    EffectSystemInfo* getEffectSystemInfo() { return &mEffectSystemInfo; }

    const EffectSystemInfo* getEffectSystemInfo() const { return &mEffectSystemInfo; }

private:
    sead::Heap* mHeap;
    EffectSystemInfo mEffectSystemInfo;
    EffectCameraHolder* mEffectCameraHolder;
    s32 mExecutorSize;
    void* mExecutorList;
    s32 mResourcePathSize;
    s32 mResourcePathCapacity;
    const char** mResourcePaths;
    void* filler[3];
    bool _68;
    bool _69;
    bool _6a;
    void* filler2[99];
    s32 mEffectLayoutDrawerSize;
    EffectLayoutDrawer** mEffectLayoutDrawerList;
    EffectShaderHolder* mEffectShaderHolder;
    void* filler3[4];
    agl::DrawContext* mDrawContext;
    EffectEnvParam* mEffectEnvParam;
    void* filler4;
};

static_assert(sizeof(EffectSystem) == 0x3d8);

}  // namespace al
