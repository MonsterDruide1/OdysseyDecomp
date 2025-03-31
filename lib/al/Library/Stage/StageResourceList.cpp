#include "Library/Stage/StageResourceList.h"

#include "Library/Execute/ExecuteTableHolderUpdate.h"
#include "Library/LiveActor/ActorInitInfo.h"

namespace al {

StageSyncCounter::StageSyncCounter() {}

void StageSyncCounter::execute() {
    mCounter++;
}

const char* StageSyncCounter::getSceneObjName() const {
    return "ステージ同期カウンタ";
}

void StageSyncCounter::initAfterPlacementSceneObj(const ActorInitInfo& initInfo) {
    registerExecutorUser(this, initInfo.executeDirector, getSceneObjName());
}

}  // namespace al
