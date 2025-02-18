#include "Library/Execute/ExecuteDirector.h"

#include "Library/Base/StringUtil.h"
#include "Library/Execute/ActorExecuteInfo.h"
#include "Library/Execute/ExecuteRequestKeeper.h"
#include "Library/Execute/ExecuteTablesImpl.h"
#include "Library/Execute/LayoutExecuteInfo.h"
#include "Library/Layout/LayoutActor.h"
#include "Library/LiveActor/LiveActor.h"

namespace al {

ExecuteDirector::ExecuteDirector(s32 count) : mRequestCount(count) {};

ExecuteDirector::~ExecuteDirector() {
    for (s32 i = 0; i < mDrawTableCount; i++)
        delete mDrawTables[i];

    for (s32 i = 0; i < mUpdateTableCount; i++)
        delete mUpdateTables[i];
}

void ExecuteDirector::init(const ExecuteSystemInitInfo& initInfo) {
    mUpdateTableCount = UpdateTableSize;
    mUpdateTables = new ExecuteTableHolderUpdate*[UpdateTableSize];

    for (s32 i = 0; i < mUpdateTableCount; ++i) {
        mUpdateTables[i] = new ExecuteTableHolderUpdate();
        mUpdateTables[i]->init(UpdateTable[i].mName, initInfo, UpdateTable[i].mExecuteOrders,
                               UpdateTable[i].mExecuteOrderCount);
    }

    mDrawTableCount = DrawTableSize;
    mDrawTables = new ExecuteTableHolderDraw*[DrawTableSize];

    for (s32 i = 0; i < mDrawTableCount; ++i) {
        mDrawTables[i] = new ExecuteTableHolderDraw();
        mDrawTables[i]->init(DrawTable[i].mName, initInfo, DrawTable[i].mExecuteOrders,
                             DrawTable[i].mExecuteOrderCount);
    }

    mRequestKeeper = new ExecuteRequestKeeper(mRequestCount);
}

void ExecuteDirector::registerActorUpdate(LiveActor* actor, const char* listName) {
    if (!actor->getExecuteInfo())
        actor->initExecuteInfo(new ActorExecuteInfo(mRequestKeeper));

    for (s32 i = 0; i < mUpdateTableCount; i++)
        mUpdateTables[i]->tryRegisterActor(actor, listName);
}

void ExecuteDirector::registerActorDraw(LiveActor* actor, const char* listName) {
    if (!actor->getExecuteInfo())
        actor->initExecuteInfo(new ActorExecuteInfo(mRequestKeeper));

    for (s32 i = 0; i < mDrawTableCount; i++) {
        mDrawTables[i]->tryRegisterActor(actor, listName);
        mDrawTables[i]->tryRegisterActorModel(actor, listName);
    }
}

void ExecuteDirector::registerActorModelDraw(LiveActor* actor, const char* listName) {
    if (!actor->getExecuteInfo())
        actor->initExecuteInfo(new ActorExecuteInfo(mRequestKeeper));

    for (s32 i = 0; i < mDrawTableCount; i++)
        mDrawTables[i]->tryRegisterActorModel(actor, listName);
}

void ExecuteDirector::registerLayoutUpdate(LayoutActor* layout, const char* listName) {
    if (!layout->getExecuteInfo())
        layout->initExecuteInfo(new LayoutExecuteInfo());

    for (s32 i = 0; i < mUpdateTableCount; i++)
        mUpdateTables[i]->tryRegisterLayout(layout, listName);
}

void ExecuteDirector::registerLayoutDraw(LayoutActor* layout, const char* listName) {
    if (!layout->getExecuteInfo())
        layout->initExecuteInfo(new LayoutExecuteInfo());

    for (s32 i = 0; i < mDrawTableCount; i++)
        mDrawTables[i]->tryRegisterLayout(layout, listName);
}

void ExecuteDirector::registerUser(IUseExecutor* user, const char* listName) {
    for (s32 i = 0; i < mUpdateTableCount; i++)
        mUpdateTables[i]->tryRegisterUser(user, listName);

    for (s32 i = 0; i < mDrawTableCount; i++)
        mDrawTables[i]->tryRegisterUser(user, listName);
}

void ExecuteDirector::registerFunctor(const FunctorBase& functor, const char* listName) {
    for (s32 i = 0; i < mUpdateTableCount; i++)
        mUpdateTables[i]->tryRegisterFunctor(functor, listName);

    for (s32 i = 0; i < mDrawTableCount; i++)
        mDrawTables[i]->tryRegisterFunctor(functor, listName);
}

void ExecuteDirector::registerFunctorDraw(const FunctorBase& functor, const char* listName) {
    for (s32 i = 0; i < mDrawTableCount; i++)
        mDrawTables[i]->tryRegisterFunctor(functor, listName);
}

void ExecuteDirector::createExecutorListTable() {
    for (s32 i = 0; i < mUpdateTableCount; i++)
        mUpdateTables[i]->createExecutorListTable();

    for (s32 i = 0; i < mDrawTableCount; i++)
        mDrawTables[i]->createExecutorListTable();
}

// NON_MATCHING
void ExecuteDirector::execute(const char* tableName) const {
    if (!tableName) {
        mRequestKeeper->executeRequestActorMovementAllOn();
        mRequestKeeper->executeRequestActorDrawAllOn();

        mUpdateTables[0]->execute();

        mRequestKeeper->executeRequestActorMovementAllOff();
        mRequestKeeper->executeRequestActorDrawAllOff();

        return;
    }

    bool isActorEnabled = false;

    if (isEqualString(tableName, mUpdateTables[0]->getName())) {
        mRequestKeeper->executeRequestActorMovementAllOn();
        mRequestKeeper->executeRequestActorDrawAllOn();
        isActorEnabled = true;
    }

    for (s32 i = 0; i < mUpdateTableCount; i++) {
        if (isEqualString(tableName, mUpdateTables[i]->getName())) {
            mUpdateTables[i]->execute();
            break;
        }
    }

    if (isActorEnabled) {
        mRequestKeeper->executeRequestActorMovementAllOff();
        mRequestKeeper->executeRequestActorDrawAllOff();
    }
}

void ExecuteDirector::executeList(const char* tableName, const char* listName) const {
    mRequestKeeper->executeRequestActorMovementAllOn();
    mRequestKeeper->executeRequestActorDrawAllOn();
    if (tableName) {
        for (s32 i = 0; i < mUpdateTableCount; ++i)
            if (isEqualString(tableName, mUpdateTables[i]->getName()))
                mUpdateTables[i]->executeList(listName);
    } else {
        mUpdateTables[0]->executeList(listName);
    }
    mRequestKeeper->executeRequestActorMovementAllOff();
    mRequestKeeper->executeRequestActorDrawAllOff();
}

void ExecuteDirector::draw(const char* tableName) const {
    if (!tableName) {
        mDrawTables[0]->execute();
        return;
    }

    for (s32 i = 0; i < mDrawTableCount; ++i) {
        if (isEqualString(tableName, mDrawTables[i]->getName())) {
            mDrawTables[i]->execute();
            return;
        }
    }
}

void ExecuteDirector::drawList(const char* tableName, const char* listName) const {
    for (s32 i = 0; i < mDrawTableCount; ++i) {
        if (isEqualString(tableName, mDrawTables[i]->getName())) {
            mDrawTables[i]->executeList(listName);
            return;
        }
    }
}

bool ExecuteDirector::isActiveDraw(const char* tableName) const {
    if (!tableName)
        return mDrawTables[0]->isActive();

    for (s32 i = 0; i < mDrawTableCount; ++i)
        if (isEqualString(tableName, mDrawTables[i]->getName()))
            return mDrawTables[i]->isActive();

    return false;
}

}  // namespace al
