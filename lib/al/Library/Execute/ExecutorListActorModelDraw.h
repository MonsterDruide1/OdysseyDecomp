#pragma once

#include <basis/seadTypes.h>

#include "Library/Execute/ExecutorListBase.h"

namespace agl {
class DrawContext;
}

namespace al {
class DrawContext;
struct ExecuteSystemInitInfo;
class LiveActor;
class ModelDrawerBase;

class ExecutorListActorModelDrawBase : public ExecutorListBase {
public:
    ExecutorListActorModelDrawBase(const char* name, s32 size,
                                   const ExecuteSystemInitInfo& initInfo);
    ~ExecutorListActorModelDrawBase() override;

    bool isActive() const override;
    void executeList() const override;

    void registerActorModel(LiveActor* actor);
    void createList();

    virtual ModelDrawerBase* createDrawer(const char*) const = 0;

private:
    s32 mCapacity = 0;
    s32 mSize = 0;
    ModelDrawerBase** mList = nullptr;
    agl::DrawContext* mContext = nullptr;
};

static_assert(sizeof(ExecutorListActorModelDrawBase) == 0x28);

class ExecutorListActorModelDrawForward : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawForward(const char*, s32, const ExecuteSystemInitInfo&);
    ModelDrawerBase* createDrawer(const char*) const override;
};

class ExecutorListActorModelDrawForwardOnly : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawForwardOnly(const char*, s32, const ExecuteSystemInitInfo&);
    ModelDrawerBase* createDrawer(const char*) const override;
};

class ExecutorListActorModelDrawForwardForce : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawForwardForce(const char*, s32, const ExecuteSystemInitInfo&);
    ModelDrawerBase* createDrawer(const char*) const override;
};

class ExecutorListActorModelDrawForwardPostEffectMask : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawForwardPostEffectMask(const char*, s32, const ExecuteSystemInitInfo&);
    ModelDrawerBase* createDrawer(const char*) const override;
};

class ExecutorListActorModelDrawIndirect : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawIndirect(const char*, s32, const ExecuteSystemInitInfo&);
    ModelDrawerBase* createDrawer(const char*) const override;
};

class ExecutorListActorModelDrawIndirectOnly : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawIndirectOnly(const char*, s32, const ExecuteSystemInitInfo&);
    ModelDrawerBase* createDrawer(const char*) const override;
};

class ExecutorListActorModelDrawDeferred : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawDeferred(const char*, s32, const ExecuteSystemInitInfo&);
    ModelDrawerBase* createDrawer(const char*) const;
};

class ExecutorListActorModelDrawDeferredOnly : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawDeferredOnly(const char*, s32, const ExecuteSystemInitInfo&);
    ModelDrawerBase* createDrawer(const char*) const;
};

class ExecutorListActorModelDrawDeferredOpa : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawDeferredOpa(const char*, s32, const ExecuteSystemInitInfo&);
    ModelDrawerBase* createDrawer(const char*) const;
};

class ExecutorListActorModelDrawDeferredXlu : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawDeferredXlu(const char*, s32, const ExecuteSystemInitInfo&);
    ModelDrawerBase* createDrawer(const char*) const;
};

class ExecutorListActorModelDrawDeferredSilhouette : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawDeferredSilhouette(const char*, s32, const ExecuteSystemInitInfo&);
    ModelDrawerBase* createDrawer(const char*) const;
};

class ExecutorListActorModelDrawDeferredSilhouetteRide : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawDeferredSilhouetteRide(const char*, s32,
                                                     const ExecuteSystemInitInfo&);
    ModelDrawerBase* createDrawer(const char*) const;
};

class ExecutorListActorModelDrawDeferredMarchingCube : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawDeferredMarchingCube(const char*, s32, const ExecuteSystemInitInfo&);
    ModelDrawerBase* createDrawer(const char*) const;
};

class ExecutorListActorModelDrawDepthShadow : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawDepthShadow(const char*, s32, const ExecuteSystemInitInfo&);
    ModelDrawerBase* createDrawer(const char*) const;
};

class ExecutorListActorModelDrawDepthShadowMarchingCube : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawDepthShadowMarchingCube(const char*, s32,
                                                      const ExecuteSystemInitInfo&);
    ModelDrawerBase* createDrawer(const char*) const;
};

class ExecutorListActorModelDrawStaticDepthShadow : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawStaticDepthShadow(const char*, s32, const ExecuteSystemInitInfo&);
    ModelDrawerBase* createDrawer(const char*) const;
};

class ExecutorListActorModelDrawDepthOnly : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawDepthOnly(const char*, s32, const ExecuteSystemInitInfo&);
    ModelDrawerBase* createDrawer(const char*) const;
};

class ExecutorListActorModelDrawDepthForce : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawDepthForce(const char*, s32, const ExecuteSystemInitInfo&);
    ModelDrawerBase* createDrawer(const char*) const;
};

class ExecutorListActorModelDrawCulling : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawCulling(const char*, s32, const ExecuteSystemInitInfo&);
    ModelDrawerBase* createDrawer(const char*) const;
};

class ExecutorListActorModelDrawDepthXlu : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawDepthXlu(const char*, s32, const ExecuteSystemInitInfo&);
    ModelDrawerBase* createDrawer(const char*) const;
};

class ExecutorListActorModelDrawDepthIndirect : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawDepthIndirect(const char*, s32, const ExecuteSystemInitInfo&);
    ModelDrawerBase* createDrawer(const char*) const;
};

class ExecutorListActorModelDrawDepthDither : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawDepthDither(const char*, s32, const ExecuteSystemInitInfo&);
    ModelDrawerBase* createDrawer(const char*) const;
};

class ExecutorListActorModelDrawDepthDitherIndirect : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawDepthDitherIndirect(const char*, s32, const ExecuteSystemInitInfo&);
    ModelDrawerBase* createDrawer(const char*) const;
};

class ExecutorListActorModelDrawDepthChromakey : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawDepthChromakey(const char*, s32, const ExecuteSystemInitInfo&);
    ModelDrawerBase* createDrawer(const char*) const;
};

class ExecutorListActorModelDrawDitherChromakey : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawDitherChromakey(const char*, s32, const ExecuteSystemInitInfo&);
    ModelDrawerBase* createDrawer(const char*) const;
};

class ExecutorListActorModelDrawDeferredSky : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawDeferredSky(const char*, s32, const ExecuteSystemInitInfo&);
    ModelDrawerBase* createDrawer(const char*) const;
};

class ExecutorListActorModelDrawDeferredFootPrint : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawDeferredFootPrint(const char*, s32, const ExecuteSystemInitInfo&);
    ModelDrawerBase* createDrawer(const char*) const;
};

class ExecutorListActorModelDrawWorldAo : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawWorldAo(const char*, s32, const ExecuteSystemInitInfo&);
    ModelDrawerBase* createDrawer(const char*) const;
};

class ExecutorListActorModelDrawUpdate : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawUpdate(const char*, s32, const ExecuteSystemInitInfo&);
    ModelDrawerBase* createDrawer(const char*) const;
};

class ExecutorListActorModelDrawPlayerChromakey : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawPlayerChromakey(const char*, s32, const ExecuteSystemInitInfo&);
    ModelDrawerBase* createDrawer(const char*) const;
};

class ExecutorListActorModelDrawPlayerChromakeyOpa : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawPlayerChromakeyOpa(const char*, s32, const ExecuteSystemInitInfo&);
    ModelDrawerBase* createDrawer(const char*) const;
};

class ExecutorListActorModelDrawPlayerChromakeyXlu : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawPlayerChromakeyXlu(const char*, s32, const ExecuteSystemInitInfo&);
    ModelDrawerBase* createDrawer(const char*) const;
};

class ExecutorListActorModelDrawCharacterChromakey : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawCharacterChromakey(const char*, s32, const ExecuteSystemInitInfo&);
    ModelDrawerBase* createDrawer(const char*) const;
};

}  // namespace al
