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
    void init(const char*, const ExecuteSystemInitInfo&, const ExecuteOrder*, s32);
    void registerExecutorListActorModel(ExecutorListActorModelDrawBase*);
    void registerExecutorListActor(ExecutorListActorDraw*);
    void registerExecutorListLayout(ExecutorListLayoutDrawBase*);
    void registerExecutorListUser(ExecutorListIUseExecutorDraw*);
    void registerExecutorListFunctor(ExecutorListFunctor*);
    void registerExecutorListAll(ExecutorListBase*);
    void tryRegisterActor(LiveActor*, const char*);
    void tryRegisterActorModel(LiveActor*, const char*);
    void tryRegisterLayout(LayoutActor*, const char*);
    void tryRegisterUser(IUseExecutor*, const char*);
    void tryRegisterFunctor(const FunctorBase&, const char*);
    void createExecutorListTable();
    void execute() const;
    void executeList(const char*) const;
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
}  // namespace al
