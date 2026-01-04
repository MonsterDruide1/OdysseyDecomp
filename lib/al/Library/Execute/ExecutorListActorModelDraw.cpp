#include "Library/Execute/ExecutorListActorModelDraw.h"

#include "Library/Base/StringUtil.h"
#include "Library/Execute/ActorExecuteInfo.h"
#include "Library/LiveActor/ActorSceneInfo.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Model/ModelDrawerBase.h"
#include "Library/Model/ModelDrawerBufferUpdate.h"
#include "Library/Model/ModelDrawerChromakey.h"
#include "Library/Model/ModelDrawerDeferred.h"
#include "Library/Model/ModelDrawerDeferredFootPrint.h"
#include "Library/Model/ModelDrawerDeferredSilhouette.h"
#include "Library/Model/ModelDrawerDeferredSky.h"
#include "Library/Model/ModelDrawerDepthOnly.h"
#include "Library/Model/ModelDrawerDepthShadow.h"
#include "Library/Model/ModelDrawerForward.h"
#include "Library/Model/ModelDrawerPrepassCulling.h"
#include "Library/Model/ModelDrawerStaticDepthShadow.h"
#include "Library/Model/ModelDrawerWorldAo.h"
#include "Library/Model/ModelKeeper.h"
#include "Project/Execute/ExecuteSystemInitInfo.h"

namespace al {

ExecutorListActorModelDrawBase::ExecutorListActorModelDrawBase(
    const char* name, s32 size, const ExecuteSystemInitInfo& initInfo)
    : ExecutorListBase(name), mCapacity(size) {
    mList = new ModelDrawerBase*[size];
    for (s32 i = 0; i < mCapacity; i++)
        mList[i] = nullptr;
    mContext = initInfo.drawCtx;
}

ExecutorListActorModelDrawBase::~ExecutorListActorModelDrawBase() {
    for (s32 i = 0; i < mCapacity; i++)
        delete mList[i];
}

bool ExecutorListActorModelDrawBase::isActive() const {
    return mSize > 0;
}

void ExecutorListActorModelDrawBase::executeList() const {
    if (!ExecutorListActorModelDrawBase::isActive())
        return;

    // Note: What is the purpose of these?
    isEqualString(getName(), "シルエット[プレイヤー]");
    isEqualString(getName(), "シルエット[乗り物]");
    for (s32 i = 0; i < mSize; i++)
        mList[i]->draw();
}

void ExecutorListActorModelDrawBase::registerActorModel(LiveActor* actor) {
    const char* name = actor->getModelKeeper()->getName();
    ModelCtrl* modelCtrl = actor->getModelKeeper()->getModelCtrl();
    for (s32 i = 0; i < mSize; i++) {
        ModelDrawerBase* modelDrawer = mList[i];
        if (isEqualString(modelDrawer->getName(), name)) {
            modelDrawer->registerModel(modelCtrl);
            actor->getExecuteInfo()->addDrawer(modelDrawer);
            return;
        }
    }

    ModelDrawerBase* modelDrawer = createDrawer(name);
    modelDrawer->setDrawInfo(mContext, actor->getSceneInfo()->graphicsSystemInfo,
                             actor->getSceneInfo()->modelDrawBufferCounter,
                             actor->getModelKeeper());
    modelDrawer->registerModel(modelCtrl);
    actor->getExecuteInfo()->addDrawer(modelDrawer);
    mList[mSize] = modelDrawer;
    mSize++;
}

void ExecutorListActorModelDrawBase::createList() {
    for (s32 i = 0; i < mSize; i++)
        mList[i]->createTable();
}

ExecutorListActorModelDrawForward::ExecutorListActorModelDrawForward(
    const char* name, s32 size, const ExecuteSystemInitInfo& initInfo)
    : ExecutorListActorModelDrawBase(name, size, initInfo) {}

ModelDrawerBase* ExecutorListActorModelDrawForward::createDrawer(const char* name) const {
    return new ModelDrawerForward(name, false, false, false, false, false, false);
}

ExecutorListActorModelDrawForwardOnly::ExecutorListActorModelDrawForwardOnly(
    const char* name, s32 size, const ExecuteSystemInitInfo& initInfo)
    : ExecutorListActorModelDrawBase(name, size, initInfo) {}

ModelDrawerBase* ExecutorListActorModelDrawForwardOnly::createDrawer(const char* name) const {
    return new ModelDrawerForward(name, true, false, false, false, false, false);
}

ExecutorListActorModelDrawForwardForce::ExecutorListActorModelDrawForwardForce(
    const char* name, s32 size, const ExecuteSystemInitInfo& initInfo)
    : ExecutorListActorModelDrawBase(name, size, initInfo) {}

ModelDrawerBase* ExecutorListActorModelDrawForwardForce::createDrawer(const char* name) const {
    return new ModelDrawerForward(name, false, false, true, false, false, false);
}

ExecutorListActorModelDrawForwardPostEffectMask::ExecutorListActorModelDrawForwardPostEffectMask(
    const char* name, s32 size, const ExecuteSystemInitInfo& initInfo)
    : ExecutorListActorModelDrawBase(name, size, initInfo) {}

ModelDrawerBase*
ExecutorListActorModelDrawForwardPostEffectMask::createDrawer(const char* name) const {
    return new ModelDrawerForward(name, false, true, false, false, false, false);
}

ExecutorListActorModelDrawIndirect::ExecutorListActorModelDrawIndirect(
    const char* name, s32 size, const ExecuteSystemInitInfo& initInfo)
    : ExecutorListActorModelDrawBase(name, size, initInfo) {}

ModelDrawerBase* ExecutorListActorModelDrawIndirect::createDrawer(const char* name) const {
    return new ModelDrawerForward(name, false, false, false, true, false, false);
}

ExecutorListActorModelDrawIndirectOnly::ExecutorListActorModelDrawIndirectOnly(
    const char* name, s32 size, const ExecuteSystemInitInfo& initInfo)
    : ExecutorListActorModelDrawBase(name, size, initInfo) {}

ModelDrawerBase* ExecutorListActorModelDrawIndirectOnly::createDrawer(const char* name) const {
    return new ModelDrawerForward(name, true, false, false, true, false, false);
}

ExecutorListActorModelDrawDeferred::ExecutorListActorModelDrawDeferred(
    const char* name, s32 size, const ExecuteSystemInitInfo& initInfo)
    : ExecutorListActorModelDrawBase(name, size, initInfo) {}

ModelDrawerBase* ExecutorListActorModelDrawDeferred::createDrawer(const char* name) const {
    return new ModelDrawerDeferred(name, false, true, true, false);
}

ExecutorListActorModelDrawDeferredOnly::ExecutorListActorModelDrawDeferredOnly(
    const char* name, s32 size, const ExecuteSystemInitInfo& initInfo)
    : ExecutorListActorModelDrawBase(name, size, initInfo) {}

ModelDrawerBase* ExecutorListActorModelDrawDeferredOnly::createDrawer(const char* name) const {
    return new ModelDrawerDeferred(name, true, true, true, false);
}

ExecutorListActorModelDrawDeferredOpa::ExecutorListActorModelDrawDeferredOpa(
    const char* name, s32 size, const ExecuteSystemInitInfo& initInfo)
    : ExecutorListActorModelDrawBase(name, size, initInfo) {}

ModelDrawerBase* ExecutorListActorModelDrawDeferredOpa::createDrawer(const char* name) const {
    return new ModelDrawerDeferred(name, true, true, false, false);
}

ExecutorListActorModelDrawDeferredXlu::ExecutorListActorModelDrawDeferredXlu(
    const char* name, s32 size, const ExecuteSystemInitInfo& initInfo)
    : ExecutorListActorModelDrawBase(name, size, initInfo) {}

ModelDrawerBase* ExecutorListActorModelDrawDeferredXlu::createDrawer(const char* name) const {
    return new ModelDrawerDeferred(name, true, false, true, false);
}

ExecutorListActorModelDrawDeferredSilhouette::ExecutorListActorModelDrawDeferredSilhouette(
    const char* name, s32 size, const ExecuteSystemInitInfo& initInfo)
    : ExecutorListActorModelDrawBase(name, size, initInfo) {}

ModelDrawerBase*
ExecutorListActorModelDrawDeferredSilhouette::createDrawer(const char* name) const {
    return new ModelDrawerDeferredSilhouette(name, SilhouetteDrawCategory::Default);
}

ExecutorListActorModelDrawDeferredSilhouetteRide::ExecutorListActorModelDrawDeferredSilhouetteRide(
    const char* name, s32 size, const ExecuteSystemInitInfo& initInfo)
    : ExecutorListActorModelDrawBase(name, size, initInfo) {}

ModelDrawerBase*
ExecutorListActorModelDrawDeferredSilhouetteRide::createDrawer(const char* name) const {
    return new ModelDrawerDeferredSilhouette(name, SilhouetteDrawCategory::Ride);
}

ExecutorListActorModelDrawDeferredMarchingCube::ExecutorListActorModelDrawDeferredMarchingCube(
    const char* name, s32 size, const ExecuteSystemInitInfo& initInfo)
    : ExecutorListActorModelDrawBase(name, size, initInfo) {}

ModelDrawerBase*
ExecutorListActorModelDrawDeferredMarchingCube::createDrawer(const char* name) const {
    return new ModelDrawerDeferred(name, false, true, true, true);
}

ExecutorListActorModelDrawDepthShadow::ExecutorListActorModelDrawDepthShadow(
    const char* name, s32 size, const ExecuteSystemInitInfo& initInfo)
    : ExecutorListActorModelDrawBase(name, size, initInfo) {}

ModelDrawerBase* ExecutorListActorModelDrawDepthShadow::createDrawer(const char* name) const {
    return new ModelDrawerDepthShadow(name, false, false);
}

ExecutorListActorModelDrawDepthShadowMarchingCube::
    ExecutorListActorModelDrawDepthShadowMarchingCube(const char* name, s32 size,
                                                      const ExecuteSystemInitInfo& initInfo)
    : ExecutorListActorModelDrawBase(name, size, initInfo) {}

ModelDrawerBase*
ExecutorListActorModelDrawDepthShadowMarchingCube::createDrawer(const char* name) const {
    return new ModelDrawerDepthShadow(name, false, true);
}

ExecutorListActorModelDrawStaticDepthShadow::ExecutorListActorModelDrawStaticDepthShadow(
    const char* name, s32 size, const ExecuteSystemInitInfo& initInfo)
    : ExecutorListActorModelDrawBase(name, size, initInfo) {}

ModelDrawerBase* ExecutorListActorModelDrawStaticDepthShadow::createDrawer(const char* name) const {
    return new ModelDrawerStaticDepthShadow(name);
}

ExecutorListActorModelDrawDepthOnly::ExecutorListActorModelDrawDepthOnly(
    const char* name, s32 size, const ExecuteSystemInitInfo& initInfo)
    : ExecutorListActorModelDrawBase(name, size, initInfo) {}

ModelDrawerBase* ExecutorListActorModelDrawDepthOnly::createDrawer(const char* name) const {
    return new ModelDrawerDepthOnly(name, false, false, false, false, false);
}

ExecutorListActorModelDrawDepthForce::ExecutorListActorModelDrawDepthForce(
    const char* name, s32 size, const ExecuteSystemInitInfo& initInfo)
    : ExecutorListActorModelDrawBase(name, size, initInfo) {}

ModelDrawerBase* ExecutorListActorModelDrawDepthForce::createDrawer(const char* name) const {
    return new ModelDrawerDepthOnly(name, true, false, false, false, false);
}

ExecutorListActorModelDrawCulling::ExecutorListActorModelDrawCulling(
    const char* name, s32 size, const ExecuteSystemInitInfo& initInfo)
    : ExecutorListActorModelDrawBase(name, size, initInfo) {}

ModelDrawerBase* ExecutorListActorModelDrawCulling::createDrawer(const char* name) const {
    return new ModelDrawerPrepassCulling(name);
}

ExecutorListActorModelDrawDepthXlu::ExecutorListActorModelDrawDepthXlu(
    const char* name, s32 size, const ExecuteSystemInitInfo& initInfo)
    : ExecutorListActorModelDrawBase(name, size, initInfo) {}

ModelDrawerBase* ExecutorListActorModelDrawDepthXlu::createDrawer(const char* name) const {
    return new ModelDrawerDepthOnly(name, false, false, true, false, false);
}

ExecutorListActorModelDrawDepthIndirect::ExecutorListActorModelDrawDepthIndirect(
    const char* name, s32 size, const ExecuteSystemInitInfo& initInfo)
    : ExecutorListActorModelDrawBase(name, size, initInfo) {}

ModelDrawerBase* ExecutorListActorModelDrawDepthIndirect::createDrawer(const char* name) const {
    return new ModelDrawerDepthOnly(name, false, false, false, true, false);
}

ExecutorListActorModelDrawDepthDither::ExecutorListActorModelDrawDepthDither(
    const char* name, s32 size, const ExecuteSystemInitInfo& initInfo)
    : ExecutorListActorModelDrawBase(name, size, initInfo) {}

ModelDrawerBase* ExecutorListActorModelDrawDepthDither::createDrawer(const char* name) const {
    return new ModelDrawerDepthOnly(name, false, true, false, false, false);
}

ExecutorListActorModelDrawDepthDitherIndirect::ExecutorListActorModelDrawDepthDitherIndirect(
    const char* name, s32 size, const ExecuteSystemInitInfo& initInfo)
    : ExecutorListActorModelDrawBase(name, size, initInfo) {}

ModelDrawerBase*
ExecutorListActorModelDrawDepthDitherIndirect::createDrawer(const char* name) const {
    return new ModelDrawerDepthOnly(name, false, true, false, true, false);
}

ExecutorListActorModelDrawDepthChromakey::ExecutorListActorModelDrawDepthChromakey(
    const char* name, s32 size, const ExecuteSystemInitInfo& initInfo)
    : ExecutorListActorModelDrawBase(name, size, initInfo) {}

ModelDrawerBase* ExecutorListActorModelDrawDepthChromakey::createDrawer(const char* name) const {
    return new ModelDrawerDepthOnly(name, false, false, false, false, true);
}

ExecutorListActorModelDrawDitherChromakey::ExecutorListActorModelDrawDitherChromakey(
    const char* name, s32 size, const ExecuteSystemInitInfo& initInfo)
    : ExecutorListActorModelDrawBase(name, size, initInfo) {}

ModelDrawerBase* ExecutorListActorModelDrawDitherChromakey::createDrawer(const char* name) const {
    return new ModelDrawerDepthOnly(name, false, true, false, false, true);
}

ExecutorListActorModelDrawDeferredSky::ExecutorListActorModelDrawDeferredSky(
    const char* name, s32 size, const ExecuteSystemInitInfo& initInfo)
    : ExecutorListActorModelDrawBase(name, size, initInfo) {}

ModelDrawerBase* ExecutorListActorModelDrawDeferredSky::createDrawer(const char* name) const {
    return new ModelDrawerDeferredSky(name);
}

ExecutorListActorModelDrawDeferredFootPrint::ExecutorListActorModelDrawDeferredFootPrint(
    const char* name, s32 size, const ExecuteSystemInitInfo& initInfo)
    : ExecutorListActorModelDrawBase(name, size, initInfo) {}

ModelDrawerBase* ExecutorListActorModelDrawDeferredFootPrint::createDrawer(const char* name) const {
    return new ModelDrawerDeferredFootPrint(name);
}

ExecutorListActorModelDrawWorldAo::ExecutorListActorModelDrawWorldAo(
    const char* name, s32 size, const ExecuteSystemInitInfo& initInfo)
    : ExecutorListActorModelDrawBase(name, size, initInfo) {}

ModelDrawerBase* ExecutorListActorModelDrawWorldAo::createDrawer(const char* name) const {
    return new ModelDrawerWorldAo(name);
}

ExecutorListActorModelDrawUpdate::ExecutorListActorModelDrawUpdate(
    const char* name, s32 size, const ExecuteSystemInitInfo& initInfo)
    : ExecutorListActorModelDrawBase(name, size, initInfo) {}

ModelDrawerBase* ExecutorListActorModelDrawUpdate::createDrawer(const char* name) const {
    return new ModelDrawerBufferUpdate(name);
}

ExecutorListActorModelDrawPlayerChromakey::ExecutorListActorModelDrawPlayerChromakey(
    const char* name, s32 size, const ExecuteSystemInitInfo& initInfo)
    : ExecutorListActorModelDrawBase(name, size, initInfo) {}

ModelDrawerBase* ExecutorListActorModelDrawPlayerChromakey::createDrawer(const char* name) const {
    return new ModelDrawerChromakey(name, true, false, false, false);
}

ExecutorListActorModelDrawPlayerChromakeyOpa::ExecutorListActorModelDrawPlayerChromakeyOpa(
    const char* name, s32 size, const ExecuteSystemInitInfo& initInfo)
    : ExecutorListActorModelDrawBase(name, size, initInfo) {}

ModelDrawerBase*
ExecutorListActorModelDrawPlayerChromakeyOpa::createDrawer(const char* name) const {
    return new ModelDrawerChromakey(name, true, false, true, false);
}

ExecutorListActorModelDrawPlayerChromakeyXlu::ExecutorListActorModelDrawPlayerChromakeyXlu(
    const char* name, s32 size, const ExecuteSystemInitInfo& initInfo)
    : ExecutorListActorModelDrawBase(name, size, initInfo) {}

ModelDrawerBase*
ExecutorListActorModelDrawPlayerChromakeyXlu::createDrawer(const char* name) const {
    return new ModelDrawerChromakey(name, true, false, false, true);
}

ExecutorListActorModelDrawCharacterChromakey::ExecutorListActorModelDrawCharacterChromakey(
    const char* name, s32 size, const ExecuteSystemInitInfo& initInfo)
    : ExecutorListActorModelDrawBase(name, size, initInfo) {}

ModelDrawerBase*
ExecutorListActorModelDrawCharacterChromakey::createDrawer(const char* name) const {
    return new ModelDrawerChromakey(name, false, true, false, false);
}

}  // namespace al
