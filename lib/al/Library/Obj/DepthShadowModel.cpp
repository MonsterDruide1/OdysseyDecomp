#include "Library/Obj/DepthShadowModel.h"

#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorDrawFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"

namespace al {
DepthShadowModel::DepthShadowModel(LiveActor* parent, const ActorInitInfo& info,
                                   const char* executorDrawName, bool isVisible)
    : LiveActor("デプスシャドウモデル") {
    initActorSceneInfo(this, info);
    initPoseKeeper(parent->getPoseKeeper());
    initActorModelKeeperByHost(this, parent);
    initExecutorDraw(this, info, executorDrawName);
    initActorClipping(this, info);

    mPos.set(getClippingCenterPos(parent));

    setClippingInfo(this, getClippingRadius(parent), &mPos);

    mIsVisible = isVisible;

    makeActorAlive();
}

DepthShadowModel::DepthShadowModel(LiveActor* parent, const ActorInitInfo& info,
                                   const char* childArchiveName, const char* childSuffix,
                                   bool isVisible)
    : LiveActor("デプスシャドウモデル") {
    initChildActorWithArchiveNameWithPlacementInfo(this, info, childArchiveName, childSuffix);

    mIsVisible = isVisible;

    makeActorAlive();
}

void DepthShadowModel::initAfterPlacement() {
    if (!mIsVisible)
        return;

    tryExpandClippingByDepthShadowLength(this, &mPos);
}
}  // namespace al
