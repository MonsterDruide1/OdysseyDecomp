#include "Library/Execute/ExecuteRequestKeeper.h"

#include "Library/Execute/ActorExecuteInfo.h"
#include "Library/Execute/ExecutorActorExecuteBase.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Model/ModelDrawerBase.h"
#include "Library/Model/ModelKeeper.h"

namespace al {

ExecuteRequestInfo::ExecuteRequestInfo() = default;

ExecuteRequestTable::ExecuteRequestTable(s32 maxSize) : mMaxSize{maxSize} {
    LiveActor** actors = new LiveActor*[mMaxSize];

    if (mMaxSize != 0)
        memset(actors, 0, (s64)mMaxSize * 8);

    mRequests = actors;
}

ExecuteRequestKeeper::ExecuteRequestKeeper(s32 maxSize) {
    for (s32 i = 0; i < 4; i++)
        mRequestTables[i] = new ExecuteRequestTable(maxSize);
}

void ExecuteRequestKeeper::executeRequestActorMovementAllOn() {
    ExecuteRequestTable* movementOn = mRequestTables[Request_Movement];

    for (s32 i = 0; i < movementOn->getSize(); i++) {
        LiveActor* actor = movementOn->getRequest(i);
        ActorExecuteInfo* info = actor->getExecuteInfo();
        for (s32 j = 0; j < info->getUpdaterCount(); j++)
            info->getUpdater(j)->addActor(actor);
    }

    movementOn->clear();
}

void ExecuteRequestKeeper::executeRequestActorMovementAllOff() {
    ExecuteRequestTable* movementOff = mRequestTables[Request_RemoveFromMovement];

    for (s32 i = 0; i < movementOff->getSize(); i++) {
        LiveActor* actor = movementOff->getRequest(i);
        ActorExecuteInfo* info = actor->getExecuteInfo();
        for (s32 j = 0; j < info->getUpdaterCount(); j++)
            info->getUpdater(j)->removeActor(actor);
    }

    movementOff->clear();
}

void ExecuteRequestKeeper::executeRequestActorDrawAllOn() {
    ExecuteRequestTable* drawOn = mRequestTables[Request_Draw];

    for (s32 i = 0; i < drawOn->getSize(); i++) {
        LiveActor* actor = drawOn->getRequest(i);
        ActorExecuteInfo* info = actor->getExecuteInfo();
        for (s32 j = 0; j < info->getDrawerCount(); j++)
            info->getDrawer(j)->addModel(actor->getModelKeeper()->getModelCtrl());
    }

    drawOn->clear();
}

void ExecuteRequestKeeper::executeRequestActorDrawAllOff() {
    ExecuteRequestTable* drawOff = mRequestTables[Request_RemoveFromDraw];

    for (s32 i = 0; i < drawOff->getSize(); i++) {
        LiveActor* actor = drawOff->getRequest(i);
        ActorExecuteInfo* info = actor->getExecuteInfo();
        for (s32 j = 0; j < info->getDrawerCount(); j++)
            info->getDrawer(j)->removeModel(actor->getModelKeeper()->getModelCtrl());
    }

    drawOff->clear();
}

void ExecuteRequestKeeper::request(LiveActor* actor, s32 requestType) {
    ExecuteRequestTable* addRequestTable = mRequestTables[requestType];
    ExecuteRequestTable* removeRequestTable = nullptr;

    switch (requestType) {
    case Request_Movement:
        removeRequestTable = mRequestTables[Request_RemoveFromMovement];
        break;
    case Request_RemoveFromMovement:
        removeRequestTable = mRequestTables[Request_Movement];
        break;
    case Request_Draw:
        removeRequestTable = mRequestTables[Request_RemoveFromDraw];
        break;
    case Request_RemoveFromDraw:
        removeRequestTable = mRequestTables[Request_Draw];
        break;
    }

    removeRequestTable->removeRequest(actor);
    addRequestTable->addRequest(actor);
}

}  // namespace al
