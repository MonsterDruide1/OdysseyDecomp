#include "Library/Execute/ExecuteTableHolderDraw.h"

#include "Library/Base/StringUtil.h"
#include "Library/Execute/ExecuteOrder.h"
#include "Library/Execute/ExecutorListActorDraw.h"
#include "Library/Execute/ExecutorListActorModelDraw.h"
#include "Library/Execute/ExecutorListBase.h"
#include "Library/Execute/ExecutorListFunctor.h"
#include "Library/Execute/ExecutorListLayoutDraw.h"
#include "Library/Execute/ExecutorListUser.h"
#include "Project/Execute/ExecuteFunction.h"

namespace al {
ExecuteTableHolderDraw::ExecuteTableHolderDraw() = default;

ExecuteTableHolderDraw::~ExecuteTableHolderDraw() {
    for (s32 i = 0; i < mExecutorsActorModelSize; i++)
        if (mExecutorsActorModel[i])
            delete (mExecutorsActorModel[i]);
};

void ExecuteTableHolderDraw::init(const char* name, const ExecuteSystemInitInfo& initInfo,
                                  const ExecuteOrder* orders, s32 orderCount) {
    mName = name;
    mExecutorsAllCapacity = orderCount;
    mExecutorsAll = new ExecutorListBase*[orderCount];

    mExecutorsActorCapacity = alExecutorFunction::calcDrawListActorNumMax(orders, orderCount);
    mExecutorsActor = new ExecutorListActorDraw*[mExecutorsActorCapacity];

    mExecutorsActorModelCapacity =
        alExecutorFunction::calcDrawListActorModelNumMax(orders, orderCount);
    mExecutorsActorModel = new ExecutorListActorModelDrawBase*[mExecutorsActorModelCapacity];

    mExecutorsLayoutCapacity = alExecutorFunction::calcDrawListLayoutNumMax(orders, orderCount);
    mExecutorsLayout = new ExecutorListLayoutDrawBase*[mExecutorsLayoutCapacity];

    mExecutorsUserCapacity = alExecutorFunction::calcExecutorListNumMax(orders, orderCount, "Draw");
    mExecutorsUser = new ExecutorListIUseExecutorDraw*[mExecutorsUserCapacity];

    mExecutorsFunctorCapacity =
        alExecutorFunction::calcExecutorListNumMax(orders, orderCount, "Functor");
    mExecutorsFunctor = new ExecutorListFunctor*[mExecutorsFunctorCapacity];

    for (s64 i = 0; i < mExecutorsAllCapacity; i++, orders++) {
        ExecutorListBase* listObjBase = nullptr;
        if (alExecutorFunction::isListName(*orders, "ActorModelDraw")) {
            listObjBase = registerExecutorListActorModel(new ExecutorListActorModelDrawForward(
                orders->listName, orders->listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(*orders, "ActorModelDrawForwardOnly")) {
            listObjBase = registerExecutorListActorModel(new ExecutorListActorModelDrawForwardOnly(
                orders->listName, orders->listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(*orders, "ActorModelDrawForwardForce")) {
            listObjBase = registerExecutorListActorModel(new ExecutorListActorModelDrawForwardForce(
                orders->listName, orders->listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(*orders, "ActorModelDrawPostEffectMask")) {
            listObjBase =
                registerExecutorListActorModel(new ExecutorListActorModelDrawForwardPostEffectMask(
                    orders->listName, orders->listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(*orders, "ActorModelDrawIndirect")) {
            listObjBase = registerExecutorListActorModel(new ExecutorListActorModelDrawIndirect(
                orders->listName, orders->listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(*orders, "ActorModelDrawIndirectOnly")) {
            listObjBase = registerExecutorListActorModel(new ExecutorListActorModelDrawIndirectOnly(
                orders->listName, orders->listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(*orders, "ActorModelDrawDeferred")) {
            listObjBase = registerExecutorListActorModel(new ExecutorListActorModelDrawDeferred(
                orders->listName, orders->listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(*orders, "ActorModelDrawDeferredMarchingCube")) {
            listObjBase =
                registerExecutorListActorModel(new ExecutorListActorModelDrawDeferredMarchingCube(
                    orders->listName, orders->listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(*orders, "ActorModelDrawDeferredOnly")) {
            listObjBase = registerExecutorListActorModel(new ExecutorListActorModelDrawDeferredOnly(
                orders->listName, orders->listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(*orders, "ActorModelDrawDeferredOpa")) {
            listObjBase = registerExecutorListActorModel(new ExecutorListActorModelDrawDeferredOpa(
                orders->listName, orders->listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(*orders, "ActorModelDrawDeferredXlu")) {
            listObjBase = registerExecutorListActorModel(new ExecutorListActorModelDrawDeferredXlu(
                orders->listName, orders->listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(*orders, "ActorModelDrawDepthOnly")) {
            listObjBase = registerExecutorListActorModel(new ExecutorListActorModelDrawDepthOnly(
                orders->listName, orders->listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(*orders, "ActorModelDrawCulling")) {
            listObjBase = registerExecutorListActorModel(new ExecutorListActorModelDrawCulling(
                orders->listName, orders->listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(*orders, "ActorModelDrawDepthForce")) {
            listObjBase = registerExecutorListActorModel(new ExecutorListActorModelDrawDepthForce(
                orders->listName, orders->listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(*orders, "ActorModelDrawDepthXlu")) {
            listObjBase = registerExecutorListActorModel(new ExecutorListActorModelDrawDepthXlu(
                orders->listName, orders->listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(*orders, "ActorModelDrawDepthIndirect")) {
            listObjBase =
                registerExecutorListActorModel(new ExecutorListActorModelDrawDepthIndirect(
                    orders->listName, orders->listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(*orders, "ActorModelDrawDepthDither")) {
            listObjBase = registerExecutorListActorModel(new ExecutorListActorModelDrawDepthDither(
                orders->listName, orders->listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(*orders, "ActorModelDrawDepthDitherIndirect")) {
            listObjBase =
                registerExecutorListActorModel(new ExecutorListActorModelDrawDepthDitherIndirect(
                    orders->listName, orders->listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(*orders, "ActorModelDrawDepthShadow")) {
            listObjBase = registerExecutorListActorModel(new ExecutorListActorModelDrawDepthShadow(
                orders->listName, orders->listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(*orders,
                                                  "ActorModelDrawDepthShadowMarchingCube")) {
            listObjBase = registerExecutorListActorModel(
                new ExecutorListActorModelDrawDepthShadowMarchingCube(
                    orders->listName, orders->listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(*orders, "ActorModelDrawStaticDepthShadow")) {
            listObjBase =
                registerExecutorListActorModel(new ExecutorListActorModelDrawStaticDepthShadow(
                    orders->listName, orders->listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(*orders, "ActorModelDrawDeferredSilhouette")) {
            listObjBase =
                registerExecutorListActorModel(new ExecutorListActorModelDrawDeferredSilhouette(
                    orders->listName, orders->listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(*orders,
                                                  "ActorModelDrawDeferredSilhouetteRide")) {
            listObjBase =
                registerExecutorListActorModel(new ExecutorListActorModelDrawDeferredSilhouetteRide(
                    orders->listName, orders->listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(*orders, "ActorModelDrawDeferredFootPrint")) {
            listObjBase =
                registerExecutorListActorModel(new ExecutorListActorModelDrawDeferredFootPrint(
                    orders->listName, orders->listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(*orders, "ActorModelDrawDeferredSky")) {
            listObjBase = registerExecutorListActorModel(new ExecutorListActorModelDrawDeferredSky(
                orders->listName, orders->listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(*orders, "ActorModelDrawWorldAo")) {
            listObjBase = registerExecutorListActorModel(new ExecutorListActorModelDrawWorldAo(
                orders->listName, orders->listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(*orders, "ActorModelDrawUpdate")) {
            listObjBase = registerExecutorListActorModel(new ExecutorListActorModelDrawUpdate(
                orders->listName, orders->listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(*orders, "ActorModelDrawPlayerChromakey")) {
            listObjBase =
                registerExecutorListActorModel(new ExecutorListActorModelDrawPlayerChromakey(
                    orders->listName, orders->listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(*orders, "ActorModelDrawPlayerChromakeyOpa")) {
            listObjBase =
                registerExecutorListActorModel(new ExecutorListActorModelDrawPlayerChromakeyOpa(
                    orders->listName, orders->listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(*orders, "ActorModelDrawPlayerChromakeyXlu")) {
            listObjBase =
                registerExecutorListActorModel(new ExecutorListActorModelDrawPlayerChromakeyXlu(
                    orders->listName, orders->listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(*orders, "ActorModelDrawCharacterChromakey")) {
            listObjBase =
                registerExecutorListActorModel(new ExecutorListActorModelDrawCharacterChromakey(
                    orders->listName, orders->listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(*orders, "ActorModelDrawDepthChromakey")) {
            listObjBase =
                registerExecutorListActorModel(new ExecutorListActorModelDrawDepthChromakey(
                    orders->listName, orders->listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(*orders, "ActorModelDrawDitherChromakey")) {
            listObjBase =
                registerExecutorListActorModel(new ExecutorListActorModelDrawDitherChromakey(
                    orders->listName, orders->listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(*orders, "ActorDraw")) {
            listObjBase = registerExecutorListActor(
                new ExecutorListActorDraw(orders->listName, orders->listMaxSize));

        } else if (alExecutorFunction::isListName(*orders, "LayoutDraw")) {
            listObjBase = registerExecutorListLayout(
                new ExecutorListLayoutDrawNormal(orders->listName, orders->listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(*orders, "Draw")) {
            listObjBase = registerExecutorListUser(
                new ExecutorListIUseExecutorDraw(orders->listName, orders->listMaxSize));

        } else if (alExecutorFunction::isListName(*orders, "Functor")) {
            listObjBase = registerExecutorListFunctor(
                new ExecutorListFunctor(orders->listName, orders->listMaxSize));
        }

        registerExecutorListAll(listObjBase);
    }
}

ExecutorListActorModelDrawBase* ExecuteTableHolderDraw::registerExecutorListActorModel(
    ExecutorListActorModelDrawBase* listActorModel) {
    mExecutorsActorModel[mExecutorsActorModelSize] = listActorModel;
    mExecutorsActorModelSize++;
    return listActorModel;
}

ExecutorListActorDraw*
ExecuteTableHolderDraw::registerExecutorListActor(ExecutorListActorDraw* listActor) {
    mExecutorsActor[mExecutorsActorSize] = listActor;
    mExecutorsActorSize++;
    return listActor;
}

ExecutorListLayoutDrawBase*
ExecuteTableHolderDraw::registerExecutorListLayout(ExecutorListLayoutDrawBase* listLayout) {
    mExecutorsLayout[mExecutorsLayoutSize] = listLayout;
    mExecutorsLayoutSize++;
    return listLayout;
}

ExecutorListIUseExecutorDraw*
ExecuteTableHolderDraw::registerExecutorListUser(ExecutorListIUseExecutorDraw* listUser) {
    mExecutorsUser[mExecutorsUserSize] = listUser;
    mExecutorsUserSize++;
    return listUser;
}

ExecutorListFunctor*
ExecuteTableHolderDraw::registerExecutorListFunctor(ExecutorListFunctor* listFunctor) {
    mExecutorsFunctor[mExecutorsFunctorSize] = listFunctor;
    mExecutorsFunctorSize++;
    return listFunctor;
}

void ExecuteTableHolderDraw::registerExecutorListAll(ExecutorListBase* list) {
    mExecutorsAll[mExecutorsAllSize] = list;
    mExecutorsAllSize++;
}

bool ExecuteTableHolderDraw::tryRegisterActor(LiveActor* actor, const char* listName) {
    bool isRegistered = false;
    for (s32 i = 0; i < mExecutorsActorSize; i++) {
        ExecutorListActorDraw* listActor = mExecutorsActor[i];
        if (isEqualString(listActor->getName(), listName)) {
            listActor->registerActor(actor);
            isRegistered = true;
        }
    }
    return isRegistered;
}

bool ExecuteTableHolderDraw::tryRegisterActorModel(LiveActor* actor, const char* listName) {
    bool isRegistered = false;
    for (s32 i = 0; i < mExecutorsActorModelSize; i++) {
        ExecutorListActorModelDrawBase* listActor = mExecutorsActorModel[i];
        if (isEqualString(listActor->getName(), listName)) {
            listActor->registerActorModel(actor);
            isRegistered = true;
        }
    }
    return isRegistered;
}

bool ExecuteTableHolderDraw::tryRegisterLayout(LayoutActor* layout, const char* listName) {
    bool isRegistered = false;
    for (s32 i = 0; i < mExecutorsLayoutSize; i++) {
        ExecutorListLayoutDrawBase* listLayout = mExecutorsLayout[i];
        if (isEqualString(listLayout->getName(), listName)) {
            listLayout->registerLayout(layout);
            isRegistered = true;
        }
    }
    return isRegistered;
}

bool ExecuteTableHolderDraw::tryRegisterUser(IUseExecutor* user, const char* listName) {
    bool isRegistered = false;
    for (s32 i = 0; i < mExecutorsUserSize; i++) {
        ExecutorListIUseExecutorBase* listLayout = mExecutorsUser[i];
        if (isEqualString(listLayout->getName(), listName)) {
            listLayout->registerUser(user);
            isRegistered = true;
        }
    }
    return isRegistered;
}

bool ExecuteTableHolderDraw::tryRegisterFunctor(const FunctorBase& functor, const char* listName) {
    bool isRegistered = false;
    for (s32 i = 0; i < mExecutorsFunctorSize; i++) {
        ExecutorListFunctor* listLayout = mExecutorsFunctor[i];
        if (isEqualString(listLayout->getName(), listName)) {
            listLayout->registerFunctor(functor);
            isRegistered = true;
        }
    }
    return isRegistered;
}

void ExecuteTableHolderDraw::createExecutorListTable() {
    for (s32 i = 0; i < mExecutorsActorSize; i++)
        mExecutorsActor[i]->createList();

    for (s32 i = 0; i < mExecutorsActorModelSize; i++)
        mExecutorsActorModel[i]->createList();

    mActiveExecutorsSize = 0;
    for (s32 i = 0; i < mExecutorsAllSize; i++)
        if (mExecutorsAll[i]->isActive())
            mActiveExecutorsSize++;

    s32 activeIndex = 0;
    mActiveExecutors = new ExecutorListBase*[mActiveExecutorsSize];
    for (s32 i = 0; i < mExecutorsAllSize; i++) {
        ExecutorListBase* list = mExecutorsAll[i];
        if (list->isActive())
            mActiveExecutors[activeIndex++] = list;
    }
}

void ExecuteTableHolderDraw::execute() const {
    for (s32 i = 0; i < mActiveExecutorsSize; i++)
        mActiveExecutors[i]->executeList();
}

void ExecuteTableHolderDraw::executeList(const char* listName) const {
    for (s32 i = 0; i < mActiveExecutorsSize; i++)
        if (isEqualString(mActiveExecutors[i]->getName(), listName))
            mActiveExecutors[i]->executeList();
}

bool ExecuteTableHolderDraw::isActive() const {
    for (s32 i = 0; i < mActiveExecutorsSize; i++)
        if (!mActiveExecutors[i]->isActive())
            return false;

    return mActiveExecutorsSize > 0;
}
}  // namespace al
