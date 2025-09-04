#include "Project/LiveActor/ActorExecuteFunction.h"

#include <basis/seadTypes.h>

#include "Library/Draw/GraphicsSystemInfo.h"
#include "Library/Execute/ExecuteDirector.h"
#include "Library/LiveActor/LiveActorKit.h"
#include "Library/Model/ModelDrawBufferUpdater.h"
#include "Library/Model/SkyDirector.h"
#include "Project/Execute/ExecuteAsyncExecutor.h"

namespace al {
void executeUpdate(LiveActorKit* actorKit, const char* tableName) {
    actorKit->update(tableName);
}

void executeUpdateTable(LiveActorKit* actorKit, const char* tableName) {
    actorKit->getExecuteDirector()->execute(tableName);
}

void executeUpdateList(LiveActorKit* actorKit, const char* tableName, const char* listName) {
    actorKit->getExecuteDirector()->executeList(tableName, listName);
}

void executeDraw(const LiveActorKit* actorKit, const char* tableName) {
    actorKit->getExecuteDirector()->draw(tableName);
}

void executeDrawList(const LiveActorKit* actorKit, const char* tableName, const char* listName) {
    actorKit->getExecuteDirector()->drawList(tableName, listName);
}

bool isActiveDraw(const LiveActorKit* actorKit, const char* tableName) {
    return actorKit->getExecuteDirector()->isActiveDraw(tableName);
}

void executeUpdateDrawBuffer(const LiveActorKit* actorKit) {
    actorKit->getModelDrawBufferUpdater()->tryUpdateAsync();
}

void waitUpdateDrawBuffer(const LiveActorKit* actorKit) {
    actorKit->getModelDrawBufferUpdater()->waitAsync();
}

void executeUpdateCalcView(const LiveActorKit* actorKit) {
    actorKit->getExecutorCore1()->executeAsync();
    actorKit->getExecutorCore2()->executeAsync();
}

void waitUpdateCalcView(const LiveActorKit* actorKit) {
    actorKit->getExecutorCore1()->waitAsync();
    actorKit->getExecutorCore2()->waitAsync();
    if (actorKit->getGraphicsSystemInfo() != nullptr &&
        actorKit->getGraphicsSystemInfo()->getSkyDirector() != nullptr) {
        actorKit->getGraphicsSystemInfo()->getSkyDirector()->update();
    }
}

void incrementDrawBufferCounter(const LiveActorKit* actorKit) {
    ModelDrawBufferCounter* counter = actorKit->getModelDrawBufferCounter();
    s32 currentPlus = counter->currentDrawBuffer + 1;
    s32 mult = 0;
    if (counter->maxDrawBuffer != 0)
        mult = currentPlus / counter->maxDrawBuffer;
    counter->currentDrawBuffer +=
        1 - counter->maxDrawBuffer * (currentPlus / counter->maxDrawBuffer);
}

}  // namespace al
