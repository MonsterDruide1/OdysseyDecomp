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
            delete mExecutorsActorModel[i];
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
    for (s64 i = 0; i < mExecutorsAllCapacity; i++) {
        const ExecuteOrder& order = orders[i];

        ExecutorListBase* list = nullptr;
        if (alExecutorFunction::isListName(order, "ActorModelDraw")) {
            list = registerExecutorListActorModel(
                new ExecutorListActorModelDrawForward(order.listName, order.listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(order, "ActorModelDrawForwardOnly")) {
            list = registerExecutorListActorModel(new ExecutorListActorModelDrawForwardOnly(
                order.listName, order.listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(order, "ActorModelDrawForwardForce")) {
            list = registerExecutorListActorModel(new ExecutorListActorModelDrawForwardForce(
                order.listName, order.listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(order, "ActorModelDrawPostEffectMask")) {
            list =
                registerExecutorListActorModel(new ExecutorListActorModelDrawForwardPostEffectMask(
                    order.listName, order.listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(order, "ActorModelDrawIndirect")) {
            list = registerExecutorListActorModel(new ExecutorListActorModelDrawIndirect(
                order.listName, order.listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(order, "ActorModelDrawIndirectOnly")) {
            list = registerExecutorListActorModel(new ExecutorListActorModelDrawIndirectOnly(
                order.listName, order.listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(order, "ActorModelDrawDeferred")) {
            list = registerExecutorListActorModel(new ExecutorListActorModelDrawDeferred(
                order.listName, order.listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(order, "ActorModelDrawDeferredMarchingCube")) {
            list =
                registerExecutorListActorModel(new ExecutorListActorModelDrawDeferredMarchingCube(
                    order.listName, order.listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(order, "ActorModelDrawDeferredOnly")) {
            list = registerExecutorListActorModel(new ExecutorListActorModelDrawDeferredOnly(
                order.listName, order.listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(order, "ActorModelDrawDeferredOpa")) {
            list = registerExecutorListActorModel(new ExecutorListActorModelDrawDeferredOpa(
                order.listName, order.listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(order, "ActorModelDrawDeferredXlu")) {
            list = registerExecutorListActorModel(new ExecutorListActorModelDrawDeferredXlu(
                order.listName, order.listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(order, "ActorModelDrawDepthOnly")) {
            list = registerExecutorListActorModel(new ExecutorListActorModelDrawDepthOnly(
                order.listName, order.listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(order, "ActorModelDrawCulling")) {
            list = registerExecutorListActorModel(
                new ExecutorListActorModelDrawCulling(order.listName, order.listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(order, "ActorModelDrawDepthForce")) {
            list = registerExecutorListActorModel(new ExecutorListActorModelDrawDepthForce(
                order.listName, order.listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(order, "ActorModelDrawDepthXlu")) {
            list = registerExecutorListActorModel(new ExecutorListActorModelDrawDepthXlu(
                order.listName, order.listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(order, "ActorModelDrawDepthIndirect")) {
            list = registerExecutorListActorModel(new ExecutorListActorModelDrawDepthIndirect(
                order.listName, order.listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(order, "ActorModelDrawDepthDither")) {
            list = registerExecutorListActorModel(new ExecutorListActorModelDrawDepthDither(
                order.listName, order.listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(order, "ActorModelDrawDepthDitherIndirect")) {
            list = registerExecutorListActorModel(new ExecutorListActorModelDrawDepthDitherIndirect(
                order.listName, order.listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(order, "ActorModelDrawDepthShadow")) {
            list = registerExecutorListActorModel(new ExecutorListActorModelDrawDepthShadow(
                order.listName, order.listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(order, "ActorModelDrawDepthShadowMarchingCube")) {
            list = registerExecutorListActorModel(
                new ExecutorListActorModelDrawDepthShadowMarchingCube(order.listName,
                                                                      order.listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(order, "ActorModelDrawStaticDepthShadow")) {
            list = registerExecutorListActorModel(new ExecutorListActorModelDrawStaticDepthShadow(
                order.listName, order.listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(order, "ActorModelDrawDeferredSilhouette")) {
            list = registerExecutorListActorModel(new ExecutorListActorModelDrawDeferredSilhouette(
                order.listName, order.listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(order, "ActorModelDrawDeferredSilhouetteRide")) {
            list =
                registerExecutorListActorModel(new ExecutorListActorModelDrawDeferredSilhouetteRide(
                    order.listName, order.listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(order, "ActorModelDrawDeferredFootPrint")) {
            list = registerExecutorListActorModel(new ExecutorListActorModelDrawDeferredFootPrint(
                order.listName, order.listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(order, "ActorModelDrawDeferredSky")) {
            list = registerExecutorListActorModel(new ExecutorListActorModelDrawDeferredSky(
                order.listName, order.listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(order, "ActorModelDrawWorldAo")) {
            list = registerExecutorListActorModel(
                new ExecutorListActorModelDrawWorldAo(order.listName, order.listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(order, "ActorModelDrawUpdate")) {
            list = registerExecutorListActorModel(
                new ExecutorListActorModelDrawUpdate(order.listName, order.listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(order, "ActorModelDrawPlayerChromakey")) {
            list = registerExecutorListActorModel(new ExecutorListActorModelDrawPlayerChromakey(
                order.listName, order.listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(order, "ActorModelDrawPlayerChromakeyOpa")) {
            list = registerExecutorListActorModel(new ExecutorListActorModelDrawPlayerChromakeyOpa(
                order.listName, order.listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(order, "ActorModelDrawPlayerChromakeyXlu")) {
            list = registerExecutorListActorModel(new ExecutorListActorModelDrawPlayerChromakeyXlu(
                order.listName, order.listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(order, "ActorModelDrawCharacterChromakey")) {
            list = registerExecutorListActorModel(new ExecutorListActorModelDrawCharacterChromakey(
                order.listName, order.listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(order, "ActorModelDrawDepthChromakey")) {
            list = registerExecutorListActorModel(new ExecutorListActorModelDrawDepthChromakey(
                order.listName, order.listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(order, "ActorModelDrawDitherChromakey")) {
            list = registerExecutorListActorModel(new ExecutorListActorModelDrawDitherChromakey(
                order.listName, order.listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(order, "ActorDraw")) {
            list = registerExecutorListActor(
                new ExecutorListActorDraw(order.listName, order.listMaxSize));

        } else if (alExecutorFunction::isListName(order, "LayoutDraw")) {
            list = registerExecutorListLayout(
                new ExecutorListLayoutDrawNormal(order.listName, order.listMaxSize, initInfo));

        } else if (alExecutorFunction::isListName(order, "Draw")) {
            list = registerExecutorListUser(
                new ExecutorListIUseExecutorDraw(order.listName, order.listMaxSize));

        } else if (alExecutorFunction::isListName(order, "Functor")) {
            list = registerExecutorListFunctor(
                new ExecutorListFunctor(order.listName, order.listMaxSize));
        }

        registerExecutorListAll(list);
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
