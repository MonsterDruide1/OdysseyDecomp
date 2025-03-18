#include "Library/Obj/SilhouetteModel.h"

#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"

namespace al {
class ActorInitInfo;

SilhouetteModel::SilhouetteModel(LiveActor* parent, const ActorInitInfo& initInfo,
                                 const char* category)
    : LiveActor("シルエットモデル") {
    initActorSceneInfo(this, initInfo);
    initPoseKeeper(parent->getPoseKeeper());
    initActorModelKeeperByHost(this, parent);
    initActorClipping(this, initInfo);
    invalidateClipping(this);
    initExecutorDraw(this, initInfo, category);
    makeActorAlive();
}

void SilhouetteModel::movement() {}
}  // namespace al
