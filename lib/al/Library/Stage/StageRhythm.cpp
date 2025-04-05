#include "Library/Stage/StageRhythm.h"

#include "Library/Execute/ExecuteUtil.h"
#include "Library/LiveActor/ActorInitInfo.h"

namespace al {

StageSyncCounter::StageSyncCounter() = default;

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
