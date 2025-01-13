#pragma once

#include <basis/seadTypes.h>

#include "Library/HostIO/HioNode.h"

namespace al {
class ExecuteRequestKeeper;
class ExecuteTableHolderDraw;
class ExecuteTableHolderUpdate;
struct ExecuteSystemInitInfo;
class FunctorBase;
class IUseExecutor;
class LayoutActor;
class LiveActor;

class ExecuteDirector : public HioNode {
public:
    ExecuteDirector(s32 count);
    virtual ~ExecuteDirector();
    void init(const ExecuteSystemInitInfo& initInfo);
    void registerActorUpdate(LiveActor* actor, const char* listName);
    void registerActorDraw(LiveActor* actor, const char* listName);
    void registerActorModelDraw(LiveActor* actor, const char* listName);
    void registerLayoutUpdate(LayoutActor* layout, const char* listName);
    void registerLayoutDraw(LayoutActor* layout, const char* listName);
    void registerUser(IUseExecutor* user, const char* listName);
    void registerFunctor(const FunctorBase& functor, const char* listName);
    void registerFunctorDraw(const FunctorBase& functor, const char* listName);
    void createExecutorListTable();
    void execute(const char* tableName) const;
    void executeList(const char* tableName, const char* listName) const;
    void draw(const char* tableName) const;
    void drawList(const char* tableName, const char* listName) const;
    bool isActiveDraw(const char* tableName) const;

private:
    s32 mRequestCount;
    s32 mUpdateTableCount = 0;
    ExecuteTableHolderUpdate** mUpdateTables = nullptr;
    s32 mDrawTableCount = 0;
    ExecuteTableHolderDraw** mDrawTables = nullptr;
    ExecuteRequestKeeper* mRequestKeeper = nullptr;
};

static_assert(sizeof(ExecuteDirector) == 0x30);

}  // namespace al
