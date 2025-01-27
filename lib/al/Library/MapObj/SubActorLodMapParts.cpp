#include "Library/MapObj/SubActorLodMapParts.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/SubActorKeeper.h"
#include "Library/MapObj/SubActorLodExecutor.h"
#include "Library/Placement/PlacementFunction.h"

namespace al {
SubActorLodMapParts::SubActorLodMapParts(const char* name) : LiveActor(name) {}

void SubActorLodMapParts::init(const ActorInitInfo& info) {
    const char* suffix = nullptr;
    tryGetStringArg(&suffix, info, "Suffix");
    initMapPartsActor(this, info, suffix);

    mSubActorLodExecutor = new SubActorLodExecutor(this, info, 0);
    if (getModelKeeper() != nullptr && !isExistAction(this) && !isViewDependentModel(this))
        mIsControlled = true;

    trySyncStageSwitchAppearAndKill(this);
    makeActorAlive();
}

void SubActorLodMapParts::control() {
    mSubActorLodExecutor->control();
}

void SubActorLodMapParts::movement() {
    if (mIsControlled) {
        control();

        return;
    }

    if (isHideModel(this)) {
        LiveActor::movement();

        return;
    }

    ((SubActorLodMapParts*)getSubActor(this, 0))->movement();
}

void SubActorLodMapParts::calcAnim() {
    if (mIsControlled) {
        calcViewModel(this);

        return;
    }

    LiveActor::calcAnim();
}
}  // namespace al
