#pragma once

#include <container/seadPtrArray.h>

namespace al {
class ExecuteDirector;
struct ExecuteOrder;
struct ExecuteSystemInitInfo;
class ExecutorListActorExecuteBase;
class ExecutorListBase;
class ExecutorListFunctor;
class ExecutorListIUseExecutorDraw;
class ExecutorListIUseExecutorUpdate;
class ExecutorListLayoutUpdate;
class FunctorBase;
class IUseExecutor;
class LayoutActor;
class LiveActor;

class ExecuteTableHolderUpdate {
public:
    ExecuteTableHolderUpdate();
    virtual ~ExecuteTableHolderUpdate();
    void init(const char* name, const ExecuteSystemInitInfo& initInfo, const ExecuteOrder* orders, s32 orderCount);
    void registerExecutorListActor(ExecutorListActorExecuteBase* listActor);
    void registerExecutorListLayout(ExecutorListLayoutUpdate* listLayout);
    void registerExecutorListUser(ExecutorListIUseExecutorUpdate* listUser);
    void registerExecutorListFunctor(ExecutorListFunctor* listFunctor);
    void registerExecutorListAll(ExecutorListBase* list);
    void tryRegisterActor(LiveActor* actor, const char* listName);
    void tryRegisterLayout(LayoutActor* layout, const char* listName);
    void tryRegisterUser(IUseExecutor* user, const char* listName);
    void tryRegisterFunctor(const FunctorBase& functor, const char* listName);
    void createExecutorListTable();
    void execute() const;
    void executeList(const char* listName) const;

    const char* getName() { return mName; }

private:
    const char* mName;
    sead::PtrArray<ExecutorListBase> mActiveExecutors;
    sead::PtrArray<ExecutorListBase> mExecutorsAll;
    sead::PtrArray<ExecutorListActorExecuteBase> mExecutorsActor;
    sead::PtrArray<ExecutorListLayoutUpdate> mExecutorsLayout;
    sead::PtrArray<ExecutorListIUseExecutorUpdate> mExecutorsUser;
    sead::PtrArray<ExecutorListFunctor> mExecutorsFunctor;
};

static_assert(sizeof(ExecuteTableHolderUpdate) == 0x70);

void registerExecutorUser(IUseExecutor* user, ExecuteDirector* director, const char* listName);
}  // namespace al

namespace alActorSystemFunction {
void addToExecutorMovement(al::LiveActor* actor);
void addToExecutorDraw(al::LiveActor* actor);
void removeFromExecutorMovement(al::LiveActor* actor);
void removeFromExecutorDraw(al::LiveActor* actor);
void updateExecutorDraw(al::LiveActor* actor);
}  // namespace alActorSystemFunction
