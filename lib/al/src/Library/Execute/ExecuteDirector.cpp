#include "Library/Execute/ExecuteDirector.h"

#include "Library/Base/StringUtil.h"
#include "Library/Execute/ActorExecuteInfo.h"
#include "Library/Execute/ExecuteRequestKeeper.h"
#include "Library/Execute/ExecuteTablesImpl.h"
#include "Library/Execute/LayoutExecuteInfo.h"
#include "Library/Layout/LayoutActor.h"
#include "Library/LiveActor/LiveActor.h"

namespace al {

ExecuteDirector::ExecuteDirector(s32 count) {
    mRequestCount = count;
}

ExecuteDirector::~ExecuteDirector() {}

void ExecuteDirector::init(ExecuteSystemInitInfo const& initInfo) {
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

void ExecuteDirector::registerActorUpdate(LiveActor* actor, const char* tableName) {
    if (!actor->getExecuteInfo()) {
        actor->initExecuteInfo(new ActorExecuteInfo(mRequestKeeper));
    }
    if (mUpdateTableCount >= 1) {
        for (s32 i = 0; i < mUpdateTableCount; ++i) {
            mUpdateTables[i]->tryRegisterActor(actor, tableName);
        }
    }
}

void ExecuteDirector::registerActorDraw(LiveActor* actor, const char* tableName) {
    if (!actor->getExecuteInfo()) {
        actor->initExecuteInfo(new ActorExecuteInfo(mRequestKeeper));
    }
    if (mDrawTableCount >= 1) {
        for (s32 i = 0; i < mDrawTableCount; ++i) {
            mDrawTables[i]->tryRegisterActor(actor, tableName);
            mDrawTables[i]->tryRegisterActorModel(actor, tableName);
        }
    }
}

void ExecuteDirector::registerActorModelDraw(LiveActor* actor, const char* tableName) {
    if (!actor->getExecuteInfo()) {
        actor->initExecuteInfo(new ActorExecuteInfo(mRequestKeeper));
    }
    if (mDrawTableCount >= 1) {
        for (s32 i = 0; i < mDrawTableCount; ++i) {
            mDrawTables[i]->tryRegisterActorModel(actor, tableName);
        }
    }
}

void ExecuteDirector::registerLayoutUpdate(LayoutActor* lyt, const char* tableName) {
    if (!lyt->getExecuteInfo()) {
        lyt->initExecuteInfo(new LayoutExecuteInfo());
    }
    if (mUpdateTableCount >= 1) {
        for (s32 i = 0; i < mUpdateTableCount; ++i) {
            mUpdateTables[i]->tryRegisterLayout(lyt, tableName);
        }
    }
}

void ExecuteDirector::registerLayoutDraw(LayoutActor* lyt, const char* tableName) {
    if (!lyt->getExecuteInfo()) {
        lyt->initExecuteInfo(new LayoutExecuteInfo());
    }
    if (mDrawTableCount >= 1) {
        for (s32 i = 0; i < mDrawTableCount; ++i) {
            mDrawTables[i]->tryRegisterLayout(lyt, tableName);
        }
    }
}

void ExecuteDirector::registerUser(IUseExecutor* user, const char* tableName) {
    if (mUpdateTableCount >= 1) {
        for (s32 i = 0; i < mUpdateTableCount; ++i) {
            mUpdateTables[i]->tryRegisterUser(user, tableName);
        }
    }
    if (mDrawTableCount >= 1) {
        for (s32 i = 0; i < mDrawTableCount; ++i) {
            mDrawTables[i]->tryRegisterUser(user, tableName);
        }
    }
}

void ExecuteDirector::registerFunctor(const FunctorBase& functor, const char* tableName) {
    if (mUpdateTableCount >= 1) {
        for (s32 i = 0; i < mUpdateTableCount; ++i) {
            mUpdateTables[i]->tryRegisterFunctor(functor, tableName);
        }
    }
    if (mDrawTableCount >= 1) {
        for (s32 i = 0; i < mDrawTableCount; ++i) {
            mDrawTables[i]->tryRegisterFunctor(functor, tableName);
        }
    }
}

void ExecuteDirector::registerFunctorDraw(const FunctorBase& functor, const char* tableName) {
    if (mDrawTableCount >= 1) {
        for (s32 i = 0; i < mDrawTableCount; ++i) {
            mDrawTables[i]->tryRegisterFunctor(functor, tableName);
        }
    }
}

void ExecuteDirector::createExecutorListTable() {
    if (mUpdateTableCount >= 1) {
        for (s32 i = 0; i < mUpdateTableCount; ++i) {
            mUpdateTables[i]->createExecutorListTable();
        }
    }
    if (mDrawTableCount >= 1) {
        for (s32 i = 0; i < mDrawTableCount; ++i) {
            mDrawTables[i]->createExecutorListTable();
        }
    }
}

void ExecuteDirector::execute(const char* tableName) const {
    if (!tableName) {
        mRequestKeeper->executeRequestActorMovementAllOn();
        mRequestKeeper->executeRequestActorDrawAllOn();

        mUpdateTables[0]->execute();

        mRequestKeeper->executeRequestActorMovementAllOff();
        mRequestKeeper->executeRequestActorDrawAllOff();
    } else {
        bool isActorEnabled;

        if (isEqualString(tableName, mUpdateTables[0]->getName())) {
            mRequestKeeper->executeRequestActorMovementAllOn();
            mRequestKeeper->executeRequestActorDrawAllOn();
            isActorEnabled = true;
        } else {
            isActorEnabled = false;
        }

        if (mUpdateTableCount >= 1) {
            for (s32 i = 0; i < mUpdateTableCount; ++i) {
                if (isEqualString(tableName, mUpdateTables[i]->getName())) {
                    mUpdateTables[i]->execute();

                    if (isActorEnabled) {
                        mRequestKeeper->executeRequestActorMovementAllOff();
                        mRequestKeeper->executeRequestActorDrawAllOff();
                    }

                    return;
                }
            }
        }

        if (isActorEnabled) {
            mRequestKeeper->executeRequestActorMovementAllOff();
            mRequestKeeper->executeRequestActorDrawAllOff();
        }
    }
}

void ExecuteDirector::executeList(const char* tableName, const char* listName) const {
    mRequestKeeper->executeRequestActorMovementAllOn();
    mRequestKeeper->executeRequestActorDrawAllOn();
    if (tableName) {
        if (mUpdateTableCount >= 1) {
            for (s32 i = 0; i < mUpdateTableCount; ++i) {
                if (isEqualString(tableName, mUpdateTables[i]->getName())) {
                    mUpdateTables[i]->executeList(listName);
                }
            }
        }
    } else {
        mUpdateTables[0]->executeList(listName);
    }
    mRequestKeeper->executeRequestActorMovementAllOff();
    mRequestKeeper->executeRequestActorDrawAllOff();
}

void ExecuteDirector::draw(const char* tableName) const {
    if (tableName) {
        if (mDrawTableCount >= 1) {
            for (s32 i = 0; i < mDrawTableCount; ++i) {
                if (isEqualString(tableName, mDrawTables[i]->getName())) {
                    mDrawTables[i]->execute();
                    return;
                }
            }
        }
    } else {
        mDrawTables[0]->execute();
    }
}

void ExecuteDirector::drawList(const char* tableName, const char* listName) const {
    if (mDrawTableCount >= 1) {
        for (s32 i = 0; i < mDrawTableCount; ++i) {
            if (isEqualString(tableName, mDrawTables[i]->getName())) {
                mDrawTables[i]->executeList(listName);
                return;
            }
        }
    }
}

bool ExecuteDirector::isActiveDraw(const char* tableName) const {
    if (tableName) {
        if (mDrawTableCount >= 1) {
            for (s32 i = 0; i < mDrawTableCount; ++i) {
                if (isEqualString(tableName, mDrawTables[i]->getName())) {
                    return mDrawTables[i]->isActive();
                }
            }
        }
    } else {
        return mDrawTables[0]->isActive();
    }
}

}  // namespace al
