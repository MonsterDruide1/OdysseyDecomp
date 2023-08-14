#pragma once

#include <container/seadPtrArray.h>

namespace al {
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
    void init(const char*, const ExecuteSystemInitInfo&, const ExecuteOrder*, s32);
    void registerExecutorListActor(ExecutorListActorExecuteBase*);
    void registerExecutorListLayout(ExecutorListLayoutUpdate*);
    void registerExecutorListUser(ExecutorListIUseExecutorUpdate*);
    void registerExecutorListFunctor(ExecutorListFunctor*);
    void registerExecutorListAll(ExecutorListBase*);
    void tryRegisterActor(LiveActor*, const char*);
    void tryRegisterLayout(LayoutActor*, const char*);
    void tryRegisterUser(IUseExecutor*, const char*);
    void tryRegisterFunctor(const FunctorBase&, const char*);
    void createExecutorListTable();
    void execute() const;
    void executeList(const char*) const;

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
}  // namespace al
