#pragma once

#include <basis/seadTypes.h>

namespace al {
struct ExecuteOrder;
struct ExecuteSystemInitInfo;
class ExecutorListActorModelDrawBase;
class ExecutorListActorDraw;
class ExecutorListBase;
class ExecutorListFunctor;
class ExecutorListIUseExecutorDraw;
class ExecutorListLayoutDrawBase;
class FunctorBase;
class IUseExecutor;
class LayoutActor;
class LiveActor;

class ExecuteTableHolderDraw {
public:
    ExecuteTableHolderDraw();
    virtual ~ExecuteTableHolderDraw();
    void init(const char* name, const ExecuteSystemInitInfo& initInfo, const ExecuteOrder* orders,
              s32 orderCount);
    ExecutorListActorModelDrawBase*
    registerExecutorListActorModel(ExecutorListActorModelDrawBase* listActorModel);
    ExecutorListActorDraw* registerExecutorListActor(ExecutorListActorDraw* listActor);
    ExecutorListLayoutDrawBase* registerExecutorListLayout(ExecutorListLayoutDrawBase* listLayout);
    ExecutorListIUseExecutorDraw* registerExecutorListUser(ExecutorListIUseExecutorDraw* listUser);
    ExecutorListFunctor* registerExecutorListFunctor(ExecutorListFunctor* listFunctor);
    void registerExecutorListAll(ExecutorListBase* list);
    bool tryRegisterActor(LiveActor* actor, const char* listName);
    bool tryRegisterActorModel(LiveActor* actor, const char* listName);
    bool tryRegisterLayout(LayoutActor* layout, const char* listName);
    bool tryRegisterUser(IUseExecutor* user, const char* listName);
    bool tryRegisterFunctor(const FunctorBase& functor, const char* listName);
    void createExecutorListTable();
    void execute() const;
    void executeList(const char* listName) const;
    bool isActive() const;

    const char* getName() { return mName; }

private:
    const char* mName = nullptr;
    s32 mActiveExecutorsSize = 0;
    ExecutorListBase** mActiveExecutors = nullptr;

    s32 mExecutorsAllSize = 0;
    s32 mExecutorsAllCapacity = 0;
    ExecutorListBase** mExecutorsAll = nullptr;

    s32 mExecutorsActorCapacity = 0;
    s32 mExecutorsActorSize = 0;
    ExecutorListActorDraw** mExecutorsActor = nullptr;

    s32 mExecutorsActorModelCapacity = 0;
    s32 mExecutorsActorModelSize = 0;
    ExecutorListActorModelDrawBase** mExecutorsActorModel = nullptr;

    s32 mExecutorsLayoutCapacity = 0;
    s32 mExecutorsLayoutSize = 0;
    ExecutorListLayoutDrawBase** mExecutorsLayout = nullptr;

    s32 mExecutorsUserCapacity = 0;
    s32 mExecutorsUserSize = 0;
    ExecutorListIUseExecutorDraw** mExecutorsUser = nullptr;

    s32 mExecutorsFunctorCapacity = 0;
    s32 mExecutorsFunctorSize = 0;
    ExecutorListFunctor** mExecutorsFunctor = nullptr;
};

static_assert(sizeof(ExecuteTableHolderDraw) == 0x80);

}  // namespace al
