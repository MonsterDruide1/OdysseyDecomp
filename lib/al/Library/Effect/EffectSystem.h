#pragma once

#include "Library/Effect/EffectSystemInfo.h"

namespace agl {
class DrawContext;
}

namespace sead {
class Heap;
}

namespace al {
class CameraDirector;
class EffectCameraHolder;
class EffectEnvParam;
class EffectLayoutDrawer;
class EffectShaderHolder;
class EffectSystemInfo;
class ExecuteDirector;
class GraphicsSystemInfo;

class EffectSystem {
public:
    static EffectSystem* initializeSystem(agl::DrawContext*, sead::Heap*);

    EffectSystem();

    void initScene();
    void startScene(ExecuteDirector*);
    void endScene();
    void endInit();
    void setCameraDirector(CameraDirector*);
    void setGraphicsSystemInfo(const GraphicsSystemInfo*);

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
