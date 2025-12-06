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

    virtual ModelDrawerBase* createDrawer(const char* name) const = 0;

private:
    s32 mCapacity = 0;
    s32 mSize = 0;
    ModelDrawerBase** mList = nullptr;
    agl::DrawContext* mContext = nullptr;
};

static_assert(sizeof(ExecutorListActorModelDrawBase) == 0x28);

class ExecutorListActorModelDrawForward : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawForward(const char* name, s32 size,
                                      const ExecuteSystemInitInfo& initInfo);
    ModelDrawerBase* createDrawer(const char* name) const override;
};

class ExecutorListActorModelDrawForwardOnly : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawForwardOnly(const char* name, s32 size,
                                          const ExecuteSystemInitInfo& initInfo);
    ModelDrawerBase* createDrawer(const char* name) const override;
};

class ExecutorListActorModelDrawForwardForce : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawForwardForce(const char* name, s32 size,
                                           const ExecuteSystemInitInfo& initInfo);
    ModelDrawerBase* createDrawer(const char* name) const override;
};

class ExecutorListActorModelDrawForwardPostEffectMask : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawForwardPostEffectMask(const char* name, s32 size,
                                                    const ExecuteSystemInitInfo& initInfo);
    ModelDrawerBase* createDrawer(const char* name) const override;
};

class ExecutorListActorModelDrawIndirect : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawIndirect(const char* name, s32 size,
                                       const ExecuteSystemInitInfo& initInfo);
    ModelDrawerBase* createDrawer(const char* name) const override;
};

class ExecutorListActorModelDrawIndirectOnly : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawIndirectOnly(const char* name, s32 size,
                                           const ExecuteSystemInitInfo& initInfo);
    ModelDrawerBase* createDrawer(const char* name) const override;
};

class ExecutorListActorModelDrawDeferred : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawDeferred(const char* name, s32 size,
                                       const ExecuteSystemInitInfo& initInfo);
    ModelDrawerBase* createDrawer(const char* name) const;
};

class ExecutorListActorModelDrawDeferredOnly : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawDeferredOnly(const char* name, s32 size,
                                           const ExecuteSystemInitInfo& initInfo);
    ModelDrawerBase* createDrawer(const char* name) const;
};

class ExecutorListActorModelDrawDeferredOpa : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawDeferredOpa(const char* name, s32 size,
                                          const ExecuteSystemInitInfo& initInfo);
    ModelDrawerBase* createDrawer(const char* name) const;
};

class ExecutorListActorModelDrawDeferredXlu : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawDeferredXlu(const char* name, s32 size,
                                          const ExecuteSystemInitInfo& initInfo);
    ModelDrawerBase* createDrawer(const char* name) const;
};

class ExecutorListActorModelDrawDeferredSilhouette : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawDeferredSilhouette(const char* name, s32 size,
                                                 const ExecuteSystemInitInfo& initInfo);
    ModelDrawerBase* createDrawer(const char* name) const;
};

class ExecutorListActorModelDrawDeferredSilhouetteRide : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawDeferredSilhouetteRide(const char* name, s32 size,
                                                     const ExecuteSystemInitInfo& initInfo);
    ModelDrawerBase* createDrawer(const char* name) const;
};

class ExecutorListActorModelDrawDeferredMarchingCube : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawDeferredMarchingCube(const char* name, s32 size,
                                                   const ExecuteSystemInitInfo& initInfo);
    ModelDrawerBase* createDrawer(const char* name) const;
};

class ExecutorListActorModelDrawDepthShadow : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawDepthShadow(const char* name, s32 size,
                                          const ExecuteSystemInitInfo& initInfo);
    ModelDrawerBase* createDrawer(const char* name) const;
};

class ExecutorListActorModelDrawDepthShadowMarchingCube : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawDepthShadowMarchingCube(const char* name, s32 size,
                                                      const ExecuteSystemInitInfo& initInfo);
    ModelDrawerBase* createDrawer(const char* name) const;
};

class ExecutorListActorModelDrawStaticDepthShadow : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawStaticDepthShadow(const char* name, s32 size,
                                                const ExecuteSystemInitInfo& initInfo);
    ModelDrawerBase* createDrawer(const char* name) const;
};

class ExecutorListActorModelDrawDepthOnly : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawDepthOnly(const char* name, s32 size,
                                        const ExecuteSystemInitInfo& initInfo);
    ModelDrawerBase* createDrawer(const char* name) const;
};

class ExecutorListActorModelDrawDepthForce : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawDepthForce(const char* name, s32 size,
                                         const ExecuteSystemInitInfo& initInfo);
    ModelDrawerBase* createDrawer(const char* name) const;
};

class ExecutorListActorModelDrawCulling : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawCulling(const char* name, s32 size,
                                      const ExecuteSystemInitInfo& initInfo);
    ModelDrawerBase* createDrawer(const char* name) const;
};

class ExecutorListActorModelDrawDepthXlu : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawDepthXlu(const char* name, s32 size,
                                       const ExecuteSystemInitInfo& initInfo);
    ModelDrawerBase* createDrawer(const char* name) const;
};

class ExecutorListActorModelDrawDepthIndirect : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawDepthIndirect(const char* name, s32 size,
                                            const ExecuteSystemInitInfo& initInfo);
    ModelDrawerBase* createDrawer(const char* name) const;
};

class ExecutorListActorModelDrawDepthDither : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawDepthDither(const char* name, s32 size,
                                          const ExecuteSystemInitInfo& initInfo);
    ModelDrawerBase* createDrawer(const char* name) const;
};

class ExecutorListActorModelDrawDepthDitherIndirect : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawDepthDitherIndirect(const char* name, s32 size,
                                                  const ExecuteSystemInitInfo& initInfo);
    ModelDrawerBase* createDrawer(const char* name) const;
};

class ExecutorListActorModelDrawDepthChromakey : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawDepthChromakey(const char* name, s32 size,
                                             const ExecuteSystemInitInfo& initInfo);
    ModelDrawerBase* createDrawer(const char* name) const;
};

class ExecutorListActorModelDrawDitherChromakey : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawDitherChromakey(const char* name, s32 size,
                                              const ExecuteSystemInitInfo& initInfo);
    ModelDrawerBase* createDrawer(const char* name) const;
};

class ExecutorListActorModelDrawDeferredSky : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawDeferredSky(const char* name, s32 size,
                                          const ExecuteSystemInitInfo& initInfo);
    ModelDrawerBase* createDrawer(const char* name) const;
};

class ExecutorListActorModelDrawDeferredFootPrint : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawDeferredFootPrint(const char* name, s32 size,
                                                const ExecuteSystemInitInfo& initInfo);
    ModelDrawerBase* createDrawer(const char* name) const;
};

class ExecutorListActorModelDrawWorldAo : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawWorldAo(const char* name, s32 size,
                                      const ExecuteSystemInitInfo& initInfo);
    ModelDrawerBase* createDrawer(const char* name) const;
};

class ExecutorListActorModelDrawUpdate : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawUpdate(const char* name, s32 size,
                                     const ExecuteSystemInitInfo& initInfo);
    ModelDrawerBase* createDrawer(const char* name) const;
};

class ExecutorListActorModelDrawPlayerChromakey : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawPlayerChromakey(const char* name, s32 size,
                                              const ExecuteSystemInitInfo& initInfo);
    ModelDrawerBase* createDrawer(const char* name) const;
};

class ExecutorListActorModelDrawPlayerChromakeyOpa : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawPlayerChromakeyOpa(const char* name, s32 size,
                                                 const ExecuteSystemInitInfo& initInfo);
    ModelDrawerBase* createDrawer(const char* name) const;
};

class ExecutorListActorModelDrawPlayerChromakeyXlu : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawPlayerChromakeyXlu(const char* name, s32 size,
                                                 const ExecuteSystemInitInfo& initInfo);
    ModelDrawerBase* createDrawer(const char* name) const;
};

class ExecutorListActorModelDrawCharacterChromakey : public ExecutorListActorModelDrawBase {
public:
    ExecutorListActorModelDrawCharacterChromakey(const char* name, s32 size,
                                                 const ExecuteSystemInitInfo& initInfo);
    ModelDrawerBase* createDrawer(const char* name) const;
};

}  // namespace al
