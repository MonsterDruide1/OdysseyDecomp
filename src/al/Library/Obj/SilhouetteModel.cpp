#include "al/Library/Obj/SilhouetteModel.h"
#include "al/Library/LiveActor/ActorClippingFunction.h"
#include "al/Library/LiveActor/ActorInitInfo.h"

namespace al {
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
