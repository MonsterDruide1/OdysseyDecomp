#include "Library/MapObj/SubActorLodExecutor.h"

#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/LiveActor/SubActorKeeper.h"
#include "Library/Model/ModelShapeUtil.h"
#include "Library/Placement/PlacementFunction.h"

namespace al {
class ModelKeeper;

SubActorLodExecutor::SubActorLodExecutor(LiveActor* actor, const ActorInitInfo& info,
                                         s32 subActorInfoIndex)
    : mActor(actor), mSubActorInfoIndex(subActorInfoIndex) {
    LiveActor* subActor = getLodSubActor();

    const char* cubeMapUnitName = nullptr;
    tryGetStringArg(&cubeMapUnitName, info, "CubeMapUnitName");

    if (cubeMapUnitName != nullptr)
        forceApplyCubeMap(subActor, cubeMapUnitName);

    subActor->makeActorDead();
}

void SubActorLodExecutor::control() {
    ModelKeeper* modelKeeper = mActor->getModelKeeper();
    LiveActor* subActor = getLodSubActor();

    if (mLodAction == LodAction::HideActor)
        hideModel(mActor);
    else if (mLodAction == LodAction::KillSubActor)
        subActor->makeActorDead();
    mLodAction = LodAction::None;

    if (isLessMaxLodLevelNoClamp(modelKeeper))
        showModelIfHide(mActor);
    else if (isDead(subActor))
        subActor->makeActorAlive();

    if (isGreaterEqualMaxLodLevelNoClamp(modelKeeper) && !isHideModel(mActor))
        mLodAction = LodAction::HideActor;
    else if (isLessMaxLodLevelNoClamp(modelKeeper) && isAlive(subActor))
        mLodAction = LodAction::KillSubActor;
}

LiveActor* SubActorLodExecutor::getLodSubActor() {
    return getSubActor(mActor, mSubActorInfoIndex);
}
}  // namespace al
