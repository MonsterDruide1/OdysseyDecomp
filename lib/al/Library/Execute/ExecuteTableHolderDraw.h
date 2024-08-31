#pragma once

#include <container/seadPtrArray.h>

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
    void registerExecutorListActorModel(ExecutorListActorModelDrawBase* listActorModel);
    void registerExecutorListActor(ExecutorListActorDraw* listActor);
    void registerExecutorListLayout(ExecutorListLayoutDrawBase* listLayout);
    void registerExecutorListUser(ExecutorListIUseExecutorDraw* listUser);
    void registerExecutorListFunctor(ExecutorListFunctor* listFunctor);
    void registerExecutorListAll(ExecutorListBase* list);
    void tryRegisterActor(LiveActor* actor, const char* listName);
    void tryRegisterActorModel(LiveActor* actor, const char* listName);
    void tryRegisterLayout(LayoutActor* layout, const char* listName);
    void tryRegisterUser(IUseExecutor* user, const char* listName);
    void tryRegisterFunctor(const FunctorBase& functor, const char* listName);
    void createExecutorListTable();
    void execute() const;
    void executeList(const char* listName) const;
    bool isActive() const;

    const char* getName() { return mName; }

private:
    const char* mName;
    sead::PtrArray<ExecutorListBase> mActiveExecutors;
    sead::PtrArray<ExecutorListBase> mExecutorsAll;
    sead::PtrArray<ExecutorListActorDraw> mExecutorsActor;
    sead::PtrArray<ExecutorListActorModelDrawBase> mExecutorsActorModel;
    sead::PtrArray<ExecutorListLayoutDrawBase> mExecutorsLayout;
    sead::PtrArray<ExecutorListIUseExecutorDraw> mExecutorsUser;
    sead::PtrArray<ExecutorListFunctor> mExecutorsFunctor;
};

static_assert(sizeof(ExecuteTableHolderDraw) == 0x80);

}  // namespace al
